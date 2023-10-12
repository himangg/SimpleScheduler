#include <stdio.h>
#include "helper.h"
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>// Include for time function

extern int NCPU;
extern queue q;
void add_process(int pid, int priority){
    // create a new member with pid=pid and priority=priority
    member m;
    m.pid=pid;
    m.priority=priority;
    m.finished=0;
    // now add this member into the priority queue
    q.members[q.high]=m;
    q.high++;
    m.lastime=clock();
}

void schedule_process(){
    // iterate through the members and select the NCPU process out of it
    int i;
    // pausing the currently running process 
    printf("SCHEDULING PROCESS");
    fflush(stdout);
    for(i=q.low;i<=q.high;i++){
      if(q.members[i].running==1){
        q.members[i].running=0;
        kill(q.members[i].pid,SIGSTOP);
        q.members[i].lastime=clock();
        q.members[i].execution_time+=((double) (clock() - q.members[i].lastime2))/CLOCKS_PER_SEC;
      }
    }
    
    // now selecting the new process to be run
   int j,k;
  
   int ncpu= NCPU;
     for(j=4;j>=1;j--){
     for(i=q.low;i<=q.high;i++){
      if(q.members[i].finished==0){
      if(ncpu>0){
         kill(q.members[i].pid,SIGCONT);
         q.members.lastime2=clock();
         q.members[i].wait_time+=((double) (clock() - q.members[i].lastime))/CLOCKS_PER_SEC;
         ncpu--;
          }
        }
      }
    }
}
void remove_process(int pid){
       int i;
       for(i=q.low;i<=q.high;i++){
         if(q.members[i].pid==pid){
          q.members[i].finished=1;
          q.members[i].execution_time+=((double) (clock() - q.members[i].lastime))/CLOCKS_PER_SEC;
          break;
         }
       }
}
