/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_printf.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 08:46:04 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/23 08:54:21 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_PRINTF_H
# define LIBFT_PRINTF_H

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

int		ft_printf(const char *const format, ...);
int		ft_dprintf(const int fd, const char *const format, ...);
int		ft_asprintf(char **const ret, const char *const format, ...);
int		ft_bprintf(int (*function)(const char *const, const size_t), const char *const format, ...);

int		ft_vprintf(const char *const format, va_list *const args);
int		ft_vdprintf(const int fd, const char *const format, va_list *const args);
int		ft_vasprintf(char **const ret, const char *const format, va_list *const args);
int		ft_vbprintf(int (*function)(const char *const, const size_t), const char *const format, va_list *const args);

#endif
