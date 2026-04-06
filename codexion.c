/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:05:30 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/06 11:57:15 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_data(t_data *data, char **av)
{
	int				error;
	struct timeval	tv;

	error = 0;
	data->num_coders = (int)string_to_long_safe(av[1], &error);
	data->time_to_burnout = (int)string_to_long_safe(av[2], &error);
	data->time_to_compile = (int)string_to_long_safe(av[3], &error);
	data->time_to_debug = (int)string_to_long_safe(av[4], &error);
	data->time_to_refactor = (int)string_to_long_safe(av[5], &error);
	data->compiles_required = (int)string_to_long_safe(av[6], &error);
	data->dongle_cooldown = (int)string_to_long_safe(av[7], &error);
	if (strcmp(av[8], "fifo") == 0)
		data->scheduler_type = 0;
	else
		data->scheduler_type = 1;
	data->simulation_stop = 0;
	gettimeofday(&tv, NULL);
	data->start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int	main(int ac, char **av)
{
	t_coder		*a;
	t_data		*data;

	if (ac != 9)
		return (0);
	if (!parser(ac, av))
		return (0);
	init_data(data, av);
	return (0);
}
