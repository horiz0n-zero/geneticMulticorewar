/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 11:55:58 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/17 10:48:43 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar_shared.h"

static void							*argument_error(const char *src,
		char **const error,
		const char *arg)
{
	static char						buffer[2048];
	int								index;

	index = 0;
	*error = buffer;
	while (*src && index < (int)(sizeof(buffer) - 1))
	{
		if (*src == '#')
		{
			while (*arg && index < (int)(sizeof(buffer) - 1))
				buffer[index++] = *arg++;
			src++;
		}
		else if (*src == '%' && ++src)
			buffer[index++] = *arg;
		else
			buffer[index++] = *src++;
	}
	buffer[index] = 0;
	return (NULL);
}

static inline char					**argument_short(char **argv,
		const struct s_argument *const arguments,
		int *const flags,
		char **const error)
{
	const char						*ptr;
	const struct s_argument			*arg;

	ptr = *argv++ + 1;
	while (*ptr)
	{
		arg = arguments + (int)*ptr++;
		if (!arg->name)
			return (argument_error("illegal option -- %", error, ptr - 1));
		*flags |= arg->flags;
		if (arg->require)
		{
			if (!*argv)
				return (argument_error("option requires an argument -- %", error, ptr - 1));
			else
				*arg->required = *argv++;
		}
	}
	return (argv);
}

static inline char					**argument_long(char **argv,
		const struct s_argument *const arguments,
		int *const flags,
		char **const error)
{
	const char						*name = *argv++ + 2;
	const struct s_argument			*arg = arguments + (int)*name;

	if (!arg->name || ft_strcmp(arg->name, name))
		return (argument_error("illegal option --#", error, name));
	*flags |= arg->flags;
	if (arg->require)
	{
		if (!*argv)
			return (argument_error("option requires an argument --#", error, name));
		else
			*arg->required = *argv++;
	}
	return (argv);
}

char								**arguments_get(char **argv,
		const struct s_argument *const arguments,
		int *const flags,
		char **const error)
{
	*flags = 0;
	while (argv && *argv)
	{
		if (**argv == '-')
		{
			if (!*(*argv + 1))
				break ;
			else if (*(*argv + 1) == '-')
			{
				if (!*(*argv + 2) && ++argv)
					break ;
				argv = argument_long(argv, arguments, flags, error);
			}
			else
				argv = argument_short(argv, arguments, flags, error);
		}
		else
			break ;
	}
	return (argv);
}
