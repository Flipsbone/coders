/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:25:00 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 18:36:55 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include <stdio.h>

void	*ft_monitor_routine(void *param)
{
	t_data	*data;

	data = (t_data *)param;
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
	printf("Monitor is now watching...\n");
	return (NULL);
}
