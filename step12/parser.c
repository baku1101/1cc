#include <stdlib.h>

#include "1cc.h"

// tokens[]のインデックス
int pos = 0;

// extern
Vector *code;
Map *valmap;
int numval;

void register_val(char *name) {
	// valmapに入っていなかったら登録
	if (map_get(valmap, name) == NULL) {
		numval++;
		int *offset = malloc(sizeof(int));
		*offset = numval * 8;
		map_put(valmap, name, offset);
	}
	// 登録済みなら特になし
	return;
}

// ノード作成(2項演算子用)
Node *new_node(int ty, Node *lhs, Node *rhs) {
	Node *node = malloc(sizeof(Node));
	node->ty = ty;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

// ノード作成(数値用)
Node *new_node_num(int val) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_NUM;
	node->val = val;
	return node;
}

// ノード作成(識別子用)
Node *new_node_ident(char *name) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_IDENT;
	node->name = name;
	return node;
}

// 次のトークンが期待した型かどうかをチェックする
int consume(int ty) {
	if (((Token *)tokens->data[pos])->ty != ty) return 0;
	pos++;
	return 1; // 真
}

// パーサ本体
void program() {
	code = new_vector();
	valmap = new_map();
	numval = 0;
	while (!consume(TK_EOF)) {
		vec_push(code, stmt());
	}
	vec_push(code, NULL);
}

Node *stmt() {
	Node *node;
	if (consume(TK_RETURN)) {
		node = malloc(sizeof(Node));
		node->ty = ND_RETURN;
		node->lhs = expr();
	}
	else {
		node = expr();
	}
	if (!consume(';')) {
		error_at(((Token *)tokens->data[pos])->input, "';'ではないトークンです");
	}
	return node;
}

Node *expr() {
	return assign();
}

Node *assign() {
	Node *node = equality();
	if (consume('=')) {
		return new_node('=', node, assign());
	}
	return node;
}

Node *equality() {
	Node *node = relational();
	if (consume(TK_EQ)) node = new_node(ND_EQ, node, relational());
	else if (consume(TK_NE)) node = new_node(ND_NE, node, relational());
	return node;
}

Node *relational() {
	Node *node = add();
	if (consume(TK_GE)) node = new_node(ND_LE, add(), node); // GEは左右ひっくり返してLEにする
	else if (consume(TK_LE)) node = new_node(ND_LE, node, add());
	else if (consume('>')) node = new_node('<', add(), node); // これも同様
	else if (consume('<')) node = new_node('<', node, add());
	return node;
}

Node *add() {
	Node *node = mul();
	for (;;) {
		if (consume('+')) node = new_node('+', node, mul());
		else if (consume('-')) node = new_node('-', node, mul());
		else return node;
	}
}

Node *mul() {
	Node *node = unary();
	for (;;) {
		if (consume('*')) node = new_node('*', node, unary());
		else if (consume('/')) node = new_node('/', node, unary());
		else return node;
	}
}

Node *unary() {
	if (consume('+')) return term();
	if (consume('-')) return new_node('-', new_node_num(0), term());
	return term();

}

Node *term() {
	// 次のトークンが'('なら，"(" expr ")" のはず
	if (consume('(')) {
		Node *node = expr();
		if (!consume(')')) {
			error_at(((Token *)tokens->data[pos])->input, "開き括弧に対応する閉じ括弧がありません");
		}
		return node;
	}

	if (((Token *)tokens->data[pos])->ty == TK_NUM) {
		return new_node_num(((Token *)tokens->data[pos++])->val);
	}

	if (((Token *)tokens->data[pos])->ty == TK_IDENT) {
		char *name = ((Token *)tokens->data[pos++])->name;
		register_val(name);
		return new_node_ident(name);
	}

	error_at(((Token *)tokens->data[pos])->input, "数値でも開き括弧でも無いトークンです");
	return 0; // ここまではこないはず
}
