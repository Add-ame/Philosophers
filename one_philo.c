#include "philo.h"

void	*one_thread(void	*data)
{
	t_philo		*p;

	p = (t_philo *)data;
	pthread_mutex_lock(p->print);
	printf("%ld %d is thinking\n", get_real_time(p, 1), p->idx);
	pthread_mutex_unlock(p->print);
	pthread_mutex_lock(p->print);
	printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
	pthread_mutex_unlock(p->print);
	usleep(p->plate->time_to_die * 1000);
	return (NULL);
}

int		one_philo(t_philo *p, t_plate *table)
{
	if (pthread_mutex_init(&p[0].left_fork, NULL) == ERROR)
		return (ERROR);
	if (pthread_mutex_init(&table->mutex, NULL) == ERROR)
		return (ERROR);
	init(p, 0, table);
	if (pthread_create(&p[0].thread, NULL, one_thread, &p[0]) == ERROR)
		return (ERROR);
	if (pthread_join(p[0].thread, NULL) == ERROR)
		return (ERROR);
	pthread_mutex_destroy(&p[0].left_fork);
	pthread_mutex_destroy(&table->mutex);
	free(p);
	free(table);
	return (0);
}
