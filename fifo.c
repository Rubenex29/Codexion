/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:17:03 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/13 12:13:37 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fifo_lock(t_fifo *f)
{
	unsigned long	my_ticket;

	pthread_mutex_lock(&f->mtx);
	my_ticket = f->next_ticket;
	f->next_ticket++;
	while (f->serving_ticket != my_ticket)
		pthread_cond_wait(&f->cond, &f->mtx);
	pthread_mutex_unlock(&f->mtx);
}

void	fifo_unlock(t_fifo *f)
{
	pthread_mutex_lock(&f->mtx);
	f->serving_ticket++;
	pthread_cond_broadcast(&f->cond);
	pthread_mutex_unlock(&f->mtx);
}
