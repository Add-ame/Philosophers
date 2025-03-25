#include "philo.h"

int	ft_print(t_philo *p, char *s, int flag)
{
	pthread_mutex_lock(p->print);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
	{
		pthread_mutex_unlock(p->checking);
		pthread_mutex_unlock(p->print);
		unlock_fork(p, flag);
		return (SIMULATION_END);
	}
	pthread_mutex_unlock(p->checking);
	printf("%ld %d %s\n", get_time(p, DIFF_START_TO_NEW), p->idx, s);
	pthread_mutex_unlock(p->print);
	return (0);
}

int	eating(t_philo *p)
{
	lock_fork(p, LEFT_THEN_RIGHT);
	if (ft_print(p, "has taken a fork", FIRST_FORK) == SIMULATION_END)
		return (SIMULATION_END);
	lock_fork(p, RIGHT_THEN_LEFT);
	if (ft_print(p, "has taken a fork", BOTH_FORKS) == SIMULATION_END)
		return (SIMULATION_END);
	if (ft_print(p, "is eating", BOTH_FORKS) == SIMULATION_END)
		return (SIMULATION_END);
	pthread_mutex_lock(p->last_meal);
	p->meals_eaten++;
	p->last_meal_time = get_time(p, CURRENT_TIME);
	pthread_mutex_unlock(p->last_meal);
	usleep(p->table->time_to_eat * 1000);
	if (p->meals_eaten == p->table->num_to_eat)
	{
		pthread_mutex_lock(p->all_eat);
		p->table->all_eaten++;
		pthread_mutex_unlock(p->all_eat);
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return (SIMULATION_END);
	}
	pthread_mutex_unlock(&p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	return (0);
}

void	*philo_thread(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;
	if (p->idx % 2 == EVEN_NUMBER || p->idx == p->table->num_philos)
		usleep(1000);
	while (1)
	{
		if (ft_print(p, "is thinking", THINK) == SIMULATION_END)
			break ;
		if (p->idx % 2 == EVEN_NUMBER && p->table->num_philos % 2)
			usleep(1000);
		if (eating(p) == SIMULATION_END)
			break ;
		if (ft_print(p, "is sleeping", SLEEP) == SIMULATION_END)
			break ;
		usleep(p->table->time_to_sleep * 1000);
	}
	return (NULL);
}

int	create_threads(t_philo *p, t_table *table)
{
	pthread_t	id;
	int			i;

	i = 0;
	while (i < table->num_philos)
	{
		init(&p[i], i, table);
		p[i].right_fork = &p[(i + 1) % table->num_philos].left_fork;
		if (pthread_create(&p[i].t_id, NULL, philo_thread, &p[i]) != SUCESS)
			return (ERROR);
		i++;
	}
	pthread_create(&id, NULL, monitoring, p);
	pthread_join(id, NULL);
	i = -1;
	while (++i < table->num_philos)
	{
		if (pthread_join(p[i].t_id, NULL) != SUCESS)
			return (ERROR);
	}
	return (SUCESS);
}

int	main(int ac, char **av)
{
	t_philo		*p;
	t_table		*table;

	table = malloc(sizeof(t_table));
	if (table == NULL)
		return (FAIL);
	if (init_table(table, ac, av) == INV_ARG)
		return (INV_ARG);
	if (table->num_philos == 1)
		return (one_philo(table));
	p = malloc(sizeof(t_philo) * table->num_philos);
	if (p == NULL)
		return (free(table), FAIL);
	if (init_mutexes(p, table) == ERROR)
		return (FAIL);
	if (create_threads(p, table) == ERROR)
		return (FAIL);
	if (clean(p, table) == ERROR)
		return (FAIL);
	return (SUCESS);
}
