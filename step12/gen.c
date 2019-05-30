#include <stdio.h>

#include "1cc.h"

// ラベル番号
int l_num = 0;

void gen_lval(Node *node) {
	if (node->ty != ND_IDENT) {
		error("代入の左辺値が変数ではありません");
	}

	// 変数のアドレスをスタックにpushする
	int *offset = map_get(valmap, node->name);
	printf("  mov rax, rbp\n");
	printf("  sub rax, %d\n", *offset);
	printf("  push rax\n");
}

// アセンブラを用いたスタックマシンの生成
void gen(Node *node) {
	if (node->ty == ND_NUM) {
		printf("  push %d\n", node->val);
		return;
	}

	if (node->ty == ND_IDENT) {
		gen_lval(node);
		printf("  pop rax\n");
		printf("  mov rax, [rax]\n");
		printf("  push rax\n");
		return;
	}

	if (node->ty == '=') {
		gen_lval(node->lhs);
		gen(node->rhs);

		printf("  pop rdi\n");
		printf("  pop rax\n");
		printf("  mov [rax], rdi\n");
		printf("  push rdi\n");
		return;
	}

	if (node->ty == ND_RETURN) {
		gen(node->expr);
		printf("  pop rax\n");
		printf("  mov rsp, rbp\n");
		printf("  pop rbp\n");
		printf("  ret\n");
		return;
	}

	if (node->ty == ND_IF) {
		gen(node->cond);
		printf("  pop rax\n");
		printf("  cmp rax, 0\n");
		if (node->els == NULL) {
			printf("  je  .Lend%d\n", l_num++);
			gen(node->then);
			printf(".Lend%d:\n", --l_num);
		}
		else {
			printf("  je  .Lelse%d\n", l_num);
			gen(node->then);
			printf("  jmp  .Lend%d\n", l_num);
			printf(".Lelse%d:\n", l_num++);
			gen(node->els);
			printf(".Lend%d:\n",--l_num);
		}
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
			printf("  imul rdi\n"); // 符号付き乗算
			break;
		case '/':
			printf("  cqo\n"); // raxの64bitの値を128bitに伸ばしてrdx,raxにセット
			printf("  idiv rdi\n");
			break;
		default:
			error("パースしたけどgenerateする命令が無いよ");
	}

	// 演算結果はスタックに格納
	printf("  push rax\n");
	return;
}
