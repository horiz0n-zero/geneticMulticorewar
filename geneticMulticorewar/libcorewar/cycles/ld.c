/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ld.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 14:07:31 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/09 15:04:38 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_arena_cycle.h"

void	cycle_ld(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	if (!process->opcode_data.params[0])
		process->cf = 1;
	process->r[process->opcode_data.params[1]] = process->opcode_data.params[0];
}
