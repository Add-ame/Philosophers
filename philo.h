#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

# define STARVED 1
# define FULL 2
# define CHECK_STARVED 1
# define CHECK_FULL 2

typedef enum {
	def,
	picked,
	throwed,
	eating,
	thinking,
	sleeping
}	t_enum;


typedef	struct s_data
{
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		simulation_end;
	int		must_eat_num;
	pthread_mutex_t	mutex;
}	t_plate;

typedef struct s_philo
{
	long	start_time;
	int		die;
	int		idx;
	int		real_time;
	int		meals_eaten;
	long	last_meal_time;
	t_plate		plate;
	pthread_t	thread;
	pthread_mutex_t	*print;
	pthread_mutex_t	update;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
}   t_philo;


#endif