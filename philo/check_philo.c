/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:21 by maddame           #+#    #+#             */
/*   Updated: 2025/03/16 01:09:12 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo(t_philo *p, int flag)
{
	if (flag == CHECK_FULL)
	{
		if (p->meals_eaten == p->table->num_to_eat)
		{
			return (FULL);
		}
	}
	return (0);
}
