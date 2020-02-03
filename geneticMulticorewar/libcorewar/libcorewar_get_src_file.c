/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_src_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 11:47:27 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/13 11:06:55 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_get_src_file.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winitializer-overrides"
static const t_state_func							g_state_header[256] =
{
	[0 ... 255] = libcorewar_state_unexpected,
	['.'] = libcorewar_state_head_point,
	[' '] = libcorewar_state_whitespace,
	['\t'] = libcorewar_state_whitespace,
	['\n'] = libcorewar_state_whitespace,
	['#'] = libcorewar_state_comment,
	[';'] = libcorewar_state_comment
};

static const t_state_func							g_state_opcode[256] =
{
	[0 ... 255] = libcorewar_state_unexpected,
	[' '] = libcorewar_state_whitespace,
	['\t'] = libcorewar_state_whitespace,
	['\n'] = libcorewar_state_whitespace,
	['#'] = libcorewar_state_comment,
	[';'] = libcorewar_state_comment,
	['a' ... 'z'] = libcorewar_state_opcode,
	['0' ... '9'] = libcorewar_state_opcode,
	['_'] = libcorewar_state_opcode
};
#pragma clang diagnostic pop

static const t_state_func							*g_state[2] =
{
	[STATE_HEADER] = g_state_header,
	[STATE_OPCODE] = g_state_opcode
};

static struct s_libcorewar_opcode_src				*libcorewar_get_src_labels_opaddr(struct s_libcorewar_src_file *const file,
		char **const error, const char *const target)
{
	struct s_libcorewar_opcode_src					*op;

	op = file->opcodes;
	while (op)
	{
		if (op->label && op->label[0] == target[0] && !ft_strcmp(op->label, target))
				return (op);
		op = op->next;
	}
	return (NULL);
}

static void											libcorewar_get_src_labels_resolve(struct s_libcorewar_src_file *const file, char **const error)
{
	struct s_libcorewar_opcode_src					*op;
	struct s_libcorewar_opcode_src					*target;
	int												index;

	op = file->opcodes;
	while (op)
	{
		index = 0;
		while (index < op->info->parameters)
		{
			if (op->parameters_labels[index])
			{
				target = libcorewar_get_src_labels_opaddr(file, error, op->parameters_labels[index]);
				if (!target)
				{
					ft_asprintf(error, "unreachable label %%:%s", op->parameters_labels[index]);
					break ;
				}
				op->parameters_oplabels[index] = target;
			}
			++index;
		}
		op = op->next;
	}
}

static void											libcorewar_get_src_loop(struct s_libcorewar_src_file *const file, char **const error)
{
	char											*content;
	int												state;

	state = 0;
	content = file->content;
	*error = NULL;
	while (content < file->content_end)
	{
		content = g_state[state][(int)*content & 0xFF](file, content, &state, error);
		if (*error)
			break ;
	}
}

struct s_libcorewar_src_file						*libcorewar_get_src_file(const char *const named, char **const error)
{
	const int										fd = open(named, O_RDONLY);
	struct s_libcorewar_src_file					*file;

	if (!(file = ft_memalloc(sizeof(struct s_libcorewar_src_file))))
		return (strerror_para(error, file));
	if (fd < 0 || (file->content_size = lseek(fd, 0, SEEK_END)) < 0)
		return (strerror_para(error, file));
	lseek(fd, 0, SEEK_SET);
	if (!file->content_size)
		return (seterror_para("file empty", error, file));
	if (!(file->content = malloc(file->content_size)))
		return (strerror_para(error, file));
	if (read(fd, file->content, file->content_size) < 0)
		return (strerror_para(error, file));
	file->content_end = file->content + file->content_size;
	libcorewar_get_src_loop(file, error);
	libcorewar_get_src_labels_resolve(file, error);
	close(fd);
	return (file);
}
