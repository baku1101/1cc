#include <stdlib.h>

#include "cc1.h"

Token tokens[100];
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
	if (tokens[pos].ty != ty) return 0;
	pos++;
	return 1; // 真
}

// パーサ本体
Node *expr() {
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
			error_at(tokens[pos].input, "開き括弧に対応する閉じ括弧がありません");
		}
		return node;
	}

	// そうでなければ数値のはず
	if (tokens[pos].ty == TK_NUM) return new_node_num(tokens[pos++].val);

	error_at(tokens[pos].input, "数値でも開き括弧でも無いトークンです");
	return 0; // dummy
}
