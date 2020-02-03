/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_arena_cycle_check.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 15:35:00 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/09 14:53:31 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_arena_cycle_check.h"

static void						arena_cycle_check_process(struct s_libcorewar_arena *const arena)
{
	struct s_libcorewar_process	*procs;
	struct s_libcorewar_process	*last_procs;

	procs = arena->procs;
	last_procs = NULL;
	while (procs)
	{
		if (!procs->isalive)
			if (!last_procs)
				arena->procs = libcorewar_unset_process(last_procs);
			else
				last_procs->next = libcorewar_unset_process(procs);
		else
			procs->isalive = 0;
		last_procs = procs;
		procs = procs->next;
	}
}

int								libcorewar_arena_cycle_check(struct s_libcorewar_arena *const arena)
{

	++arena->cycles_all;
	++arena->cycles;
	if (arena->cycles >= arena->cycle_to_die)
	{
		arena->cycles = 0;
		arena_cycle_check_process(arena);
		if (++arena->checks_count == MAX_CHECKS || arena->lives_count >= NBR_LIVE)
		{
			arena->cycle_to_die -= CYCLE_DELTA;
			arena->checks_count = 0;
		}
		arena->lives_count = 0;
	}
	if (!arena->procs)
		return (0);
	return (1);
}
