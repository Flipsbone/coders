/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_dongle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:35:17 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 17:18:47 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>

void ft_drop_dongles(t_coder *coder)
{
	ft_drop_one_dongle(coder, coder->left_dongle);
	if (coder->left_dongle == coder->right_dongle)
		return ;
	ft_drop_one_dongle(coder, coder->right_dongle);
}

static void ft_drop_one_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = true;
	dongle->available_at = ft_get_time() + coder->data->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}


int	ft_check_simulation_stop(t_data *data)
{
	int	status;

    pthread_mutex_lock(&data->stop_mutex);
    status = data->stop_simulation;
    pthread_mutex_unlock(&data->stop_mutex);

	return (status);
}
static void	ft_take_one_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	ft_add_to_queue(dongle, coder);

	while (dongle->is_available == false || !ft_is_my_turn(dongle, coder)
		|| ft_get_time() < dongle->available_at)
	{
		if (dongle->is_available == true && ft_is_my_turn(dongle, coder))
		{
			pthread_mutex_unlock(&dongle->mutex);
			usleep(100);
			pthread_mutex_lock(&dongle->mutex);
		}
		else
		{
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		}
	}
	dongle->is_available = false;
	ft_remove_from_queue(dongle, coder);
	ft_print_status(coder->data, coder->id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
}

void ft_take_dongles(t_coder *coder)
{
    t_dongle *first;
    t_dongle *second;

    if (coder->left_dongle->id < coder->right_dongle->id)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }

    ft_take_one_dongle(coder, first);

    if (first == second)
        return ;

    ft_take_one_dongle(coder, second);
}
