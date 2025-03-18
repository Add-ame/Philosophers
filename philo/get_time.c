#include "philo.h"

long	get_time(t_philo *p, int flag)
{
	long				result;
	struct timeval		current_time;

	gettimeofday(&current_time, NULL);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	if (flag == DIFF_START_TO_NEW)
		result = result - p->start_time;
	else if (flag == DIFF_LAST_TO_NEW)
	{
		pthread_mutex_lock(p->last_meal);
		result = result - p->last_meal_time;
		pthread_mutex_unlock(p->last_meal);
	}
	return (result);
}
