/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:30:00 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/14 10:20:47 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_closest_to_burnout(t_coder *coder, t_data *data)
{
	long long	deadline;
	long long	best_deadline;
	int			best_id;
	int			i;

	best_deadline = LLONG_MAX;
	best_id = INT_MAX;
	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_lock(&data->coders[i].last_compile_mutex);
		deadline = data->coders[i].last_compile_time + data->time_to_burnout;
		pthread_mutex_unlock(&data->coders[i].last_compile_mutex);
		if (deadline < best_deadline
			|| (deadline == best_deadline && data->coders[i].id < best_id))
		{
			best_deadline = deadline;
			best_id = data->coders[i].id;
		}
		i++;
	}
	if (coder->id == best_id)
		return (1);
	return (0);
}
