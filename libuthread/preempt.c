#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

//preempt_enabled tells is whether or not we do anything
static int preempt_enabled;

//timer
static struct itimerval current_timer; 
static struct itimerval previous_timer;


//action and sigsets
static struct sigaction sig_action;
static struct sigaction previous_sig_action;
static sigset_t sig_set;
static sigset_t previous_sig_set;





/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100


void signal_handler(int input_signal){
	if (input_signal == SIGVTALRM){
		uthread_yield();
	}
}
void preempt_disable(void)
{
	if (preempt_enabled == 1){
		sigprocmask(SIG_BLOCK,&sig_set, NULL);
	} else {
		return;
	}
}

void preempt_enable(void)
{
	if (preempt_enabled==1){
		sigprocmask(SIG_UNBLOCK,&sig_set, NULL );
	} else {
		return;
	}
}

void preempt_start(bool preempt)
{
	if (preempt){
		preempt_enabled = 1;
	} else {
		return;
	}

	//initialize timer
	current_timer.it_interval.tv_usec = 10000; //10,000 = 1,000,000 milliseconds * 1/100 times per second = 10,000
	current_timer.it_value.tv_usec = 10000;
	setitimer(ITIMER_VIRTUAL, &current_timer, &previous_timer);




	//initialize sigset
	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGVTALRM);

	//set appropriate handlers
	sig_action.sa_handler = signal_handler;
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags = 0;


	//NOTE: we save state to be restored later
	sigaction(SIGVTALRM, &sig_action, &previous_sig_action);
	sigprocmask(SIG_SETMASK, NULL, &previous_sig_set);

	return;
}

void preempt_stop(void)
{
	if (preempt_enabled==1){
		setitimer(SIGVTALRM, &previous_timer, NULL);
		sigaction(SIGVTALRM, &previous_sig_action, NULL);
		sigprocmask(SIG_SETMASK, &previous_sig_set, NULL);
	} else {
		return;
	}
}

