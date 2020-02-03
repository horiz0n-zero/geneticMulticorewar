/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_arena.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 10:07:50 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/13 08:51:10 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_get_arena.h"

static void							get_arena_champion_place(struct s_libcorewar_arena *const arena,
		struct s_libcorewar_champion *const champion, char **const error)
{
	const int						offset = MEM_SIZE / arena->champions_count;
	char							*start;
	unsigned char					*start_colorsid;
	char							*source;
	unsigned int					i;

	if (champion->file->header->prog_size > CHAMP_MAX_SIZE)
		return ((void)ft_asprintf(error, "%s has too large a code (%u bytes > %u bytes)", champion->name,
					champion->file->header->prog_size, CHAMP_MAX_SIZE));
	start = arena->memory + (offset * (arena->champions_count - 1));
	start_colorsid = arena->colorsid + (offset * (arena->champions_count - 1));
	source = champion->file->content + sizeof(struct s_asm_header);
	i = 0;
	while (i < champion->file->header->prog_size)
	{
		*(start + i) = *(source + i);
		*(start_colorsid + i) = champion->colorid;
		++i;
	}
}

static void							get_arena_champion(struct s_libcorewar_arena *const arena,
		const char *const named, const int32_t id, char **const error)
{
	struct s_libcorewar_champion	*champion;
	struct s_libcorewar_process		*process;

	if (arena->champions_count + 1 > MAX_PLAYERS)
		return ((void)ft_asprintf(error, "too many players"));
	champion = arena->champions + arena->champions_count++;
	champion->id = id;
	champion->colorid = libcorewar_colorid_next();
	champion->file = libcorewar_get_asm_file(named, error, NULL);
	if (*error)
		return (libcorewar_unset_asm_file(champion->file));
	champion->name = named;
	if ((process = libcorewar_get_process(champion)))
	{
		process->next = arena->procs;
		arena->procs = process;
	}
	else
		return (strerror_void(error));
	get_arena_champion_place(arena, champion, error);
}

struct s_libcorewar_arena			*libcorewar_get_arena(const t_libcorewar_opcode_function *const functions, char **const error, ...)
{
	struct s_libcorewar_arena		*arena;
	va_list							args;
	const char						*named;

	va_start(args, error);
	if ((arena = ft_memalloc(sizeof(struct s_libcorewar_arena))))
	{
		named = va_arg(args, const char*);
		while (named)
		{
			get_arena_champion(arena, named, va_arg(args, int32_t), error);
			if (*error)
				break ;
			named = va_arg(args, const char*);
		}
	}
	else
		strerror_void(error);
	arena->functions = functions;
	arena->cycle_to_die = CYCLE_TO_DIE;
	
	va_end(args);
	return (arena);
}
