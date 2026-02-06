/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:36:12 by miguel-f          #+#    #+#             */
/*   Updated: 2026/02/06 13:21:37 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* ========== GIVEN CODE ========== */

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
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
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
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

/* ========== FIN GIVEN CODE ========== */

/* Declaraciones de funciones para el parser */
node	*SUM(char **s);
node	*MUL(char **s);
node	*PAR(char **s);

/*
** PAR - Factor/Parentesis: Parsea el nivel más bajo de precedencia
** 
** Esta función maneja:
** - Números individuales (0-9)
** - Expresiones entre paréntesis (recursivamente llama a SUM)
** 
** Es la base del parser recursivo descendente. Procesa los elementos
** atómicos de la expresión.
*/
node	*PAR(char **s)
{
    node    *ret;

    if (isdigit(**s))
    {
        ret = new_node((node){.type = VAL, .val = **s - '0'});
        (*s)++;
        return (ret);
    }
    if (accept(s, '('))
    {
        ret = SUM(s);
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

/*
** MUL - Term/Multiplicación: Parsea operaciones de multiplicación
** 
** Esta función maneja:
** - Secuencias de multiplicaciones (ej: 3*4*5)
** - Asociatividad izquierda (3*4*5 se evalúa como (3*4)*5)
** 
** Tiene mayor precedencia que la suma. Llama a PAR para obtener
** los operandos y construye un árbol binario para cada '*' encontrado.
*/
node    *MUL(char **s)
{
    node    *ret = PAR(s);
    node    *rhs;

    while (ret && accept(s, '*'))
    {
        if (!(rhs = PAR(s)))
        {
            destroy_tree(ret);
            return (NULL);
        }
        if (!(ret = new_node((node){.type = MULTI, .l = ret, .r = rhs})))
        {
            destroy_tree(rhs);
            return (NULL);
        }
    }
    return (ret);
}

/*
** SUM - Expression/Suma: Parsea operaciones de suma
** 
** Esta función maneja:
** - Secuencias de sumas (ej: 1+2+3)
** - Asociatividad izquierda (1+2+3 se evalúa como (1+2)+3)
** 
** Tiene la menor precedencia. Llama a MUL para obtener los términos
** y construye un árbol binario para cada '+' encontrado.
** Es el punto de entrada principal del parser.
*/
node    *SUM(char **s)
{
    node    *ret = MUL(s);
    node    *rhs;

    while (ret && accept(s, '+'))
    {
        if (!(rhs = MUL(s)))
        {
            destroy_tree(ret);
            return (NULL);
        }
        if (!(ret = new_node((node){.type = ADD, .l = ret, .r = rhs})))
        {
            destroy_tree(rhs);
            return (NULL);
        }
    }
    return (ret);
}

/* ========== GIVEN CODE (con modificación) ========== */

node    *parse_expr(char *s)
{
    node	*ret = SUM(&s);

    if (ret && *s) 
    {
		unexpected(*s);
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
	return (0);
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
	return(0);
}