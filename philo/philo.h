/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:59 by maddame           #+#    #+#             */
/*   Updated: 2025/03/16 17:45:40 by maddame          ###   ########.fr       */
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
# define SIMULATION_END 1
# define INV_ARG 22
# define SUCESS 0
# define FAIL 1

typedef pthread_mutex_t	t_mutex;

typedef enum s_enum
{
	EVEN_NUMBER,
	LEFT_THEN_RIGHT,
	RIGHT_THEN_LEFT,
	CURRENT_TIME,
	DIFF_START_TO_NEW,
	DIFF_LAST_TO_NEW,
	FIRST_FORK,
	BOTH_FORKS,
	THINK,
	SLEEP
}	t_enum;

typedef struct s_table
{
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		simulation_end_time;
	long		num_to_eat;
	int			num_philos;
	int			all_eaten;
	t_mutex		mutex;
	t_mutex		checking;
	t_mutex		last_meal;
	t_mutex		all_eat;
}	t_table;

typedef struct s_philo
{
	long		start_time;
	int			idx;
	int			meals_eaten;
	long		last_meal_time;
	t_table		*table;
	t_mutex		*print;
	t_mutex		*checking;
	t_mutex		*all_eat;
	t_mutex		*last_meal;
	t_mutex		left_fork;
	t_mutex		*right_fork;
	pthread_t	t_id;
}	t_philo;

/*                     init                          */
int		init_table(t_table *table, int ac, char **av);
void	init(t_philo *p, int i, t_table *pl);
int		init_mutexes(t_philo *p, t_table *table);

/*                     clean                        */
int		clean(t_philo *p, t_table *table);

/*                     check_philo                  */
void	*monitoring(void *data);

/*                     check_philo                  */
void	*one_thread(void	*data);
int		one_philo(t_table *table);

/*                     get_time                     */
long	get_time(t_philo *p, int flag);

#endif
