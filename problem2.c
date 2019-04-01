#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int ticketCount = 1000;
int temp;

sem_t product;  //全局变量，所有线程可见。主程序开始
sem_t consume;

void *sale(void *arg) {
	for (int i = 0; i < 1050; i++){
		
		sem_wait(&consume);
		temp = ticketCount;
		pthread_yield();
		temp = temp - 1;
		pthread_yield();
		ticketCount = temp;
		printf("sale: %d ticketCount: %d ",i, ticketCount);
		sem_post(&product);
	}
	return NULL;
}


void *refund(void *arg) {

	for (int i = 0; i < 1050; i++){
		sem_wait(&product);
		temp = ticketCount;
		pthread_yield();
		temp = temp + 1;
		pthread_yield();
		ticketCount = temp;
		printf("refund: %d ticketCount: %d ",i, ticketCount);
		sem_post(&consume);
	}
	return NULL;

}

int main(int argc, char *argv[])

{	
	sem_init(&product, 0, 0);//在多线程之前初始化信号量
	sem_init(&consume, 0, 1000);

	pthread_t p1, p2;
	pthread_create(&p1, NULL, sale, NULL);
	pthread_create(&p2, NULL, refund, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	sem_destroy(&consume);
	sem_destroy(&product);

	return 0;
}