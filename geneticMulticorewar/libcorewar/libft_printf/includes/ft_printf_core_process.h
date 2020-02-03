/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_core_process.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 13:36:31 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/03 13:35:31 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_CORE_PROCESS_H
# define FT_PRINTF_CORE_PROCESS_H

# include "libft_printf_internal.h"

static char		*ft_printf_core_process_array(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent *percents, char *ptr) __attribute__((always_inline));

#endif
