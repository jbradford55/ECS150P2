/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void hello(void *arg)
{
	(void)arg;

	printf("Hello world!\n");
}

int main(void)
{
	uthread_run(false, hello, NULL);

	return 0;
}
