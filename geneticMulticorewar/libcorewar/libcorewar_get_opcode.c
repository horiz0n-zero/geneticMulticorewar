/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_opcode.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 13:26:48 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/12 14:07:32 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_get_opcode.h"

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

// todo for refacoring 
int												libcorewar_get_opcode_data(const char *const start, struct s_libcorewar_opcode_data *const data)
{
	const char									*ptr;
	char										encode;

	if ((data->info = g_opcodes_info + *start)->name)
	{
		ptr = (start + 1);
		if (data->info->parameters_encoding)
			encode = *ptr++;
		else
			encode = 0b10000000;
	}
	else
		return (0);
	return (1);
}











