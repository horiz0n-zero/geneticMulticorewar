/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_arena_cycle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 08:40:36 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/18 09:10:03 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_arena_cycle.h"

static const struct s_libcorewar_opcode_info	g_opcodes_info[256] =
{
	[0x01] = {"live",  1, 10,   0, 0, 0x01, 0, {T_DIR}},
	[0x02] = {"ld",    2, 5,    1, 0, 0x02, 1, {T_DIR | T_IND, T_REG}},
	[0x03] = {"st",    2, 5,    1, 0, 0x03, 0, {T_REG, T_IND | T_REG}},
	[0x04] = {"add",   3, 10,   1, 0, 0x04, 1, {T_REG, T_REG, T_REG}},
	[0x05] = {"sub",   3, 10,   1, 0, 0x05, 1, {T_REG, T_REG, T_REG}},
	[0x06] = {"and",   3, 6,    1, 0, 0x06, 1, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}},
	[0x07] = {"or",    3, 6,    1, 0, 0x07, 1, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
	[0x08] = {"xor",   3, 6,    1, 0, 0x08, 1, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}},
	[0x09] = {"zjmp",  1, 20,   0, 1, 0x09, 0, {T_DIR}},
	[0x0a] = {"ldi",   3, 25,   1, 1, 0x0a, 0, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
	[0x0b] = {"sti",   3, 25,   1, 1, 0x0b, 0, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}},
	[0x0c] = {"fork",  1, 800,  0, 1, 0x0c, 0, {T_DIR}},
	[0x0d] = {"lld",   2, 10,   1, 0, 0x0d, 1, {T_DIR | T_IND, T_REG}},
	[0x0e] = {"lldi",  3, 50,   1, 1, 0x0e, 1, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}},
	[0x0f] = {"lfork", 1, 1000, 0, 1, 0x0f, 0, {T_DIR}},
	[0x10] = {"aff",   1, 2,    1, 0, 0x10, 0, {T_REG}}
};

static const t_cycle_function					g_arena_cycle_opcode[256] =
{
	[0x00] = cycle_aff,
	[0x01] = cycle_live,
	[0x02] = cycle_ld,
	[0x03] = cycle_st,
	[0x04] = cycle_add,
	[0x05] = cycle_sub,
	[0x06] = cycle_and,
	[0x07] = cycle_or,
	[0x08] = cycle_xor,
	[0x09] = cycle_zjmp,
	[0x0a] = cycle_ldi,
	[0x0b] = cycle_sti,
	[0x0c] = cycle_fork,
	[0x0d] = cycle_lld,
	[0x0e] = cycle_lldi,
	[0x0f] = cycle_lfork,
	[0x10] = cycle_aff
};

static const int								g_code_to_type[] =
{
	[REG_CODE] = T_REG,
	[IND_CODE] = T_IND,
	[DIR_CODE] = T_DIR
};

static void										arena_cycle_param(
		struct s_libcorewar_arena *const arena,
		struct s_libcorewar_process *const process, char *dst, int size)
{
	while (size--)
		*dst++ = arena->memory[process->tmpc++];
}

static int										arena_cycle_params(
		struct s_libcorewar_arena *const arena,
		struct s_libcorewar_process *const process,
		const struct s_libcorewar_opcode_info *const info,
		const int i)
{
	process->opcode_data.params[i] = 0;
	if (process->opcode_data.types[i] & T_REG)
	{
		arena_cycle_param(arena, process, (char*)&process->opcode_data.params[i], 1);
		if (!(process->opcode_data.params[i] >= 0 && process->opcode_data.params[i] < REG_NUMBER))
			return (1);
	}
	else if (process->opcode_data.types[i] & T_IND || info->parameters_direct_small)
		arena_cycle_param(arena, process, (char*)&process->opcode_data.params[i], 2);
	else
		arena_cycle_param(arena, process, (char*)&process->opcode_data.params[i], 4);
	return (0);
}

static int										arena_cycle_read(
		struct s_libcorewar_arena *const arena,
		struct s_libcorewar_process *const process,
		const struct s_libcorewar_opcode_info *const info)
{
	int											i;
	char										e;

	i = 0;
	process->tmpc = process->pc + 1;
	if (info->parameters_encoding)
		e = arena->memory[process->tmpc++];
	else
		e = 0b10000000;
	while (i < info->parameters)
	{
		process->opcode_data.types[i] = (((int)e << (i << 1)) >> 6) & 0b11;
		process->opcode_data.types[i] = g_code_to_type[process->opcode_data.types[i]];
		if (!(process->opcode_data.types[i] & info->parameters_type[i]))
			return (0);
		if (arena_cycle_params(arena, process, info, i))
			return (0);
		++i;
	}
	process->opcode_cycles = info->cycles;
	process->pc = process->tmpc;
	return (1);
}

int												libcorewar_arena_cycle(struct s_libcorewar_arena *const arena)
{
	struct s_libcorewar_process					*procs;
	const struct s_libcorewar_opcode_info		*info;

	procs = arena->procs;
	while (procs)
	{
		if (!procs->opcode_data.info)
		{
			info = g_opcodes_info + arena->memory[procs->pc];
			if (info->name && arena_cycle_read(arena, procs, info))
				procs->opcode_data.info = info;
			else
				++procs->pc;
		}
		else if (!procs->opcode_cycles)
		{
			g_arena_cycle_opcode[procs->opcode_data.info->opvalue](arena, procs);
			if (arena->functions[procs->opcode_data.info->opvalue])
				arena->functions[procs->opcode_data.info->opvalue](arena, procs);
			procs->opcode_data.info = NULL;
		}
		else
			--procs->opcode_cycles;
		procs = procs->next;
	}
	return (libcorewar_arena_cycle_check(arena));
}
