/**
 A demo example of how to create a pthread and run it from a main thread
 Plus how to pass data between a main thread and a child thread
 
 gcc PthreadCreate2.c -lpthread
 **/

#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

void *threadFunc(void *arg) /*main thread*/
{
  int *p = (int *) arg;
  printf("ThreadNumber %d\n", *p);
  return 0;
}

int main(void)
{
    int i;
    pthread_t tid[NUM_THREADS];
    
    for (i = 0; i < NUM_THREADS; i++)
      pthread_create(&tid[i], NULL, threadFunc, &i);
    
    for (i = 0; i < NUM_THREADS; i++)
      pthread_join(tid[i], NULL);
    
    return 0;
}

/**
The possible output of this program is(run it 4 times):

run1:
ThreadNumber 2
ThreadNumber 0
ThreadNumber 1
ThreadNumber 3

run2:
ThreadNumber 1
ThreadNumber 3
ThreadNumber 2
ThreadNumber 3

run3:
ThreadNumber 1
ThreadNumber 2
ThreadNumber 3
ThreadNumber 0

run4:
ThreadNumber 2
ThreadNumber 3
ThreadNumber 1
ThreadNumber 1



Note that run 2 and run 4 "seem" to be strange since one of the number is missing.
The reason here is that the variable "i" in man function is  a globally visible variable. 
So when &i changes in one thread, all the other thread can see it. 
To solve this "data race" problem, we can use a private copy of each i. 
int numthread[NUM_THREADS]

for (i = 0; i < NUM_THREADS; i++)
{
  numthread[i] = i;
  pthread_create(&tid[i], NULL, threadFunc, &numthread[i]);
}
  
  
  
  
  
 **/