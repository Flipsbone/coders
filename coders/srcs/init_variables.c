#include "../include/struct.h"
#include <stdlib.h>
#include <string.h>

static int	ft_init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (-1);
	memset(data->coders, 0, sizeof(t_coder) * data->number_of_coders);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].data = data;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = (&data->dongles[(i + 1)
				% data->number_of_coders]);
		i++;
	}
	return (0);
}
static int	ft_init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (-1);
	memset(data->dongles, 0, sizeof(t_dongle) * data->number_of_coders);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].id = i + 1;
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		i++;
	}
	return (0);
}

int	ft_init_variables(t_data *data)
{
	if (ft_init_dongles(data) == -1)
		return (-1);
	if (ft_init_coders(data) == -1)
		return (-1);
	return (0);
}
