/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_static_world.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 14:45:57 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/14 10:59:56 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar.h"

char			*ft_world(char *content, char *const content_end, const int *const source)
{
	while (content < content_end && source[(int)*content & 0xFF])
		++content;
	return (content);
}

char			*ft_static_world(char *content, char *const content_end, const int *const source, int *const length)
{
	static char			world_buffer[2048];
	static char *const	world_buffer_end = world_buffer + sizeof(world_buffer) - 1;
	char				*ptr;

	ptr = world_buffer;
	while (source[(int)*content & 0xFF] && content < content_end && ptr < world_buffer_end)
		*ptr++ = *content++;
	*ptr = 0;
	*length = (int)(ptr - world_buffer);
	return (world_buffer);
}
