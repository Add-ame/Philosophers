#include "philo.h"

pthread_mutex_t		mutex;

int		ii = 0;


void	*add(void *d)
{
	int		flag;

	flag = 0;
	pthread_mutex_lock(&mutex);

	pthread_mutex_lock(&((t_data *)d)->fork[((t_data *)d)->philo[ii].idx - 1]);
	printf("%d ", ((t_data *)d)->philo[ii].idx);
	printf("1 fork\n");
	flag++;
	pthread_mutex_unlock(&((t_data *)d)->fork[((t_data *)d)->philo[ii++].idx - 1]);

	pthread_mutex_lock(&((t_data *)d)->fork[((t_data *)d)->philo[ii].idx - 1]);
	printf("%d ", ((t_data *)d)->philo[ii - 1].idx);
	printf("2 fork\n");
	flag++;
	pthread_mutex_unlock(&((t_data *)d)->fork[((t_data *)d)->philo[ii++].idx - 1]);
	if (flag == 2)
	{
		printf("eating\n");
		usleep(((t_data *)d)->time_to_eat);
	}

	pthread_mutex_unlock(&mutex);
	return NULL;
}

void	init(t_data *d, int ac, char **av)
{
	int		i;

	if (ac != 5 && ac != 6)
		exit(22);
	d->num_philos = atoi(av[1]);
	d->time_to_die = atoi(av[2]);
	d->time_to_eat = atoi(av[3]);
	d->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		d->must_eat_time = atoi(av[5]);

	i = 0;
	while (i <= d->num_philos)
	{
		d->philo[i].idx = i + 1;
		d->philo[i].last_meal_time = 0;
		d->philo[i].meals_eaten = 0;
		i++;
	}
}

int		main(int ac, char **av)
{
	t_data		d;
	int		i;

	init(&d, ac, av);
	pthread_mutex_init(&mutex, NULL);
	i = 0;
	while (i < 5)
	{
		pthread_mutex_init(&d.fork[i], NULL);
		// exit(1);
		if (pthread_create(&d.philo[i].thread, NULL, add, &d) != 0)
			return (perror("fail"), 1);
		i++;
	}
	i = 0;
	while (i < 5)
	{
		if (pthread_join(d.philo[i].thread, NULL) != 0)
			return (perror("fail"), 1);
		pthread_mutex_destroy(&d.fork[i]);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	printf("\ndied\n");

	return (0);
}
