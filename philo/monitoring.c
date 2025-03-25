#include "philo.h"

void	*die(t_philo *p, int i)
{
	pthread_mutex_lock(p[i].checking);
	p[i].table->simulation_end_time = \
	get_time(&p[i], DIFF_START_TO_NEW);
	pthread_mutex_unlock(p[i].checking);
	pthread_mutex_lock(p[i].print);
	pthread_mutex_lock(p->last_meal);
	if (p->meals_eaten != p->table->num_to_eat)
		printf("%ld %d died\n", p[i].table->simulation_end_time, \
		p[i].idx);
	pthread_mutex_unlock(p->last_meal);
	pthread_mutex_unlock(p[i].print);
	return (NULL);
}

void	*monitoring(void *data)
{
	t_philo		*p;
	int			i;

	p = (t_philo *)data;
	while (1)
	{
		i = 0;
		while (i < p[0].table->num_philos)
		{
			pthread_mutex_lock(p->last_meal);
			if (p[i].meals_eaten == p[i].table->num_to_eat)
				i++;
			pthread_mutex_unlock(p->last_meal);
			if (i >= p[0].table->num_philos)
				break ;
			if (get_time(&p[i], DIFF_LAST_TO_NEW) >= p[i].table->time_to_die)
				return (die(p, i));
			pthread_mutex_lock(p->all_eat);
			if (p[0].table->all_eaten == p[0].table->num_philos)
				return (pthread_mutex_unlock(p->all_eat), NULL);
			pthread_mutex_unlock(p->all_eat);
			i++;
		}
	}
	return (NULL);
}
