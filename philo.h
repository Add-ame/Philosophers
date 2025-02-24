#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>



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
	int		must_eat_time;
}	t_plate;

typedef struct s_philo
{
	int		idx;
	t_plate		plate;
	pthread_t	thread;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
}   t_philo;



#endif