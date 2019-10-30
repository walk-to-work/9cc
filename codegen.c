#include "9cc.h"

void gen_lval( Node *node ){
	if( node->kind != ND_LVAR )
		error("代入の左辺値が変数ではありません");

	printf("  mov rax, rbp\n");
	printf("  sub rax, %d\n", node->offset);
	printf("  push rax\n");
}

void gen(Node *node){
	switch ( node->kind ){
		case ND_NUM:
			PRINT("gen : %s\n" , "NUM" );
			printf("  push %d\n" , node->val );
			return;
		case ND_LVAR:
			PRINT("gen : %s\n" , "LVAR" );
			gen_lval(node);
			printf("  pop rax\n");
			printf("  mov rax, [rax]\n");
			printf("  push rax\n");
			return;
		case ND_ASSIGN:
			PRINT("gen : %s\n" , "ASSIGN" );
			gen_lval( node->lhs );
			gen(node->rhs);

			printf("  pop rdi\n");
			printf("  pop rax\n");
			printf("  mov [rax] , rdi\n");
			printf("  push rdi\n");
			return;
	}

	gen( node->lhs );
	gen( node->rhs );

	printf("  pop rdi\n");
	printf("  pop rax\n");

	switch ( node->kind ){
		case ND_ADD:
			PRINT("gen : %s\n" , "ADD" );
			printf("  add rax, rdi\n");
			break;
		case ND_SUB:
			PRINT("gen : %s\n" , "SUB" );
			printf("  sub rax, rdi\n");
			break;
		case ND_MUL:
			PRINT("gen : %s\n" , "MUL" );
			printf("  imul rax, rdi\n");
			break;
		case ND_DIV:
			PRINT("gen : %s\n" , "DIV" );
			printf("  cqo\n");
			printf("  idiv rdi\n");
			break;
		case ND_EQU:
			PRINT("gen : %s\n" , "EQU" );
			printf("  cmp rax, rdi\n");
			printf("  sete al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_NEQ:
			PRINT("gen : %s\n" , "NEQ" );
			printf("  cmp rax, rdi\n");
			printf("  setne al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LOW:
			PRINT("gen : %s\n" , "LOW" );
			printf("  cmp rax, rdi\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LEQ:
			PRINT("gen : %s\n" , "LEQ" );
			printf("  cmp rax, rdi\n");
			printf("  setle al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_HIG:
			PRINT("gen : %s\n" , "HIG" );
			printf("  cmp rdi, rax\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_HEQ:
			PRINT("gen : %s\n" , "HEQ" );
			printf("  cmp rdi, rax\n");
			printf("  setle al\n");
			printf("  movzb rax, al\n");
			break;
		default:
			printf("unsupported kind %s\n" , KIND_CHECK(node->kind));
			exit(1);
			break;
	}

	printf("  push rax\n");

}


