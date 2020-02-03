/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 09:16:30 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/12 13:58:12 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_arena_cycle.h"

void	cycle_add(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	process->r[process->opcode_data.params[2]] = process->r[process->opcode_data.params[0]] + process->r[process->opcode_data.params[1]];
}

void	cycle_sub(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	process->r[process->opcode_data.params[2]] = process->r[process->opcode_data.params[0]] - process->r[process->opcode_data.params[1]];
}

void	cycle_and(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	process->r[process->opcode_data.params[2]] = process->r[process->opcode_data.params[0]] & process->r[process->opcode_data.params[1]];
}

void	cycle_or(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	process->r[process->opcode_data.params[2]] = process->r[process->opcode_data.params[0]] | process->r[process->opcode_data.params[1]];
}

void	cycle_xor(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	process->r[process->opcode_data.params[2]] = process->r[process->opcode_data.params[0]] ^ process->r[process->opcode_data.params[1]];
}
