#include "philo.h"

int		clean(t_philo *p, t_plate *table)
{
	int		i;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_mutex_destroy(&p[i].left_fork) == ERROR)
			return (ERROR);
		i++;
	}
	if (pthread_mutex_destroy(&table->mutex) == ERROR)
		return (ERROR);
	if (pthread_mutex_destroy(&table->checking) == ERROR)
		return (ERROR);
	free(p);
	free(table);
	return (0);
}
