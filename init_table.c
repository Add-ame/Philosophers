#include "philo.h"

void	init_table(t_plate *table, int ac, char **av)
{
	if (ac != 5 && ac != 6)
		exit(22);
	table->num_philos = atoi(av[1]);
	table->time_to_die = atoi(av[2]);
	table->time_to_eat = atoi(av[3]);
	table->time_to_sleep = atoi(av[4]);
	table->simulation_end_time = 0;
	table->must_eat_num = 0;
	if (ac == 6)
		table->must_eat_num = atoi(av[5]);
}
