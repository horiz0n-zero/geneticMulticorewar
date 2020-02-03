/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 11:55:57 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/01 11:36:41 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar.h"

void			*libcorewar_error(char *const ptr, char **const error_ptr, ...)
{
	va_list		args;
	void		*p;

	va_start(args, error_ptr);
	p = va_arg(args, void*);
	while (p)
	{
		free(p);
		p = va_arg(args, void*);
	}
	va_end(args);
	*error_ptr = ft_memcopy(ptr, ft_strlen(ptr));
	return (NULL);
}
