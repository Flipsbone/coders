#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>

static  void	*ft_coder_routine(void *param)
{
    int i;
    t_coder	*coder;
    coder = (t_coder *)param;
    i = 0;

    while (i < coder->data->number_of_coders)
    {
        printf("Coder %d is ready to code!\n", coder->id);
        i++;
    }
	return (NULL);
}


static  int ft_finish_simulation(t_data *data)
{
    int i;
    i = 0;

    while (i < data->number_of_coders)
	{
        if (pthread_join(data->coders[i].thread_id, NULL) != 0)
		{
			fprintf(stderr, "Error: Failed to join thread coders %d\n", i);
			return (-1);
		}
		i++;
	}
    return (0);
}

int	ft_start_simulation(t_data *data)
{
    int i;
    i = 0;

    while (i < data->number_of_coders)
    {
        if (pthread_create(&data->coders[i].thread_id, NULL,
			&ft_coder_routine, &data->coders[i]) != 0)
		{
			fprintf(stderr, "Error: Failed to create thread %d\n", i);
			return (-1);
		}
        i++;
    }
    if (ft_finish_simulation(data) == -1)
        return (-1);
    return (0);
}
