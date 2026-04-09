/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:32:33 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/09 12:05:40 by rumontei         ###   ########.fr       */
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

static t_dongle	*init_dongles(int num_coders)
{
	t_dongle	*dongles;
	int			i;

	dongles = malloc(sizeof(t_dongle) * num_coders);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < num_coders)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		dongles[i].last_use = 0;
		i++;
	}
	return (dongles);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->num_coders);
	data->thread_ids = malloc(sizeof(pthread_t) * data->num_coders);
	data->dongles = init_dongles(data->num_coders);
	if (!data->coders || !data->thread_ids || !data->dongles)
		return (0);
	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[
			(i + 1) % data->num_coders];
		data->coders[i].data = data;
		data->coders[i].last_compile_time = data->start_time;
		data->coders[i].compiles_done = 0;
		pthread_mutex_init(&data->coders[i].last_compile_mutex, NULL);
		pthread_mutex_init(&data->coders[i].compiles_mutex, NULL);
		i++;
	}
	return (1);
}

void	init_compile_mutex(t_coder *coder)
{
	pthread_mutex_init(&coder->last_compile_mutex, NULL);
	pthread_mutex_init(&coder->compiles_mutex, NULL);
}

int	init_all(t_data	*data, char **av)
{
	init_data(data, av);
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	if (!init_coders(data))
		return (0);
	return (1);
}
