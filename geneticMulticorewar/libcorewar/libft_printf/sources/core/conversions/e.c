/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 09:05:37 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/14 11:07:46 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft_printf_internal.h"

static const char	*g_source_e[256] =
{
	[0] = "^@",
	[1] = "^A",
	[2] = "^B",
	[3] = "^C",
	[4] = "^D",
	[5] = "^E",
	[6] = "^F",
	[7] = "^G",
	[8] = "^H",
	[10] = "$\n",
	[11] = "^K",
	[12] = "^L",
	[13] = "^M",
	[14] = "^N",
	[15] = "^O",
	[16] = "^P",
	[17] = "^Q",
	[18] = "^R",
	[19] = "^S",
	[20] = "^T",
	[21] = "^U",
	[22] = "^V",
	[23] = "^W",
	[24] = "^X",
	[25] = "^Y",
	[26] = "^Z",
	[27] = "^[",
	[28] = "^\\",
	[29] = "^]",
	[30] = "^^",
	[31] = "^_",
};

size_t			precalculate_e(struct s_printformat *const printformat, struct s_percent *const percent)
{
	if (g_source_e[(int)percent->data & 0xFF])
		return (2);
	else
		return (1);
}

char			*transform_e(char *dst, struct s_percent *const percent)
{
	const int	c = (int)percent->data;

	if (g_source_e[c])
	{
		*dst++ = g_source_e[c][0];
		*dst++ = g_source_e[c][1];
	}
	else
		*dst++ = c;
	return (dst);
}
