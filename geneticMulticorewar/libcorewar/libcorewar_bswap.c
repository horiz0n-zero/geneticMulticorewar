/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_bswap.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 13:17:59 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/01 11:35:20 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_bswap.h"

void								libcorewar_bswap_src_file(struct s_libcorewar_src_file *const file)
{
	struct s_libcorewar_opcode_src	*op;
	int								p;

	op = file->opcodes;
	while (op)
	{
		p = 0;
		while (p < op->info->parameters)
		{
			if (op->parameters_type[p] == T_IND)
				op->parameters[p] = (int)__builtin_bswap16((short)op->parameters[p]);
			else if (op->parameters_type[p] == T_DIR)
			{
				if (op->info->parameters_direct_small)
					op->parameters[p] = (int)__builtin_bswap16((short)op->parameters[p]);
				else
					op->parameters[p] = __builtin_bswap32(op->parameters[p]);
			}
			++p;
		}
		op = op->next;
	}
}

void								libcorewar_bswap_asm_file(struct s_libcorewar_asm_file *const file)
{
	struct s_libcorewar_opcode_asm	*op;
	int								p;

	op = file->opcodes;
	file->header->magic = __builtin_bswap32(file->header->magic);
	file->header->prog_size = __builtin_bswap32(file->header->prog_size);
	while (op)
	{
		p = 0;
		while (p < op->info->parameters)
		{
			if (op->parameters_type[p] == T_IND)
				op->parameters[p] = (int)__builtin_bswap16((short)op->parameters[p]);
			else if (op->parameters_type[p] == T_DIR)
			{
				if (op->info->parameters_direct_small)
					op->parameters[p] = (int)__builtin_bswap16((short)op->parameters[p]);
				else
					op->parameters[p] = __builtin_bswap32(op->parameters[p]);
			}
			++p;
		}
		op = op->next;
	}
}
