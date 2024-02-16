/*
This program creates 2 threads, however the first thread enters an infinite
while loop. The only way the second thread can run is if you enable preemption.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

//thread only can run if preamption is enabled
void thread2(void *arg)
{
	(void)arg;


	printf("interupting infinite while loop!\n");
}


//thread runs no matter what, enters infinite loop
void thread1(void *arg)
{

	(void)arg;

	uthread_create(thread2, NULL);
	printf("entering infinite while loop\n");

    //infinite loop
    while (true){

    }
    printf("exited infinite while loop");
}


int main(void)
{
    //switch the preempt value to test whether or not the infinite loop will be 
    //interupted
	uthread_run(true, thread1, NULL);
	return 0;

}
