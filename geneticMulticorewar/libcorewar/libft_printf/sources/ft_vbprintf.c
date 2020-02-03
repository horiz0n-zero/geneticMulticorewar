/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vbprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 15:17:06 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/22 15:19:17 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft_printf_internal.h"

int											ft_vbprintf(int (*function)(const char *const, const size_t), const char *const format, va_list *const args)
{
	static struct s_printformat				printformat = (struct s_printformat){
		.format = NULL,
		.args = NULL,
		.percents = NULL,
		.length = 0,
		.ret = 0,
		.r1 = 0,
		.r2 = 0,
		.function = NULL
	};

	printformat.length = 0;
	printformat.format = format;
	printformat.args = args;
	printformat.function = function;
	ft_printf_core(&printformat, format, &printformat.percents);
	return (printformat.ret);
}
