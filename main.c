#include "9cc.h"

int main( int argc , char **argv ){
	if( argc != 2 ){
		fprintf(stderr , "引数の個数が正しくありません\n");
		return 1;
	}

	// トークナイズしてパースする
	user_input = argv[1];
	token = tokenize(user_input);

	program();

	// アセンブリの前半部分を出力
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// プロローグ
	// 変数26個分の領域を確保する
	printf("  push rbp\n");
	printf("  mov rbp , rsp\n");
	printf("  sub rsp , 208\n"); //208 = 8 * 26

	// 先頭の式から順にコードを生成
	for( int i = 0 ; code[i] ; i++ ){
		gen( code[i] );

		// 式の評価結果としてスタックに一つ値が残っている
		// はずなので，スタックが溢れないようにポップしておく
		printf("  pop rax\n");
	}

	// エピローグ
	// 最後の式の結果がRAXに残っているのでそれが返り値になる
	printf("  mov rsp, rbp\n");
	printf("  pop rbp\n");
	printf("  ret\n");

	return 0;
}
