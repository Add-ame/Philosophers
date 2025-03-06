/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:53 by maddame           #+#    #+#             */
/*   Updated: 2025/03/06 15:52:54 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_philo *p, int flag)
{
	long				result;
	struct timeval		current_time;

	gettimeofday(&current_time, NULL);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	if (flag == diff_start_to_new)
		result = result - p->start_time;
	else if (flag == diff_last_to_new)
		result = result - p->last_meal_time;
	return (result);
}

int	ft_print(t_philo *p, char *s, int flag)
{
	pthread_mutex_lock(p->print);
	pthread_mutex_lock(p->checking);
	if (p->table->simulation_end_time)
	{
		pthread_mutex_unlock(p->checking);
		pthread_mutex_unlock(p->print);
		unlock_fork(p, flag);
		return (END);
	}
	pthread_mutex_unlock(p->checking);
	printf("%ld %d %s\n", get_time(p, diff_start_to_new), p->idx, s);
	pthread_mutex_unlock(p->print);
	return (0);
}

void	unlock_fork(t_philo *p, int flag)
{
	if (flag == first_fork)
	{
		if (p->idx % 2 == even_number)
			pthread_mutex_unlock(p->right_fork);
		else
			pthread_mutex_unlock(&p->left_fork);
	}
	else if (flag == both_forks)
	{
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	return ;
}
