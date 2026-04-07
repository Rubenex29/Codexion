/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:05:30 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/07 10:27:22 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"	

int	main(int ac, char **av)
{
	t_data		data;

	if (ac != 9)
		return (1);
	if (!parser(ac, av))
		return (1);
	memset(&data, 0, sizeof(t_data));
	if (!init_all(&data, av))
		return (1);
	create_thread(&data);
	return (0);
}
