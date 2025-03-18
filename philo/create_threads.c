#include "philo.h"

int	create_threads(t_philo *p, t_table *table)
{
	pthread_t	id;
	int			i;

	i = 0;
	while (i < table->num_philos)
	{
		init(&p[i], i, table);
		p[i].right_fork = &p[(i + 1) % table->num_philos].left_fork;
		if (pthread_create(&p[i].thread_id, NULL, philo_thread, &p[i]) \
		!= SUCESS)
			return (ERROR);
		i++;
	}
	pthread_create(&id, NULL, monitor, p);
	pthread_join(id, NULL);
	i = -1;
	while (++i < table->num_philos)
	{
		if (pthread_join(p[i].thread_id, NULL) != SUCESS)
			return (ERROR);
	}
	return (SUCESS);
}

void	*die(t_philo *p, int i)
{
	pthread_mutex_lock(p[i].checking);
	p[i].table->simulation_end_time = \
	get_time(&p[i], DIFF_START_TO_NEW);
	pthread_mutex_unlock(p[i].checking);
	pthread_mutex_lock(p[i].print);
	printf("%ld %d died\n", p[i].table->simulation_end_time, \
	p[i].idx);
	pthread_mutex_unlock(p[i].print);
	return (NULL);
}

void	*monitor(void *data)
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

int	clean(t_philo *p, t_table *table)
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
	if (pthread_mutex_destroy(&table->last_meal) == ERROR)
		return (ERROR);
	if (pthread_mutex_destroy(&table->all_eat) == ERROR)
		return (ERROR);
	free(p);
	free(table);
	return (0);
}
