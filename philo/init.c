/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddame <maddame@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:52:36 by maddame           #+#    #+#             */
/*   Updated: 2025/03/16 01:04:13 by maddame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_error(char *s)
{
	int		i;
	int		j;
	int		neg;
	int		dig;
	char	*tmp;

	i = 0;
	j = -1;
	neg = 0;
	tmp = s;
	while (s[++j] == ' ' || (s[j] >= 9 && s[j] <= 13) || \
	(s[j] >= '0' && s[j] <= '9') || s[j] == '-' || s[j] == '+')
		if (++neg && (s[j] == '-' || s[j] == '+') && (!(s[j + 1] >= '0' \
		&& s[j + 1] <= '9') || s[j - 1] != ' '))
			if (++i == 2 || (s[j - 1] >= '0' && s[j - 1] <= '9') || \
				s[j + 1] == ' ' || s[j + 1] == '\0')
				break ;
	dig = 0;
	i = -1;
	while (tmp[++i])
		if (tmp[i] >= '0' && tmp[i] <= '9')
			dig++;
	return (s[j] == '\0' && !(neg == 1 && dig == 0));
}

long	ft_atoi(char *s)
{
	long	i;
	long	sign;
	long	dec;

	if (check_error(s) == 0)
		return (0);
	i = 0;
	dec = 0;
	sign = 1;
	while (s[i] == 32 || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '-' || s[i] == '+')
		if (s[i++] == '-')
			sign = -1;
	while (s[i] >= '0' && s[i] <= '9')
		dec = (dec * 10) + (s[i++] - 48);
	return (dec * sign);
}

void	init(t_philo *p, int i, t_table *pl)
{
	p->idx = i + 1;
	p->meals_eaten = 0;
	p->start_time = get_time(p, CURRENT_TIME);
	p->last_meal_time = get_time(p, CURRENT_TIME);
	p->table = pl;
	p->print = &pl->mutex;
	p->checking = &pl->checking;
}

int	init_table(t_table *table, int ac, char **av)
{
	if (ac != 5 && ac != 6)
	{
		printf("Invalid argument\n");
		free(table);
		return (INV_ARG);
	}
	table->simulation_end_time = 0;
	table->died = 0;
	table->should_die = 0;
	table->num_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->num_to_eat = -1;
	if (ac == 6)
		table->num_to_eat = ft_atoi(av[5]);
	if (table->num_philos <= 0 || table->time_to_die <= 0 || \
		table->time_to_eat <= 0 || table->time_to_sleep <= 0 || \
		table->num_to_eat == 0 || table->num_to_eat < -1)
	{
		printf("Invalid argument\n");
		free(table);
		return (INV_ARG);
	}
	if (table->num_philos % 2 == EVEN_NUMBER)
	{
		if (table->time_to_eat * 2 >= table->time_to_die)
			table->should_die = 1;
	}
	else
		if (table->time_to_eat * 3 >= table->time_to_die)
			table->should_die = 1;
	return (0);
}

int	init_mutexes(t_philo *p, t_table *table)
{
	int		i;

	i = 0;
	if (pthread_mutex_init(&table->mutex, NULL) == ERROR)
		return (ERROR);
	if (pthread_mutex_init(&table->checking, NULL) == ERROR)
		return (ERROR);
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&p[i].left_fork, NULL) == ERROR)
			return (ERROR);
		i++;
	}
	return (0);
}
