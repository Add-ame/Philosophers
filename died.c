#include "philo.h"

int		died(t_philo *p)
{
	int		i;

	while (1)
	{
		i = 0;
		while (i < p[i].plate->num_philos)
		{
			if (p[i].die == STARVED)
			{
				printf("%ld %d died\n", p[i].plate->simulation_end_time, p[i].idx);
				return (STARVED);
			}
			if (p[i].die == FULL)
			{
				printf("Rahom cheb3o\n");
				return (FULL);
			}
			i++;
		}
	}
	return (0);
}
