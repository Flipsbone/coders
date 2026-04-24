/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:24:45 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 10:45:53 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/struct.h"

int ft_find_next_coder(t_dongle *dongle, t_data *data)
{
	int i;
	int best_index;

	if (dongle->queue_size == 0)
		return (-1);

	best_index = 0;
	i = 0;
	if (strcmp(data->scheduler, "edf") == 0)
	{
		while ( i != data->number_of_coders)
		{
			last_time = data->start_time - data->coders->last_compile_start;
			time_to_burnout = data->time_to_burnout - last_time;
			if (time_to_burnout > 0)
			{
				if (time_to_burnout < tmp)
				{
					best_index = i;
					tmp = time_to_burnout;
				}
			}
			else
			(
				data->coders->flag_burnout = 1;
			)
		}
	}
}