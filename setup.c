/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:32:33 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/06 13:35:38 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_data(t_data *data, char **av)
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

static int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	data->dongles = malloc(sizeof(pthread_mutex_t) * data->num_coders);
	if (!data->dongles)
		return (0);
	while (i < data->num_coders)
	{
		pthread_mutex_init(&data->dongles[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->log_mutex, NULL);
	return (1);
}

static int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->num_coders);
	if (!data->coders)
		return (0);
	while (i < data->num_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles
		[(i + 1) % data->num_coders];
		data->coders[i].data = data;
		data->coders[i].last_compile_time = data->start_time;
		data->coders[i].compiles_done = 0;
		i++;
	}
	return (1);
}

int	init_all(t_data	*data, char **av)
{
	init_data(data, av);
	if (!init_dongles(data))
		return (0);
	if (!init_coders(data))
		return (0);
	return (1);
}
