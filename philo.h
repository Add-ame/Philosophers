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
# define END 1

// typedef enum {
// 	def,
// 	picked,
// 	throwed,
// 	eating,
// 	thinking,
// 	sleeping
// }	t_enum;


typedef	struct s_data
{
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	long	simulation_end_time;
	int		must_eat_num;
	int		died;
	int		all_eaten;
	pthread_mutex_t mutex;
	pthread_mutex_t checking;
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
	pthread_mutex_t	*checking;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
}   t_philo;


void	*add(void *data);

/*                    init                    */
void    init_table(t_plate *table, int ac, char **av);
void	init(t_philo *p, int i, t_plate *pl);
int		init_mutexes(t_philo *p, t_plate *table);

/*                     time                         */
long	get_real_time(t_philo *p, int flag);

/*                     clean                         */
int		clean(t_philo *p, t_plate *table);

/*                     check_philo                         */
int		check_philo(t_philo *p, int flag);

/*                     check_philo                         */
void	*one_thread(void	*data);
int		one_philo(t_philo *p, t_plate *table);

/*                     create_threads                         */
int		create_threads(t_philo *p, t_plate *table);

int		_printf(t_philo *p, char *s);

#endif