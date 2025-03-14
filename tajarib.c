#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/fs.h>
#include </usr/include/linux/fs.h>
#include <semaphore.h>

sem_t	sem;
pthread_mutex_t	mutex;


void* thread_func(void *arg)
{
	printf("waiting\n");
	sem_wait(&sem);
	// pthread_mutex_lock(&mutex);
	sleep(1);
	printf("Test\n");
	sem_post(&sem);
	// pthread_mutex_unlock(&mutex);
	return NULL;
}  

int main()
{
	pthread_t thread_1;
	pthread_t thread_2;
	pthread_t thread_3;
	pthread_t thread_4;

	sem_init(&sem, 0, 2);
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&thread_1, NULL, thread_func, NULL);
	pthread_create(&thread_2, NULL, thread_func, NULL);
	pthread_create(&thread_3, NULL, thread_func, NULL);
	pthread_create(&thread_4, NULL, thread_func, NULL);

	// printf("%lu\n", thread_2);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	pthread_join(thread_3, NULL);
	pthread_join(thread_4, NULL);
	sem_destroy(&sem);
	pthread_mutex_destroy(&mutex);
	// sleep(1);
	// pthread_detach(thread_2);
	return 0;
}
