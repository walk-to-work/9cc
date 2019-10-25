#include "9cc.h"

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

	return new_node_num(expect_number());
}


