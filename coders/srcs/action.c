#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>


void	ft_drop_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
	{
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}


int	ft_check_simulation_stop(t_data *data)
{
	int	status;

    pthread_mutex_lock(&data->stop_mutex);
    status = data->stop_simulation;
    pthread_mutex_unlock(&data->stop_mutex);
	
	return (status);
}


void	ft_take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long 		timestamp;

	if (coder->left_dongle == coder->right_dongle)
	{
		timestamp = ft_get_time() - coder->data->start_time;
		pthread_mutex_lock(&coder->left_dongle->mutex);
    	printf("%ld %d has taken a dongle\n", timestamp, coder->id);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		return;
	}

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

	timestamp = ft_get_time() - coder->data->start_time;

    pthread_mutex_lock(&first->mutex);
    printf("%ld %d has taken a dongle\n", timestamp, coder->id);

    pthread_mutex_lock(&second->mutex);
	printf("%ld %d has taken a dongle\n", timestamp, coder->id);
}