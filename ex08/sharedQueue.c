/******************************************************************************
 * File:         main.c
 * Version:      1.2
 * Date:         2017-10-18
 * Author:       J. Onokiewicz, M. van der Sluys
 * Description:  OPS exercise 5: Queues
 ******************************************************************************/


#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void sigintHandler(int sig);

void *producer(void *arguments);
void *consumer(void *arguments);

struct thread_args {
  queue_t *queue;
  data_t data;
  int interval;
};

pthread_mutex_t lock;

int endProgram = 0;

int main() {
  signal(SIGINT, sigintHandler);

  pthread_t producerA, producerB, producerC, consumerA;
  queue_t queue = {NULL};  // Note: element of queue = NULL
  struct thread_args argsP_A = {NULL}, argsP_B = {NULL}, argsP_C = {NULL}, argsC_A = {NULL};

  data_t baseData = {0, "new queue"};
  data_t dataP_A = {1, "producer A"};
  data_t dataP_B = {2, "producer B"};
  data_t dataP_C = {3, "producer C"};
  
  printf("setting arguments\n");
  argsP_A.queue = &queue;
  argsP_B.queue = &queue;
  argsP_C.queue = &queue;
  argsC_A.queue = &queue;

  argsP_A.data = dataP_A;
  argsP_B.data = dataP_B;
  argsP_C.data = dataP_C;

  argsP_A.interval = 2;
  argsP_B.interval = 3;
  argsP_C.interval = 4;
  argsC_A.interval = 15;

  printf("initializing mutex\n");
  pthread_mutex_init(&lock, NULL);
  
  printf("creating queue\n");
  createQueue(&queue, baseData);

  printf("creating threads\n");
  pthread_create(&producerA, NULL, producer, (void *) &argsP_A);
  pthread_create(&producerB, NULL, producer, (void *) &argsP_B);
  pthread_create(&producerC, NULL, producer, (void *) &argsP_C);
  pthread_create(&consumerA, NULL, consumer, (void *) &argsC_A);

  printf("threads created\n");

  pthread_join(producerA, NULL);
  pthread_join(producerB, NULL);
  pthread_join(producerC, NULL);
  pthread_join(consumerA, NULL);
  pthread_mutex_destroy(&lock);
  
  printf("threads joined and mutex destroyed\n");

  deleteQueue(&queue);
  printf("queue deleted\n");
  return 0;
}

void *producer(void *arguments)
{  
  struct thread_args *args = (struct thread_args*)arguments;
  printf("new producer %d\n", args->data.intVal);
  while(!endProgram)
    {
      pthread_mutex_lock(&lock);
      printf("producer %d pushing\n", args->data.intVal);
      pushQueue(&args->queue, args->data);
      pthread_mutex_unlock(&lock);
	
      sleep(args->interval);
    }
  return;
}
void *consumer(void *arguments)
{
  printf("new consumer\n");
  struct thread_args *args = (struct thread_args*)arguments;

  printf("open file\n");
  FILE *fp;
  fp = fopen ("queueFile.txt", "a");
  if(fp = NULL)
    {
      printf("file failed to open\n");
      exit(1);
    }
  while(!endProgram)
    {
      pthread_mutex_lock(&lock);
      printf("now consuming\n");
      printf("write queue to file\n");
      //writeQueueToFile(&args->queue, fp);
      printf("print queue\n");
      //showQueue(&args->queue);
      printf("empty queue\n");
      emptyQueue(&args->queue);
      pthread_mutex_unlock(&lock);
	
      sleep(args->interval);
    }
  printf("close file\n");
  //fclose(fp);
  return;
}

void sigintHandler(int sig)
{
  endProgram = 1;
  return;
}
