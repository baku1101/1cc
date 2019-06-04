#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "1cc.h"

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt); // 可変長引数の受け取りをする
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

// エラー箇所を報告するための関数
void error_at(char *loc, char *msg) {
	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, ""); // pos個の空白を出力
	fprintf(stderr, "^ %s\n", msg);
	exit(1);
}
