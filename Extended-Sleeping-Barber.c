#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<semaphore.h>
#include<fcntl.h>
#include<pthread.h>
#include<time.h>
#include<assert.h>
#include<errno.h>
#include<unistd.h>

sem_t semb[3];
sem_t semch[3];
sem_t mutex;

int sal1 = 0, sal2 = 0, sal3 = 0;

void *gotoshop(void *param);
void scheduler(int *cut );
void rescheduler();
void cutting(int *cut);

void main()
{
	int i, j;
	int **cut;
	cut = (int **)malloc(40*sizeof(int*)); 

	for(i=0; i < 40; i++)
		cut[i]=(int*)malloc(2*sizeof(int));
	i = 0;

	pthread_t tid[40];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	sem_init(&semb[0],0,1);
	sem_init(&semb[1],0,1);
	sem_init(&semb[2],0,1);
	sem_init(&semch[0],0,10);
	sem_init(&semch[1],0,10);
	sem_init(&semch[2],0,10);
	sem_init(&mutex,0,1);

	while(1)
	{
		i++;
		printf("\nCustomer #%d; Enter type of haircut [1-3, otherwise exit]: \n", i);
		scanf("%d", cut[i-1]);
		*(cut[i-1]+1) = i;
		fflush(stdin);

		if((*cut[i-1] != 1) && (*cut[i-1] != 2) && (*cut[i-1] != 3))
		{
			printf("\nThanks for taking part in the 3 sleeping barbers problem\n");			
			break;
		} 
		pthread_create(&tid[i-1],&attr,gotoshop,(void *)cut[i-1]);
	}

	for(j=0; j<i-1; j++)
		pthread_join(tid[j],NULL);

	sem_destroy(&semb[0]);
	sem_destroy(&semb[1]);
	sem_destroy(&semb[2]);
	sem_destroy(&semch[0]);
	sem_destroy(&semch[1]);
	sem_destroy(&semch[2]);
}

void *gotoshop(void *param)
{
	int* cut = (int*) param;
	int s;
	if((s=sem_trywait(&semb[*cut-1]))==-1)
		scheduler(cut);
	printf("\nWaking up Barber #%d\n",*cut);
	cutting(cut);
}

void scheduler(int *cut)
{
	int val;int s;
	struct timespec ts;

	if((s=sem_trywait(&semch[*cut-1]))==-1)
	{
		printf("\nCustomer #%d is leaving the shop. The queue is full\n",*(cut+1));
		pthread_exit(0);
	}

	if(clock_gettime(CLOCK_REALTIME,&ts)==-1)
		perror("clock_gettime");
	ts.tv_sec+=20;

	if((s=sem_timedwait(&semb[*cut-1],&ts))==-1)
	{
		if(errno == ETIMEDOUT)
			rescheduler(cut);
		else
			perror("sem_timedwait");
	}
	sem_post(&semch[*cut-1]);
	cutting(cut);
}

void rescheduler(int *cut )
{
	int ch1, ch2, ch3;
	int *i;
	i = (int *)malloc(2*sizeof(int));

	sem_wait(&mutex);
	sem_getvalue(&semch[0],&ch1);
	sem_getvalue(&semch[1],&ch2);
	sem_getvalue(&semch[2],&ch3);
	sem_post(&mutex);

	if((ch1 >= ch2) && (ch1 >= ch3))
		*i = 1;
	else if((ch2 >= ch1) && (ch2 >= ch3)) 
		*i = 2;
	else
		*i = 3;

	printf("\nCustomer #%d waited for 20 sec and went to Barber #%d\n", *(cut+1), *i); 
	*(i+1) = *(cut+1);

	sem_post(&semch[*cut-1]);
	gotoshop((void*)i);
	
	return;
}

void cutting(int *cut)
{
	int val;

	if (*cut == 1)
	{
		sleep(5);
		sal1++;
		printf("\nCustomer #%d got a haircut from Barber #1. His current salary is Rs.%d\n", *(cut+1), (sal1*20));
	}
 	else if(*cut==2)
	{
		sleep(10);
		sal2++;
		printf("\nCustomer #%d got a haircut from Barber #2. His current salary is Rs.%d\n", *(cut+1), (sal2*20));
	}
	else if (*cut == 3)
	{
		sleep(5);
		sal3++;
		printf("\nCustomer #%d got a haircut from Barber #3. His current salary is Rs.%d\n", *(cut+1), (sal3*20));
	}

	sem_post(&semb[*cut-1]);
	sem_getvalue(&semch[*cut-1],&val);

	if(val == 10)
		printf("\nBarber #%d has gone to sleep\n",*cut);
	
	pthread_exit(0);
}
