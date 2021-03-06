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

// callノード作成
Node *new_node_call(char *name) {
	Node *node = malloc(sizeof(Node));
	node->ty = ND_CALL;
	node->name = name;
	node->stmt_vec = new_vector();
	while (!consume(')')) {
		Node *arg = term();
		vec_push(node->stmt_vec, arg);
		consume(',');
	}
	return node;
}

// ifノード作成
Node *new_node_if() {
	Node *node;
	if (!consume('(')) {
		error_at(((Token *)tokens->data[pos])->input, "'if'の次のトークンが'('ではないです");
	}
	node = malloc(sizeof(Node));
	node->ty = ND_IF;
	node->cond = expr();
	if (!consume(')')) {
		error_at(((Token *)tokens->data[pos])->input, "開き括弧に対応する閉じ括弧がありません");
	}
	node->then = stmt();
	node->els = NULL;
	if (consume(TK_ELSE)) {
		node->els = stmt();
	}
	return node;
}

// forノード作成
Node *new_node_for() {
	Node *node;
	if (!consume('(')) {
		error_at(((Token *)tokens->data[pos])->input, "'for'の次のトークンが'('ではないです");
	}
	node = malloc(sizeof(Node));
	node->ty = ND_FOR;
	node->init = consume(';') ? NULL : stmt();
	node->cond = consume(';') ? NULL : stmt();
	node->inc  = consume(';') ? NULL : expr();
	if (!consume(')')) {
		error_at(((Token *)tokens->data[pos])->input, "開き括弧に対応する閉じ括弧がありません");
	}
	node->body = consume(';') ? NULL : stmt();
	return node;
}

// whileノード作成
Node *new_node_while() {
	Node *node;
	if (!consume('(')) {
		error_at(((Token *)tokens->data[pos])->input, "'for'の次のトークンが'('ではないです");
	}
	if (consume(')')) {
		error_at(((Token *)tokens->data[pos])->input, "'条件式が必要です");
	}
	node = malloc(sizeof(Node));
	node->ty = ND_WHILE;
	node->cond = expr();
	if (!consume(')')) {
		error_at(((Token *)tokens->data[pos])->input, "開き括弧に対応する閉じ括弧がありません");
	}
	node->body = stmt();
	return node;
}

// blockノード作成
Node *new_node_block() {
	Node *node = malloc(sizeof(Node));
	node->stmt_vec = new_vector();
	node->ty = ND_BLOCK;
	while (!consume('}')) {
		vec_push(node->stmt_vec, stmt());
	}
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
		node->expr = expr();
	}
	else if (consume('{')) {
		node = new_node_block();
		return node;
	}
	else if (consume(TK_IF)) {
		node = new_node_if();
		return node;
	}
	else if (consume(TK_FOR)) {
		node = new_node_for();
		return node;
	}
	else if (consume(TK_WHILE)) {
		node = new_node_while();
		return node;
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
	Token *token = tokens->data[pos];
	// 次のトークンが'('なら，"(" expr ")" のはず
	if (consume('(')) {
		Node *node = expr();
		if (!consume(')')) {
			error_at(((Token *)tokens->data[pos])->input, "開き括弧に対応する閉じ括弧がありません");
		}
		return node;
	}

	if (token->ty == TK_NUM) {
		pos++;
		return new_node_num(token->val);
	}

	if (token->ty == TK_IDENT) {
		pos++;
		char *name = token->name;
		// 次に "(" が来たら関数名(関数呼び出し), 来なかったら変数名
		if (consume('(')) {
			Node *node = new_node_call(name);
			return node;
		}
		register_val(name);
		return new_node_ident(name);
	}

	error_at(((Token *)tokens->data[pos])->input, "期待しないトークンです");
	return NULL; // ここまではこないはず
}
