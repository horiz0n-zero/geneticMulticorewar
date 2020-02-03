/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_arena_cycle.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 08:42:36 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/06 11:13:56 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCOREWAR_ARENA_CYCLE_H
# define LIBCOREWAR_ARENA_CYCLE_H

# include "libcorewar.h"

typedef void	(*t_cycle_function)(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);

void			cycle_live(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_ld(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_st(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_add(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_sub(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_and(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_or(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_xor(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_zjmp(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_ldi(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_sti(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_fork(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_lld(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_lldi(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_lfork(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);
void			cycle_aff(struct s_libcorewar_arena *const arena, struct s_libcorewar_process *const process);

#endif
