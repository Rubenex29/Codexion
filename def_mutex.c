/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_mutex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:15:09 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/07 16:49:05 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_sim_stop(t_coder *coder)
{
	int	status;

	pthread_mutex_lock(&coder->data->log_mutex);
	status = coder->data->simulation_stop;
	pthread_mutex_unlock(&coder->data->log_mutex);
	return (status);
}

void	stop_mutex(t_data *data)
{
	pthread_mutex_lock(&data->log_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->log_mutex);
}

void compile_mutex(t_coder *coder)
{
	init_compile_mutex(coder);
	pthread_mutex_lock(&coder->last_compile_mutex);
	pthread_mutex_lock(&coder->compiles_mutex);
	coder->last_compile_time = get_time_in_ms();
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->last_compile_mutex);
	pthread_mutex_unlock(&coder->compiles_mutex);
}

int get_stop_mutex(t_data *data)
{
	int stop;
    pthread_mutex_lock(&data->log_mutex);
    stop = data->simulation_stop;
    pthread_mutex_unlock(&data->log_mutex);
    return stop;
}
