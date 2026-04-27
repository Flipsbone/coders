/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:24:45 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 18:34:26 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <string.h>
#include <stdio.h>

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

static int	ft_check_fifo(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue[0] == coder->id)
		return (1);
	return (0);
}

static int	ft_check_edf(t_dongle *dongle, t_coder *coder)
{
	int		index;
	long	current_deadline;
	long	other_deadline;
	t_coder	*other_coder;

	current_deadline = coder->last_compile_start + coder->data->time_to_burnout;
	index = 0;
	while (index < dongle->queue_size)
	{
		if (dongle->queue[index] != coder->id)
		{
			other_coder = &coder->data->coders[dongle->queue[index] - 1];
			other_deadline = other_coder->last_compile_start 
				+ coder->data->time_to_burnout;
			if (other_deadline < current_deadline)
				return (0);
			if (other_deadline == current_deadline)
			{
				if (dongle->queue[index] < coder->id)
					return (0);
			}
		}
		index++;
	}
	return (1);
}

int	ft_is_my_turn(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size <= 1)
		return (1);
	if (strcmp(coder->data->scheduler, "fifo") == 0)
		return (ft_check_fifo(dongle, coder));
	return (ft_check_edf(dongle, coder));
}
