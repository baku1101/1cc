// user_inputが指している文字列を
// トークンに分割してtokensに保存する
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "1cc.h"

Vector *tokens;

int is_alnum(char c) {
	return  ('a' <= c && c <= 'z') ||
			('A' <= c && c <= 'Z') ||
			('1' <= c && c <= '9') ||
			(c == '_');
}

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

		if (*p == '>' || *p == '<' || *p == '=') {
			token->ty = *p;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p++;
			continue;
		}

		if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
			token->ty = TK_RETURN;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p += 6;
			continue;
		}

		if (strncmp(p, "if", strlen("if")) == 0 && !is_alnum(p[strlen("if")])) {
			token->ty = TK_IF;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p += strlen("if");
			continue;
		}

		if (strncmp(p, "else", strlen("else")) == 0 && !is_alnum(p[strlen("else")])) {
			token->ty = TK_ELSE;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p += strlen("else");
			continue;
		}

		if (strncmp(p, "for", strlen("for")) == 0 && !is_alnum(p[strlen("for")])) {
			token->ty = TK_FOR;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p += strlen("for");
			continue;
		}

		if (strncmp(p, "while", strlen("while")) == 0 && !is_alnum(p[strlen("while")])) {
			token->ty = TK_WHILE;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p += strlen("while");
			continue;
		}

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/'
			|| *p == '(' || *p == ')' || *p == ';' || *p == '{'
			|| *p == '}' || *p == ',') {
			token->ty = *p;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p++;
			continue;
		}

		if ('a' <= *p && *p <= 'z') {
			int len;
			for (len = 0;; len++) {
				if (!is_alnum(p[len])) {
					break;
				}
			}
			char *name = malloc(sizeof(char));
			memcpy(name, p, len);
			token->ty = TK_IDENT;
			token->name = name;
			token->input = p;
			vec_push(tokens, token);
			i++;
			p += len;
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
