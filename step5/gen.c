#include <stdio.h>

#include "cc1.h"

// アセンブラを用いたスタックマシンの生成
void gen(Node *node) {
	if (node->ty == ND_NUM) {
		printf("  push %d\n", node->val);
		return;
	}

	gen(node->lhs); // 左の木をコンパイル
	gen(node->rhs); // 右の木をコンパイ ル

	// スタックから2つ取り出して演算を行う
	printf("  pop rdi\n");
	printf("  pop rax\n");

	switch (node->ty) {
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

}
