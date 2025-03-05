#include "philo.h"

int		check_philo(t_philo *p, int flag)
{
	if (flag == CHECK_STARVED)
	{
		if (get_real_time(p, 2) >= p->plate->time_to_die)
		{
			pthread_mutex_lock(p->print);
			p->plate->died++;
			pthread_mutex_unlock(p->print);
			p->die = STARVED;
			pthread_mutex_lock(p->checking);
			p->plate->simulation_end_time = get_real_time(p, 1);
			pthread_mutex_unlock(p->checking);
			pthread_mutex_lock(p->print);
			if (p->plate->died == 1)
				printf("%ld %d died\n", p->plate->simulation_end_time, p->idx);
			pthread_mutex_unlock(p->print);
			return (STARVED);
		}
	}
	else if (flag == CHECK_FULL)
	{
		if (p->meals_eaten == p->plate->must_eat_num)
		{
			p->die = FULL;
			return (FULL);
		}
	}
	return (0);
}
