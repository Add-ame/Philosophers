#include "philo.h"

pthread_mutex_t		mutex;

void	*add(void *d)
{
	pthread_mutex_lock(&mutex);
	printf("%d ", ((t_data *)d)->philo.id);
	printf("thinking\n");


	pthread_mutex_unlock(&mutex);
	return NULL;
}

int		main()
{
	t_data		d[4];
	int		i;
	// pthread_t	thread[4];

	// if (parse_data(ac, av) != 0)
	// 	return (0);
	pthread_mutex_init(&mutex, NULL);
	i = 0;
	while (i < 3)
	{
		d[i].philo.id = i;
		if (pthread_create(&d[i].philo.thread, NULL, add, &d[i]) != 0)
			return (perror("fail"), 1);
		// exit(1);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		if (pthread_join(d[i].philo.thread, NULL) != 0)
			return (perror("fail"), 1);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	printf("died\n");

	return (0);
}
