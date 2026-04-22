#include "../include/prototype.h"
#include <sys/time.h>
#include <stddef.h>

long	ft_get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}