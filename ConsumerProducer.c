#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE   3 /*size of shared buffer*/
/**
 This is the in-class example of consumer and producer. 
 A consumer and a producer share the product buffer.
 A producer produces one product once and a consumer eats up one product once. 
 When the buffer is empty, consumer has nothing to eat while the buffer is full
 producer is not allowed to produce. 
 
 so in this case, buffer should be in critical area, plus other 3 variables(add & rem & num) 
 are supposed to be critical. We use a mutex to control the exlusive access of the varaiables 
 in the critical area, making sure that only one thread (either producer or consumer)
 can access the critical area at a time. In order to handle the wait condition, we need to cond_t 
 variables (c_prod, c_cons)to indicate the condition that the buffer is no long empty and the buffer 
 is no longer full.
 

 **/


int buffer[BUFF_SIZE]; /*shared buffer*/
int add = 0; /*place to add next element*/
int rem = 0; /*place to remove next ele*/
int num = 0; /*number eles in buffer*/


pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; /*mutex for buffer*/
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /*consumer waits on cv*/
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /*producer waits on cv*/

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[])
{
  pthread_t tid1, tid2;
  int i;
  
  if (pthread_create(&tid1, NULL, producer, NULL) != 0)
  {
    fprintf(stderr, "Unable to create producer thread\n");
    exit(1);
  }
  
  if (pthread_create(&tid2, NULL, consumer, NULL) != 0)
  {
    fprintf(stderr, "Unable to create consumer thread\n");
    exit(1);
  }
  
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  
  printf("Parent quiting\n");
}

void *producer(void *param)
{
  int i;
  for (i = 0; i <= 20; i++)
  {
      pthread_mutex_lock(&m);
      if (num > BUFF_SIZE) exit(1); /*overflow*/

      while (num == BUFF_SIZE)
	pthread_cond_wait(&c_prod, &m);
    
      buffer[add] = i;
      add = (add + 1) % BUFF_SIZE;
      num++;
      pthread_mutex_unlock(&m);
      
      pthread_cond_signal(&c_cons);
      printf("producer: inserted %d \n", i);
      fflush(stdout);
  }
  printf("producer quiting \n");
  fflush(stdout);
  return 0;
}

void *consumer(void *param)
{
  int consnum;
  while (1)
  {
    pthread_mutex_lock(&m);
      if (num < 0) exit(1); // underflow
      
      while (num == 0)
	pthread_cond_wait(&c_cons, &m);
      
      consnum = buffer[rem];
      rem = (rem + 1) % BUFF_SIZE;
      num--;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&c_prod);
    printf("Consumer: consumed %d \n",  consnum);fflush(stdout);
  }
}