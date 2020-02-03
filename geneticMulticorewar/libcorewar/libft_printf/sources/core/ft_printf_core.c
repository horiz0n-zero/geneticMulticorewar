/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 08:59:55 by afeuerst          #+#    #+#             */
/*   Updated: 2020/01/13 13:44:35 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf_core.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winitializer-overrides"
static const struct s_conversion	g_conversions[256] =
{
	[0 ... 255] = {0, 0, precalculate_void, transform_void},
	['%'] = {0, 0, precalculate_percent, transform_percent},
	['E'] = {CONV_EE, SIZE_CHAR, precalculate_ee, transform_ee},
	['X'] = {CONV_XX, SIZE_INT, precalculate_xx, transform_xx},
	['b'] = {CONV_B, SIZE_INT, precalculate_b, transform_b},
	['c'] = {CONV_C, SIZE_CHAR, precalculate_c, transform_c},
	['d'] = {CONV_I, SIZE_INT, precalculate_i, transform_i},
	['e'] = {CONV_E, SIZE_CHAR, precalculate_e, transform_e},
	['i'] = {CONV_I, SIZE_INT, precalculate_i, transform_i},
	['o'] = {CONV_O, SIZE_INT, precalculate_o, transform_o},
	['p'] = {CONV_P, SIZE_LONG, precalculate_p, transform_p},
	['s'] = {CONV_S, SIZE_LONG, precalculate_s, transform_s},
	['u'] = {CONV_U, SIZE_INT, precalculate_u, transform_u},
	['w'] = {CONV_W, SIZE_INT, precalculate_w, transform_w},
	['x'] = {CONV_X, SIZE_INT, precalculate_x, transform_x}
};
#pragma clang diagnostic pop

static const int					g_ft_printf_core_flags[256] =
{
	['+'] = FLAGS_PLUS,
	['-'] = FLAGS_MINUS,
	['0'] = FLAGS_ZERO,
	['#'] = FLAGS_HASHTAG
};

static const char		*ft_printf_core_get_number(struct s_printformat *const printformat,  const char *format, int *const number)
{
	if (*format == '*')
	{
		*number = va_arg(*printformat->args, int);
		return (++format);
	}
	*number = 0;
	if (*format >= '0' && *format <= '9')
	{
		while (*format >= '0' && *format <= '9')
			*number = *number * 10 + (*format++ - '0');
	}
	return (format);
}

static const char		*ft_printf_core_fill_percent_value(struct s_printformat *const printformat, const char *format, struct s_percent *const percent)
{
	if (percent->size & SIZE_LONG)
		percent->data = (unsigned long)va_arg(*printformat->args, long);
	else if (percent->size & (SIZE_SHORT | SIZE_CHAR))
		percent->data = ((unsigned long)va_arg(*printformat->args, int)) & (0xFFFFFF >> percent->size * CHAR_BIT);
	else if (percent->size & SIZE_INT)
		percent->data = (unsigned long)va_arg(*printformat->args, int);
	printformat->length += percent->conversion->precalculate(printformat, percent);
	return (format);
}

static const char		*ft_printf_core_fill_percent(struct s_printformat *const printformat, const char *format, struct s_percent *percent)
{
	percent->flags = 0;
	while (g_ft_printf_core_flags[(int)*format])
		percent->flags |= g_ft_printf_core_flags[(int)*format++];
	format = ft_printf_core_get_number(printformat, format, &percent->width);
	if (*format == '.')
		format = ft_printf_core_get_number(printformat, ++format, &percent->precision);
	else
		percent->precision = 0;
	if (*format == 'l')
		(percent->size = SIZE_LONG) && ++format;
	else if (*format == 'h')
		if (*++format == 'h')
			(percent->size = SIZE_CHAR) && ++format;
		else
			(percent->size = SIZE_SHORT);
	else
		percent->size = 0;
	if (*format == '[')
		return (ft_printf_array_prepare(printformat, ++format, percent));
	if (*format)
		percent->conversion = g_conversions + *format++;
	else
		percent->conversion = (const struct s_conversion*)g_conversions;
	percent->size |= percent->conversion->size;
	percent->format = format;
	return (ft_printf_core_fill_percent_value(printformat, format, percent));
}

void					ft_printf_core(struct s_printformat *const printformat, const char *format, struct s_percent **const next)
{
	struct s_percent	percent;

	while (*format && *format != '%')
	{
		++format;
		++printformat->length;
	}
	if (*format == '%')
	{
		*next = &percent;
		format = ft_printf_core_fill_percent(printformat, ++format, &percent);
		if (percent.flags & FLAGS_ISARRAY)
			return ;
		else
			return (ft_printf_core(printformat, format, &percent.next));
	}
	else
	{
		*next = NULL;
		ft_printf_core_process(printformat, printformat->format);
	}
}
