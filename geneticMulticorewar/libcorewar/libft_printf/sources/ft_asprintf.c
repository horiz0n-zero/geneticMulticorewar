/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_asprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 10:09:33 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/22 14:55:37 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft_printf_internal.h"

static int						ft_asprintf_function(const char *const formatted, const size_t length);

static struct s_printformat		printformat =
{
	.format = NULL,
	.args = NULL,
	.percents = NULL,
	.length = 0,
	.r1 = 0,
	.r2 = 0,
	.function = ft_asprintf_function
};

static char						**g_ret = NULL;

static int						ft_asprintf_function(const char *const formatted, const size_t length)
{
	char *const					alloc = malloc(length + 1);
	size_t						index;

	if (!alloc)
	{
		*g_ret = NULL;
		return (0);
	}
	else
		*g_ret = alloc;
	index = 0;
	while (index < length)
	{
		alloc[index] = formatted[index];
		++index;
	}
	alloc[index] = 0;
	return (int)length;
}

int								ft_asprintf(char **const ret, const char *const format, ...)
{
	va_list						args;

	g_ret = ret;
	va_start(args, format);
	printformat.length = 0;
	printformat.format = format;
	printformat.args = &args;
	ft_printf_core(&printformat, format, &printformat.percents);
	va_end(args);
	return (printformat.ret);
}





