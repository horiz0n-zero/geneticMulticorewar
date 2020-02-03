/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_core.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 09:00:10 by afeuerst          #+#    #+#             */
/*   Updated: 2019/10/31 14:10:53 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_CORE_H
# define FT_PRINTF_CORE_H

# include "libft_printf_internal.h"

static const char	*ft_printf_core_get_number(struct s_printformat *const printformat, const char *format, int *const number) __attribute__((always_inline));
static const char	*ft_printf_core_fill_percent(struct s_printformat *const printformat, const char *format, struct s_percent *const percent) __attribute__((always_inline));
static const char	*ft_printf_core_fill_percent_value(struct s_printformat *const printformat, const char *format, struct s_percent *const percent) __attribute__((always_inline));

#endif



