#include "philo.h"

void	init(t_philo *p, int i, t_plate *pl)
{
	p->idx = i + 1;
	p->meals_eaten = 0;
	p->start_time = get_real_time(p, 0);
	p->last_meal_time = get_real_time(p, 0);
	p->die = 0;
	p->plate = pl;
	p->print = &pl->mutex;
	p->checking = &pl->checking;
}

void	init_table(t_plate *table, int ac, char **av)
{
	if (ac != 5 && ac != 6)
		exit(22);
	table->simulation_end_time = 0;
	table->must_eat_num = 0;
	table->died = 0;
	table->all_eaten = 1;
	table->num_philos = atoi(av[1]);
	table->time_to_die = atoi(av[2]);
	table->time_to_eat = atoi(av[3]);
	table->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		table->must_eat_num = atoi(av[5]);
	if (table->num_philos <= 0 || table->time_to_die < 0 || table->time_to_eat < 0 || table->time_to_sleep < 0 || table->must_eat_num < 0)
		exit(1);
}

int		init_mutexes(t_philo *p, t_plate *table)
{
	int		i;

	i = 0;
	if (pthread_mutex_init(&table->mutex, NULL) == ERROR)
		return (ERROR);
	if (pthread_mutex_init(&table->checking, NULL) == ERROR)
		return (ERROR);
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&p[i].left_fork, NULL) == ERROR)
			return (ERROR);
		i++;
	}
	return (0);
}
