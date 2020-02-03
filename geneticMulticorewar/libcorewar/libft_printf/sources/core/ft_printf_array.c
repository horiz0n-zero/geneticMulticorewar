/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 13:16:00 by afeuerst          #+#    #+#             */
/*   Updated: 2020/01/13 13:07:50 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf_array.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winitializer-overrides"
static const struct s_conversion	g_conversions[256] =
{
	[0 ... 255] = {0, 0, precalculate_void, transform_void},
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

static const struct s_array_conv	g_array_conv[] =
{
	[CONV_VOID ... CONV_EE] = {(void*)ft_printf_array_loop_void_count, (void*)ft_printf_array_loop_void},
	[CONV_U] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_I] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_B] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_O] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_X] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_XX] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_W] = {(void*)ft_printf_array_loop_int_count, (void*)ft_printf_array_loop_int},
	[CONV_C] = {(void*)ft_printf_array_loop_char_count, (void*)ft_printf_array_loop_char},
	[CONV_S] = {(void*)ft_printf_array_loop_stars_count, (void*)ft_printf_array_loop_stars},
	[CONV_P] = {(void*)ft_printf_array_loop_stars_count, (void*)ft_printf_array_loop_stars},
	[CONV_E] = {(void*)ft_printf_array_loop_char_count, (void*)ft_printf_array_loop_char},
	[CONV_EE] = {(void*)ft_printf_array_loop_char_count, (void*)ft_printf_array_loop_char}
};
#pragma clang diagnostic pop

static const char	*ft_printf_array_prepare_flags(struct s_printformat *const printformat, const char *format, struct s_percent *const percent)
{
	if (*format == '*' && ++format)
		percent->r3 = (size_t)va_arg(*printformat->args, int);
	else if (*format >= '0' && *format <= '9')
	{
		percent->r3 = 0;
		while (*format >= '0' && *format <= '9')
			percent->r3 = percent->r3 * 10 + (*format++ - '0');
	}
	else
		percent->flags |= FLAGS_ARRAY_REQUIRED_COUNT;
	return (format);
}

// r1 -> start format output
// r2 -> output length
// r3 -> array count
const char							*ft_printf_array_prepare(struct s_printformat *const printformat, const char *format, struct s_percent *const percent)
{
	const struct s_array_conv		*conv;

	percent->r2 = 0;
	percent->r3 = 0;
	format = ft_printf_array_prepare_flags(printformat, format, percent);
	percent->r1 = (unsigned long)format;
	while (*format && *format != ']')
	{
		++percent->r2;
		++format;
	}
	if (*format == ']' && *++format)
		percent->conversion = g_conversions + *format++;
	else
		percent->conversion = (const struct s_conversion*)g_conversions;
	percent->size |= percent->conversion->size;
	conv = g_array_conv + percent->conversion->conv;
	if (*format == ']')
		; // double array
	percent->flags |= FLAGS_ISARRAY;
	percent->format = format;
	percent->data = (unsigned long)va_arg(*printformat->args, void*);

	if (percent->size & SIZE_LONG)
	{
		if (percent->flags & FLAGS_ARRAY_REQUIRED_COUNT)
			percent->r3 += ft_printf_array_loop_long_count((const long*)percent->data);
		return (ft_printf_array_loop_long(printformat, percent, &percent->array, (const long*)percent->data));
	}
	else if (percent->size & SIZE_SHORT)
	{
		if (percent->flags & FLAGS_ARRAY_REQUIRED_COUNT)
			percent->r3 += ft_printf_array_loop_short_count((const short*)percent->data);
		return (ft_printf_array_loop_short(printformat, percent, &percent->array, (const short*)percent->data));
	}
	else if (percent->size & SIZE_CHAR)
	{
		if (percent->flags & FLAGS_ARRAY_REQUIRED_COUNT)
			percent->r3 += ft_printf_array_loop_char_count((const char*)percent->data);
		return (ft_printf_array_loop_char(printformat, percent, &percent->array, (const char*)percent->data));
	}
	if (percent->flags & FLAGS_ARRAY_REQUIRED_COUNT)
		percent->r3 = conv->loop_count((const void*)percent->data);
	return (conv->loop(printformat, percent, &percent->array, (const void*)percent->data));
}









