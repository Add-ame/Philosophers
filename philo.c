/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:48 by maddame           #+#    #+#             */
/*   Updated: 2025/03/06 16:15:34 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	_printf(t_philo *p, char *s)
{
	if (check_philo(p, CHECK_STARVED) == STARVED)
		return (END);
	if (check_philo(p, CHECK_FULL) == FULL)
		return (END);
	pthread_mutex_lock(p->print);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
	{
		pthread_mutex_unlock(p->checking);
		pthread_mutex_unlock(p->print);
		return (END);
	}
	pthread_mutex_unlock(p->checking);
	printf("%ld %d %s\n", get_time(p, diff_start_to_new), p->idx, s);
	pthread_mutex_unlock(p->print);
	return (0);
}

void	lock_fork(t_philo *p, int flag)
{
	if (flag == left_then_right)
	{
		if (p->idx % 2 == even_number)
			pthread_mutex_lock(p->right_fork);
		else
			pthread_mutex_lock(&p->left_fork);
	}
	else if (flag == right_then_left)
	{
		if (p->idx % 2 == even_number)
			pthread_mutex_lock(&p->left_fork);
		else
			pthread_mutex_lock(p->right_fork);
	}
	return ;
}

int	eating(t_philo *p)
{
	if (check_philo(p, CHECK_STARVED) == STARVED)
		return (END);
	lock_fork(p, left_then_right);
	if (check_philo(p, CHECK_STARVED) == STARVED)
	{
		unlock_fork(p, first_fork);
		return (END);
	}
	if (ft_print(p, "has taken a fork", first_fork) == END)
		return (END);
	lock_fork(p, right_then_left);
	if (ft_print(p, "has taken a fork", both_forks) == END)
		return (END);
	if (ft_print(p, "is eating", both_forks) == END)
		return (END);
	p->meals_eaten++;
	p->last_meal_time = get_time(p, current_time);
	usleep(p->table->time_to_eat * 1000);
	pthread_mutex_unlock(&p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
		return (pthread_mutex_unlock(p->checking), END);
	pthread_mutex_unlock(p->checking);
	return (0);
}

void	*add(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;
	if (p->idx % 2 == even_number || p->idx == p->table->num_philos)
	{
		pthread_mutex_lock(p->print);
		printf("%ld %d is thinking\n", get_time(p, diff_start_to_new), p->idx);
		pthread_mutex_unlock(p->print);
		usleep(1000);
	}
	while (1)
	{
		if (eating(p) == END)
			break ;
		if (_printf(p, "is sleeping") == END)
			break ;
		usleep(p->table->time_to_sleep * 1000);
		if (_printf(p, "is thinking") == END)
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
