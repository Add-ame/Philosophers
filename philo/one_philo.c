#include "philo.h"

void	*one_thread(void	*data)
{
	long		start;
	long		time;
	t_table		*table;
	struct timeval		current_time;

	table = (t_table *)data;
	gettimeofday(&current_time, NULL);
	start = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	printf("%d %d is thinking\n", 0, 1);
	printf("%d %d has taken a fork\n", 0, 1);
	usleep(table->time_to_die * 1000);
	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	printf("%ld %d died\n", time - start, 1);
	return (NULL);
}

int	one_philo(t_table *table)
{
	pthread_t	t_id;

	if (pthread_create(&t_id, NULL, one_thread, table) == ERROR)
		return (ERROR);
	if (pthread_join(t_id, NULL) == ERROR)
		return (ERROR);
	free(table);
	return (0);
}
