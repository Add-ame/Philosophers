#include "philo.h"

void	init(t_philo *p, int i, pthread_mutex_t *mutex, t_plate *pl)
{
	p->idx = i + 1;
	p->meals_eaten = 0;
	p->print = mutex;
	p->start_time = get_real_time();
	p->last_meal_time = get_real_time();
	p->die = 0;
	p->plate = pl;
}
