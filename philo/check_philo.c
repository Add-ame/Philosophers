/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:21 by maddame           #+#    #+#             */
/*   Updated: 2025/03/14 02:52:06 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo(t_philo *p, int flag)
{
	if (flag == CHECK_STARVED)
	{
		if (get_time(p, DIFF_LAST_TO_NEW) >= p->table->time_to_die)
		{
			pthread_mutex_lock(p->print);
			p->table->died++;
			pthread_mutex_unlock(p->print);
			pthread_mutex_lock(p->checking);
			p->table->simulation_end_time = get_time(p, DIFF_START_TO_NEW);
			pthread_mutex_unlock(p->checking);
			pthread_mutex_lock(p->print);
			if (p->table->died == 1)
				printf("%ld %d died\n", p->table->simulation_end_time, p->idx);
			pthread_mutex_unlock(p->print);
			return (STARVED);
		}
	}
	else if (flag == CHECK_FULL)
	{
		if (p->meals_eaten == p->table->num_to_eat)
		{
			return (FULL);
		}
	}
	return (0);
}
