/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_dump.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 09:28:00 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/18 09:34:52 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar.h"

void				libcorewar_dump_binary(const int fd, struct s_libcorewar_arena *const arena)
{
	write(fd, arena->memory, MEM_SIZE);
}

void				libcorewar_dump_colors(const int fd, struct s_libcorewar_arena *const arena)
{
	int				index;
	int				pos;
	const char		*ptr;
	unsigned char	*color;
	unsigned char	colorid;

	index = 0;
	while (index < LINES)
	{
		pos = 0;
		while (pos < BYTES_LINE)
		{
			ptr = arena->memory + (index * BYTES_LINE) + pos;
			color = arena->colorsid + (index * BYTES_LINE) + pos;
			colorid = *color;
			while (pos < BYTES_LINE && colorid == *(color + pos))
				++pos;
			if (pos < BYTES_LINE)
				ft_dprintf(fd, "%s%#04hh[* ]x", libcorewar_color(colorid), pos + 1, ptr);
			else
				ft_dprintf(fd, "%s%#04hh[* ]x\n", libcorewar_color(colorid), pos + 1, ptr);
		}
		++index;
	}
	ft_dprintf(fd, "\n");
}

void				libcorewar_dump(const int fd, struct s_libcorewar_arena *const arena)
{
	int				index;
	const char		*ptr;

	ptr = arena->memory;
	index = 0;
	while (index < LINES)
	{
		ft_dprintf(fd, "%#04hh[* ]x\n", BYTES_LINE, ptr);
		ptr += BYTES_LINE;
		++index;
	}
	write(fd, "\n", 1);
}
