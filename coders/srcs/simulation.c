#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>


static  void	*ft_coder_routine(void *thread)
{
    t_coder	*coder;
    t_data  *data;

    coder = (t_coder *)thread;
    data = coder->data;

    pthread_mutex_lock(&data->sim_mutex);
    while (data->is_ready == 0)
    {
        pthread_cond_wait(&data->start_cond, &data->sim_mutex);
    }
    if (data->is_ready == -1)
    {
        pthread_mutex_unlock(&data->sim_mutex);
        return (NULL);
    }
    pthread_mutex_unlock(&data->sim_mutex);
    ft_print_status(data, coder->id, "is ready to code!");
	return (NULL);
}

static  int ft_finish_simulation(t_data *data)
{
    int i;
    i = 0;

    while (i < data->number_of_coders)
	{
        if (pthread_join(data->coders[i].thread_id, NULL) != 0)
		{
			fprintf(stderr, "Error: Failed to join thread coders %d\n", i);
			return (-1);
		}
		i++;
	}
    return (0);
}

static  int ft_create_coders(t_data *data)
{
    int i;
    i = 0;

    while (i < data->number_of_coders)
    {
        if (pthread_create(&data->coders[i].thread_id, NULL,
			&ft_coder_routine, &data->coders[i]) != 0)
		{

			fprintf(stderr, "Error: Failed to create thread %d\n", i);
            pthread_mutex_lock(&data->sim_mutex);
            data->is_ready = -1;
            pthread_cond_broadcast(&data->start_cond);
            pthread_mutex_unlock(&data->sim_mutex);
            while (--i >= 0)
            {
                pthread_join(data->coders[i].thread_id, NULL);
            }
			return (-1);
		}
        i++;
    }
    return (0);
}

int	ft_start_simulation(t_data *data)
{
    if (ft_create_coders(data) == -1)
        return (-1);

    if (pthread_create(&data->monitor, NULL, &ft_monitor_routine, data) != 0)
    {
        pthread_mutex_lock(&data->sim_mutex);
        data->is_ready = -1;
        pthread_cond_broadcast(&data->start_cond);
        pthread_mutex_unlock(&data->sim_mutex);
        ft_finish_simulation(data);
        return (-1);
    }

    pthread_mutex_lock(&data->sim_mutex);
    data->is_ready = 1;
    data->start_time = ft_get_time();
    pthread_cond_broadcast(&data->start_cond);
    pthread_mutex_unlock(&data->sim_mutex);

    if (ft_finish_simulation(data) == -1)
        return (-1);
    if (pthread_join(data->monitor, NULL) != 0)
        return (-1);
    return (0);
}
