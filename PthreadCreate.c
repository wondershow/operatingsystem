/**
 A demo example of how to create a pthread and run it from a main thread
 
 gcc PthreadCreate1.c -lpthread
 **/

#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

void *hello(void *arg) /*main thread*/
{
  printf("Foobar\n");
  return 0;
}

int main(void)
{
    int i;
    pthread_t tid[NUM_THREADS];
    printf("Foobar1\n");
    
    for (i = 0; i < NUM_THREADS; i++)
      pthread_create(&tid[i], NULL, hello, NULL);
    
    for (i = 0; i < NUM_THREADS; i++)
      pthread_join(tid[i], NULL);
    
    return 0;
}