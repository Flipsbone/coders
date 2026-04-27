/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:25:21 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 18:37:15 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

int ft_check_simulation_stop(t_data *data)
{
    int status;
    
    pthread_mutex_lock(&data->sim_mutex);
    status = data->stop_simulation;
    pthread_mutex_unlock(&data->sim_mutex);
    return (status);
}

void	ft_print_status(t_data *data, int id, char *status)
{
	long	time;
	int		stop;
	long	start;

	pthread_mutex_lock(&data->sim_mutex);
	stop = data->stop_simulation;
	start = data->start_time;
	pthread_mutex_unlock(&data->sim_mutex);
	pthread_mutex_lock(&data->print_mutex);
	if (stop && strcmp(status, "burned out") != 0)
	{
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	time = ft_get_time() - start;
	printf("%ld %d %s\n", time, id, status);
	pthread_mutex_unlock(&data->print_mutex);
}

long	ft_get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
