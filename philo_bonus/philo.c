#include "philo.h"

int ft_print(t_philo *p, char *s, int flag)
{
    sem_wait(p->table->print);
    printf("%ld %d %s\n", get_time(p, DIFF_START_TO_NEW), p->idx, s);
    sem_post(p->table->print);
    return (0);
}

int eating(t_philo *p)
{
    sem_wait(p->table->forks);
    ft_print(p, "has taken a fork", 0);
    sem_wait(p->table->forks);
    ft_print(p, "has taken a fork", 0);
    ft_print(p, "is eating", 0);
    p->meals_eaten++;
    p->last_meal_time = get_time(p, 0);
    usleep(p->table->time_to_eat * 1000);
    sem_post(p->table->forks);
    sem_post(p->table->forks);
    return (0);
}

void *philo_thread(void *data)
{
    t_philo *p = (t_philo *)data;

	if (p->idx % 2 == EVEN_NUMBER)
		usleep(1000);
    while (1)
    {
		if (ft_print(p, "is thinking", 0) == SIMULATION_END)
			break ;
        if (eating(p) == SIMULATION_END)
            break ;
        if (ft_print(p, "is sleeping", 0) == SIMULATION_END)
            break ;
        usleep(p->table->time_to_sleep * 1000);
    }
    return (NULL);
}

int main(int ac, char **av)
{
    t_philo *p;
    t_table *table;
    pid_t pid;
    int i;

    table = malloc(sizeof(t_table));
    if (table == NULL)
        return (FAIL);
    if (init_table(table, ac, av) == INV_ARG)
        return (INV_ARG);
    p = malloc(sizeof(t_philo) * table->num_philos);
    if (p == NULL)
        return (free(table), FAIL);
	sem_unlink("/forks");
    table->forks = sem_open("/forks", O_CREAT, 0644, table->num_philos);
	sem_unlink("/print");
    table->print = sem_open("/print", O_CREAT, 0644, 1);
	sem_unlink("/checking");
    table->checking = sem_open("/checking", O_CREAT, 0644, 1);

	i = 0;
    while (i < table->num_philos)
    {
        p[i].table = table;
        p[i].idx = i + 1;
        p[i].meals_eaten = 0;
        p[i].start_time = get_time(&p[i], CURRENT_TIME);
        p[i].last_meal_time = get_time(&p[i], CURRENT_TIME);
        pid = fork();
        if (pid == 0)
        {
            pthread_create(&p[i].thread_id, NULL, philo_thread, &p[i]);
            pthread_join(p[i].thread_id, NULL);
            exit(EXIT_SUCCESS);
        }
		i++;
    }

	i = 0;
    while (i < table->num_philos)
	{
        wait(NULL);
		i++;
	}

    sem_close(table->forks);
    sem_unlink("/forks");
    sem_close(table->print);
    sem_unlink("/print");
    sem_close(table->checking);
    sem_unlink("/checking");
    free(p);
    free(table);

    return (0);
}