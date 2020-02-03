/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 09:05:54 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/14 09:51:05 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft_printf_internal.h"

static const char	g_p_base[] = "0123456789abcdef";

size_t				precalculate_p(struct s_printformat *const printformat, struct s_percent *const percent)
{
	return ((percent->r1 = ft_printf_unumber_length(percent->data, 16)) + 2);
}

char				*transform_p(char *dst, struct s_percent *const percent)
{
	*dst++ = '0';
	*dst++ = 'x';
	dst += percent->r1;
	ft_printf_unumber_transform(percent->data, 16, g_p_base, dst);
	return (dst);
}
