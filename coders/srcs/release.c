#include "../include/prototype.h"
#include <stdlib.h>

void	ft_destroy_dongle(int i, t_data *data)
{
	int j;
	j = 0;

	while (j < i)
	{
		pthread_mutex_destroy(&data->dongles[j].mutex);
		j++;
	}
	free(data->dongles);
}


void	ft_release(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	free(data->dongles);
	free(data->coders);
}