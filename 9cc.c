#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEBUG 0

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

//現在見ているトークン
Token *token;


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

#if DEBUG

void PRINT( char *fmt , ... ){
	va_list arg;
	va_start( arg , fmt );
	vprintf( fmt , arg);
}

char *KIND_CHECK( int KIND ){
	switch(KIND){
		case ND_HEQ : 
			return "ND_HEQ";
		case ND_SUB :
			return "ND_SUB";
		case ND_NEQ :
			return "ND_NEQ";
		case ND_EQU :
			return "ND_EQU";
		case ND_ADD :
			return "ND_ADD";
		case ND_NUM :
			return "ND_NUM";
		case ND_DIV :
			return "ND_DIV";
		case ND_HIG : 
			return "ND_HIG";
		case ND_LEQ :
			return "ND_LEQ";
		case ND_LOW :
			return "ND_LOW";
		case ND_MUL :
			return "ND_MUL";
		default:
			return "unsupported kind";
	}
}

#else

void PRINT( char *fmt , ... ){}
char *KIND_CHECK( int king){}

#endif

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

bool consume(char *op);
void expect( char *op );
int expect_number();

Node *new_node( NodeKind kind , Node *lhs , Node *rhs ){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = calloc( 1 , sizeof(Node));
	node->kind = kind;
	node->lhs  = lhs;
	node->rhs  = rhs;
	return node;
}

Node *new_node_num(int val){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = calloc( 1 , sizeof( Node ));
	node->kind = ND_NUM;
	node->val  = val;
	return node;
}

Node *expr(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = equality();
	return node;
}

Node *equality(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );

	Node *node = relational();
	for(;;){
		if( consume("==") )
			node = new_node( ND_EQU , node , relational() );
		else if( consume("!=") )
			node = new_node( ND_NEQ , node , relational() );
		else
			return node;
	}
}

Node *relational(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = add();
	for(;;){
		if( consume("<") )
			node = new_node( ND_LOW , node , add() );
		else if( consume("<=") )
			node = new_node( ND_LEQ , node , add() );
		else if( consume(">") )
			node = new_node( ND_HIG , node , add() );
		else if( consume(">=") )
			node = new_node( ND_HEQ , node , add() );
		else
			return node;
	}
}

Node *add(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = mul();
	for (;;){
		if( consume("+"))
			node = new_node( ND_ADD , node , mul() );
		else if (consume("-"))
			node = new_node( ND_SUB , node , mul() );
		else
			return node;
	}
}

Node *mul(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );

	Node *node = unary();
	for(;;){
		if ( consume("*") )
			node = new_node( ND_MUL , node , unary() );
		else if( consume("/") )
			node = new_node( ND_DIV , node , unary() );
		else 
			return node;
	}
}

Node *unary(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	for(;;){
		if( consume("+") ) return primary() ;
		if( consume("-") ) return new_node( ND_SUB , new_node_num(0) , primary() );
		return primary();
	}
}

Node *primary(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	if( consume("(") ){
		Node *node = expr();
		expect(")");
		return node;
	}

	return new_node_num(expect_number());
}

void gen(Node *node){

	PRINT("kind : %s\n" , KIND_CHECK(node->kind ));

	if( node->kind == ND_NUM ){
		printf("  push %d\n" , node->val );
		return;
	}

	gen( node->lhs );
	gen( node->rhs );

	printf("  pop rdi\n");
	printf("  pop rax\n");

	switch ( node->kind ){
		case ND_ADD:
			printf("  add rax, rdi\n");
			break;
		case ND_SUB:
			printf("  sub rax, rdi\n");
			break;
		case ND_MUL:
			printf("  imul rax, rdi\n");
			break;
		case ND_DIV:
			printf("  cqo\n");
			printf("  idiv rdi\n");
			break;
		case ND_EQU:
			printf("  cmp rax, rdi\n");
			printf("  sete al\n");
			printf("  movzb rax, al\n");
			break;
	}

	printf("  push rax\n");


}


// 入力プログラム
char *user_input;

// エラー箇所を報告する
void error_at( char *loc , char *fmt , ... ){
	va_list ap;
	va_start(ap , fmt);

	int pos = loc - user_input;
	fprintf( stderr , "%s\n" , user_input );
	fprintf( stderr , "%*s" , pos , "" ); //pos個の空白を表示
	fprintf( stderr , "^ ");
	vfprintf( stderr , fmt , ap );
	fprintf( stderr , "\n" );
	exit(1);
}

//次のトークンが期待している記号のときには，トークンを1つ読み進めて
//真を返す．それ以外の場合には偽を返す．
bool consume(char *op){
	if(token->kind != TK_RESERVED
			|| strlen(op) != token->len
			|| memcmp(token->str , op , token->len ))
		return false;
	token = token->next;
	return true;
}

//次のトークンが期待している記号のときには，トークンを1つ読み進める．
//それ以外の場合にはエラーを報告する．
void expect( char *op ){
	if( token->kind != TK_RESERVED 
			|| strlen(op) != token->len
			|| memcmp(token->str , op , token->len ))
		error_at( token->str , "'%s'ではありません" , op );
	token = token->next;
}

//次のトークンが期待している記号のときには，トークンを1つ読み進めてその数値を返す．
//それ以外の場合にはエラーを報告する．
int expect_number(){
	if( token->kind != TK_NUM && token->kind != TK_RESERVED )
		error_at( token->str , "数又は演算子ではありません");
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof(){
	return token->kind == TK_EOF;
}

//新しいトークンを作成してcurに繋げる
Token *new_token( TokenKind kind , Token *cur , char *str , unsigned int len ){
	Token *tok = calloc( 1 , sizeof(Token) );
	tok->kind = kind;
	tok->str =  str;
	tok->len =  len;
	cur->next = tok;
	return tok;
}

//入力文字列pをトークナイズしてそれを返す
Token *tokenize( char *p ){
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while ( *p ){

		// スペース:読み飛ばす
		if(isspace(*p)){
			p++;
		}
		else if( strncmp( p , "==" , 2 ) == 0
				|| strncmp( p , "!=" , 2) == 0
				|| strncmp( p , ">=" , 2 ) == 0
				|| strncmp( p , "<=" , 2 ) == 0
				)
		{
			PRINT("DONE : 2str\n");
			cur = new_token(TK_RESERVED , cur , p , 2 );
			p += 2;
		}
		// + , - , * , / : 符号・比較トークンを接続
		else if( *p == '+' 
				|| *p == '-' 
				|| *p == '*' 
				|| *p == '/' 
				|| *p == '(' 
				|| *p == ')' 
				|| *p == '>'
				|| *p == '<'
			   ){
			PRINT("DONE : 1str\n");
			cur = new_token(TK_RESERVED , cur , p++ , 1 );
		}
		// 数値    : 数トークンを追加
		else if( isdigit(*p) ){
			PRINT("DONE : digit\n");
			cur = new_token(TK_NUM , cur , p , 1 );
			cur->val = strtol(p , &p , 0 );
		}
		else{
			error_at( p , "トークナイズできません");
		}

	}

	new_token( TK_EOF , cur , p , 0);
	return head.next;
}
/*** トークナイザおわり ***/

int main( int argc , char **argv ){
	if( argc != 2 ){
		fprintf(stderr , "引数の個数が正しくありません\n");
		return 1;
	}

	// トークナイズしてパースする
	user_input = argv[1];
	token = tokenize(user_input);
	Node *node = expr();

	// アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// 抽象構文木を下りながらコード生成
	gen( node );

	// スタックトップに式全体の値が残っているはずなので
	// それをRAXにロードして関数からの返り値とする
	printf("  pop rax\n");
	printf("  ret\n");

	return 0;
}
