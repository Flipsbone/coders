/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_dongle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:35:17 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 18:35:55 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _DEFAULT_SOURCE
#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>
#include <unistd.h>


static void ft_wait_cooldown(t_dongle *dongle)
{
    struct timespec wait_until;
    
    wait_until.tv_sec = dongle->available_at / 1000;
    wait_until.tv_nsec = (dongle->available_at % 1000) * 1000000;
    pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &wait_until);
}

static void	ft_drop_one_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = true;
	dongle->available_at = ft_get_time() + coder->data->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	ft_drop_dongles(t_coder *coder)
{
	ft_drop_one_dongle(coder, coder->left_dongle);
	if (coder->left_dongle == coder->right_dongle)
		return ;
	ft_drop_one_dongle(coder, coder->right_dongle);
}

static int	ft_should_wait(t_dongle *dongle, t_coder *coder)
{
	if (ft_check_simulation_stop(coder->data))
		return (0);
	if (dongle->is_available == false)
		return (1);
	if (!ft_is_my_turn(dongle, coder))
		return (1);
	if (ft_get_time() < dongle->available_at)
		return (2);
	return (0);
}

int	ft_check_simulation_stop(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->sim_mutex);
	status = data->stop_simulation;
	pthread_mutex_unlock(&data->sim_mutex);
	return (status);
}

static void ft_take_one_dongle(t_coder *coder, t_dongle *dongle)
{
    int wait_status;

    pthread_mutex_lock(&dongle->mutex);
    ft_add_to_queue(dongle, coder);
    wait_status = ft_should_wait(dongle, coder);
    while (wait_status != 0)
    {
        if (wait_status == 1)
            pthread_cond_wait(&dongle->cond, &dongle->mutex);
        else if (wait_status == 2)
            ft_wait_cooldown(dongle);
        wait_status = ft_should_wait(dongle, coder);
    }
    if (!ft_check_simulation_stop(coder->data))
    {
        dongle->is_available = false;
        ft_print_status(coder->data, coder->id, "has taken a dongle");
    }
    ft_remove_from_queue(dongle, coder);
    pthread_mutex_unlock(&dongle->mutex);
}

int	ft_take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

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
	if (ft_check_simulation_stop(coder->data))
		return (-1);
	if (first == second)
	{
		while (!ft_check_simulation_stop(coder->data))
        	usleep(1000);
		return (-1);
	}
	ft_take_one_dongle(coder, second);
	if (ft_check_simulation_stop(coder->data))
	{
		ft_drop_one_dongle(coder, first);
		return (-1);
	}
	return (0);
}
