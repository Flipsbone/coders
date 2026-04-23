#include "../include/prototype.h"
#include <stdlib.h>

void	ft_release_all(t_data *data)
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
	pthread_mutex_destroy(&data->sim_mutex);
	pthread_cond_destroy(&data->start_cond);
	pthread_mutex_destroy(&data->print_mutex);
}
void	ft_release_simulation(t_data *data)
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
	pthread_mutex_destroy(&data->sim_mutex);
}


void	ft_release_coders(t_data *data)
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
void	ft_release_dongle(int i, t_data *data)
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