#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>


#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	int val;
	queue_t blocked_queue; //a.k.a.Queue of threads blocked by semaphore

};

sem_t sem_create(size_t count)
{
	struct semaphore *semaph = malloc(sizeof(struct semaphore*));
	semaph->blocked_queue = queue_create();
	semaph->val = count;
	
	return semaph;
}

int sem_destroy(sem_t sem)
{
	queue_destroy(sem->blocked_queue);
	free(sem);

	return 0;
}

int sem_down(sem_t sem)
{

	//if val is 0, enqueue thread to blocked queue
	if (sem->val == 0){
		queue_enqueue(sem->blocked_queue, uthread_current());
		uthread_block();
		return 0;
	//if val is not 0, no need ot worry about blocked queue
	} else {
		sem->val--;
		return 0;
	}
}

int sem_up(sem_t sem)
{
	//if length of blocked queue is 0, no need to worry about unblocking other
	//queueus
	if (queue_length(sem->blocked_queue) == 0){
		sem->val ++;
		return 0;
	}

	//get new queue from blocked queue and unblock
	struct uthread_tcb *thread_to_run = malloc(sizeof(struct uthread_tcb*));
	queue_dequeue(sem->blocked_queue, (void**)&thread_to_run);
	uthread_unblock(thread_to_run);

	return 0;
	
}

