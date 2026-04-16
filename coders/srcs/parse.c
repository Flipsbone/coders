/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 10:12:52 by advacher          #+#    #+#             */
/*   Updated: 2026/04/16 17:11:45 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int ft_numeric_str(const char *str)
{
	long result;
	size_t i;

	i = 0;
	result = 0;

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
            return (-1);
		i++;
	}
	return result;
}

int parse_arguments(int ac, char **av)
{
	if (ac != 9)
	{
		fprintf(stderr, "Error args must be :\n");
		fprintf(stderr, "number_of_coders time_to_burnout time_to_compile "
			"time_to_debug time_to_refactor number_of_compiles_required "
			"dongle_cooldown scheduler\n");
		return(-1);
	}
	int i;
	i = 1;
	int tmp_args[7];
	while (av[i] && i <=7)
	{
		tmp_args[i - 1] = ft_numeric_str(av[i]);
		if (tmp_args[i-1] == -1)
		{
			fprintf(stderr, "Error args must be integer\n");
			return (-1);
		}
		i++;
	}
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
	{
		fprintf(stderr, "Error must be : edf, fifo\n");
		return (-1);
	}
	return (0);
}
