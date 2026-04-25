#define _DEFAULT_SOURCE
#include "../include/prototype.h"
#include "../include/struct.h"
#include <unistd.h>

static void	ft_stop_all(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->sim_mutex);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->sim_mutex);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

static void	ft_check_status(t_data *data)
{
	int		i;
	int		finished;
	long	now;

	i = 0;
	finished = 0;
	now = ft_get_time();

	pthread_mutex_lock(&data->sim_mutex);
	while (i < data->number_of_coders)
	{
		if (now - data->coders[i].last_compile_start > data->time_to_burnout)
		{
			pthread_mutex_unlock(&data->sim_mutex); 
			ft_stop_all(data);
			ft_print_status(data, data->coders[i].id, "burned out");
			return ;
		}
		if (data->coders[i].nb_compiles >= data->number_of_compiles_required)
			finished++;
		i++;
	}
	if (finished == data->number_of_coders)
	{
		pthread_mutex_unlock(&data->sim_mutex);
		ft_stop_all(data);
		return ;
	}
	pthread_mutex_unlock(&data->sim_mutex);
}

void	*ft_monitor_routine(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	pthread_mutex_lock(&data->sim_mutex);
	while (data->is_ready == 0)
		pthread_cond_wait(&data->start_cond, &data->sim_mutex);
	if (data->is_ready == -1)
	{
		pthread_mutex_unlock(&data->sim_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&data->sim_mutex);
	while (ft_check_simulation_stop(data) == 0)
	{
		ft_check_status(data);
		usleep(1000);
	}
	return (NULL);
}