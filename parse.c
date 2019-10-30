#include "9cc.h"

bool  var_flag = 0;

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

Node *assign(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = equality();
	if ( consume("=") ){
		node = new_node( ND_ASSIGN , node , assign() );
	}
	return node;
}

Node *expr(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	return assign();
}

Node *stmt(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	Node *node = expr();
	expect(";");
	return node;
}

void program(){
	PRINT("do: %s str:%s \n" ,  __FUNCTION__ , token->str );
	int i = 0;
	while ( !at_eof() ){
		code[i] = stmt();
		PRINT("kind : %s\n" , KIND_CHECK(code[i]->kind ));
		i++;
	}
	code[i] = NULL;
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
		if( consume("<=") )
			node = new_node( ND_LEQ , node , add() );
		else if( consume(">=") )
			node = new_node( ND_HEQ , node , add() );
		else if( consume(">") )
			node = new_node( ND_HIG , node , add() );
		else if( consume("<") )
			node = new_node( ND_LOW , node , add() );
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

	if( token->kind == TK_NUM )
		return new_node_num(expect_number());

	Token *tok = consume_ident();
	if(tok){
		Node *node = calloc( 1 , sizeof(Node) );
		node->kind = ND_LVAR;

		LVar *lvar = find_lvar(tok);
		if( lvar != NULL ){
			PRINT("known var\n");
			node->offset = lvar->offset;
		}
		else {
			PRINT("new var\n");

			lvar = calloc( 1 , sizeof( LVar ) );
			lvar->next = locals;
			lvar->name = tok->str;
			lvar->len  = tok->len;

			if( var_flag ){
				lvar->offset = locals->offset + 8;
				PRINT("not first\n");
			}
			else{
				lvar->offset = 0;
				var_flag = 1;
				PRINT("first\n");
			}
			node->offset = lvar->offset;
			locals = lvar;
			return node;
		}
	}
}
