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
		pthread_mutex_lock(p->checking);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(p->checking), NULL);
		pthread_mutex_unlock(p->checking);
		if (check_philo(p, CHECK_STARVED) == STARVED)
			return (NULL);
		if (p->idx % 2)
		{
			pthread_mutex_lock(&p->left_fork);
			pthread_mutex_lock(p->print);
			if (check_philo(p, CHECK_STARVED) == STARVED)
			{
				pthread_mutex_unlock(p->print);
				pthread_mutex_unlock(&p->left_fork);
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
			if (check_philo(p, CHECK_STARVED) == STARVED)
			{
				pthread_mutex_unlock(p->print);
				pthread_mutex_unlock(p->right_fork);
				return (NULL);
			}
			printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
			pthread_mutex_unlock(p->print);
			pthread_mutex_lock(&p->left_fork);
		}
		pthread_mutex_lock(p->print);
		pthread_mutex_lock(p->checking);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(&p->left_fork), pthread_mutex_unlock(p->right_fork), pthread_mutex_unlock(p->checking), pthread_mutex_unlock(p->print), NULL);
		pthread_mutex_unlock(p->checking);
		printf("%ld %d has taken a fork\n", get_real_time(p, 1), p->idx);
		pthread_mutex_unlock(p->print);
		pthread_mutex_lock(p->print);
		pthread_mutex_lock(p->checking);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(&p->left_fork), pthread_mutex_unlock(p->right_fork), pthread_mutex_unlock(p->checking), pthread_mutex_unlock(p->print), NULL);
		pthread_mutex_unlock(p->checking);
		printf("%ld %d is eating\n", get_real_time(p, 1) ,p->idx);
		pthread_mutex_unlock(p->print);
		p->meals_eaten++;
		p->last_meal_time = get_real_time(p, 0);
		usleep(p->plate->time_to_eat * 1000);
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);

		if (check_philo(p, CHECK_STARVED) == STARVED || check_philo(p, CHECK_FULL) == FULL)
			return (NULL);

		pthread_mutex_lock(p->print);
		pthread_mutex_lock(p->checking);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(p->checking), pthread_mutex_unlock(p->print), NULL);
		pthread_mutex_unlock(p->checking);
		printf("%ld %d is sleeping\n", get_real_time(p, 1) ,p->idx);
		pthread_mutex_unlock(p->print);
		usleep(p->plate->time_to_sleep * 1000);

		if (check_philo(p, CHECK_STARVED) == STARVED || check_philo(p, CHECK_FULL) == FULL)
			return (NULL);

		pthread_mutex_lock(p->print);
		pthread_mutex_lock(p->checking);
		if (p->plate->simulation_end_time)
			return (pthread_mutex_unlock(p->checking), pthread_mutex_unlock(p->print), NULL);
		pthread_mutex_unlock(p->checking);
		printf("%ld %d is thinking\n", get_real_time(p, 1),p->idx);
		pthread_mutex_unlock(p->print);
	}
	return NULL;
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
	return (0);
}
