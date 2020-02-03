/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_asm_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 11:47:27 by afeuerst          #+#    #+#             */
/*   Updated: 2020/01/28 11:51:00 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_get_asm_file.h"

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

static const int									g_parameters_size[] =
{
	[REG_CODE] = REG_SIZE,
	[IND_CODE] = IND_SIZE,
	[DIR_CODE] = DIR_SIZE
};

static void											get_asm_file_resolve_labels(
	struct s_libcorewar_asm_file *const file, char **const error)
{
	struct s_libcorewar_opcode_asm					*op;
	struct s_libcorewar_opcode_asm					*target;
	int												index;

	op = file->opcodes;
	while (op)
	{
		index = 0;
		while (index < op->info->parameters)
		{
			if (op->parameters_labels[index])
			{
				target = file->opcodes;
				while (target)
				{
					if (target->start == (const char*)op->parameters_labels[index])
					{
						if (!target->label)
							ft_asprintf(&target->label, "%s%d", file->labels_prefix, file->labels_count++);
						op->parameters_labels[index] = target->label;
						break ;
					}
					target = target->next;
				}
			}
			++index;
		}
		op = op->next;
	}
}

static char											*get_asm_file_opcodes_direct(
	struct s_libcorewar_asm_file *const file,
	struct s_libcorewar_opcode_asm *const opcode,
	char *content, const int index)
{
	char											*addr;

	if (opcode->info->parameters_direct_small)
	{
		opcode->parameters[index] = (int)__builtin_bswap16(*(short*)content) & 0xFFFF;
		addr = opcode->start + (int)opcode->parameters[index];
		content -= 2;
	}
	else
	{
		opcode->parameters[index] = __builtin_bswap32(*(int*)content);
		addr = opcode->start + opcode->parameters[index];
	}
	if (opcode->parameters[index] &&
			addr >= (file->content + sizeof(struct s_asm_header)) && addr < file->content_end &&
			g_opcodes_info[(int)*addr & 0xFF].name)
		opcode->parameters_labels[index] = addr;
	else
		opcode->parameters_labels[index] = NULL;
	return (content);
}

static char											*get_asm_file_opcodes_indirect(
	struct s_libcorewar_asm_file *const file,
	struct s_libcorewar_opcode_asm *const opcode,
	char *content, const int index)
{
	char											*addr;

	opcode->parameters[index] = (int)__builtin_bswap16(*(short*)content) & 0xFFFF;
	addr = opcode->start + (int)opcode->parameters[index];
	if (opcode->parameters[index] &&
			addr >= (file->content + sizeof(struct s_asm_header)) && addr < file->content_end &&
			g_opcodes_info[(int)*addr & 0xFF].name)
		opcode->parameters_labels[index] = addr;
	else
		opcode->parameters_labels[index] = NULL;
	return (content);
}

static char											*get_asm_file_opcodes_content(
	struct s_libcorewar_asm_file *const file,
	struct s_libcorewar_opcode_asm *const opcode,
	char *content, char **const error)
{
	int												index;
	int												r;
	char											encode;// 10(DIR,4) 11(IND,2) 01(REG,1)

	index = 0;
	if (opcode->info->parameters_encoding)
		encode = *content++;
	else
	{
		//if (opcode->info->parameters_direct_small)
		//	encode = 0b11000000;
		//else
			encode = 0b10000000;
	}
	while (index < opcode->info->parameters)
	{
		r = (((int)encode << (index << 1)) >> 6) & 0b00000011;
		if (r == REG_CODE)
			opcode->parameters[index] = (int)*content;
		else if (r == IND_CODE)
			content = get_asm_file_opcodes_indirect(file, opcode, content, index);
		else if (r == DIR_CODE)
			content = get_asm_file_opcodes_direct(file, opcode, content, index);
		else
		{
			ft_asprintf(error, "bad parameters encoding");
			return (NULL);
		}
		opcode->parameters_type[index++] = r;
		content += g_parameters_size[r];
	}
	return (content);
}

static void											get_asm_file_opcodes(struct s_libcorewar_asm_file *const file, char **const error)
{
	char											*content;
	const struct s_libcorewar_opcode_info			*info;
	struct s_libcorewar_opcode_asm					**nopcodes;

	nopcodes = &file->opcodes;
	content = file->content + sizeof(struct s_asm_header);
	while (content < file->content_end)
	{
		info = g_opcodes_info + *content;
		if (info->name)
		{
			if (!(*nopcodes = ft_memalloc(sizeof(struct s_libcorewar_opcode_asm))))
				return (strerror_void(error));
			(*nopcodes)->info = info;
			(*nopcodes)->start = content++;
			if (!(content = get_asm_file_opcodes_content(file, *nopcodes, content, error)))
				return ;
			nopcodes = &(*nopcodes)->next;
		}
		else
			return ((void)ft_asprintf(error, "bad opcode"));
	}
}

struct s_libcorewar_asm_file						*libcorewar_get_asm_file(const char *const named, char **const error, const char *const prefix)
{
	const int										fd = open(named, O_RDONLY);
	struct s_libcorewar_asm_file					*file;
  
	if (!(file = ft_memalloc(sizeof(struct s_libcorewar_asm_file))))
		return (strerror_para(error, file));
	if ((file->content_size = lseek(fd, 0, SEEK_END)) < 0)
		return (strerror_para(error, file));
	lseek(fd, 0, SEEK_SET);
	if (!file->content_size)
		return (seterror_para("file empty", error, file));
	if (!(file->content = malloc(file->content_size)))
		return (strerror_para(error, file));
	if (read(fd, file->content, file->content_size) < 0)
		return (strerror_para(error, file));
	file->content_end = file->content + file->content_size;
	if (file->content_size < sizeof(int))
		return (seterror_para("file to small for checking magic number", error, file));
	file->header = (void*)file->content;
	file->header->prog_size = __builtin_bswap32(file->header->prog_size);
	if (__builtin_bswap32(file->header->magic) != COREWAR_EXEC_MAGIC)
		return (seterror_para("bad magic number", error, file));
	file->opcodes = NULL;
	get_asm_file_opcodes(file, error);
	file->labels_count = 0;
	file->labels_prefix = prefix;
	if ((unsigned long)file->labels_prefix)
		get_asm_file_resolve_labels(file, error);
	return (file);
}
