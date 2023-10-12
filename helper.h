#ifndef SOME_HEADER_GUARD_WITH_UNIQUE_NAME
#define SOME_HEADER_GUARD_WITH_UNIQUE_NAME
struct Member{
   char* command_name[10000];
   int pid;
   int priority;
   int finished;
   int running;
   clock_t lastime;
   clock_t lastime2;
   double execution_time;
   double wait_time;
   };
struct Queue{
   int low;
   int high;
   struct Member members[1000];
   };
typedef struct Queue queue;
typedef struct Member member;
extern void add_process(int a,int c);
extern void schedule_process();
extern void remove_process(int pid);
extern queue q;// now this queue will be shared by both the files
extern int NCPU;
extern int TSLICE;
// have not handled the case of garbage values
#endif
