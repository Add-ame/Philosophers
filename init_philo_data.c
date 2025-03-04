#include "philo.h"

void	init(t_philo *p, int i, t_plate *pl)
{
	p->idx = i + 1;
	p->meals_eaten = 0;
	p->start_time = get_real_time(p, 0);
	p->last_meal_time = get_real_time(p, 0);
	p->die = 0;
	p->plate = pl;
	p->print = &pl->mutex;
	p->upd = &pl->mutex_upd;
}
