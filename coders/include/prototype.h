/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:36:01 by advacher          #+#    #+#             */
/*   Updated: 2026/04/24 18:38:12 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "struct.h"

int		ft_parse_arguments(int ac, char **av, t_data *data);
int		ft_init_variables(t_data *data);
int		ft_start_simulation(t_data *data);
void	*ft_monitor_routine(void *param);
long	ft_get_time(void);
void	ft_print_status(t_data *data, int id, char *status);
int		ft_init_dongles(t_data *data);

void	ft_add_to_queue(t_dongle *dongle, t_coder *coder);
void	ft_remove_from_queue(t_dongle *dongle, t_coder *coder);
void	ft_take_dongles(t_coder *coder);
int		ft_check_simulation_stop(t_data *data);
void	ft_drop_dongles(t_coder *coder);
int		ft_is_my_turn(t_dongle *dongle, t_coder *coder);

void	ft_release_coders(t_data *data);
void	ft_release_dongle(int i, t_data *data);
void	ft_release_simulation(t_data *data);
void	ft_release_all(t_data *data);

#endif