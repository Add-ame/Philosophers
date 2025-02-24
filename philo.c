#include "philo.h"

pthread_mutex_t		mutex;


/**
 * 1 2 3 4 5 philos
 * |\
 * 1 2 3 4 5 forks
 *
 * philos 1 picks up fork 1
 * 
 * 
 */
void	*add(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;

	while (1)
	{
		printf("%d ", p->idx);
		printf("thinking\n");
		usleep(p->plate.time_to_eat * 1000);
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&p->left_fork);
		printf("%d ", p->idx);
		printf("Left fork tacked\n");
		pthread_mutex_lock(p->right_fork);
		printf("%d ", p->idx);
		printf("Right fork tacked\n");
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		printf("%d ", p->idx);
		printf("eating\n");
		usleep(p->plate.time_to_eat * 1000);
		printf("%d ", p->idx);
		printf("sleeping\n");
		usleep(p->plate.time_to_sleep * 1000);

		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void	init(t_philo *p, int ac, char **av, int i)
{
	// int		i;

	if (ac != 5 && ac != 6)
		exit(22);

	p->plate.num_philos = atoi(av[1]);
	p->plate.time_to_die = atoi(av[2]);
	p->plate.time_to_eat = atoi(av[3]);
	p->plate.time_to_sleep = atoi(av[4]);
	if (ac == 6)
		p->plate.must_eat_time = atoi(av[5]);

	p->idx = i + 1;
}

int		main(int ac, char **av)
{
	t_philo		p[200];
	int		i;

	pthread_mutex_init(&mutex, NULL);
	i = 0;
	while (i < 10)
	{
		init(&p[i], ac, av, i);
		pthread_mutex_init(&p[i].left_fork, NULL);
		p[i].right_fork = &p[(i + 1) % p[i].plate.num_philos].left_fork;
		if (pthread_create(&p[i].thread, NULL, add, &p[i]) != 0)
			return (perror("fail"), 1);
		i++;
	}
	i = 0;
	while (i < 10)
	{
		if (pthread_join(p[i].thread, NULL) != 0)
			return (perror("fail"), 1);
		pthread_mutex_destroy(&p[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	printf("\ndied\n");

	return (0);
}
