/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_src_number.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 14:25:40 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/22 09:48:25 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_get_src_file.h"

unsigned int			high_tone(unsigned int n)
{
	unsigned int		dis;

	dis = 0;
	while (n)
	{
		dis = __builtin_ffs(n);
		n = ~(1 << (dis - 1)) & n;
	}
	return (dis);
}

int						multi_issafe(int lhs, int rhs)
{
	return ((high_tone((unsigned int)lhs) + high_tone((unsigned int)rhs)) <= 32);
}

int						add_issafe(int lhs, int rhs)
{
	return (high_tone((unsigned int)lhs) < 32 && high_tone((unsigned int)rhs) < 32);
}

int						ft_src_unumber(struct s_libcorewar_src_file *const file, const char *src, const int *const minmax, char **const error)
{
	int					r;

	r = 0;
	while (*src >= '0' && *src <= '9')
	{
		if (!multi_issafe(r, 10))
		{
			oe(file, (char*)src, 16, error, minmax[0], minmax[1]);
			return (r);
		}
		r *= 10;
		if (!add_issafe(r, (int)*src - '0'))
		{
			oe(file, (char*)src, 17, error, minmax[0], minmax[1]);
			return (r);
		}
		r += (int)*src++ - '0';
	}
	if (!(r >= minmax[0] && r <= minmax[1]))
		oe(file, (char*)src, 18, error, minmax[0], minmax[1]);
	return (r);
}

int						ft_src_number(struct s_libcorewar_src_file *const file, const char *src, const int *const minmax, char **const error)
{
	int					r;

	r = 0;
	while (*src >= '0' && *src <= '9')
	{
		if (!multi_issafe(r, 10))
		{
			oe(file, (char*)src, 16, error, minmax[0], minmax[1]);
			return (r);
		}
		r *= 10;
		if (!add_issafe(r, (int)*src - '0'))
		{
			oe(file, (char*)src, 17, error, minmax[0], minmax[1]);
			return (r);
		}
		r += (int)*src++ - '0';
	}
	r *= -1;
	if (!(r >= minmax[0] && r <= minmax[1]))
		oe(file, (char*)src, 18, error, minmax[0], minmax[1]);
	return (r);
}
