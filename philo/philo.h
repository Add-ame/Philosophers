/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:59 by maddame           #+#    #+#             */
/*   Updated: 2025/03/08 17:39:15 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define STARVED 1
# define FULL 2
# define CHECK_STARVED 1
# define CHECK_FULL 2
# define ERROR -1
# define END 1
# define INV_ARG 22
# define SUCESS 0
# define FAIL 1

typedef pthread_mutex_t	t_mutex;

typedef enum s_enum
{
	even_number,
	left_then_right,
	right_then_left,
	current_time,
	diff_start_to_new,
	diff_last_to_new,
	first_fork,
	both_forks
}	t_enum;

typedef struct s_table
{
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		simulation_end_time;
	long		num_to_eat;
	int			num_philos;
	int			died;
	int			all_eaten;
	t_mutex		mutex;
	t_mutex		checking;
}	t_table;

typedef struct s_philo
{
	long		start_time;
	int			idx;
	int			real_time;
	int			meals_eaten;
	long		last_meal_time;
	t_table		*table;
	pthread_t	thread_id;
	t_mutex		*print;
	t_mutex		*checking;
	t_mutex		left_fork;
	t_mutex		*right_fork;
}	t_philo;

void	*add(void *data);

/*                    init                    */
int		init_table(t_table *table, int ac, char **av);
void	init(t_philo *p, int i, t_table *pl);
int		init_mutexes(t_philo *p, t_table *table);

/*                     clean                         */
int		clean(t_philo *p, t_table *table);

/*                     check_philo                         */
int		check_philo(t_philo *p, int flag);

/*                     check_philo                         */
void	*one_thread(void	*data);
int		one_philo(t_table *table);

/*                     create_threads                         */
int		create_threads(t_philo *p, t_table *table);

/*                    philo_utils                  */
long	get_time(t_philo *p, int flag);
int		ft_print(t_philo *p, char *s, int flag);
void	lock_fork(t_philo *p, int flag);
void	unlock_fork(t_philo *p, int flag);

#endif
