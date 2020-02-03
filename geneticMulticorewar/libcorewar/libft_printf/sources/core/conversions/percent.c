/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 08:55:18 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/23 09:01:20 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft_printf_internal.h"

size_t				precalculate_percent(struct s_printformat *const printformat, struct s_percent *const percent)
{
	return (1);
}

char				*transform_percent(char *dst, struct s_percent *const percent)
{
	*dst++ = '%';
	return (dst);
}
