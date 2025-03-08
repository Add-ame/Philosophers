/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:27 by maddame           #+#    #+#             */
/*   Updated: 2025/03/08 17:39:15 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_philo *p, t_table *table)
{
	int		i;

	i = 0;
	while (i < table->num_philos)
	{
		init(&p[i], i, table);
		p[i].right_fork = &p[(i + 1) % table->num_philos].left_fork;
		if (pthread_create(&p[i].thread_id, NULL, add, &p[i]) != SUCESS)
			return (ERROR);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_join(p[i].thread_id, NULL) != SUCESS)
			return (ERROR);
		i++;
	}
	return (SUCESS);
}
