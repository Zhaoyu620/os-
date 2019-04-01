#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


char buf[10] = {0};
sem_t product;  	//信号量，用来表示buf缓存区中空闲单元的个数
sem_t consume;		//信号量，用来表示buf缓存区中非空闲单元的个数

void *worker1(void *arg) {
	
	for (int i = 0; i < 10;) {
	sem_wait(&consume);
	scanf("%c",&buf[i]);
	getchar();
	i++;
	i = i%10;
	sem_post(&product);
	}
	
	return NULL;
}

void *worker2(void *arg) {

	for (int i = 0; i < 10;) {
	sem_wait(&product);
	printf("%c\n",buf[i]);
	sleep(1);
	i++;
	i = i%10;
	sem_post(&consume);
	}

	return NULL;
}

int main(int argc, char *argv[])
{	
	sem_init(&consume, 0, 10);
	sem_init(&product, 0, 0);

	pthread_t p1, p2;
	pthread_create(&p1, NULL, worker1, NULL);
	pthread_create(&p2, NULL, worker2, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	sem_destroy(&consume);
	sem_destroy(&product);

	return 0;
}
