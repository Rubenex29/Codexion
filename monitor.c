/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/10 11:55:11 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	monitor_one_coder(t_data *data, int i)
{
	long long	last;

	pthread_mutex_lock(&data->coders[i].last_compile_mutex);
	last = data->coders[i].last_compile_time;
	pthread_mutex_unlock(&data->coders[i].last_compile_mutex);
	if (get_time_in_ms() - last > data->time_to_burnout)
	{
		write_status("burned out", &data->coders[i]);
		stop_mutex(data);
		return (1);
	}
	return (0);
}

static int	monitor_cycle(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		if (monitor_one_coder(data, i))
			return (1);
		i++;
	}
	if (all_coders_completed(data))
	{
		stop_mutex(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!get_stop_mutex(data))
	{
		if (monitor_cycle(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
