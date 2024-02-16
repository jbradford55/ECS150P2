#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}


//queue_destroy
//null queue
void null_queue_DESTROY(void)
{
	fprintf(stderr, "*** TEST null_queue_DESTROY ***\n");

	TEST_ASSERT(queue_destroy(NULL) == -1);
}
//queue is not empty
void not_empty_queue_DESTROY(void)
{
	fprintf(stderr, "*** TEST not_empty_queue_DESTROY ***\n");

	queue_t q;


    int item_to_enqueue = 8;
	q = queue_create();
	queue_enqueue(q, &item_to_enqueue);

	TEST_ASSERT(queue_destroy(q) == -1);
}
//regular
void regular_DESTROY(void)
{
	fprintf(stderr, "*** TEST regular_DESTROY ***\n");

	queue_t q = queue_create();
	

	TEST_ASSERT(queue_destroy(q) == 0);
}

//queue_enqueue
//queue is null
void queue_is_null_ENQUEUE(void)
{

	fprintf(stderr, "*** TEST queue_is_null_ENQUEUE ***\n");

    int item_to_enqueue= 4;
	TEST_ASSERT(queue_enqueue(NULL,  &item_to_enqueue)==-1);

}
//data is null
void data_is_null_ENQUEUE(void)
{
	queue_t q;

	fprintf(stderr, "*** TEST data_is_null_ENQUEUE ***\n");

	q = queue_create();
	TEST_ASSERT(queue_enqueue(q,  NULL)==-1);

}
//regular
void regular_ENQUEUE(void)
{
	queue_t q;

	fprintf(stderr, "*** TEST regular_ENQUEUE ***\n");

	q = queue_create();
    int item_to_enqueue= 4;
	TEST_ASSERT(queue_enqueue(q,  &item_to_enqueue)==0);

}
//queue dequeue
//queue null
void queue_is_null_DEQUEUE(void)
{

	fprintf(stderr, "*** TEST queue_is_null_DEQUEUE ***\n");


    queue_t q = queue_create();
    int item_to_enqueue= 4;
    queue_enqueue(q,  &item_to_enqueue);

    int *dequeue_ptr;
	TEST_ASSERT(queue_dequeue(NULL,  (void**)&dequeue_ptr)==-1);

}
//data is null
void data_is_null_DEQUEUE(void)
{
	fprintf(stderr, "*** TEST data_is_null_DEQUEUE ***\n");


    queue_t q = queue_create();
    int item_to_enqueue= 4;
    queue_enqueue(q,  &item_to_enqueue);


	TEST_ASSERT(queue_dequeue(q,  NULL)==-1);

}
//empty queue
void empty_queue_DEQUEUE(void)
{
	fprintf(stderr, "*** TEST empty_queue_DEQUEUE ***\n");


    queue_t q = queue_create();
    int *dequeue_ptr;



	TEST_ASSERT(queue_dequeue(q,  (void**)&dequeue_ptr)==-1);

}


//regular
void regular_DEQUEUE(void)
{
	
    
    int item_to_enqueue = 3;
    int *item_to_dequeue;
	queue_t q;

	fprintf(stderr, "*** TEST regular_DEQUEUE ***\n");

	q = queue_create();
	queue_enqueue(q, &item_to_enqueue);
	queue_dequeue(q, (void**)&item_to_dequeue);
	TEST_ASSERT(item_to_dequeue == &item_to_enqueue);

}



//queue delete
//null queue
void null_queue_DELETE(void)
{
	
    int *item_to_delete;


	fprintf(stderr, "*** TEST null_queue_DELETE ***\n");

	TEST_ASSERT(queue_delete(NULL, &item_to_delete) == -1);

}
//null data
void null_data_DELETE(void)
{
    queue_t q;
    q = queue_create();

	fprintf(stderr, "*** TEST null_data_DELETE ***\n");

	TEST_ASSERT(queue_delete(q, NULL) == -1);

}
//data not found
void data_not_found_DELETE(void)
{
    int item_to_enqueue = 3;
    int *item_to_delete;

	queue_t q;

	fprintf(stderr, "*** TEST data_not_found_DELETE ***\n");

	q = queue_create();
	queue_enqueue(q, &item_to_enqueue);
	TEST_ASSERT(queue_delete(q, &item_to_delete)==-1);

}
//regular
void regular_DELETE(void)
{
    int item_to_enqueue = 3;

	queue_t q;

	fprintf(stderr, "*** TEST regular_DELETE ***\n");

	q = queue_create();
	queue_enqueue(q, &item_to_enqueue);
	TEST_ASSERT(queue_delete(q, &item_to_enqueue)==0);

}

//queue_iterate
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}


//null queue
void null_queue_ITERATE(void)
{
	fprintf(stderr, "*** TEST null_queue_ITERATE ***\n");

    assert(queue_iterate(NULL, iterator_inc)==-1);
}
//null func
void null_func_ITERATE(void)
{
    	fprintf(stderr, "*** TEST null_func_ITERATE ***\n");

    queue_t q;
    q = queue_create();

    assert(queue_iterate(q, NULL)==-1);
}
//regular
void regular_ITERATE(void)
{
        	fprintf(stderr, "*** TEST regular_ITERATE ***\n");

    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    assert(data[0] == 2);
    assert(queue_length(q) == 9);
}


//queue_length
//null queue
void null_queue_length(void)
{

	fprintf(stderr, "*** TEST null_queue_length ***\n");
	TEST_ASSERT(queue_length(NULL)==-1);

}
//regular
void regular_LENGTH(void)
{
	queue_t q;

	fprintf(stderr, "*** TEST regular_ENQUEUE ***\n");

	q = queue_create();
    int item_to_enqueue= 4;
	queue_enqueue(q,  &item_to_enqueue);

    TEST_ASSERT(queue_length(q)==1);

}
void create(void){
    test_create();
}

void destroy(void){
    null_queue_DESTROY();
    not_empty_queue_DESTROY();
    regular_DESTROY();
}

void enqueue(void){
    queue_is_null_ENQUEUE();
    data_is_null_ENQUEUE();
    regular_ENQUEUE();
}

void dequeue(void){
    queue_is_null_DEQUEUE();
    data_is_null_DEQUEUE();
    empty_queue_DEQUEUE();
    regular_DEQUEUE();
}

void Iterate(void){
    null_queue_ITERATE();
    null_func_ITERATE();
    regular_ITERATE();
}

void length(void){
    null_queue_length();
    regular_LENGTH();
}

int main(void)
{


    create();
    destroy();
    enqueue();
    dequeue();
    Iterate();
    length();




	return 0;
}
