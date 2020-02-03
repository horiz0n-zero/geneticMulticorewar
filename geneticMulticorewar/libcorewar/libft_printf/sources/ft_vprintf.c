/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 15:06:31 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/22 15:13:04 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft_printf_internal.h"

static int									ft_vprintf_function(const char *const formatted, const size_t length)
{
	return (int)write(STDOUT_FILENO, formatted, length);
}

int											ft_vprintf(const char *const format, va_list *const args)
{
	static struct s_printformat				printformat = (struct s_printformat){
		.format = NULL,
		.args = NULL,
		.percents = NULL,
		.length = 0,
		.ret = 0,
		.r1 = 0,
		.r2 = 0,
		.function = ft_vprintf_function
	};

	printformat.length = 0;
	printformat.format = format;
	printformat.args = args;
	ft_printf_core(&printformat, format, &printformat.percents);
	return (printformat.ret);
}





