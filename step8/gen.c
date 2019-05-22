#include <stdio.h>

#include "1cc.h"

// アセンブラを用いたスタックマシンの生成
void gen(Node *node) {
	if (node->ty == ND_NUM) {
		printf("  push %d\n", node->val);
		return;
	}

	gen(node->lhs); // 左の木をコンパイル
	gen(node->rhs); // 右の木をコンパイル

	// スタックから2つ取り出して演算を行う(つまり2項演算子用)
	printf("  pop rdi\n");
	printf("  pop rax\n");

	switch (node->ty) {
		case ND_EQ:
			printf("  cmp rax, rdi\n");
			printf("  sete al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_NE:
			printf("  cmp rax, rdi\n");
			printf("  setne al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LE:
			printf("  cmp rax, rdi\n");
			printf("  setle al\n");
			printf("  movzb rax, al\n");
			break;
		case '<':
			printf("  cmp rax, rdi\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;

		case '+':
			printf("  add rax, rdi\n");
			break;
		case '-':
			printf("  sub rax, rdi\n");
			break;
		case '*':
			printf("  mul rdi\n");
			break;
		case '/':
			printf("  mov rdx, 0\n"); // divで暗黙的にrdxを使うため，リセット
			printf("  div rdi\n");
			break;
	}

	// 演算結果はスタックに格納
	printf("  push rax\n");
	return;
}
