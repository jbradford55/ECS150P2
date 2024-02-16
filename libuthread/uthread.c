#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

// Declare static variables for thread_queue and blocked_queue
static queue_t thread_queue = NULL;
static queue_t blocked_queue = NULL;

struct uthread_tcb {
	uthread_ctx_t *context;
    void *stack;
	int ThreadID;
};

//this is the main running thread
static struct uthread_tcb *running_thread;

//Used to assign each thread a ThreadID(equal to number of threads at that point)
static int number_of_threads; 


struct uthread_tcb *uthread_current(void)
{
	return running_thread;
}

void uthread_yield(void)
{

	//put currently running thread back into queue
	queue_enqueue(thread_queue, running_thread);

	//initialize new thread
	struct uthread_tcb *next_thread = malloc(sizeof(struct uthread_tcb*));
	queue_dequeue(thread_queue, (void**)&next_thread);

	//switch from current thread to next thread
	struct uthread_tcb *old_running_thread = running_thread;
	running_thread = next_thread;
    //printf("%d\n", old_running_thread->context);
	uthread_ctx_switch(old_running_thread->context, running_thread->context);

}

void uthread_exit(void)
{
    if (queue_length(thread_queue)!=0){
        //very similar to uthread_yield, but we don't re-enqueue running thread and we
        //destroy stac pointer
        //initialize new thread

        preempt_disable();
        struct uthread_tcb *next_thread;
        next_thread = malloc(sizeof(struct uthread_tcb*));
        queue_dequeue(thread_queue, (void**)&next_thread);

        //destroy stack of old thread
        uthread_ctx_destroy_stack(running_thread->stack);


        //switch from current thread to next thread
        struct uthread_tcb *old_running_thread = running_thread;
        running_thread = next_thread;

        preempt_disable();


        if (old_running_thread->context && running_thread->context){
            uthread_ctx_switch(old_running_thread->context, running_thread->context);
        }
	} else {
        preempt_stop();
        return;
    }


}

int uthread_create(uthread_func_t func, void *arg)
{
	/* Phase 2: Create a new thread and initialize its context */
    struct uthread_tcb *new_thread = malloc(sizeof(struct uthread_tcb*));
    if (new_thread == NULL) {
        return -1; // Memory allocation failed
    }


    preempt_disable();
    //initialize new thread
    number_of_threads++;
    new_thread->stack = uthread_ctx_alloc_stack();
    new_thread->context = malloc(sizeof(uthread_ctx_t));
    new_thread->ThreadID = number_of_threads;


    // Initialize context for the new thread
    if (uthread_ctx_init(new_thread->context, new_thread->stack, (uthread_func_t)func, arg) == -1) {
        number_of_threads--;
        uthread_ctx_destroy_stack(new_thread->stack);
        free(new_thread);
        return -1; 
    }


    // Add the new thread to the queue
    queue_enqueue(thread_queue, new_thread);


    preempt_enable();

    return 0; // Success
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{

    preempt_start(preempt);


    thread_queue = queue_create();
    if (thread_queue == NULL) {
        return -1; // Queue creation failed
    }

    preempt_disable();
    //create main thread
    struct uthread_tcb *main_thread = malloc(sizeof(struct uthread_tcb*));
    main_thread->stack = uthread_ctx_alloc_stack();
    main_thread->context = malloc(sizeof(uthread_ctx_t));


    // Create the initial thread
    if (uthread_create(func, arg) == -1) {
        queue_destroy(thread_queue);
        return -1; // Initial thread creation failed
    }

    preempt_enable();

    // Start executing threads
    while (1) {
        queue_dequeue(thread_queue, (void**)&running_thread);
		uthread_ctx_switch(main_thread->context, running_thread->context);
    }

    // All threads finished executing
    queue_destroy(thread_queue);

    return 0; // Success
}

void uthread_block(void)
{
    //enqueue current thread in blocked queue
    queue_enqueue(blocked_queue, running_thread);

    preempt_disable();

    //initialize next thread to run
	struct uthread_tcb *next_thread = malloc(sizeof(struct uthread_tcb*));
	queue_dequeue(thread_queue, (void**)&next_thread);
	struct uthread_tcb *old_thread = running_thread;
	running_thread = next_thread;//update current thread


    preempt_enable();

    //run next thread
	uthread_ctx_switch(old_thread->context, running_thread->context);

}

void uthread_unblock(struct uthread_tcb *uthread)
{
    //delete *uthread from blocked queue
    queue_delete(blocked_queue, &uthread);
    queue_enqueue(thread_queue, (void**)uthread);

}
