#include <stdio.h>
#include <stdlib.h> //cambiar malloc.h por stdlib.h
#include <ctype.h>

typedef struct node { //no changes
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n) //no changes
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n) //no changes
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
        printf("Unexpected end of input\n"); //swap in the message file for input
}

int accept(char **s, char c)
{
    if (**s == c) //add == c
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node	*add(char **s); //add this function
node	*multi(char **s); //add this function
node	*val(char **s); //add this function

node	*add(char **s)
{
	node	*ret = multi(s);
	node	*right;
	
	while (ret && accept(s, '+'))
	{
		if (!(right = multi(s)))
		{
			destroy_tree(ret);
			return (NULL);
		}
		if (!(ret = new_node((node){.type = ADD, .l = ret, .r = right})))
		{
			destroy_tree(right);
			return (NULL);
		}
	}
	return (ret);
}

node	*multi(char **s)
{
	node	*ret = val(s);
	node	*right;
	
	while (ret && accept(s, '*'))
	{
		if (!(right = val(s)))
		{
			destroy_tree(ret);
			return (NULL);
		}
		if (!(ret = new_node((node){.type = MULTI, .l = ret, .r = right})))
		{
			destroy_tree(right);
			return (NULL);
		}
	}
	return (ret);
}

node	*val(char **s)
{
	node	*ret;
	
	if (isdigit(**s))
	{
		ret = new_node((node){.type = VAL, .val = **s - '0'});
		(*s)++;
		return (ret);
	}
	if (accept(s, '('))
	{
		ret = add(s);
		if (!ret || !expect(s, ')'))
		{
			destroy_tree(ret);
			return (NULL);
		}
		return (ret);
	}
	unexpected(**s);
	return (NULL);
}

node    *parse_expr(char *s)
{
    node	*ret = add(&s); //add this line

    if (ret && *s) //add ret &&
    {
    	unexpected(*s); //add this line
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
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
    return (0); //add this line
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return (0); //add this line
}
