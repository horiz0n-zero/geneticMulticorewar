/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_unset_arena.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 10:03:07 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/06 11:36:20 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_unset_arena.h"

void							libcorewar_unset_arena(struct s_libcorewar_arena *const arena)
{
	int							index;
	struct s_libcorewar_process	*procs;

	if (arena)
	{
		index = 0;
		while (index < arena->champions_count)
		{
			libcorewar_unset_asm_file(arena->champions[index].file);
			++index;
		}
		procs = arena->procs;
		while (procs)
			procs = libcorewar_unset_process(procs);
		free(arena);
	}
}
