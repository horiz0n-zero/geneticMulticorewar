/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 09:12:16 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/08 11:48:13 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf_numbers.h"

size_t			ft_printf_unumber_length(unsigned long number, const unsigned long base)
{
	size_t		n;

	if (!number)
		return (1);
	n = 0;
	while (number)
	{
		number /= base;
		++n;
	}
	return (n);
}

void			ft_printf_unumber_transform(unsigned long number, const unsigned long base, const char *const basesrc, char *dst)
{
	if (!number)
	{
		*--dst = '0';
		return ;
	}
	while (number)
	{
		*--dst = *(basesrc + (number % base));
		number /= base;
	}
}

size_t			ft_printf_number_length(long number, const long base)
{
	size_t		n;

	if (!number)
		return (1);
	if (number < 0)
	{
		number = -number;
		n = 1;
	}
	else
		n = 0;
	while (number)
	{
		number /= base;
		++n;
	}
	return (n);
}

void			ft_printf_number_transform(long number, const long base, const char *const basesrc, char *dst)
{
	int			neg;

	if (!number && (*--dst = '0'))
		return ;
	if (number < 0)
	{
		number = -number;
		neg = 1;
	}
	else
		neg = 0;
	while (number)
	{
		*--dst = *(basesrc + (number % base));
		number /= base;
	}
	if (neg)
		*--dst = '-';
}
