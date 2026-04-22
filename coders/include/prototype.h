/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:36:01 by advacher          #+#    #+#             */
/*   Updated: 2026/04/22 15:31:30 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "struct.h"

int		ft_parse_arguments(int ac, char **av, t_data *data);
int		ft_init_variables(t_data *data);
void	ft_release(t_data *data);
int	    ft_start_simulation(t_data *data);

#endif