/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_printf_internal.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 09:01:48 by afeuerst          #+#    #+#             */
/*   Updated: 2019/11/23 08:58:36 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_PRINTF_INTERNAL_H
# define LIBFT_PRINTF_INTERNAL_H

# include "libft_printf.h"

# define CONV_VOID 0 << 0
# define CONV_U 1 << 0
# define CONV_I 1 << 1
# define CONV_B 1 << 2
# define CONV_O 1 << 3
# define CONV_X 1 << 4
# define CONV_XX 1 << 5
# define CONV_W 1 << 6
# define CONV_C 1 << 7
# define CONV_S 1 << 8
# define CONV_P 1 << 9
# define CONV_E 1 << 10
# define CONV_EE 1 << 11
# define CONV_PERCENT 1 << 12
# define CONV_POINTER (CONV_P | CONV_S)

# define SIZE_CHAR 1 << 1
# define SIZE_SHORT 1 << 0
# define SIZE_INT 1 << 2
# define SIZE_LONG 1 << 3

# define FLAGS_HASHTAG 1 << 0
# define FLAGS_PLUS 1 << 1
# define FLAGS_MINUS 1 << 2
# define FLAGS_ZERO 1 << 3
# define FLAGS_ISARRAY 1 << 4
# define FLAGS_ARRAY_REQUIRED_COUNT 1 << 5

# define ALIGNMENT sizeof(long)

struct							s_percent
{
	int							flags;
	int							width;
	int							precision;
	int							size;

	unsigned long				data;

	size_t						r1;
	size_t						r2;
	size_t						r3;
	size_t						r4;

	const struct s_conversion	*conversion;
	const char					*format;
	struct s_percent			*next;
	struct s_percent			*array;
} __attribute__((packed,aligned(ALIGNMENT)));

struct							s_printformat
{
	const char					*format;
	va_list						*args;
	struct s_percent			*percents;
	size_t						length;
	int							ret;
	int							r1;
	long long					r2;
	int							(*function)(const char *const, const size_t);
} __attribute__((packed,aligned(ALIGNMENT)));

void							ft_printf_core(struct s_printformat *const printformat, const char *format, struct s_percent **const next) __attribute__((visibility("hidden")));
void							ft_printf_core_process(struct s_printformat *const printformat, const char *format) __attribute__((visibility("hidden")));

const char						*ft_printf_array_prepare(struct s_printformat *const printformat, const char *format, struct s_percent *const percent);

struct							s_array_conv
{
	size_t						(*loop_count)(const void *src);
	const char					*(*loop)(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const void *src);
} __attribute__((packed,aligned(ALIGNMENT)));

const char						*ft_printf_array_loop_void(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const char *src);
size_t							ft_printf_array_loop_void_count(const char *src) __attribute__((pure));
const char						*ft_printf_array_loop_char(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const char *src);
size_t							ft_printf_array_loop_char_count(const char *src) __attribute__((pure));
const char						*ft_printf_array_loop_short(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const short *src);
size_t							ft_printf_array_loop_short_count(const short *src) __attribute__((pure));
const char						*ft_printf_array_loop_int(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const int *src);
size_t							ft_printf_array_loop_int_count(const int *src) __attribute__((pure));
const char						*ft_printf_array_loop_long(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const long *src);
size_t							ft_printf_array_loop_long_count(const long *src) __attribute__((pure));
const char						*ft_printf_array_loop_stars(struct s_printformat *const printformat, struct s_percent *const array, struct s_percent **const next, const void **src);
size_t							ft_printf_array_loop_stars_count(const void **src) __attribute__((pure));



size_t							ft_printf_unumber_length(unsigned long number, const unsigned long base) __attribute__((visibility("hidden")));
void							ft_printf_unumber_transform(unsigned long number, const unsigned long base, const char *const basesrc, char *dst) __attribute__((visibility("hidden")));
size_t							ft_printf_number_length(long number, const long base) __attribute__((visibility("hidden")));
void							ft_printf_number_transform(long number, const long base, const char *const basesrc, char *dst) __attribute__((visibility("hidden")));

struct				s_conversion
{
	int				conv;
	int				size;
	size_t			(*precalculate)(struct s_printformat *const printformat, struct s_percent *const percent);
	char			*(*transform)(char *dst, struct s_percent *const percent);
} __attribute__((packed,aligned(ALIGNMENT)));


size_t				precalculate_void(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_void(char *dst, struct s_percent *const percent);

size_t				precalculate_percent(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_percent(char *dst, struct s_percent *const percent);

size_t				precalculate_c(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_c(char *dst, struct s_percent *const percent);

size_t				precalculate_s(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_s(char *dst, struct s_percent *const percent);

size_t				precalculate_i(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_i(char *dst, struct s_percent *const percent);

size_t				precalculate_u(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_u(char *dst, struct s_percent *const percent);

size_t				precalculate_b(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_b(char *dst, struct s_percent *const percent);

size_t				precalculate_o(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_o(char *dst, struct s_percent *const percent);

size_t				precalculate_x(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_x(char *dst, struct s_percent *const percent);

size_t				precalculate_xx(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_xx(char *dst, struct s_percent *const percent);

size_t				precalculate_w(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_w(char *dst, struct s_percent *const percent);

size_t				precalculate_p(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_p(char *dst, struct s_percent *const percent);

size_t				precalculate_e(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_e(char *dst, struct s_percent *const percent);

size_t				precalculate_ee(struct s_printformat *const printformat, struct s_percent *const percent);
char				*transform_ee(char *dst, struct s_percent *const percent);


#endif
