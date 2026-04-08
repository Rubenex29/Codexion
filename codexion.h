/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumontei <rumontei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:00:24 by rumontei          #+#    #+#             */
/*   Updated: 2026/04/08 14:16:21 by rumontei         ###   ########.fr       */
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
# include <unistd.h>

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long			last_use;
}	t_dongle;

typedef struct s_Coder
{
	int					id;
	long long			last_compile_time;
	int					compiles_done;
	pthread_mutex_t		last_compile_mutex;
	pthread_mutex_t		compiles_mutex;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
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
	t_dongle			*dongles;
}	t_data;

int			parser(int ac, char **av);
int			check_arg_chars(char *arg);
long		string_to_long_safe(char *arg, int *error);
int			parse_scheduler(char *av);
int			init_all(t_data	*data, char **av);
void		init_compile_mutex(t_coder *coder);
void		*coder_routine(void *arg);
void		create_thread(t_data *data);
long long	get_time_in_ms(void);
void		ft_usleep(long long time_in_ms);
void		write_status(char *msg, t_coder *coder);
int			check_sim_stop(t_coder *coder);
void		compile(t_coder *coder);
void		debug(t_coder *coder);
void		refactor(t_coder *coder);
void		*monitor_routine(void *arg);
void		stop_mutex(t_data *data);
void		compile_mutex(t_coder *coder);
int			get_stop_mutex(t_data *data);

#endif
