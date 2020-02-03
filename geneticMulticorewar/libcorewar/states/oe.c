/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oe.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 08:41:53 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/24 09:25:54 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libcorewar_get_src_file.h"

void				oe_coli(struct s_libcorewar_src_file *const file, char *content, int *const coli)
{
	int				co;
	int				li;
	char			*ptr;

	ptr = file->content;
	co = 1;
	li = 1;
	while (ptr < content)
	{
		while (ptr < content && *ptr != '\n')
		{
			++co;
			++ptr;
		}
		while (ptr < content && *ptr == '\n')
		{
			co = 1;
			++li;
			++ptr;
		}
	}
	*coli = co;
	*(coli + 1) = li;
}

char				*oe_show_content(struct s_libcorewar_src_file *const file, char *content, int li)
{
	char			*start;
	char *const		saved = content;

	if (li)
	{
		while (content > file->content && *content != '\n')
			--content;
		if (*content)
			--content;
		while (content > file->content && *content != '\n')
			--content;
		start = content;
	}
	else
	{
		while (content > file->content && *content != '\n')
			--content;
		start = content;
	}
	if (*content)
		--content;
	content = saved;
	while (content < file->content_end && *content != '\n')
		++content;
	return (ft_memcopy(start, (size_t)(content - start)));
}

static const char	*g_oe_formats[] =
{
	[0] = "illegal character found %c in indirect parameters for %s instruction",
	[1] = "parameters numbers %d cannot be indirect for %s instruction",
	[2] = "parameters numbers %d cannot be a register for %s instruction",
	[3] = "parameters numbers %d cannot be a negative number for %s instruction, the value must be between %d and %d",
	[4] = "parameters numbers %d for %s instruction must have a number between %d and %d not %c",
	[5] = "parameters numbers %d for %s instruction cannot be direct",
	[6] = "label cannot be empty",
	[7] = "direct parameters for %s instruction must be a number or a label",
	[8] = "invalid parameters for %s instruction",
	[9] = "%s cannot be redefined",
	[10] = "unknow directive",
	[11] = "invalid directive did you mean %s ?",
	[12] = "%s content must be enclose in quotes not starting with %c",
	[13] = "%s content is not closed waiting for a %c for closing them",
	[14] = "%s content is too long, must be between 1 and %d",
	[15] = "%s content is empty, must be between 1 and %d",
	[16] = "provided number overflow(*) value must be between %d and %d",
	[17] = "provided number overflow(+) value must be between %d and %d",
	[18] = "provided number must be between %d and %d",
	[19] = "unknow instruction %s",
	[20] = "unexepted character found: %c",
	[21] = "label: %s is unreachable for %s instruction"
};

void				oeh(struct s_libcorewar_src_file *const file, char *const content, const int index, char **const error, ...)
{
	va_list			args;
	int				coli[2];
	char			*newformat;
	char			*parts;
	char			*tmp;

	va_start(args, error);
	oe_coli(file, content, coli);
	parts = oe_show_content(file, content, coli[1]);
	ft_asprintf(&newformat, "error in header at column %d, line %d\n%s", coli[0], coli[1], g_oe_formats[index]);
	ft_vasprintf(error, newformat, &args);
	tmp = *error;
	ft_asprintf(error, "%s\n%s", *error, parts);
	free(tmp);
	free(parts);
	free(newformat);
	va_end(args);
}

void				oe(struct s_libcorewar_src_file *const file, char *const content, const int index, char **const error, ...)
{
	va_list			args;
	int				coli[2];
	char			*newformat;
	char			*parts;
	char			*tmp;

	va_start(args, error);
	oe_coli(file, content, coli);
	parts = oe_show_content(file, content, coli[1]);
	ft_asprintf(&newformat, "error in instructions at column %d, line %d\n%s", coli[0], coli[1], g_oe_formats[index]);
	ft_vasprintf(error, newformat, &args);
	tmp = *error;
	ft_asprintf(error, "%s\n%s", *error, parts);
	free(tmp);
	free(parts);
	free(newformat);
	va_end(args);
}


















