/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 13:12:24 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/14 11:01:46 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_color.h"

static const char			*g_colors[256] =
{
	[0] = "\033[38:5:7m",
	[HASH_CORAIL] = "\033[38:5:9m",
	[HASH_PAILLE] = "\033[38:5:11m",
	[HASH_POMMES] = "\033[38:5:10m",
	[HASH_SAPHIR] = "\033[38:5:12m",
	[HASH_CITRON] = "\033[38:5:226m",
	[HASH_ELECTR] = "\033[38:5:39m",
	[HASH_SAUMON] = "\033[38:5:209m",
	[HASH_TURQOI] = "\033[38:5:51m",
	[HASH_CERISE] = "\033[38:5:160m",
	[HASH_VIOLET] = "\033[38:5:177m",
	[HASH_FRAISE] = "\033[38:5:197m",
	[HASH_ORANGE] = "\033[38:5:202m"
};

static const char			*g_colors_name[256] =
{
	[0] = "default",
	[HASH_CORAIL] = "corail",
	[HASH_PAILLE] = "paille",
	[HASH_POMMES] = "pommes",
	[HASH_SAPHIR] = "saphir",
	[HASH_CITRON] = "citron",
	[HASH_ELECTR] = "electr",
	[HASH_SAUMON] = "saumon",
	[HASH_TURQOI] = "turqoi",
	[HASH_CERISE] = "cerise",
	[HASH_VIOLET] = "violet",
	[HASH_FRAISE] = "fraise",
	[HASH_ORANGE] = "orange"
};

static const unsigned char	g_colors_next[] =
{
	HASH_CORAIL,
	HASH_PAILLE,
	HASH_POMMES,
	HASH_SAPHIR,
	HASH_CITRON,
	HASH_ELECTR,
	HASH_SAUMON,
	HASH_TURQOI,
	HASH_CERISE,
	HASH_VIOLET,
	HASH_FRAISE,
	HASH_ORANGE
};

const char					*libcorewar_color(const unsigned char id)
{
	const char *const		color = g_colors[id];

	if (!color)
		return (g_colors[0]);
	return (color);
}

unsigned char				libcorewar_colorid(const char *const src)
{
	const unsigned char		id = ft_hash_src(src, ft_strlen(src));
	const char *const		color = g_colors[id];

	if (color && !ft_strcmp(src, g_colors_name[id]))
		return (id);
	return (0);
}

unsigned char				libcorewar_colorid_next(void)
{
	static unsigned long	next = 0;

	if (next >= sizeof(g_colors_next) / sizeof(g_colors_next[0]))
		next = 0;
	return (g_colors_next[next++]);
}

unsigned char				libcorewar_colorid_nextid(const unsigned char id)
{
	unsigned long			index;

	index = 0;
	while (index < sizeof(g_colors_next) / sizeof(g_colors_next[0]))
	{
		if (g_colors_next[index] == id)
		{
			if (++index >= sizeof(g_colors_next) / sizeof(g_colors_next[0]))
				break ;
			return (g_colors_next[index]);
		}
		++index;
	}
	return (g_colors_next[0]);
}
