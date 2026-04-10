/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 12:13:24 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/09 15:14:04 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	long long	now;

	while (1)
	{
		if (get_stop_mutex(coder->data) || all_coders_completed(coder->data))
			return (0);
		pthread_mutex_lock(&dongle->mutex);
		if (get_stop_mutex(coder->data) || all_coders_completed(coder->data))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		now = get_time_in_ms();
		if (now - dongle->last_use >= coder->data->dongle_cooldown)
			return (1);
		pthread_mutex_unlock(&dongle->mutex);
		usleep(500);
	}
}

static int	take_compile_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		if (!take_dongle(coder, coder->right_dongle))
			return (0);
		write_status("has taken a dongle", coder);
		if (!take_dongle(coder, coder->left_dongle))
		{
			pthread_mutex_unlock(&coder->right_dongle->mutex);
			return (0);
		}
		write_status("has taken a dongle", coder);
	}
	else
	{
		if (!take_dongle(coder, coder->left_dongle))
			return (0);
		write_status("has taken a dongle", coder);
		if (!take_dongle(coder, coder->right_dongle))
		{
			pthread_mutex_unlock(&coder->left_dongle->mutex);
			return (0);
		}
		write_status("has taken a dongle", coder);
	}
	return (1);
}

void	compile(t_coder *coder)
{
	long long	now;

	if (!take_compile_dongles(coder))
		return ;
	pthread_mutex_lock(&coder->last_compile_mutex);
	coder->last_compile_time = get_time_in_ms();
	pthread_mutex_unlock(&coder->last_compile_mutex);
	write_status("is compiling", coder);
	ft_usleep(coder->data->time_to_compile);
	pthread_mutex_lock(&coder->compiles_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->compiles_mutex);
	if (all_coders_completed(coder->data))
		stop_mutex(coder->data);
	now = get_time_in_ms();
	coder->left_dongle->last_use = now;
	coder->right_dongle->last_use = now;
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}

void	debug(t_coder *coder)
{
	write_status("is debugging", coder);
	ft_usleep(coder->data->time_to_debug);
}

void	refactor(t_coder *coder)
{
	write_status("is refactoring", coder);
	ft_usleep(coder->data->time_to_refactor);
}
