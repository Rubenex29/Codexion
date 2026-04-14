/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:05:00 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/14 11:09:40 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_coder_mutexes(t_data *data)
{
	int	i;

	if (!data->coders)
		return ;
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_destroy(&data->coders[i].last_compile_mutex);
		pthread_mutex_destroy(&data->coders[i].compiles_mutex);
		i++;
	}
}

static void	destroy_dongle_mutexes(t_data *data)
{
	int	i;

	if (!data->dongles)
		return ;
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
}

void	cleanup_data(t_data *data)
{
	if (!data)
		return ;
	destroy_coder_mutexes(data);
	destroy_dongle_mutexes(data);
	if (data->scheduler_type == 0)
	{
		pthread_mutex_destroy(&data->waiting_fifo.mtx);
		pthread_cond_destroy(&data->waiting_fifo.cond);
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->thread_ids);
	free(data->coders);
	free(data->dongles);
}
