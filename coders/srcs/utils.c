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

void	ft_print_status(t_data *data, int id, char *status)
{
	long	time;

	pthread_mutex_lock(&data->print_mutex);
	time = ft_get_time() - data->start_time;
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
