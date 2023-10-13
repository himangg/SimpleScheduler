#ifndef SOME_HEADER_GUARD_WITH_UNIQUE_NAME
#define SOME_HEADER_GUARD_WITH_UNIQUE_NAME
#include <time.h>
#include <semaphore.h>
struct Member{
   int pid;
   int priority;
   int finished;// to tell whether the process is finished or not
   int running;// to tell whether the process is currently running or not
   clock_t lastime;
   clock_t lastime2;
   double wait_time;
   double execution_time;
   char  command[10000];
   };
struct Queue{
   int low;
   int high;
   struct Member members[1000];
   };
typedef struct Queue queue;
typedef struct Member member;
extern void add_process(int a,int c,char *d);
extern void schedule_process();
extern void remove_process(int pid);
extern queue q;// now this queue will be shared by both the files
extern int NCPU;
extern int TSLICE;

// have not handled the case of garbage values
#endif
