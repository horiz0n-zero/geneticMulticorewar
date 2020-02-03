/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 14:51:04 by afeuerst          #+#    #+#             */
/*   Updated: 2019/10/31 09:05:33 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft_printf_internal.h"

size_t				precalculate_c(struct s_printformat *const printformat, struct s_percent *const percent)
{
	return (1);
}

char				*transform_c(char *dst, struct s_percent *const percent)
{
	*dst++ = (char)percent->data;
	return (dst);
}
