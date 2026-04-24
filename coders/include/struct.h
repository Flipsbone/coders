/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 15:35:30 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 18:01:36 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>

typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct s_data
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	char				*scheduler;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor;
	int					is_ready;
	pthread_mutex_t		sim_mutex;
	bool				stop_simulation;
	pthread_cond_t		start_cond;
	long				start_time;
	pthread_mutex_t		print_mutex;
}						t_data;

struct					s_coder
{
	int					id;
	int					nb_compiles;
	long				last_compile_start;
	pthread_t			thread_id;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_data				*data;
};

struct					s_dongle
{
	int					id;
	pthread_mutex_t		mutex;
	pthread_cond_t  	cond;
	bool				is_available;
	long				available_at;
	int					queue[2];
	int					queue_size;

};

#endif
