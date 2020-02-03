/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_cor_instructions.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 08:29:07 by afeuerst          #+#    #+#             */
/*   Updated: 2020/01/28 09:34:07 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_get_cor_instructions.h"

static const struct s_libcorewar_opcode_info		g_opcodes_info[256] =
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

static const size_t									g_parameters_size[] =
{
	[REG_CODE] = REG_SIZE,
	[IND_CODE] = IND_SIZE,
	[DIR_CODE] = DIR_SIZE
};

static size_t										get_cor_instruction_size(
	const struct s_libcorewar_opcode_info *const info,
	char *content)
{
	int												index;
	int												r;
	char											encode;// 10(DIR,4) 11(IND,2) 01(REG,1)
	size_t											size;

	index = 0;
	if (info->parameters_encoding)
	{
		encode = *++content;
		size = 2;
	}
	else
	{
		if (info->parameters_direct_small)
			encode = 0b11000000;
		else
			encode = 0b10000000;
		size = 1;
	}
	while (index < info->parameters)
	{
		r = (((int)encode << (index << 1)) >> 6) & 0b00000011;
		size += g_parameters_size[r];
		++index;
	}
	return (size);
}

struct s_libcorewar_cor_instructions			*libcorewar_get_cor_instructions(struct s_libcorewar_cor_file *const file)
{
	struct s_libcorewar_cor_instructions *const	new = malloc(sizeof(struct s_libcorewar_cor_instructions));
	size_t										index;

	const struct s_libcorewar_opcode_info		*info;
	char										*source;

	struct s_libcorewar_instruction				*ins;

	new->instructions = malloc(sizeof(void*) * 1024);
	new->instructions_count = 0;
	if (new)
	{
		source = file->instructions;
		index = 0;
		while (index < file->length)
		{
			info = g_opcodes_info + ((int)*source & 0xFF);
			if (info->name)
			{
				ins = malloc(sizeof(struct s_libcorewar_instruction));
				ins->content_size = get_cor_instruction_size(info, source);
				ins->info = info;
				memmove(ins->content, source, ins->content_size);
				source += ins->content_size;
				index += ins->content_size;
				*(new->instructions + new->instructions_count++) = ins;
			}
			else
			{
				ft_dprintf(STDERR_FILENO, "unknow opcode(%02hhx) index(%lu) length(%lu)\n", *source, index, file->length);
				break ;
			}
		}
	}
	return (new);
}

void											libcorewar_unset_cor_instructions(struct s_libcorewar_cor_instructions *const file)
{
	if (file)
	{
		if (file->instructions)
			free(file->instructions);
		free(file);
	}
}


























