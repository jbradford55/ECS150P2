# queue.c

 
## Implementation

We implemented the Queue API with a linked list. We created a node struct with
a value and a pointer to the next node, and within the queue struct we stored
the length of the queue as well as a pointer to head and tail. each time
we appended to the queue, we set the tail.next to the new node, and then 
updated the tail to actaully be the new node. Each time we dequeued a queue, 
we returned the head node, and set the head to point to the next node. 
For delete, all we had to do was visit each node and check whether or not
it is the same as the input node, and if so, we set the pointer of the 
previous node of the iteration to point to the next node, effectively excluding
the current node from the list. 
iterate and length functions were pretty simple, as iterate was similar to 
delete, except we performed the function on the node isntead of deleting it,
and for length we just returned the length member of the queue.

## Testing

queue_tester.c contains tests for each queue function.

## Limitations

There were some issues with freeing memory of deleted items, for example when
we deleted a TCB from blocking queue initially we called free on the node which
got rid of the TCB, but we want to keep the TCB to put into the main queue, 
so we don't free the deleted nodes.

## Sources

In the project2.pdf given by the professor, he gives some examples of queue
iterator functions and queue tester functions that we expanded upon. For 
queue_tester.c, we used the given queue_tester_example.c and expanded upon
the given test cases to make a test case for each function.

# uThread.c


## Implementation

We had 2 queues, a main queue and a blocked queue. The blocked queue is 
discussed in the sem section. We kicked off the program by 
running a thread, and whenever a thread creates a new thread it adds it to the
queue. When the current thread then exists (or yields) it checks if the length
of the main queue is greater than 0, and if so, dequeues and runs the next
thread. If the thread is exiting it deallocates its stack, and if a thread is 
yielding it adds itself to the queue to be continued later on. 

## Testing

Our uthread.c program produces the expected outoput for both the uthread_yield
and uthread_hello files.

## Sources

the professor provided us with a demo folder which we used to understand how
threads were switched and created.

# sem.c

## Implementation

We implemented a semapohore struct that contained an internal count of the val
of the semaphore and a queue that contains all the threads blocked by the
semaphore. When this was not 0, there was nothing to worry about, 
we just decrease or increase its value. When a thread calls sem_down and the
value is 0, we add that thread to the semaphores blocked threads list. To 
implement this we call uthread_block, which is defined in uthread.c. We simply
add the calling thread to the blocked list, and uthread.c just runs the next
thead. When a semaphore that was 0 is increased, once of the threads that 
is being blocked by the semaphore frees up to run, so we call unblock(), 
which is defined in the uthread library, which deletes the thread from the 
blocked queue and adds it to the ready queue. 

## testing

Our sem.c produces all the expected outputs for sem_simple.c, sem_prime.c, 
sem_count.c, and sem_buffer.c

# preempt.c

## Implementation

for preemption, we set up a timer that would send an alarm every 10,000 ms(thus,
100 times a second). The handler for this alarm would call yield, and this
had the affect of preempting the thread to allow the next thread to run.

## Testing

The file test_preempt.c creates 2 threads. One thread starts an infinite loop,
and the other thread prints to the console "interupted infinite loop". 
If you run it the way it is, you'll see that even though the first thread
is in an infinite loop and never calls yield, we stil see "interupted infinite 
loop" printed to the console. replace "	uthread_run(true, thread1, NULL);" with
"uthread_run(false, thread1, NULL);" and you'll notice thread 2 never runs.
