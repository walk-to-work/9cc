#include "9cc.h"

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
		case ND_NEQ:
			printf("  cmp rax, rdi\n");
			printf("  setne al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LOW:
			printf("  cmp rax, rdi\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_LEQ:
			printf("  cmp rax, rdi\n");
			printf("  setle al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_HIG:
			printf("  cmp rdi, rax\n");
			printf("  setl al\n");
			printf("  movzb rax, al\n");
			break;
		case ND_HEQ:
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


