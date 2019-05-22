#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "1cc.h"

char *user_input;

// vector test
void expect(int line, int expected, int actual) {
	if (expected == actual) {
		return;
	}
	fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
	exit(1);

}

void runtest() {
	Vector *vec = new_vector();
	expect(__LINE__, 0, vec->len);

	for (int i = 0; i < 100; i++) vec_push(vec, (void *)i);

	expect(__LINE__, 100, vec->len);
	expect(__LINE__, 0, (long)vec->data[0]);
	expect(__LINE__, 50, (long)vec->data[50]);
	expect(__LINE__, 99, (long)vec->data[99]);
	printf("OK\n");
}

int main(int argc, char **argv) {
	if (argc != 2) {
		error("引数の個数が正しくありません");
		return 1;
	}

	if (!strcmp(argv[1],"-test")) {
		runtest();
		return 0;
	}

	// トークナイズしてパースする
	user_input = argv[1];
	tokenize();
	Node *node = expr();

	// アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// 抽象構文木を下りながらコード生成
	gen(node);

	// スタックトップに式全体の値が残っているはずなので
	// それをRAXにロードして関数からの返り値とする
	printf("  pop rax\n");
	printf("  ret\n");

	return 0;
}
