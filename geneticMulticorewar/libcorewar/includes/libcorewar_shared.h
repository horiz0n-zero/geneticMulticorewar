/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcorewar_shared.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 10:06:07 by afeuerst          #+#    #+#             */
/*   Updated: 2019/12/12 13:56:30 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCOREWAR_SHARED_H
# define LIBCOREWAR_SHARED_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <string.h>
# include <sys/errno.h>
# include <stdio.h>
# include "../libft_printf/includes/libft_printf.h"

struct								s_argument
{
	const char						*name;
	const int						flags;
	const int						require;
	char **const					required;
};

char			**arguments_get(char **argv, const struct s_argument *const arguments, int *const flags, char **const error);

size_t			ft_strlen(const char *const src);
int				ft_strcmp(const char *s1, const char *s2);
void			*ft_memalloc(const size_t size);
void			*ft_memcopy(const char *src, size_t length);
unsigned char	ft_hash_src(const char *src, size_t size);
char			*ft_static_world(char *content, char *const content_end, const int *const source, int *const length);
char			*ft_world(char *content, char *const content_end, const int *const source);
size_t			ft_number_length(size_t n);

void			*strerror_null(char **const error);
void			*strerror_para(char **const error, void *const para);
void			*seterror_para(const char *const e, char **const error, void *const para);
void			strerror_void(char **const error);

unsigned int	high_tone(unsigned int n) __attribute__((const));
int				multi_issafe(int lhs, int rhs) __attribute__((const));
int				add_issafe(int lhs, int rhs) __attribute__((const));
uint32_t		ft_atouint32(const char *src, char **const error);


#endif
