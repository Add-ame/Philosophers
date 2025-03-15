/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:53 by maddame           #+#    #+#             */
/*   Updated: 2025/03/14 03:10:34 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_philo *p, int flag)
{
	long				result;
	struct timeval		current_time;

	gettimeofday(&current_time, NULL);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	if (flag == DIFF_START_TO_NEW)
		result = result - p->start_time;
	else if (flag == DIFF_LAST_TO_NEW)
		result = result - p->last_meal_time;
	return (result);
}

int	ft_print(t_philo *p, char *s, int flag)
{
	if (check_philo(p, CHECK_STARVED) == STARVED)
	{
		unlock_fork(p, flag);
		return (SIMULATION_END);
	}
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
	if (check_philo(p, CHECK_STARVED) == STARVED)
	{
		unlock_fork(p, flag);
		return (SIMULATION_END);
	}
	return (0);
}

void	unlock_fork(t_philo *p, int flag)
{
	if (flag == FIRST_FORK)
	{
		if (p->idx % 2 == EVEN_NUMBER)
			pthread_mutex_unlock(p->right_fork);
		else
			pthread_mutex_unlock(&p->left_fork);
	}
	else if (flag == BOTH_FORKS)
	{
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	return ;
}