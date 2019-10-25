#include "9cc.h"

void gen_lval( Node *node ){
	if( node->kind != ND_LVAR )
		error("代入の左辺値が変数ではありません");

	printf("  mov rax, rbp\n");
	printf("  sub rax, %d\n", node->offset);
	printf("  push rax\n");
}

void gen(Node *node){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	PRINT("kind : %s\n" , KIND_CHECK(node->kind ));

	switch ( node->kind ){
		case ND_NUM:
			printf("  push %d\n" , node->val );
			return;
		case ND_LVAR:
			printf("  pop rax\n");
			printf("  mov rax, [rax]\n");
			printf("  push rax\n");
			return;
		case ND_ASSIGN:
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


