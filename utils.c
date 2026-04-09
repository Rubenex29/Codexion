/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:24:21 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/09 12:12:00 by rumontei         ###   ########.fr       */
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

void	write_status(const char *msg, t_coder *coder)
{
	long long	now;

	pthread_mutex_lock(&coder->data->log_mutex);
	if (!get_stop_mutex(coder->data) && !all_coders_completed(coder->data))
	{
		now = get_time_in_ms() - coder->data->start_time;
		printf("%lld %d %s\n", now, coder->id, msg);
	}
	pthread_mutex_unlock(&coder->data->log_mutex);
}

int	all_coders_completed(t_data *data)
{
	int	i;
	int	done;

	if (data->compiles_required <= 0)
		return (0);
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_lock(&data->coders[i].compiles_mutex);
		done = data->coders[i].compiles_done;
		pthread_mutex_unlock(&data->coders[i].compiles_mutex);
		if (done < data->compiles_required)
			return (0);
		i++;
	}
	return (1);
}
