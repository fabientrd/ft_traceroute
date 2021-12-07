/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_swap.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ftreand <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/12/01 18:44:22 by ftreand      #+#   ##    ##    #+#       */
/*   Updated: 2017/12/01 18:46:24 by ftreand     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_swap(int *a, int *b)
{
	int *tmp;

	tmp = a;
	a = b;
	b = tmp;
}