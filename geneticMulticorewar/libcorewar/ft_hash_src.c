#include "./includes/libcorewar.h"

unsigned char		ft_hash_src(const char *src, size_t size)
{
	unsigned char	c;

	c = 0x42;
	while (size--)
		c = (c << 1) ^ *src++;
	return (c);
}

uint32_t			ft_atouint32(const char *src, char **const error)
{
	uint32_t		r;

	r = 0;
	if (*src == '-')
		return (ft_asprintf(error, "number cannot be negative"));
	while (*src >= '0' && *src <= '9')
	{
		if (!multi_issafe(r, 10))
			return (ft_asprintf(error, "number overflow(*) must be between %u and %u", 0, UINT32_MAX));
		r *= 10;
		if (!add_issafe(r, (int)*src - '0'))
			return (ft_asprintf(error, "number overflow(+) must be between %u and %u", 0, UINT32_MAX));
		r += (int)*src++ - '0';
	}
	return (r);
}
