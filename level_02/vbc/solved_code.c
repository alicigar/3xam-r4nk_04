#include <stdio.h>
#include <stdlib.h> //la biblioteca es stdlib.h en vez de malloc.h
#include <ctype.h>

typedef struct node { //ok
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n) //ok
{
    node *ret = calloc(1, sizeof(node)); //en vez de sizeof(n) -> sizeof(node);
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n) //ok
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n"); //en vez de EOF, end of input en el mensaje
}

int accept(char **s, char c)
{
    if (**s == c) //añadir == c
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c) //ok
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

// se añaden 2 funciones y se completa parse_expr, y 2 funciones de 3 son iguales

node	*parse_expr(char **s); //se declaran aquí 3 funciones, esta para +
node	*parse_factor(char **s); //esta para números y signos
node	*parse_term(char **s); // esta para *

node    *parse_expr(char **s) //ahora es char **s no char *s
{
    node *left = parse_term(s); //se mete la de mayor prioridad
    if (!left) //si falla left
    	return (NULL); //
    while (accept(s, '+'))
    {
    	node *right = parse_term(s);
    	if (!right)
    	{
    		destroy_tree(left);
    		return (NULL);
    	}
    	node n = {ADD, 0 , left, right};
    	left = new_node(n);
    	if (!left)
    	{
    		destroy_tree(right);
    		return (NULL);
    	}
    }
    return (left);
}

node	*parse_factor(char **s) //new
{
	if (isdigit(**s))
	{
		node n = {VAL, **s - '0', NULL, NULL};
		(*s)++;
		return (new_node(n));
	}
	if (accept(s, '('))
	{
		node *n = parse_expr(s);
		if (!expect(s, ')'))
		{
			destroy_tree(n);
			return (NULL);
		}
		return (n);
	}
	unexpected(**s);
	return (NULL);
}

node	*parse_term(char **s) //new
{
	node *left = parse_factor(s);
	if (!left)
	    return (NULL);
	while (accept(s, '*'))
	{
	    node *right = parse_factor(s);
	    if (!right)
	    {
	    	destroy_tree(left);
	    	return (NULL);
	    }
	    node n = {MULTI, 0, left, right};
	    left = new_node(n);
	    if (!left)
	    {
	    	destroy_tree(right);
	    	return (NULL);
	    }
	}
	return (left);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return (0); //se añade
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    char *s = argv[1]; //añadir esta linea
    node *tree = parse_expr(&s); //cambiar parse_expr(argv[1]) por parse_expr(&s);
    if (!tree)
        return (1);
    if(*s) //se añade esta condición
    {
    	destroy_tree(tree); //
    	unexpected(*s); //
    	return (1); //
    } //
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return (0); //se añade
}
