/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:37:43 by advacher          #+#    #+#             */
/*   Updated: 2026/04/28 10:41:21 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"

void	ft_lock_both_dongles(t_dongle *left, t_dongle *right)
{
	if (left->id < right->id)
	{
		pthread_mutex_lock(&left->mutex);
		pthread_mutex_lock(&right->mutex);
	}
	else
	{
		pthread_mutex_lock(&right->mutex);
		pthread_mutex_lock(&left->mutex);
	}
}

void	ft_unlock_both_dongles(t_dongle *left, t_dongle *right)
{
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_unlock(&right->mutex);
}

int	ft_can_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_available == false)
		return (0);
	if (ft_get_time() < dongle->available_at)
		return (0);
	if (ft_is_my_turn(dongle, coder) == 0)
		return (0);
	return (1);
}

void	ft_wait_for_dongle(t_dongle *target)
{
	struct timespec	wait_until;
	long			time_to_wake;

	time_to_wake = ft_get_time() + 2;
	wait_until.tv_sec = time_to_wake / 1000;
	wait_until.tv_nsec = (time_to_wake % 1000) * 1000000;
	pthread_mutex_lock(&target->mutex);
	pthread_cond_timedwait(&target->cond, &target->mutex, &wait_until);
	pthread_mutex_unlock(&target->mutex);
}
