#include "philo.h"

int		create_threads(t_philo *p, t_plate *table)
{
	int		i;

	i = 0;
	while (i < table->num_philos)
	{
		init(&p[i], i, table);
		p[i].right_fork = &p[(i + 1) % table->num_philos].left_fork;
		if (pthread_create(&p[i].thread, NULL, add, &p[i]) != 0)
			return (perror("fail"), ERROR);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_join(p[i].thread, NULL) != 0)
			return (perror("fail"), ERROR);
		i++;
	}
	return (0);
}
