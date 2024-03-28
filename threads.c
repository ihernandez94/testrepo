// 1. Pthreads is an POSIX standard based on threads, that needs to be implemented for C/C++ programming. It allows parallelism through threads. Implemented through a header and a thread library.  
// Pthreads allow multiple threads to execute under a single process. These threads share the same data but each one have their own stack. 
// Pthreads requieres threads to share process wide attributes. Each thread has their own specific attributes.
// We use the functions: sleep, create, join, cancel.
// We can only cancel a thread when the cancellation request is seen. The sleep function allows this to happen since sleep is a cancellation point in PThreads.
// 2. The sleeping thread can print the periodic messages because the main thread is blocked until I/O input comes. Which allows the child thread to keep executing the infinite loop. 

#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define handle_error_en(en, msg) \
   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

/* Child Thread that prints a messsage after sleeping for 3 seconds while in an infinite loop.*/
static void *child(void *ignored){ 
// Creates an infinite loop since any non zero integer is true for the condition
while (1) 
{
   // Call sleep function in Pthreads. The calling thread is suspended from execution for 3 seconds.
   sleep(3);
   // Print statement that prints after 3 seconds of sleep
   printf("Child is done sleeping 3 seconds.\n"); 
}

   return NULL;
}

/* Main Thread creates a second thread.
 It then reads an I/O request (keyboard), and then waits until the user inputs an Enter key.
 While the thread is in waiting state, the second thread is killed and a print statment is sent out declaring the death.
 Once the I/O input is received (Enter), the main thread will wait 5 seconds before exiting with a print statement. */
int main(int argc, char *argv[]){

   // Character variable to store user input
   char input;

   void *res;
   //  Creates thread identifier, child_thread, for pthread_t data type
   pthread_t child_thread; 
   int s;
   // Create thread with 4 attributes: the pointer to the thread id, pointer to a structure, null for default attribute, name of the function that creates the thread, pointet to arguments for the function, null for default arguments
   s = pthread_create(&child_thread, NULL, &child, NULL); // This is the second thread (named child)
   // If statement to check if the thread failed creating
   if(s)
   {
      fprintf(stderr, "pthread_create failed with code %d\n", s);
   }

   // Print statement asking user to press Enter to kill second thread
   printf("Press ENTER to end child thread.\n");
    input = fgetc(stdin);
   // Check if user pressed enter, if not then wait. 
   while (input !='\n')
   {
    input = fgetc(stdin);
   }
    // Send cancelation request for child thread
    pthread_cancel(child_thread);
   // Print statement showing that user pressed Enter.
   printf("User pressed Enter. Killing Child.\n");
   printf("Child thread has been successfully killed.\n");

   // Call sleep function in Pthreads. The calling thread is suspended from execution for 5 seconds
   sleep(5); 
   // Print statement that prints after 5 seconds of sleep
   printf("Parent is done sleeping 5 seconds.\n");
   // Print statement that prints that the main thread is exiting
   printf("Main thread is exiting.\n");
   return 0;
}
