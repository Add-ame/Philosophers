#include "philo.h"

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

	// pthread_mutex_lock(&p->print);
	// if (p->idx % 2 == 0 || p->idx == p->plate.num_philos)
	// {
	// 	printf("%d %d is thinking\n", 0,p->idx);
	// 	usleep(100);
	// }

	// p->start_time = get_real_time();
	// p->last_meal_time = get_real_time();
	// pthread_mutex_unlock(&p->print);
	// while (1)
	// {
	// 	// if (get_real_time() - p->last_meal_time > p->plate.time_to_die)
	// 	// {
	// 	// 	// printf("%ld %d is eating\n", get_real_time() - p->start_time ,p->idx);
	// 	// 	printf("%ld\n", get_real_time() - p->start_time);
	// 	// 	exit(1);
	// 	// }
	// 	// pthread_mutex_lock(&p->left_fork);
	// 	// pthread_mutex_lock(&p->print);
	// 	// printf("%ld %d has taken a fork\n", get_real_time() - p->start_time ,p->idx);
	// 	// pthread_mutex_unlock(&p->print);
	// 	// pthread_mutex_lock(p->right_fork);
	// 	// pthread_mutex_lock(&p->print);
	// 	// printf("%ld %d has taken a fork\n", get_real_time() - p->start_time ,p->idx);
	// 	// printf("%ld %d is eating\n", get_real_time() - p->start_time ,p->idx);
	// 	// pthread_mutex_unlock(&p->print);
	// 	// p->meals_eaten++;
	// 	// p->last_meal_time = get_real_time();
	// 	// pthread_mutex_unlock(&p->left_fork);
	// 	// usleep(p->plate.time_to_eat * 1000); // (p.n = 5, 4)
	// 	// pthread_mutex_unlock(p->right_fork);

	// 	// pthread_mutex_lock(&p->print);
	// 	// printf("%ld %d is sleeping\n", get_real_time() - p->start_time ,p->idx);
	// 	// pthread_mutex_unlock(&p->print);
	// 	// usleep(p->plate.time_to_sleep * 1000);

	// 	// if (p->plate.must_eat_num == p->meals_eaten)
	// 	// 	break;

	// 	pthread_mutex_lock(&p->print);
	// 	printf("thinbking\n");
	// 	// printf("%ld %d is thinking\n", get_real_time() - p->start_time ,p->idx);
	// 	usleep(p->plate.time_to_eat * 1000);
	// 	pthread_mutex_unlock(&p->print);
	// 	break ;
	// }
	pthread_mutex_lock(&p->print);
	printf("thinbking\n");
	// printf("%ld %d is thinking\n", get_real_time() - p->start_time ,p->idx);
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
	t_philo		p[2];
	int		i;

	i = 0;
	init(&p[0], ac, av, i);
	init(&p[1], ac, av, i);
	pthread_mutex_init(&p[0].print, NULL);
	pthread_mutex_init(&p[1].print, NULL);
	pthread_create(&p[0].thread, NULL, add, &p[i]);
	pthread_create(&p[1].thread, NULL, add, &p[i]);
	pthread_join(p[0].thread, NULL);
	pthread_join(p[1].thread, NULL);
	pthread_mutex_destroy(&p[0].print);
	pthread_mutex_destroy(&p[1].print);

	return (0);
}
