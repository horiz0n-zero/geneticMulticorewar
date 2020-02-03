/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_arena.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 11:47:15 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/06 11:25:26 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCOREWAR_GET_ARENA_H
# define LIBCOREWAR_GET_ARENA_H

# include "libcorewar.h"

static void	get_arena_champion_place(struct s_libcorewar_arena *const arena,
		struct s_libcorewar_champion *const champion, char **const error) __attribute__((always_inline));

#endif
