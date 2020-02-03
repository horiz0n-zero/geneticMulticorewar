/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 15:20:50 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/18 15:27:57 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar.h"

void			*ft_memalloc(const size_t size)
{
	char *const	ptr = malloc(size);
	size_t		index;

	if (ptr)
	{
		index = 0;
		while (index < size)
			*(ptr + index++) = 0;
	}
	return (ptr);
}
