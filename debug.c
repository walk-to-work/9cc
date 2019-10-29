#include "9cc.h"

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
		case ND_ASSIGN : 
			return "ND_ASSIGN";
		case ND_LVAR:
			return "ND_LVAR";
		default:
			return "unsupported kind";
	}
}

void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

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

#if DEBUG

void PRINT( char *fmt , ... ){
	va_list arg;
	va_start( arg , fmt );
	vprintf( fmt , arg);
}
#else

void PRINT( char *fmt , ... ){}

#endif


