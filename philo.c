#include "philo.h"

void	*add(void *i)
{
	printf("%d thinking\n", *((int *)i));

	return NULL;
}

int		main()
{
	pthread_t	t[5000];
	int		i;

	i = 0;
	while (i <= 5000)
	{
		if (pthread_create(&t[i], NULL, add, (void *)&i) != 0)
			return (perror("fail"), 1);
		i++;
	}
	i = 0;
	while (i <= 5000)
	{
		if (pthread_join(t[i], NULL) != 0)
			return (perror("fail"), 1);
		i++;
	}
	printf("died\n");

	return (0);
}
