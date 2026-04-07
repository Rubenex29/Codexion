/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 12:13:24 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/07 16:47:44 by rumontei         ###   ########.fr       */
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
	write_status("is compiling", coder);
	ft_usleep(coder->data->time_to_compile);
	pthread_mutex_unlock(coder->left_dongle);
	pthread_mutex_unlock(coder->right_dongle);
	compile_mutex(coder);
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

void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;

    while (!get_stop_mutex(data))
    {
        i = 0;
        while (i < data->num_coders)
        {
            pthread_mutex_lock(&data->coders[i].last_compile_mutex);
            long long last = data->coders[i].last_compile_time;
            pthread_mutex_unlock(&data->coders[i].last_compile_mutex);

            if (get_time_in_ms() - last > data->time_to_burnout)
            {
                write_status("burned out", &data->coders[i]);
                stop_mutex(data);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
    return NULL;
}
