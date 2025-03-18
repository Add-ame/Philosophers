#include "philo.h"

void	*one_thread(void	*data)
{
	t_philo		*p;
	long		start;

	p = (t_philo *)data;
	start = get_time(p, CURRENT_TIME);
	printf("%d %d is thinking\n", 0, 1);
	printf("%d %d has taken a fork\n", 0, 1);
	usleep(p->table->time_to_die * 1000);
	printf("%ld %d died\n", get_time(p, CURRENT_TIME) - start, 1);
	return (NULL);
}

int	one_philo(t_table *table)
{
	pthread_t	thread_id;

	if (pthread_create(&thread_id, NULL, one_thread, &table) == ERROR)
		return (ERROR);
	if (pthread_join(thread_id, NULL) == ERROR)
		return (ERROR);
	free(table);
	return (0);
}
