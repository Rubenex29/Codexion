/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:45:14 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/14 10:20:46 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_for_turn(t_coder *coder)
{
	if (coder->data->scheduler_type == 0)
	{
		fifo_lock(&coder->data->waiting_fifo);
		return (1);
	}
	if (!is_closest_to_burnout(coder, coder->data))
	{
		usleep(500);
		return (0);
	}
	return (1);
}

static int	run_step(t_coder *coder, void (*action)(t_coder *))
{
	if (get_stop_mutex(coder->data) || all_coders_completed(coder->data))
		return (1);
	action(coder);
	if (get_stop_mutex(coder->data) || all_coders_completed(coder->data))
		return (1);
	return (0);
}

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

static int	run_cycle(t_coder *coder)
{
	int	use_fifo;
	int	has_dongles;

	if (all_coders_completed(coder->data))
		return (1);
	use_fifo = (coder->data->scheduler_type == 0);
	if (!wait_for_turn(coder))
		return (0);
	if (all_coders_completed(coder->data))
	{
		if (use_fifo)
			fifo_unlock(&coder->data->waiting_fifo);
		return (1);
	}
	has_dongles = take_compile_dongles(coder);
	if (use_fifo)
		fifo_unlock(&coder->data->waiting_fifo);
	if (!has_dongles)
		return (1);
	if (run_step(coder, compile) || run_step(coder, debug)
		|| run_step(coder, refactor))
		return (1);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->num_coders == 1)
	{
		pthread_mutex_lock(&coder->right_dongle->mutex);
		write_status("has taken a dongle", coder);
		ft_usleep(coder->data->time_to_burnout);
		pthread_mutex_unlock(&coder->right_dongle->mutex);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		usleep(1000);
	while (!get_stop_mutex(coder->data) && !all_coders_completed(coder->data))
	{
		if (run_cycle(coder))
			break ;
	}
	return (NULL);
}
