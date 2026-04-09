/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_mutex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:15:09 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/09 10:04:10 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop_mutex(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}

void	compile_mutex(t_coder *coder)
{
	init_compile_mutex(coder);
	pthread_mutex_lock(&coder->last_compile_mutex);
	coder->last_compile_time = get_time_in_ms();
	pthread_mutex_unlock(&coder->last_compile_mutex);
}

int	get_stop_mutex(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->simulation_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}
