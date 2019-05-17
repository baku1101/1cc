// 引数で与えられた数をreturnする
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "引数の個数が正しくない\n");
		return 1;
	}

	char *p = argv[1];

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");
	printf("  mov rax, %1d\n", strtol(p, &p, 10));
	// strtol(変換文字列, 変換不可能部分の格納アドレス, 基数)
	// 返り値は変換できた先頭の数値
	// 変換できないのが続いた時は0が返る
	while (*p) {
		if (*p == '+') {
			p++;
			printf("  add rax, %1d\n", strtol(p, &p, 10));
			continue;
		}
		if (*p == '-') {
			p++;
			printf("  sub rax, %1d\n", strtol(p, &p, 10));
			continue;
		}

		fprintf(stderr, "予期しない文字: '%c'\n", *p);
		return 1;
	}
	printf("  ret\n");
	return 0;
}
