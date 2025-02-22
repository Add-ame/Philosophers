#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define	THINKING 0
#define	EATING 1
#define	SLEEPING 2
#define SUCCESS 0

typedef struct s_philo
{
	int		idx;
	pthread_t	thread;
	int		last_meal_time;
	int		meals_eaten;
	int		fork_flag;
}   t_philo;

/*
The simulation end if they all eat num... or if a philo passed time_die and still hangry
*/

typedef	struct s_data
{
	pthread_mutex_t		fork[6];
	t_philo       philo[5];
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		simulation_end;
	int		must_eat_time;

}	t_data;


struct timeval start, end;

#endif