#include <ctype.h>
#include <stdlib.h>

#include "cc1.h"
// user_inputが指している文字列を
// トークンに分割してtokensに保存する
Token tokens[100];

void tokenize() {
	char *p = user_input;

	int i = 0;
	while (*p) {
		// 空白文字をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}

		if (isdigit(*p)) {
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p, &p, 10);
			i++;
			continue;
		}

		error_at(p, "トークナイズできません");
	}

	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
}
