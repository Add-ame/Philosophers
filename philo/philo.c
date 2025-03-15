/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:48 by maddame           #+#    #+#             */
/*   Updated: 2025/03/14 03:10:34 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	_printf(t_philo *p, char *s)
{
	if (check_philo(p, CHECK_STARVED) == STARVED || p->table->simulation_end_time)
		return (SIMULATION_END);
	if (check_philo(p, CHECK_FULL) == FULL)
		return (SIMULATION_END);
	pthread_mutex_lock(p->print);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
	{
		pthread_mutex_unlock(p->checking);
		pthread_mutex_unlock(p->print);
		return (SIMULATION_END);
	}
	pthread_mutex_unlock(p->checking);
	printf("%ld %d %s\n", get_time(p, DIFF_START_TO_NEW), p->idx, s);
	pthread_mutex_unlock(p->print);
	return (0);
}

void	lock_fork(t_philo *p, int flag)
{
	if (flag == LEFT_THEN_RIGHT)
	{
		if (p->idx % 2 == EVEN_NUMBER)
			pthread_mutex_lock(p->right_fork);
		else
			pthread_mutex_lock(&p->left_fork);
	}
	else if (flag == RIGHT_THEN_LEFT)
	{
		if (p->idx % 2 == EVEN_NUMBER)
			pthread_mutex_lock(&p->left_fork);
		else
			pthread_mutex_lock(p->right_fork);
	}
	return ;
}

int	eating(t_philo *p)
{
	if (check_philo(p, CHECK_STARVED) == STARVED)
		return (SIMULATION_END);
	lock_fork(p, LEFT_THEN_RIGHT);
	if (check_philo(p, CHECK_STARVED) == STARVED)
	{
		unlock_fork(p, FIRST_FORK);
		return (SIMULATION_END);
	}
	if (ft_print(p, "has taken a fork", FIRST_FORK) == SIMULATION_END)
		return (SIMULATION_END);
	lock_fork(p, RIGHT_THEN_LEFT);
	if (ft_print(p, "has taken a fork", BOTH_FORKS) == SIMULATION_END)
		return (SIMULATION_END);
	if (ft_print(p, "is eating", BOTH_FORKS) == SIMULATION_END)
		return (SIMULATION_END);
	p->meals_eaten++;
	p->last_meal_time = get_time(p, CURRENT_TIME);
	usleep(p->table->time_to_eat * 1000);
	pthread_mutex_unlock(&p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
		return (pthread_mutex_unlock(p->checking), SIMULATION_END);
	pthread_mutex_unlock(p->checking);
	return (0);
}

void	*philo_thread(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;
	if (p->idx % 2 == EVEN_NUMBER || p->idx == p->table->num_philos)
	{
		pthread_mutex_lock(p->print);
		printf("%ld %d is thinking\n", get_time(p, DIFF_START_TO_NEW), p->idx);
		pthread_mutex_unlock(p->print);
		usleep(1000);
	}
	while (1)
	{
		if (eating(p) == SIMULATION_END)
			break ;
		if (_printf(p, "is sleeping") == SIMULATION_END)
			break ;
		usleep(p->table->time_to_sleep * 1000);
		if (_printf(p, "is thinking") == SIMULATION_END)
			break ;
	}
	return (NULL);
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
