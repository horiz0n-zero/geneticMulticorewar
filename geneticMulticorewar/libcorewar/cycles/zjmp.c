/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zjmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 09:25:13 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/09 14:43:26 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_arena_cycle.h"

void	cycle_zjmp(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	if (process->cf)
		process->pc = process->opcode_data.params[0] % IDX_MOD;
}
