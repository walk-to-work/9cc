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

bool consume(char *op);
void expect( char *op );
int expect_number();
bool at_eof();
Token *new_token( TokenKind kind , Token *cur , char *str , unsigned int len );
Token *tokenize( char *p );


/*** パーサ ***/
typedef enum{
	ND_EQU,
	ND_NEQ,
	ND_LOW,
	ND_LEQ,
	ND_HIG,
	ND_HEQ,
	ND_ADD, // +
	ND_SUB, // -
	ND_MUL, // *
	ND_DIV, // /
	ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;
struct Node{
	NodeKind kind; // ノードの型
	Node *lhs;     // 左辺
	Node *rhs;     // 右辺
	int val;       // kindがND_NUMのときのみ使う
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

/*** コードジェネレータ ***/
void gen(Node *node);

/*** デバッグ関数 ***/
char *KIND_CHECK( int KIND );
void PRINT( char *fmt , ... );
void error_at( char *loc , char *fmt , ... );


/*** グローバル変数 ***/
char *user_input; // 入力プログラム
Token *token;     //現在見ているトークン

#endif
