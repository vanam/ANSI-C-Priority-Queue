/* C standard libraries */
#include <stdio.h> 
#include <stdlib.h> 

/* My libraries */
#include "priority_queue.h"             /* Priority queue definition and routines */
#include "constants.h"                  /* Contains constants */


int comparator( const void *item_1, const void *item_2 )
{
	int value_1, value_2;

        value_1 = *( ( int * ) item_1 );
        value_2 = *( ( int * ) item_2 );

        if( value_1 < value_2 )
        {
                return SMALLER;
        }
        else if( value_1 == value_2 )
        {
                return EQUAL;
        }
        else
        {
                return GREATER;
        }
}

int main()
{
	priority_queue *pq = NULL;
	int a = 55;
	int b = 48;
	int c = 1289;
	int d = 78;

	int r = 0;

	/* 48 is minimal priority queue size */
    	pq = create_priority_queue( 48, &comparator );

    	priority_queue_insert( pq, &a );
    	priority_queue_insert( pq, &b );
    	priority_queue_insert( pq, &c );
    	priority_queue_insert( pq, &d );

    	r = *(int *) priority_queue_poll( pq );

    	printf("First is %d\n", r);

	r = *(int *) priority_queue_poll( pq );

    	printf("Second is %d\n", r);

    	free_priority_queue( &pq );

    	return EXIT_SUCCESS;
}