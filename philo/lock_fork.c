#include "philo.h"

void	lock_fork(t_philo *p, int flag)
{
	if (flag == LEFT_THEN_RIGHT)
	{
		if (p->table->num_philos % 2 != EVEN_NUMBER)
			pthread_mutex_lock(&p->left_fork);
		else if (p->idx % 2 == EVEN_NUMBER)
			pthread_mutex_lock(p->right_fork);
		else
			pthread_mutex_lock(&p->left_fork);
	}
	else if (flag == RIGHT_THEN_LEFT)
	{
		if (p->table->num_philos % 2 != EVEN_NUMBER)
			pthread_mutex_lock(p->right_fork);
		else if (p->idx % 2 == EVEN_NUMBER)
			pthread_mutex_lock(&p->left_fork);
		else
			pthread_mutex_lock(p->right_fork);
	}
	return ;
}

void	unlock_fork(t_philo *p, int flag)
{
	if (p->table->num_philos % 2 != EVEN_NUMBER)
	{
		if (flag == BOTH_FORKS)
		{
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(&p->left_fork);
		}
		else if (flag == FIRST_FORK)
			pthread_mutex_unlock(&p->left_fork);
		return ;
	}
	if (flag == FIRST_FORK)
	{
		if (p->idx % 2 == EVEN_NUMBER)
			pthread_mutex_unlock(p->right_fork);
		else
			pthread_mutex_unlock(&p->left_fork);
	}
	else if (flag == BOTH_FORKS)
	{
		pthread_mutex_unlock(&p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	return ;
}
