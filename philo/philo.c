/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:48 by maddame           #+#    #+#             */
/*   Updated: 2025/03/16 23:20:03 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_print(t_philo *p, char *s, int flag)
{
	pthread_mutex_lock(p->print);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
	{
		pthread_mutex_unlock(p->checking);
		pthread_mutex_unlock(p->print);
		if (flag == BOTH_FORKS)
		{
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(&p->left_fork);
		}
		else if (flag == FIRST_FORK)
			pthread_mutex_unlock(&p->left_fork);
		return (SIMULATION_END);
	}
	pthread_mutex_unlock(p->checking);
	printf("%ld %d %s\n", get_time(p, DIFF_START_TO_NEW), p->idx, s);
	pthread_mutex_unlock(p->print);
	return (0);
}

int	eating(t_philo *p)
{
	pthread_mutex_lock(&p->left_fork);
	if (ft_print(p, "has taken a fork", FIRST_FORK) == SIMULATION_END)
		return (SIMULATION_END);
	pthread_mutex_lock(p->right_fork);
	if (ft_print(p, "has taken a fork", BOTH_FORKS) == SIMULATION_END)
		return (SIMULATION_END);
	if (ft_print(p, "is eating", BOTH_FORKS) == SIMULATION_END)
		return (SIMULATION_END);
	pthread_mutex_lock(p->last_meal);
	p->meals_eaten++;
	p->last_meal_time = get_time(p, CURRENT_TIME);
	pthread_mutex_unlock(p->last_meal);
	if (p->meals_eaten == p->table->num_to_eat)
	{
		pthread_mutex_lock(p->all_eat);
		p->table->all_eaten++;
		pthread_mutex_unlock(p->all_eat);
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return (SIMULATION_END);
	}
	usleep(p->table->time_to_eat * 1000);
	pthread_mutex_unlock(&p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	return (0);
}

int	_printf(t_philo *p, char *s)
{
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

void	*philo_thread(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;
	if (p->idx % 2 == EVEN_NUMBER || p->idx == p->table->num_philos)
		usleep(1000);
	while (1)
	{
		if (_printf(p, "is thinking") == SIMULATION_END)
			break ;
		if (eating(p) == SIMULATION_END)
			break ;
		if (_printf(p, "is sleeping") == SIMULATION_END)
			break ;
		usleep(p->table->time_to_sleep * 1000);
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
