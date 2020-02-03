/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_states_skip.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 09:34:47 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/19 15:56:38 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_get_src_file.h"

static const int	g_whitespace[256] =
{
	[' '] = 1,
	['\t'] = 1,
	['\n'] = 1
};

static const int	g_virguspace[256] =
{
	[' '] = 1,
	['\t'] = 1,
	['\n'] = 1,
	[SEPARATOR_CHAR] = 1
};

char				*libcorewar_state_numbers(struct s_libcorewar_src_file *const file, char *content, int *const state, char **const error)
{
	while (*content >= '0' && *content <= '9')
		++content;
	return (content);
}

char				*libcorewar_state_virguspace(struct s_libcorewar_src_file *const file, char *content, int *const state, char **const error)
{
	while (content < file->content_end && g_virguspace[(int)*content & 0xFF])
		++content;
	return (content);
}

char				*libcorewar_state_whitespace(struct s_libcorewar_src_file *const file, char *content, int *const state, char **const error)
{
	while (content < file->content_end && g_whitespace[(int)*content & 0xFF])
		++content;
	return (content);
}

char				*libcorewar_state_comment(struct s_libcorewar_src_file *const file, char *content, int *const state, char **const error)
{
	while (content < file->content_end && *content != '\n')
		++content;
	if (*content == '\n')
		return (++content);
	return (content);
}
