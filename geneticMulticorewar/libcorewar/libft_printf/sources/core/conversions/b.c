/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 09:05:29 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/08 11:50:46 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft_printf_internal.h"

size_t			precalculate_b(struct s_printformat *const printformat, struct s_percent *const percent)
{
	return ((percent->r1 = ft_printf_unumber_length(percent->data, 2)));
}

char			*transform_b(char *dst, struct s_percent *const percent)
{
	dst += percent->r1;
	ft_printf_unumber_transform(percent->data, 2, "01", dst);
	return (dst);
}
