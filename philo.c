#include "philo.h"

int		mails = 0;

void    *f1()
{
	for (int i = 0; i < 100000; i++)
		mails++;

	return NULL;
}

void    *f2()
{
	printf("philo 2\n");
	sleep(1);
	printf("2 Ended\n");
	return NULL;
}

int     main()
{
	pthread_t	t1;
	pthread_t	t2;

	pthread_create(&t1, NULL, &f1, NULL);
	pthread_create(&t2, NULL, &f1, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	// printf("%d\n", mails);
	return (0);
}