#include "philo.h"

int		balance = 0;

int		read_balance()
{
	usleep(250000);
	return (balance);
}

void	write_balance(int new_balance)
{
	usleep(250000);
	balance = new_balance;
}

pthread_mutex_t	mutex;

void	*deposit(void *amount)
{
	int		acc_amount;

	pthread_mutex_lock(&mutex);
	acc_amount = read_balance();
	acc_amount += *((int *)amount);
	write_balance(acc_amount);
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int		main()
{
	printf("before: %d\n", read_balance());

	pthread_t	t1;
	pthread_t	t2;

	int		v1 = 300;
	int		v2 = 200;

	pthread_mutex_init(&mutex, NULL);
	pthread_create(&t1, NULL, deposit, (void *) &v1);
	pthread_create(&t2, NULL, deposit, (void *) &v2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	pthread_mutex_destroy(&mutex);
	printf("after: %d\n", read_balance());
	return (0);
}
