/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 09:59:17 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/22 14:55:26 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft_printf_internal.h"

static int						ft_dprintf_function(const char *const formatted, const size_t length);

static struct s_printformat		printformat =
{
	.format = NULL,
	.args = NULL,
	.percents = NULL,
	.length = 0,
	.ret = 0,
	.r1 = 0,
	.r2 = 0,
	.function = ft_dprintf_function
};

static int						ft_dprintf_function(const char *const formatted, const size_t length)
{
	return (int)write(printformat.r1, formatted, length);
}

int								ft_dprintf(const int fd, const char *const format, ...)
{
	va_list						args;

	va_start(args, format);
	printformat.length = 0;
	printformat.format = format;
	printformat.args = &args;
	printformat.r1 = fd;
	ft_printf_core(&printformat, format, &printformat.percents);
	va_end(args);
	return (printformat.ret);
}
