/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:40 by maddame           #+#    #+#             */
/*   Updated: 2025/03/06 16:09:18 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_thread(void	*data)
{
	t_philo		*p;

	p = (t_philo *)data;
	printf("%d %d is thinking\n", 0, 1);
	printf("%d %d has taken a fork\n", 0, 1);
	usleep(p->table->time_to_die * 1000);
	return (NULL);
}

int	one_philo(t_table *table)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, one_thread, &table) == ERROR)
		return (ERROR);
	if (pthread_join(thread, NULL) == ERROR)
		return (ERROR);
	free(table);
	return (0);
}
