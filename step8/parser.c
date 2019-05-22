#include <stdlib.h>

#include "1cc.h"

// tokens[]のインデックス
int pos = 0;

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

// 次のトークンが期待した型かどうかをチェックする
int consume(int ty) {
	if (((Token *)tokens->data[pos])->ty != ty) return 0;
	pos++;
	return 1; // 真
}

// パーサ本体
Node *expr() {
	return equality();
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

	// そうでなければ数値のはず
	if (((Token *)tokens->data[pos])->ty == TK_NUM) return new_node_num(((Token *)tokens->data[pos++])->val);

	error_at(((Token *)tokens->data[pos])->input, "数値でも開き括弧でも無いトークンです");
	return 0; // ここまではこないはず
}
