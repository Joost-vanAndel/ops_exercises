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

void *producer(void *arguments);
void *consumer(void *arguments);

struct thread_args {
  queue_t *queue;
  data_t data;
  int interval;
};

int main() {
  pthread_t producerA, producerB, producerC, consumerA;
  queue_t queue = {NULL};  // Note: element of queue = NULL
  struct thread_args argsP_A = {NULL}, argsP_B = {NULL}, argsP_C = {NULL}, argsC_A = {NULL};

  printf("setting arguments\n");
  argsP_A.queue = &queue;
  argsP_B.queue = &queue;
  argsP_C.queue = &queue;
  argsC_A.queue = &queue;

  argsP_A.data.text[20] = "producer A";
  argsP_A.data.intVal = 1;
  argsP_B.data.text[20] = "producer B";
  argsP_B.data.intVal = 2;
  argsP_C.data.text[20] = "producer C";
  argsP_C.data.intVal = 3;
  //argsP_A.data = {1, "this is producer A"};
  //argsP_B.data = {2, "this is producer B"};
  //argsP_C.data = {3, "this is producer C"};
  //argsC_A.data = {5, "this is consumer A"};

  argsP_A.interval = 2;
  argsP_B.interval = 3;
  argsP_C.interval = 4;
  argsC_A.interval = 15;

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
  
  return 0;
}

void *producer(void *arguments)
{
  struct thread_args *args = (struct thread_args*)arguments;
  printf("new producer %d\n", args->data.intVal);
  while(1)
    {
      printf("producer %d pushing\n", args->data.intVal);
      pushQueue(&args->queue, args->data);
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
  while(1)
    {
      printf("now consuming\n");
      writeQueueToFile(&args->queue, &fp);
      showQueue(&args->queue);
      emptyQueue(&args->queue);
      sleep(args->interval);
    }
  fclose(fp);
  return;
}
