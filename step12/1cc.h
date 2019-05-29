#ifndef CC1_H
#define CC1_H

//============================================================================================
// container
/// vector
typedef struct {
	void **data;
	int capacity;
	int len;
} Vector;  // 可変長ベクタ

Vector *new_vector();
void vec_push(Vector *vec, void *elem);

/// map
typedef struct {
	Vector *keys;
	Vector *vals;
} Map;

Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);


//============================================================================================
// tokenizer
// トークンの型を表す値
enum {
	TK_NUM = 256,	// 整数トークン(256から始めるのはcharの範囲と被らないため)
	TK_IDENT,		// 識別子
	TK_EQ,			// == (equal)
	TK_NE,			// != (not equal)
	TK_LE,			// <= (less than or equal)
	TK_GE,			// >= (greater than or equal)
	TK_RETURN,		// returnトークン
	TK_IF,			// ifトークン
	TK_ELSE,		// elseトークン
	TK_FOR,			// forトークン
	TK_WHILE,		// whileトークン
	TK_EOF,			// 入力の終わりを表すトークン
};

// トークンの型
typedef struct {
	int ty;			// トークンの型
	int val;		// tyがTK_NUMの場合、その数値
	char *name;		// tyがTK_IDENTの場合，変数名
	char *input;	// トークン文字列（エラーメッセージ用）
} Token;

// 入力プログラム
extern char *user_input;

// トークナイズした結果のトークン列
extern Vector *tokens;

void tokenize();

//============================================================================================
// parser
enum {
	ND_NUM = 256,	// 整数のノードの型
	ND_IDENT, 		// 識別子のノードの型
	ND_EQ,			// == (equal)
	ND_NE,			// != (not equal)
	ND_LE,			// <= (less than or equal)
	ND_RETURN,  	// returnノード
};

// 抽象構文木のノードの型
typedef struct Node {
	int ty;				// 演算子かND_NUM
	struct Node *lhs;	// 左辺(left-hand side)
	struct Node *rhs;	// 右辺(right-hand side)
	int val;			// tyがND_NUMの場合に値が入る
	char *name;			// tyがND_IDENTの場合に変数名が入る
} Node;

// ノード作成(2項演算子用)
Node *new_node(int ty, Node *lhs, Node *rhs);
// ノード作成(数値用)
Node *new_node_num(int val);
// 次のトークンが期待した型かどうかをチェックする
int consume(int ty);
// 変数を登録する
void register_val(char *name);

// パーサ本体
/* 文法
	program 	= stmt*
	stmt    	= expr ";"
				| "if" "(" expr ")" stmt ("else" stmt)?
				| "while" "(" expr ")" stmt
				| "for" "(" expr? ";" expr? ";" expr? ")" stmt
				| "return" expr ";"
	expr       	= assign
	assign     	= equality ("=" assign)?
	equality   	= relational ("==" relational | "!=" relational)*
	relational 	= add ("<" add | "<=" add | ">" add | ">=" add)*
	add        	= mul ("+" mul | "-" mul)*
	mul        	= unary ("*" unary | "/" unary)*
	unary      	= ("+" | "-")? term
	term       	= num | ident | "(" expr ")"
*/

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

// ノードの配列
extern Vector *code;
// 変数を格納するmap, <変数名，スタックのoffset>
extern Map *valmap;
// 変数の数を格納(その分rspを下げる)
extern int numval;

//============================================================================================
// generate
void gen(Node *node);


//============================================================================================
// error
// エラーを報告するための関数, printfと同じ引数を取る
void error(char *fmt, ...);
// エラー箇所を報告するための関数
void error_at(char *loc, char *msg);

//============================================================================================
// test

void test_vec();
void test_map();
void expect(int line, int expected, int actual);
void runtest();

#endif
