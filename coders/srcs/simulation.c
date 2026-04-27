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

static void	ft_set_start_time(t_data *data)
{
	int	i;

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
}

int	ft_start_simulation(t_data *data)
{
	void	*monitor_return;

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
	ft_set_start_time(data);
	if (ft_finish_simulation(data) == -1)
		return (-1);
	if (pthread_join(data->monitor, &monitor_return) != 0
		|| monitor_return != NULL)
	{
		fprintf(stderr, "Error: Monitor join failed or runtime error\n");
		return (-1);
	}
	return (0);
}
