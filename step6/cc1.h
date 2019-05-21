#ifndef CC1_H
#define CC1_H

//============================================================================================
// tokenizer
// トークンの型を表す値
enum {
	TK_NUM, 	// 整数トークン
	TK_EQ,		// == (equal)
	TK_NE,		// != (not equal)
	TK_LE,		// <= (less than or equal)
	TK_GE,		// >= (greater than or equal)
	TK_EOF,		// 入力の終わりを表すトークン
};

// トークンの型
typedef struct {
	int ty;      // トークンの型
	int val;     // tyがTK_NUMの場合、その数値
	char *input; // トークン文字列（エラーメッセージ用）
} Token;

// 入力プログラム
char *user_input;

// トークナイズした結果のトークン列はこの配列に保存する
// 100個以上のトークンは来ないものとする
extern Token tokens[100];

void tokenize();

//============================================================================================
// parser
enum {
	ND_NUM,		// 整数のノードの型
	ND_EQ,		// == (equal)
	ND_NE,		// != (not equal)
	ND_LE,		// <= (less than or equal)
};

// 抽象構文木のノードの型
typedef struct Node {
	int ty;				// 演算子かND_NUM
	struct Node *lhs;	// 左辺(left-hand side)
	struct Node *rhs;	// 右辺(right-hand side)
	int val;			// tyがND_NUMの場合に値が入る
} Node;

// ノード作成(2項演算子用)
Node *new_node(int ty, Node *lhs, Node *rhs);
// ノード作成(数値用)
Node *new_node_num(int val);
// 次のトークンが期待した型かどうかをチェックする
int consume(int ty);

// パーサ本体
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

// tokens[]のインデックス
extern int pos;

//============================================================================================
// generate
void gen(Node *node);

//============================================================================================
// error
// エラーを報告するための関数, printfと同じ引数を取る
void error(char *fmt, ...);
// エラー箇所を報告するための関数
void error_at(char *loc, char *msg);

#endif
