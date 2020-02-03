/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 09:05:45 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/08 14:10:02 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft_printf_internal.h"

static const char	g_base_i[] = "0123456789";
static const long	g_base_i_length = 10;

size_t				precalculate_i(struct s_printformat *const printformat, struct s_percent *const percent)
{
	return ((percent->r1 = ft_printf_number_length((long)percent->data, g_base_i_length)));
}

char				*transform_i(char *dst, struct s_percent *const percent)
{
	dst += percent->r1;
	ft_printf_number_transform((long)percent->data, g_base_i_length, g_base_i, dst);
	return (dst);
}
