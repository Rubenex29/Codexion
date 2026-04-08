/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 12:13:24 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/08 15:09:53 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	long long	now;

	while (1)
	{
		pthread_mutex_lock(&dongle->mutex);
		now = get_time_in_ms();
		if (now - dongle->last_use >= coder->data->dongle_cooldown)
		{
			dongle->last_use = now;
			break ;
		}
		pthread_mutex_unlock(&dongle->mutex);
		usleep(500);
	}
}

void	compile(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		take_dongle(coder, coder->right_dongle);
		write_status("has taken a dongle", coder);
		take_dongle(coder, coder->left_dongle);
		write_status("has taken a dongle", coder);
	}
	else
	{
		take_dongle(coder, coder->left_dongle);
		write_status("has taken a dongle", coder);
		take_dongle(coder, coder->right_dongle);
		write_status("has taken a dongle", coder);
	}
	pthread_mutex_lock(&coder->last_compile_mutex);
	coder->last_compile_time = get_time_in_ms();
	pthread_mutex_unlock(&coder->last_compile_mutex);
	write_status("is compiling", coder);
	ft_usleep(coder->data->time_to_compile);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
	pthread_mutex_lock(&coder->compiles_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->compiles_mutex);
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
