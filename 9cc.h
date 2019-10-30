#ifndef NINE_CC_H

#define NINE_CC_H

#define DEBUG 0

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*** トークナイザ ***/
typedef enum{
	TK_RESERVED,  //記号
	TK_IDENT   ,  //識別子
	TK_NUM,       //整数トークン
	TK_EOF,       //入力終わりを表すトークン
	TK_ERR,       //エラー処理用
} TokenKind;

typedef struct Token Token;
struct Token{
	TokenKind kind; //Tokenの型
	Token *next;    //次の入力トークン
	int val;        //kindがTK_NUMの場合，その数値
	char *str;      //トークン文字列
	int len;        //トークンの長さ
};

typedef struct LVar LVar;

//ローカル変数の型
struct LVar{
	LVar *next;
	char *name;
	int   len;
	int   offset;
};

bool consume(char *op);
Token *consume_ident();
void expect( char *op );
int expect_number();
bool at_eof();
Token *new_token( TokenKind kind , Token *cur , char *str , unsigned int len );
Token *tokenize( char *p );
LVar *find_lvar( Token *tok );

/*** パーサ ***/
typedef enum{
	ND_EQU,    // ==
	ND_NEQ,    // !=
	ND_LOW,    // <
	ND_LEQ,    // <=
	ND_HIG,    // >
	ND_HEQ,    // >=
	ND_ADD,    // +
	ND_SUB,    // -
	ND_MUL,    // *
	ND_DIV,    // /
	ND_NUM,    // 整数
	ND_LVAR,   // ローカル変数
	ND_ASSIGN, // アサイン
} NodeKind; 

typedef struct Node Node;

struct Node{
	NodeKind kind; // ノードの型
	Node *lhs;     // 左辺
	Node *rhs;     // 右辺
	int val;       // kindがND_NUMのときのみ使う
	int offset;    // lindがLVARの場合のみ使う
};

Node *new_node( NodeKind kind , Node *lhs , Node *rhs );
Node *new_node_num(int val);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *unary();
Node *mul();
Node *primary();
Node *assign();
Node *stmt();
void program();

/*** コードジェネレータ ***/
void gen(Node *node);

/*** デバッグ関数 ***/
char *KIND_CHECK( int KIND );
void PRINT( char *fmt , ... );
void error_at( char *loc , char *fmt , ... );
void error(char *fmt, ...);

/*** グローバル変数 ***/
char *user_input; // 入力プログラム
Token *token;     // 現在見ているトークン
Node *code[100];  // `;`で区切られたコードごとのノード
LVar *locals;

#endif
