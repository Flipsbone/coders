/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:25:15 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 17:58:37 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>
#include <unistd.h>

static int	ft_wait_for_start(t_data *data)
{
	pthread_mutex_lock(&data->sim_mutex);
	while (data->is_ready == 0)
		pthread_cond_wait(&data->start_cond, &data->sim_mutex);
	if (data->is_ready == -1)
	{
		pthread_mutex_unlock(&data->sim_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&data->sim_mutex);
	return (0);
}

static void	*ft_coder_routine(void *thread)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)thread;
	data = coder->data;
	if (ft_wait_for_start(data) == -1)
		return (NULL);
	while (ft_check_simulation_stop(data) == 0)
	{
		if (ft_take_dongles(coder) == 0)
		{
			pthread_mutex_lock(&data->sim_mutex);
			coder->last_compile_start = ft_get_time();
			coder->nb_compiles++;
			pthread_mutex_unlock(&data->sim_mutex);
			ft_print_status(data, coder->id, "is compiling");
			usleep(data->time_to_compile * 1000);
			ft_drop_dongles(coder);
			ft_print_status(data, coder->id, "is debugging");
			usleep(data->time_to_debug * 1000);
			ft_print_status(data, coder->id, "is refactoring");
			usleep(data->time_to_refactor * 1000);
		}
	}
	return (NULL);
}

int	ft_finish_simulation(t_data *data)
{
	int	i;

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

static int	ft_create_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread_id, NULL, &ft_coder_routine,
				&data->coders[i]) != 0)
		{
			fprintf(stderr, "Error: Failed to create thread %d\n", i);
			pthread_mutex_lock(&data->sim_mutex);
			data->is_ready = -1;
			pthread_cond_broadcast(&data->start_cond);
			pthread_mutex_unlock(&data->sim_mutex);
			while (--i >= 0)
			{
				if (pthread_join(data->coders[i].thread_id, NULL) != 0 )
				{
					fprintf(stderr, "Error: Failed to join thread coders %d\n", i);
					return (-1);
				}
			}
			return (-1);
		}
		i++;
	}
	return (0);
}

int	ft_start_simulation(t_data *data)
{
	int	i;
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
	data->start_time = ft_get_time();
	data->is_ready = 1;
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].last_compile_start = data->start_time; 
    	i++;
	}
	pthread_cond_broadcast(&data->start_cond);
	pthread_mutex_unlock(&data->sim_mutex);
	if (ft_finish_simulation(data) == -1)
		return (-1);
	if (pthread_join(data->monitor, NULL) != 0)
	{
    	fprintf(stderr, "Error: Failed to join monitor thread\n");
		return (-1);
	}

	return (0);
}
