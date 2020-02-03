/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_core_process.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 13:38:25 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/03 14:14:38 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf_core_process.h"

static char						*ft_printf_core_process_array(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent *percents, char *ptr)
{
	const size_t				r2 = array->r2;
	const char *const			r1 = (const char*)array->r1;
	size_t						index;

	while (percents)
	{
		ptr = percents->conversion->transform(ptr, percents);
		if ((percents = percents->next) && r2)
		{
			index = 0;
			while (index < r2)
				*ptr++ = *(r1 + index++);
		}
	}
	return (ptr);
}

void							ft_printf_core_process(struct s_printformat *const printformat, const char *format)
{
	register struct s_percent	*percent;
	register char				*ptr;
	char						buffer[printformat->length];

	percent = printformat->percents;
	ptr = buffer;
	while (*format)
	{
		if (*format == '%')
		{
			format = percent->format;
			if (percent->flags & FLAGS_ISARRAY)
				ptr = ft_printf_core_process_array(printformat, percent, percent->array, ptr);
			else
				ptr = percent->conversion->transform(ptr, percent);
			percent = percent->next;
		}
		else
			*ptr++ = *format++;
	}
	printformat->ret = printformat->function(buffer, printformat->length);
}
