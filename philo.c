#include "philo.h"

long	get_real_time(t_philo *p, int flag)
{
	struct timeval	current_time;
	long	result;

	gettimeofday(&current_time, NULL);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	if (flag == 1)
		result = result - p->start_time;
	else if (flag == 2)
		result = result - p->last_meal_time;
	return (result);
}

int		simulation_end(t_philo *p, int flag)
{
	if (flag == CHECK_STARVED)
	{
		if (get_real_time(p, 2) >= p->plate->time_to_die)
		{
			p->die = STARVED;
			pthread_mutex_lock(p->upd);
			p->plate->simulation_end_time = get_real_time(p, 2);
			pthread_mutex_unlock(p->upd);
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

	p->start_time = get_real_time(p, 0);
	if (p->idx % 2 == 0 || p->idx == p->plate->num_philos)
	{
		pthread_mutex_lock(p->print);
		printf("%ld %d is thinking\n", get_real_time(p, 1), p->idx);
		pthread_mutex_unlock(p->print);
		usleep(1000);
	}

	while (1)
	{
		pthread_mutex_lock(p->upd);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(p->upd), NULL);
		pthread_mutex_unlock(p->upd);
		if (simulation_end(p, CHECK_STARVED) == STARVED)
			return (NULL);
		if (p->idx % 2)
		{
			pthread_mutex_lock(&p->left_fork);
			pthread_mutex_lock(p->print);
			if (simulation_end(p, CHECK_STARVED) == STARVED)
			{
				pthread_mutex_unlock(p->print);
				pthread_mutex_unlock(&p->left_fork);
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
			pthread_mutex_unlock(p->print);
			pthread_mutex_lock(p->right_fork);
		}
		else
		{
			pthread_mutex_lock(p->right_fork);
			pthread_mutex_lock(p->print);
			if (simulation_end(p, CHECK_STARVED) == STARVED)
			{
				pthread_mutex_unlock(p->print);
				pthread_mutex_unlock(&p->left_fork);
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
			pthread_mutex_unlock(p->print);
			pthread_mutex_lock(&p->left_fork);
		}
		pthread_mutex_lock(p->print);
		printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
		pthread_mutex_unlock(p->print);
		pthread_mutex_lock(p->print);
		printf("%ld %d is eating\n", get_real_time(p, 1) ,p->idx);
		pthread_mutex_unlock(p->print);
		p->meals_eaten++;
		p->last_meal_time = get_real_time(p, 0);
		usleep(p->plate->time_to_eat * 1000);
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);

		if (simulation_end(p, CHECK_STARVED) == STARVED)
			return (NULL);

		pthread_mutex_lock(p->print);
		pthread_mutex_lock(p->upd);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(p->upd), pthread_mutex_unlock(p->print), NULL);
		pthread_mutex_unlock(p->upd);
		printf("%ld %d is sleeping\n", get_real_time(p, 1) ,p->idx);
		pthread_mutex_unlock(p->print);
		usleep(p->plate->time_to_sleep * 1000);

		if (simulation_end(p, CHECK_STARVED) == STARVED || simulation_end(p, CHECK_FULL) == FULL)
			return (NULL);

		pthread_mutex_lock(p->print);
		pthread_mutex_lock(p->upd);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(p->upd), pthread_mutex_unlock(p->print), NULL);
		pthread_mutex_unlock(p->upd);
		printf("%ld %d is thinking\n", get_real_time(p, 1),p->idx);
		pthread_mutex_unlock(p->print);
	}
	return NULL;
}

int		init_mutexes(t_philo *p, t_plate *table)
{
	int		i;

	i = 0;
	if (pthread_mutex_init(&table->mutex, NULL) == ERROR)
		return (ERROR);
	if (pthread_mutex_init(&table->mutex_upd, NULL) == ERROR)
		return (ERROR);
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&p[i].left_fork, NULL) == ERROR)
			return (ERROR);
		i++;
	}
	return (0);
}

int		create_threads(t_philo *p, t_plate *table)
{
	int		i;

	i = 0;
	while (i < table->num_philos)
	{
		init(&p[i], i, table);
		p[i].right_fork = &p[(i + 1) % table->num_philos].left_fork;
		if (pthread_create(&p[i].thread, NULL, add, &p[i]) != 0)
			return (perror("fail"), ERROR);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_join(p[i].thread, NULL) != 0)
			return (perror("fail"), ERROR);
		i++;
	}
	return (0);
}

int		clean(t_philo *p, t_plate *table)
{
	int		i;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_mutex_destroy(&p[i].left_fork) == ERROR)
			return (ERROR);
		i++;
	}
	if (pthread_mutex_destroy(&table->mutex) == ERROR)
		return (ERROR);
	if (pthread_mutex_destroy(&table->mutex_upd) == ERROR)
		return (ERROR);
	return (0);
}

void	*one_thread(void	*data)
{
	t_philo		*p;

	p = (t_philo *)data;
	pthread_mutex_lock(p->print);
	printf("%ld %d is thinking\n", get_real_time(p, 1), p->idx);
	pthread_mutex_unlock(p->print);
	pthread_mutex_lock(p->print);
	printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
	pthread_mutex_unlock(p->print);
	usleep(p->plate->time_to_die * 1000);
	return (NULL);
}

int		one_philo(t_philo *p, t_plate *table)
{
	if (pthread_mutex_init(&p[0].left_fork, NULL) == ERROR)
		return (ERROR);
	if (pthread_mutex_init(&table->mutex, NULL) == ERROR)
		return (ERROR);
	init(p, 0, table);
	if (pthread_create(&p[0].thread, NULL, one_thread, &p[0]) == ERROR)
		return (ERROR);
	if (pthread_join(p[0].thread, NULL) == ERROR)
		return (ERROR);
	pthread_mutex_destroy(&p[0].left_fork);
	pthread_mutex_destroy(&table->mutex);
	free(p);
	free(table);
	return (0);
}

int		main(int ac, char **av)
{
	t_philo		*p;
	t_plate		*table;

	table = malloc(sizeof(t_plate));
	if (table == NULL)
		return (ERROR);
	init_table(table, ac, av);
	p = malloc(sizeof(t_philo) * table->num_philos);
	if (p == NULL)
		return (free(table), ERROR);
	if (table->num_philos == 1 && one_philo(p, table) == 0)
		return (0);
	if (init_mutexes(p, table) == ERROR)
		return (ERROR);
	if (create_threads(p, table) == ERROR)
		return (ERROR);
	if (clean(p, table) == ERROR)
		return (ERROR);
	if (died(p))
		return (free(p), free(table), 1);
	free(p), free(table);
	return (0);
}
