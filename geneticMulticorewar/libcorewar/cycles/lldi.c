/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lldi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:08:41 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/14 10:58:10 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_arena_cycle.h"

void			cycle_lldi(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process)
{
	uint32_t	p1;
	uint32_t	p2;

	if (process->opcode_data.types[0] & T_REG)
		p1 = process->r[process->opcode_data.params[0]];
	else
		p2 = process->opcode_data.params[0];
	if (process->opcode_data.types[1] & T_REG)
		p2 = process->r[process->opcode_data.params[1]];
	else
		p1 = process->opcode_data.params[1];
}
