#include <ctype.h>
#include <stdlib.h>

#include "1cc.h"
// user_inputが指している文字列を
// トークンに分割してtokensに保存する

Vector *tokens;


void tokenize() {
	tokens = new_vector();
	char *p = user_input;

	int i = 0;
	while (*p) {
		Token *token = malloc(sizeof(Token));
		// 空白文字をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
			token->ty = *p;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p++;
			continue;
		}

		if (*p == '!' && *(p+1) == '=') {
			token->ty = TK_NE;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p+=2;
			continue;
		}

		if (*p == '=' && *(p+1) == '=') {
			token->ty = TK_EQ;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p+=2;
			continue;
		}

		if (*p == '<' && *(p+1) == '=') {
			token->ty = TK_LE;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p+=2;
			continue;
		}

		if (*p == '>' && *(p+1) == '=') {
			token->ty = TK_GE;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p+=2;
			continue;
		}

		if (*p == '>' || *p == '<') {
			token->ty = *p;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p++;
			continue;
		}

		if (isdigit(*p)) {
			token->ty = TK_NUM;
			token->input = p;
			token->val = strtol(p, &p, 10);
			vec_push(tokens, token);
			i++;
			continue;
		}

		error_at(p, "トークナイズできません");
	}

	Token *token = malloc(sizeof(Token));
	token->ty = TK_EOF;
	token->input = p;
	vec_push(tokens, token);

}
