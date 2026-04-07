/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:20:38 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/07 09:32:07 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parser(int ac, char **av)
{
	int	i;
	int	error;

	error = 0;
	i = 1;
	while (i < ac - 1)
	{
		if (!check_arg_chars(av[i]))
			return (0);
		string_to_long_safe(av[i], &error);
		if (error)
			return (0);
		i++;
	}
	if (!parse_scheduler(av[i]))
		return (0);
	return (1);
}

int	check_arg_chars(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (arg[i] == '\0')
		return (0);
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

long	string_to_long_safe(char *arg, int *error)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	while (arg[i] >= '0' && arg[i] <= '9')
	{
		result = result * 10 + (arg[i] - '0');
		i++;
	}
	result = result * sign;
	if (result > INT_MAX || result < 0)
		*error = 1;
	return (result);
}

int	parse_scheduler(char *av)
{
	if (strcmp(av, "fifo") == 0 || strcmp(av, "edf") == 0)
		return (1);
	return (0);
}
