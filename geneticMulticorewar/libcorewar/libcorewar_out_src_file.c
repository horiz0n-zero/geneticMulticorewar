/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_out_src_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 11:33:19 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/01 11:45:10 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_out_src_file.h"

static void							out_parameters_labels_resolve(struct s_libcorewar_src_file *const file)
{
	struct s_libcorewar_opcode_src	*op;
	int								index;

	op = file->opcodes;
	while (op)
	{
		index = 0;
		while (index < op->info->parameters)
		{
			if (op->parameters_oplabels[index])
			{
				if (op->parameters_type[index] & T_IND || op->info->parameters_direct_small)
					*(short*)op->parameters_addrlabels[index] = __builtin_bswap16((short)(op->parameters_oplabels[index]->ins - op->ins));
				else
					*(int*)op->parameters_addrlabels[index] = __builtin_bswap32((int)(op->parameters_oplabels[index]->ins - op->ins));
			}
			++index;
		}
		op = op->next;
	}
}

static char							*out_opcode_parameters(struct s_libcorewar_opcode_src *const op, const int index, char *ins)
{
	op->parameters_addrlabels[index] = ins;
	if (op->parameters_type[index] & T_REG)
		*ins++ = (char)op->parameters[index];
	else if (op->parameters_type[index] & T_IND || op->info->parameters_direct_small)
	{
		*(short*)ins = (short)op->parameters[index];
		ins += 2;
	}
	else
	{
		*(int*)ins = (int)op->parameters[index];
		ins += 4;
	}
	return (ins);
}

static char							*out_opcodes(struct s_libcorewar_src_file *const file, char *ins)
{
	struct s_libcorewar_opcode_src	*op;
	int								index;

	op = file->opcodes;
	while (op)
	{
		op->ins = ins;
		*ins++ = (char)op->info->opvalue;
		if (op->info->parameters_encoding)
			*ins++ = libcorewar_opcode_src_encoded_parameters(op);
		index = 0;
		while (index < op->info->parameters)
		{
			ins = out_opcode_parameters(op, index, ins);
			++index;
		}
		op = op->next;
	}
	out_parameters_labels_resolve(file);
	return (ins);
}

void								libcorewar_out_src_file(const int fd, struct s_libcorewar_src_file *const file, char **const error)
{
	char							ins[file->opcodes_count * MAX_OPCODE_SIZE];
	size_t							ins_length;

	ins_length = (size_t)(out_opcodes(file, ins) - ins);
	file->header.magic = __builtin_bswap32(COREWAR_EXEC_MAGIC);
	file->header.prog_size = __builtin_bswap32((int)ins_length);
	if ((write(fd, &file->header, sizeof(struct s_asm_header))) == -1)
		return (strerror_void(error));
	if ((write(fd, ins, ins_length)) == -1)
		return (strerror_void(error));
}
