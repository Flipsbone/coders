/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:38:49 by advacher          #+#    #+#             */
/*   Updated: 2026/04/28 10:38:50 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"

void	ft_add_to_queue(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size < 2)
	{
		dongle->queue[dongle->queue_size] = coder->id;
		dongle->queue_size++;
	}
}

void	ft_remove_from_queue(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size == 0)
		return ;
	if (dongle->queue[0] == coder->id)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = 0;
		dongle->queue_size--;
	}
	else if (dongle->queue_size == 2 && dongle->queue[1] == coder->id)
	{
		dongle->queue[1] = 0;
		dongle->queue_size--;
	}
}

long	ft_get_last_compile(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->data->sim_mutex);
	time = coder->last_compile_start;
	pthread_mutex_unlock(&coder->data->sim_mutex);
	return (time);
}
