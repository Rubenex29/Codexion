/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:24:21 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/07 16:15:07 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time_in_ms)
{
	long long	start_time;

	start_time = get_time_in_ms();
	while ((get_time_in_ms() - start_time) < time_in_ms)
		usleep(500);
}

void	write_status(char *msg, t_coder *coder)
{
	long long	now;

	if (coder->data->simulation_stop)
		return ;
	pthread_mutex_lock(&coder->data->log_mutex);
	now = get_time_in_ms() - coder->data->start_time;
	printf("%lld %d %s\n", now, coder->id, msg);
	pthread_mutex_unlock(&coder->data->log_mutex);
}
