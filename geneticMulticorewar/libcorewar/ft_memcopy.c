/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcopy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 15:35:17 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/24 16:26:43 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar.h"

void				strerror_void(char **const error)
{
	char			*ptr;
	char *const		src = strerror(errno);
	const size_t	len = ft_strlen(src);
	size_t			index;

	if ((ptr = malloc(len + 1)))
	{
		index = 0;
		while (index < len)
		{
			ptr[index] = src[index];
			++index;
		}
		ptr[index] = 0;
		*error = ptr;
	}
}

void				*strerror_para(char **const error, void *const para)
{
	char			*ptr;
	char *const		src = strerror(errno);
	const size_t	len = ft_strlen(src);
	size_t			index;

	if ((ptr = malloc(len + 1)))
	{
		index = 0;
		while (index < len)
		{
			ptr[index] = src[index];
			++index;
		}
		ptr[index] = 0;
		*error = ptr;
	}
	return (para);
}

void				*seterror_para(const char *const e, char **const error, void *const para)
{
	char			*ptr;
	const size_t	len = ft_strlen(e);
	size_t			index;

	if ((ptr = malloc(len + 1)))
	{
		index = 0;
		while (index < len)
		{
			ptr[index] = e[index];
			++index;
		}
		ptr[index] = 0;
		*error = ptr;
	}
	return (para);
}

void				*strerror_null(char **const error)
{
	char			*ptr;
	char *const		src = strerror(errno);
	const size_t	len = ft_strlen(src);
	size_t			index;

	if ((ptr = malloc(len + 1)))
	{
		index = 0;
		while (index < len)
		{
			ptr[index] = src[index];
			++index;
		}
		ptr[index] = 0;
		*error = ptr;
	}
	return (NULL);
}

void			*ft_memcopy(const char *src, size_t len)
{
	char *const	ptr = malloc(len + 1);
	size_t		index;

	if (ptr)
	{
		index = 0;
		while (index < len)
		{
			*(ptr + index) = *(src + index);
			++index;
		}
		*(ptr + index) = 0;
	}
	return (ptr);
}
