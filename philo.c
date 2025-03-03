#include "philo.h"

long	get_real_time()
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

int		simulation_end(t_philo *p, int flag)
{
	if (flag == CHECK_STARVED)
	{
		if (get_real_time() - p->last_meal_time >= p->plate->time_to_die)
		{
			p->die = STARVED;
			p->plate->simulation_end_time = get_real_time() - p->last_meal_time;
			return (STARVED);
		}
	}
	else if (flag == CHECK_FULL)
	{
		if (p->meals_eaten == p->plate->must_eat_num)
		{
			p->die = FULL;
			return (FULL);
		}
	}
	return (0);
}

void	*add(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;

	if (p->idx % 2 == 0 || p->idx == p->plate->num_philos)
	{
		printf("%ld %d is thinking\n", get_real_time() - p->start_time,p->idx);
		usleep(1000);
	}
	while (1)
	{
		if (p->plate->simulation_end_time)
			return (NULL);
		if (simulation_end(p, CHECK_STARVED) == STARVED)
			return (NULL);
		pthread_mutex_lock(&p->left_fork);
		pthread_mutex_lock(p->print);
		printf("%ld %d has taken a fork\n", get_real_time() - p->start_time ,p->idx);
		pthread_mutex_unlock(p->print);
		pthread_mutex_lock(p->right_fork);
		if (simulation_end(p, CHECK_STARVED) == STARVED)
		{
			pthread_mutex_unlock(&p->left_fork);
			pthread_mutex_unlock(p->right_fork);
			return (NULL);
		}
		pthread_mutex_lock(p->print);
		printf("%ld %d has taken a fork\n", get_real_time() - p->start_time ,p->idx);
		printf("%ld %d is eating\n", get_real_time() - p->start_time ,p->idx);
		pthread_mutex_unlock(p->print);
		pthread_mutex_lock(&p->update);
		p->meals_eaten++;
		p->last_meal_time = get_real_time();
		pthread_mutex_unlock(&p->update);
		usleep(p->plate->time_to_eat * 1000);
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);

		if (p->plate->simulation_end_time)
			return (NULL);
		pthread_mutex_lock(p->print);
		printf("%ld %d is sleeping\n", get_real_time() - p->start_time ,p->idx);
		pthread_mutex_unlock(p->print);
		usleep(p->plate->time_to_sleep * 1000);

		if (simulation_end(p, CHECK_STARVED) == STARVED || simulation_end(p, CHECK_FULL) == FULL)
			return (NULL);

		pthread_mutex_lock(p->print);
		printf("%ld %d is thinking\n", get_real_time() - p->start_time ,p->idx);
		pthread_mutex_unlock(p->print);
	}
	return NULL;
}

void	init(t_philo *p, int ac, int i, pthread_mutex_t *mutex, t_plate *pl)
{
	// int		i;

	if (ac != 5 && ac != 6)
		exit(22);

	p->idx = i + 1;
	p->real_time = get_real_time();
	p->meals_eaten = 0;
	p->print = mutex;
	p->start_time = get_real_time();
	p->last_meal_time = get_real_time();
	p->die = 0;
	p->plate = pl;
}

int		died(t_philo *p)
{
	int		i;

	while (1)
	{
		i = 0;
		while (i < p[i].plate->num_philos)
		{
			if (p[i].die == STARVED)
			{
				printf("%ld %d died\n", p[i].plate->simulation_end_time, p[i].idx);
				return (STARVED);
			}
			if (p[i].die == FULL)
			{
				printf("Rahom cheb3o\n");
				return (FULL);
			}
			i++;
		}
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_philo		*p;
	int		i;
	int		num;
	t_plate		*pl;
	pthread_mutex_t mutex;

	pl = malloc(sizeof(t_plate));
	pl->num_philos = atoi(av[1]);
	pl->time_to_die = atoi(av[2]);
	pl->time_to_eat = atoi(av[3]);
	pl->time_to_sleep = atoi(av[4]);
	pl->simulation_end_time = 0;
	if (ac == 6)
		pl->must_eat_num = atoi(av[5]);
	i = 0;
	pthread_mutex_init(&mutex, NULL);
	num = atoi(av[1]);
	p = malloc(sizeof(t_philo) * num);
	while (i < num)
	{
		init(&p[i], ac, i, &mutex, pl);
		pthread_mutex_init(&p[i].left_fork, NULL);
		p[i].right_fork = &p[(i + 1) % pl->num_philos].left_fork;
		if (pthread_create(&p[i].thread, NULL, add, &p[i]) != 0)
			return (perror("fail"), 1);
		i++;
	}

	i = 0;
	while (i < num)
	{
		if (pthread_join(p[i].thread, NULL) != 0)
			return (perror("fail"), 1);
		i++;
	}
	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&p[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	if (died(p))
		return (free(p), 1);
	free(p);
	return (0);
}
