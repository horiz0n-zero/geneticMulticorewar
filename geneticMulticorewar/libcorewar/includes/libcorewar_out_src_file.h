/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_out_src_file.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 11:04:35 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/24 10:16:20 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCOREWAR_OUT_SRC_FILE_H
# define LIBCOREWAR_OUT_SRC_FILE_H

# include "libcorewar.h"

static void	out_parameters_labels_resolve(struct s_libcorewar_src_file *const file) __attribute__((always_inline));
static char	*out_opcode_parameters(struct s_libcorewar_opcode_src *const op, const int index, char *ins) __attribute__((always_inline));

#endif
