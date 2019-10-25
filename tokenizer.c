#include "9cc.h"

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


