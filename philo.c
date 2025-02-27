#include "philo.h"

pthread_mutex_t		mutex;

long	get_real_time()
{
	struct timeval	current_time;
	long			real;

	gettimeofday(&current_time, NULL);
	real = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (real);
}

/**
 * 1 2 3 4 5 philos
 * |\
 * 1 2 3 4 5 forks
 * 
 * 
 */
void	*add(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;

	pthread_mutex_lock(&p->print);
	printf("here\n");
	pthread_mutex_unlock(&p->print);

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
		p->plate.must_eat_num = atoi(av[5]);

	p->idx = i + 1;
	p->real_time = get_real_time();
	p->meals_eaten = 0;
}

int		main(int ac, char **av)
{
	t_philo		p[200];
	int		i;

	i = 0;
	init(&p[0], ac, av, i);
	init(&p[1], ac, av, i);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&p[0].print, NULL);
	pthread_mutex_init(&p[1].print, NULL);
	pthread_create(&p[0].thread, NULL, add, &p[0]);
	pthread_create(&p[1].thread, NULL, add, &p[1]);
	pthread_join(p[0].thread, NULL);
	pthread_join(p[1].thread, NULL);
	pthread_mutex_destroy(&p[0].print);
	pthread_mutex_destroy(&p[1].print);
	pthread_mutex_destroy(&mutex);
	// while (i < p[0].plate.num_philos)
	// {
	// 	init(&p[i], ac, av, i);
	// 	pthread_mutex_init(&p[i].left_fork, NULL);
	// 	pthread_mutex_init(&p[i].print, NULL);
	// 	p[i].right_fork = &p[(i + 1) % p[i].plate.num_philos].left_fork;
	// 	if (pthread_create(&p[i].thread, NULL, add, &p[i]) != 0)
	// 		return (perror("fail"), 1);
	// 	i++;
	// }


	// i = 0;
	// while (i < p[0].plate.num_philos)
	// {
	// 	if (pthread_join(p[i].thread, NULL) != 0)
	// 		return (perror("fail"), 1);
	// 	pthread_mutex_destroy(&p[i].left_fork);
	// 	pthread_mutex_destroy(&p[i].print);
	// 	i++;
	// }
	// printf("\ndied\n");

	return (0);
}
