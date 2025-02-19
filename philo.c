#include "philo.h"

pthread_mutex_t		mutex;

void	*add(void *i)
{
	pthread_mutex_lock(&mutex);
	printf("%d thinking\n", *((int *)i));

	pthread_mutex_unlock(&mutex);
	return NULL;
}

int		main()
{
	pthread_t	t[4];
	int		i;
	int		*a;

	pthread_mutex_init(&mutex, NULL);
	i = 0;
	while (i < 3)
	{
		a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&t[i], NULL, add, a) != 0)
			return (perror("fail"), 1);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		if (pthread_join(t[i], NULL) != 0)
			return (perror("fail"), 1);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	printf("died\n");

	return (0);
}
