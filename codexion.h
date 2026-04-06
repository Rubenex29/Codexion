/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:00:24 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/06 13:33:01 by rumontei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_Coder
{
	int					id;
	long long			last_compile_time;
	int					compiles_done;
	pthread_mutex_t		*left_dongle;
	pthread_mutex_t		*right_dongle;
	struct s_data		*data;
}	t_coder;

typedef struct s_data
{
	int					num_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					compiles_required;
	int					dongle_cooldown;
	int					scheduler_type;
	long long			start_time;
	int					simulation_stop;
	pthread_t			*thread_ids;
	struct s_Coder		*coders;			
	pthread_mutex_t		log_mutex;
	pthread_mutex_t		*dongles;
}	t_data;

int		parser(int ac, char **av);
int		check_arg_chars(char *arg);
long	string_to_long_safe(char *arg, int *error);
int		parse_scheduler(char *av);
int		init_all(t_data	*data, char **av);

#endif
