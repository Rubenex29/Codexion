/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:45:14 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/08 12:00:19 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_thread(t_data *data)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	while (i < data->num_coders)
	{
		pthread_create(&data->thread_ids[i], NULL,
			&coder_routine, &data->coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, data);
	i = 0;
	while (i < data->num_coders)
	{
		pthread_join(data->thread_ids[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->num_coders == 1)
	{
		pthread_mutex_lock(coder->right_dongle);
		write_status("has taken a dongle", coder);
		ft_usleep(coder->data->time_to_burnout);
		pthread_mutex_unlock(coder->right_dongle);
		write_status("burned out", coder);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		usleep(1000);
	while (!check_sim_stop(coder))
	{
		compile(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

static int	should_stop_monitor(t_data *data, int i, long long last)
{
	if (get_time_in_ms() - last > data->time_to_burnout)
	{
		write_status("burned out", &data->coders[i]);
		stop_mutex(data);
		return (1);
	}
	if (data->coders[i].compiles_done >= data->compiles_required)
	{
		stop_mutex(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data			*data;
	int				i;
	long long		last;

	data = (t_data *)arg;
	while (!get_stop_mutex(data))
	{
		i = 0;
		while (i < data->num_coders)
		{
			pthread_mutex_init(&data->coders[i].last_compile_mutex, NULL);
			pthread_mutex_lock(&data->coders[i].last_compile_mutex);
			last = data->coders[i].last_compile_time;
			pthread_mutex_unlock(&data->coders[i].last_compile_mutex);
			if (should_stop_monitor(data, i, last))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
