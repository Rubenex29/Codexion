/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 12:13:24 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/08 11:45:37 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(coder->right_dongle);
		write_status("has taken a dongle", coder);
		pthread_mutex_lock(coder->left_dongle);
		write_status("has taken a dongle", coder);
	}
	else
	{
		pthread_mutex_lock(coder->left_dongle);
		write_status("has taken a dongle", coder);
		pthread_mutex_lock(coder->right_dongle);
		write_status("has taken a dongle", coder);
	}
	compile_mutex(coder);
	write_status("is compiling", coder);
	ft_usleep(coder->data->time_to_compile);
	pthread_mutex_unlock(coder->left_dongle);
	pthread_mutex_unlock(coder->right_dongle);
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
