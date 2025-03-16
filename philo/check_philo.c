/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:21 by maddame           #+#    #+#             */
/*   Updated: 2025/03/16 17:46:42 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *data)
{
	t_philo		*p;
	int			i;

	p = (t_philo *)data;
	while (1)
	{
		i = 0;
		while (i < p[0].table->num_philos)
		{
			if (get_time(&p[i], DIFF_LAST_TO_NEW) > p[i].table->time_to_die)
			{
				pthread_mutex_lock(p[i].checking);
				p[i].table->simulation_end_time = \
				get_time(&p[i], DIFF_START_TO_NEW);
				pthread_mutex_unlock(p[i].checking);
				pthread_mutex_lock(p[i].print);
				printf("%ld %d died\n", p[i].table->simulation_end_time, \
				p[i].idx);
				pthread_mutex_unlock(p[i].print);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}
