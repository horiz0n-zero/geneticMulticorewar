/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 10:41:37 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/25 09:21:57 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_shared.h"

size_t			ft_number_length(size_t n)
{
	size_t		r;

	r = 0;
	if (!n)
		return (1);
	while (n)
	{
		++r;
		n /= 10;
	}
	return (r);
}

size_t			ft_strlen(const char *const src)
{
	size_t		i;

	i = 0;
	while (src[i])
		++i;
	return (i);
}
