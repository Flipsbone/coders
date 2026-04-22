#include "../include/prototype.h"
#include <stdlib.h>

void	ft_release(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->coders);
	free(data->dongles);
}