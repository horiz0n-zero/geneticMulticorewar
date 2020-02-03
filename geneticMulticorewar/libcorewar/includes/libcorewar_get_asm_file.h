/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_asm_file.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 11:45:34 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/25 10:40:45 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCOREWAR_GET_ASM_FILE_H
# define LIBCOREWAR_GET_ASM_FILE_H

# include "libcorewar.h"

static void		get_asm_file_resolve_labels(struct s_libcorewar_asm_file *const file, char **const error);
static char		*get_asm_file_opcodes_direct(struct s_libcorewar_asm_file *const file,
		struct s_libcorewar_opcode_asm *const opcode, char *content, const int index) __attribute__((always_inline));

static void		get_asm_file_opcodes(struct s_libcorewar_asm_file *const file, char **const error) __attribute__((always_inline));

#endif
