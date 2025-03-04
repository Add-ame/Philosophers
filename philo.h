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
# define ERROR -1

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
	long	simulation_end_time;
	int		must_eat_num;
	pthread_mutex_t mutex;
	pthread_mutex_t mutex_upd;
}	t_plate;

typedef struct s_philo
{
	long	start_time;
	int		die;
	int		idx;
	int		real_time;
	int		meals_eaten;
	long	last_meal_time;
	t_plate		*plate;
	pthread_t	thread;
	pthread_mutex_t	*print;
	pthread_mutex_t	*upd;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
}   t_philo;


/*                    init_table                    */
void    init_table(t_plate *table, int ac, char **av);

/*                     died                         */
int		died(t_philo *p);

/*                     died                         */
void	init(t_philo *p, int i, t_plate *pl);

/*                     time                         */
long	get_real_time(t_philo *p, int flag);

#endif