/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_unset_file.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 16:38:21 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/01 11:45:35 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_unset_file.h"

void								libcorewar_unset_src_file(struct s_libcorewar_src_file *const file)
{
	struct s_libcorewar_opcode_src	*op;
	void							*tmp;
	int								index;

	if (file)
	{
		op = file->opcodes;
		while (op)
		{
			if (op->label)
				free(op->label);
			index = 0;
			while (index < op->info->parameters)
			{
				if (op->parameters_labels[index])
					free(op->parameters_labels[index]);
				++index;
			}
			tmp = op;
			op = op->next;
			free(tmp);
		}
		if (file->content)
			free(file->content);
		free(file);
	}
}

void								libcorewar_unset_asm_file(struct s_libcorewar_asm_file *const file)
{
	struct s_libcorewar_opcode_asm	*op;
	void							*tmp;

	if (file)
	{
		op = file->opcodes;
		while (op)
		{
			if (op->label)
				free(op->label);
			tmp = op;
			op = op->next;
			free(tmp);
		}
		if (file->content)
			free(file->content);
		free(file);
	}
}
