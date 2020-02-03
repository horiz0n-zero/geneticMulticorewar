/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 08:43:34 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/09 15:14:55 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_process.h"

struct s_libcorewar_process			*libcorewar_get_process(struct s_libcorewar_champion *const champion)
{
	struct s_libcorewar_process		*proc;

	if ((proc = ft_memalloc(sizeof(struct s_libcorewar_process))))
	{
		proc->id = champion->id;
		proc->r[0] = champion->id;
	}
	return (proc);
}


struct s_libcorewar_process			*libcorewar_unset_process(struct s_libcorewar_process *const process)
{
	void							*tmp;

	if (process)
	{
		tmp = process->next;
		free(process);
		return (tmp);
	}
	return (NULL);
}





