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
			p->plate->simulation_end_time = get_real_time() - p->last_meal_time;
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

	while (!p->plate->simulation_end_time)
	{
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

		pthread_mutex_lock(p->print);
		printf("%ld %d is sleeping\n", get_real_time() - p->start_time ,p->idx);
		pthread_mutex_unlock(p->print);
		usleep(p->plate->time_to_sleep * 1000);

		if (simulation_end(p, CHECK_STARVED) == STARVED || simulation_end(p, CHECK_FULL) == FULL)
			return (NULL);

		if (p->plate->simulation_end_time)
			return (NULL);
		pthread_mutex_lock(p->print);
		printf("%ld %d is thinking\n", get_real_time() - p->start_time ,p->idx);
		pthread_mutex_unlock(p->print);
	}
	return NULL;
}

int		main(int ac, char **av)
{
	t_philo		*p;
	int		i;
	t_plate		*table;
	pthread_mutex_t mutex;

	table = malloc(sizeof(t_plate));
	init_table(table, ac, av);
	pthread_mutex_init(&mutex, NULL);
	table->num_philos = atoi(av[1]);
	p = malloc(sizeof(t_philo) * table->num_philos);
	i = 0;
	while (i < table->num_philos)
	{
		init(&p[i], i, &mutex, table);
		pthread_mutex_init(&p[i].left_fork, NULL);
		pthread_mutex_init(&p[i].update, NULL);
		p[i].right_fork = &p[(i + 1) % table->num_philos].left_fork;
		if (pthread_create(&p[i].thread, NULL, add, &p[i]) != 0)
			return (perror("fail"), 1);
		i++;
	}

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_join(p[i].thread, NULL) != 0)
			return (perror("fail"), 1);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&p[i].left_fork);
		pthread_mutex_destroy(&p[i].update);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	if (died(p))
		return (free(p), free(table), 1);
	free(p), free(table);
	return (0);
}
