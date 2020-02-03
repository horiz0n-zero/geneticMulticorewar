/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 12:42:49 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/09 15:10:50 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_arena_cycle.h"

void							cycle_fork(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	struct s_libcorewar_process	*new;
	int							index;

	index = 0;
	while (index < arena->champions_count)
	{
		if (arena->champions[index].id == process->id)
		{
			new = libcorewar_get_process(arena->champions + index);
			new->isalive = process->isalive;
			new->pc = (process->opcode_data.params[0] % IDX_MOD);
			new->cf = process->cf;
			new->next = arena->procs;
			arena->procs = new;
			break ;
		}
		++index;
	}
}

void							cycle_lfork(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	struct s_libcorewar_process	*new;
	int							index;

	index = 0;
	while (index < arena->champions_count)
	{
		if (arena->champions[index].id == process->id)
		{
			new = libcorewar_get_process(arena->champions + index);
			new->isalive = process->isalive;
			new->pc = process->opcode_data.params[0];
			new->cf = process->cf;
			new->next = arena->procs;
			arena->procs = new;
			break ;
		}
		++index;
	}
}
