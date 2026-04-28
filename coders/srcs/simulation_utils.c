/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:39:25 by advacher          #+#    #+#             */
/*   Updated: 2026/04/28 15:02:47 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
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

void	*ft_coder_routine(void *thread)
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
