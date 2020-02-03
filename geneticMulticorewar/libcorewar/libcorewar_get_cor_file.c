/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_get_cor_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:31:58 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/14 11:05:38 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libcorewar.h"

struct s_libcorewar_cor_file						*libcorewar_get_cor_file(const char *const named, char **const error)
{
	static const size_t								padding = 4;
	const int										fd = open(named, O_RDONLY);
	struct s_libcorewar_cor_file					*file;
	off_t											content_size;

	if (fd < 0)
		return (strerror_para(error, NULL));
	if (!(file = ft_memalloc(sizeof(struct s_libcorewar_cor_file))))
		return (strerror_para(error, file));
	if ((content_size = lseek(fd, 0, SEEK_END)) < 0)
		return (strerror_para(error, file));
	lseek(fd, 0, SEEK_SET);
	if (!content_size)
		return (seterror_para("file empty", error, file));
	if (!(file->header = malloc((size_t)content_size + padding)))
		return (strerror_para(error, file));
	if (read(fd, file->header, (size_t)content_size) < 0)
		return (strerror_para(error, file));
	file->instructions = ((char*)file->header) + sizeof(struct s_asm_header);
	file->length = (size_t)(content_size - sizeof(struct s_asm_header));
	file->padded_length = file->length;
	while (file->padded_length % padding)
		++file->padded_length;
	*(int32_t*)(((char*)file->header) + content_size) = 0;
	return (file);
}

void												libcorewar_unset_cor_file(struct s_libcorewar_cor_file *file)
{
	if (file)
	{
		if (file->header)
			free(file->header);
		free(file);
	}
}
