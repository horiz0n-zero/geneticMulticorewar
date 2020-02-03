/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_array_loop_long.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 16:37:39 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/08 10:20:08 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf_array_loop_long.h"

size_t					ft_printf_array_loop_long_count(const long *src)
{
	const long *const	start = src;

	while (*src)
		++src;
	return ((size_t)(src - start));
}

const char				*ft_printf_array_loop_long(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const long *src)
{
	struct s_percent	percent;

	if (array->r3)
	{
		*next = &percent;
		percent.flags = array->flags & ~(FLAGS_ISARRAY);
		percent.width = array->width;
		percent.precision = array->precision;
		percent.size = array->size;
		percent.data = (unsigned long)*src;
		percent.conversion = array->conversion;
		percent.format = NULL;
		percent.array = NULL;
		printformat->length += percent.conversion->precalculate(printformat, &percent);
		if (--array->r3)
		{
			if (array->r2)
				printformat->length += array->r2;
			return (ft_printf_array_loop_long(printformat, array, &percent.next, ++src));
		}
		else
			percent.next = NULL;
	}
	else
		*next = NULL;
	ft_printf_core(printformat, array->format, &array->next);
	return (NULL);
}
