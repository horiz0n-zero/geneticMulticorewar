/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_array_loop_void.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 09:26:59 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/23 13:46:52 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf_array_loop_void.h"

size_t				ft_printf_array_loop_void_count(const char *src)
{
	return (0);
}

const char			*ft_printf_array_loop_void(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const char *src)
{
	*next = NULL;
	ft_printf_core(printformat, array->format, &array->next);
	return (NULL);
}
