#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "1cc.h"

char *user_input;

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
	// 結果はcode[]に保存される
	user_input = argv[1];
	tokenize();
	program();

	// アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// プロローグ
	// 変数26個分の領域を確保する
	printf("  push rbp\n");
	printf("  mov rbp, rsp\n");
	printf("  sub rsp, 208\n");

	// 先頭の式から順にコード生成
	for (int i = 0;code->data[i]; i++) {
		gen(code->data[i]);

		// 式の評価結果としてスタックに一つの値が残っている
		// はずなので，スタックが溢れないようにポップしておく
		printf("  pop rax\n");
	}

	// エピローグ
	// 最後の式の結果がRAXに残っているのでそれが返り値になる
	printf("  mov rsp, rbp\n");
	printf("  pop rbp\n");
	printf("  ret\n");

	return 0;
}
