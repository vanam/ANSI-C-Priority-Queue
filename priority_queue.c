/*
 * Module priority_queue.c
 * This module contains a set of functions that assures a 
 * maintenance of a priority queue. Its creating, inserting/deleting
 * items, expanding queue, reducing, etc.
 * 
 * Dialect : ANSI C
 * Compiler: any ANSI C-compatible
 * 
 * Copyright (c) 2014 Martin Váňa
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PRIORITY_QUEUE_C

#define PRIORITY_QUEUE_C

/* C standard libraries */
#include <stdio.h> 
#include <stdlib.h> 

/* My libraries */
#include "priority_queue.h"             /* Priority queue definition and routines */
#include "constants.h"                  /* Contains constants */

/* Minimum priority queue size */
const int MIN_PRIORITY_QUEUE_CAPACITY = 16;

/*
 * Returns greater integer.
 */
static int imax( int a, int b )
{
        return a > b ? a : b;
}

/*
 * Creates priority queue with initial capacity.
 * Returns priority queue on success, NULL otherwise.
 */
priority_queue *create_priority_queue( const int capacity, int ( *comparator )( const void *, const void * ) )
{
        int i;
        priority_queue *pq = NULL;

        if ( capacity < MIN_PRIORITY_QUEUE_CAPACITY )
        {
                printf( "\nBad priority queue parameters. Minimum priority queue capacity is %d.\n", MIN_PRIORITY_QUEUE_CAPACITY );
                return NULL;
        }
        else
        {
                pq = ( priority_queue * ) malloc( sizeof( priority_queue ) );

                if ( pq == NULL )
                {
                        printf( "\nPriority queue memory allocation failed.\n" );
                        return NULL;
                }
                
                pq->comparator = comparator;
                pq->min_capacity = capacity;
                pq->capacity = capacity;
                pq->heap_size = 0;

                pq->heap_array = malloc( capacity * sizeof( void * ) );

                if ( pq->heap_array == NULL )
                {
                        printf( "Priority queue heap memory allocation failed.\n" );
                        free( pq );
                        return NULL;
                }

                for ( i = 0; i < capacity; i++ )
                {
                        pq->heap_array[ i ] = NULL;
                }

                return pq;
        }
}

/*
 * Performs swap in priority queue.
 * Swaps pointers.
 */
static void swap( priority_queue *pq, const int key_1, const int key_2 )
{
        void *tmp = NULL;
        tmp = pq->heap_array[ key_1 ];
        pq->heap_array[ key_1 ] = pq->heap_array[ key_2 ];
        pq->heap_array[ key_2 ] = tmp;
}

/*
 * Repairs properties of heap from the top.
 */
static void repair_top( priority_queue *pq, int key, int heap_size )
{
        int j;

        while( 2 * key <= heap_size )
        {
                j = 2 * key;

                if( j < heap_size && pq->comparator( pq->heap_array[ j ], pq->heap_array[ j + 1 ] ) == GREATER )
                {
                        j++;
                }

                if( pq->comparator( pq->heap_array[ key ], pq->heap_array[ j ] ) == SMALLER  )
                {
                        break;
                }

                swap( pq, key, j );
                key = j;
        }
}

/*
 * Repairs properties of heap from the bottom.
 */
static void repair_bottom( priority_queue *pq, int key )
{
        while( key > 1 && pq->comparator( pq->heap_array[ key / 2], pq->heap_array[ key ] ) == GREATER )
        {
                swap( pq, key, key / 2 );
                key = key / 2;
        }
}

/*
 * Ensures sufficient capacity.
 * Returns TRUE on success, FALSE otherwise.
 */
static int ensure_capacity( priority_queue *pq, const int min_capacity )
{
        int old_capacity, new_capacity;
        void **new_heap_array;

        old_capacity = pq->capacity;

        if( min_capacity > old_capacity )
        {
                /* New capacity formula took over Java Arraylist */
                new_capacity = ( old_capacity * 3 ) / 2 + 1;
                if( new_capacity < min_capacity )
                {
                        new_capacity = min_capacity;
                }

                new_heap_array = ( void ** ) realloc( ( void * ) pq->heap_array, new_capacity * sizeof( void * ) );

                if( new_heap_array == NULL )
                {
                        return FALSE;
                }

                pq->heap_array = new_heap_array;
                pq->capacity = new_capacity;
        }
        return TRUE;
}

/*
 * Inserts item to the queue.
 * Returns TRUE on success, FALSE otherwise.
 */
int priority_queue_insert( priority_queue *pq, void *item )
{
        if ( pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
                return FALSE;
        }
        else if ( item == NULL )
        {
                printf( "Item pointer is NULL.\n" );
                return FALSE;
        }
        else
        {
                /* 2 = 1 for spare item in array + 1 for next item */
                if( ensure_capacity( pq, pq->heap_size + 2 ) == FALSE )
                {
                        printf( "\nPriority queue out of memory.\n" );
                        return FALSE;
                }

                pq->heap_size += 1;
                pq->heap_array[ pq->heap_size ] = item;
                repair_bottom( pq, pq->heap_size );

                return TRUE;
        }
        return FALSE;
}

/*
 * Returns pointer to an item on the peek.
 * Does not remove item from queue.
 */
void *priority_queue_peek( priority_queue *pq )
{
        if ( pq->heap_size > 0 )
        {
                return pq->heap_array[ 1 ];
        }
        return NULL;
}

/*
 * Returns pointer to an item on the peek.
 * Does remove item from queue.
 */
void *priority_queue_poll( priority_queue *pq )
{
        if ( pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
                return NULL;
        }
        else
        {
                if ( pq->heap_size != 0 )
                {
                        swap( pq, 1, pq->heap_size );
                        repair_top( pq, 1, pq->heap_size - 1 );

                        pq->heap_size--;

                        return pq->heap_array[ pq->heap_size + 1 ];
                }
                return NULL;
        }
}

/*
 * Checks if the priority is queue empty.
 * Returns TRUE if queue is empty, FALSE if queue is non empty, 
 * ERROR_VALUE if queue is uninitialized.
 */
int priority_queue_is_empty( const priority_queue *pq )
{
        if ( pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
                return ERROR_VALUE;
        }
        else
        {
                if ( pq->heap_size == 0  )
                {
                        return TRUE;
                }
                else
                {
                        return FALSE;
                }
        }
}

/*
 * Returns number of items in the priority queue.
 * Returns ERROR_VALUE if queue is uninitialized.
 */
int priority_queue_size( const priority_queue *pq )
{
        if ( pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
                return ERROR_VALUE;
        }
        else
        {
                return pq->heap_size;
        }
}

/*
 * Reduces priority queue size to number of inserted items.
 * Returns TRUE on success, FALSE otherwise.
 */
int priority_queue_trim_to_size( priority_queue *pq )
{
        int new_capacity;
        void **new_heap_array;

        if( pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
                return FALSE;
        }
        else
        {
                new_capacity = imax( pq->heap_size + 1, pq->min_capacity );

                if( pq->capacity > new_capacity )
                {
                        new_heap_array = ( void ** ) realloc( pq->heap_array, new_capacity * sizeof( void * ) );

                        if( new_heap_array == NULL )
                        {
                                return FALSE;
                        }

                        pq->heap_array = new_heap_array;
                        pq->capacity = new_capacity;
                }
                /* Even no trimming is success */
                return TRUE;
        }
}

/*
 * Frees data stores in priority queue using free function.
 */
void free_priority_queue_data( priority_queue **pq, void (*free_function)( void * ) )
{
        int i;
        for ( i = 1; i <= (*pq)->heap_size; i++ )
        {
                free_function( (*pq)->heap_array[ i ] );
        }
        (*pq)->heap_size = 0;
}

/*
 * Frees priority queue.
 */
void free_priority_queue( priority_queue **pq )
{
        if ( *pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
        }
        else
        {
                printf( "Frees priority queue memory..." );

                free( (*pq)->heap_array );
                free( (*pq) );
                (*pq) = NULL;

                printf( "OK\n" );
        }
}

/*
 * Prints content in priority queue as it is stored in array.
 */
void print_priority_queue( const priority_queue *pq, char* (*to_string)( void * ) )
{
        int i;
        char *item_string;

        if ( pq == NULL )
        {
                printf( "Priority queue is uninitialized.\n" );
        }
        else
        {
                printf( "%d [", pq->heap_size );
                for ( i = 1; i <= pq->heap_size; i++ )
                {
                        item_string = to_string( pq->heap_array[ i ] );
                        printf( " %s ", item_string );
                        free( item_string );
                }
                printf( "]\n" );
        }
}

#endif