#include <stdio.h>
#include "helper.h"
#include <signal.h>
#include <sys/time.h>
#include <string.h>

extern int NCPU;
extern queue q;

void add_process(int pid, int priority,char* command2){
    // create a new member with pid=pid and priority =priority
    printf("THE COMMAND IS\n");
    printf("%s",command2);
    
    member m;
    m.pid=pid;
    m.priority=priority;
    m.finished=0;
    m.running=0;
    strcpy(m.command,command2);
    // now add this member into the priority queue
    m.lastime=clock();// saving the time for calculation of wait time
    m.wait_time=0;
    m.execution_time=0;
    q.members[q.high]=m;
    q.members[q.high].lastime=time(NULL);
    q.members[q.high].finished=0;
    q.members[q.high].running=0;
    q.members[q.high].priority=priority;
    printf("THE IDX IS\n");
    printf("%d\n",q.high);
    q.high++;  
}
void schedule_process(){
    // iterate through the members and select the NCPU process out of it
    int i;
    // pausing the currently running process 
    
    printf("SCHEDULING PROCESS\n");
    fflush(stdout);
     // maintain the last index of every priority
     int l_idx[5];
      l_idx[4]=-1;
      l_idx[3]=-1;
     l_idx[2]=-1;
     l_idx[1]=-1;
    for(i=q.low;i<q.high;i++){
      if(q.members[i].running==1 && q.members[i].finished==0){q.members[i].running=0;
      kill(q.members[i].pid,SIGSTOP);
          q.members[i].lastime=time(NULL);
          time_t endtime=time(NULL);
        q.members[i].execution_time+= difftime(endtime, q.members[i].lastime2);
       l_idx[q.members[i].priority]=i;
       q.members[i].running=0;
      }
    
    }
   // now selecting the new process to be run
   int j,k;
 // printf("RESUMING THE PROCESS");
  fflush(stdout);
   int ncpu= NCPU;
   //printf("DETAILS OF THE QUEUE ARE\n");
   //printf("VALUE OF Q.LOW IS\n");
   //printf("%d",q.low);
   //printf("VALUE OF Q.HIGH IS\n");
   //printf("%d",q.high);
   
   for(i=q.low;i<q.high;i++){
   //printf("FINSIHED STATE\n");
  // printf("%d",q.members[i].finished);
    //fflush(stdout);
}
     for(j=4;j>=1;j--){ 
     int itr=l_idx[j]+1;
     //printf("EXECUTED");
     fflush(stdout);
     //printf("VALUE OF J IS\n");
     //printf("%d",j);
     //printf("VALUE OF l_idx[j] is");
     //printf("%d",l_idx[j]);
     if(itr==q.high)itr=q.low;
     if(l_idx[j]!=-1){
     while(itr!=l_idx[j]){
     printf("INSIDE");
         if(q.members[itr].finished==0 && q.members[itr].priority==j && ncpu>0){
           printf("RESUMING THE PROCESS");
           fflush(stdout);
           
            q.members[itr].running=1;
            time_t end_time=time(NULL);
            q.members[itr].wait_time+=difftime(end_time ,q.members[itr].lastime);
             q.members[itr].lastime2=time(NULL);
            printf("IDX IS");
            printf("%d",itr);
             kill(q.members[itr].pid,SIGCONT);
            ncpu--;
         }
         itr++;
         if(itr==q.high){
         itr=q.low;
         }
     }
        if(q.members[itr].finished==0 && q.members[itr].priority==j && ncpu>0){
           printf("RESUMING THE PROCESS");
           fflush(stdout);
           q.members[itr].running=1;      
            printf("%s",q.members[itr].command);
            time_t endtime=time(NULL);
            q.members[itr].wait_time+=difftime(endtime, q.members[itr].lastime);
            q.members[itr].lastime2=time(NULL);
            kill(q.members[itr].pid,SIGCONT);
            
            ncpu--;
         }
     
     }
     else{
     
       for(i=q.low;i<q.high;i++){
         if(q.members[i].finished==0  && q.members[i].priority==j && ncpu>0){
             printf("RESUMING THE PROCESS");
           fflush(stdout);
              
            
             q.members[i].running=1;
             time_t endtime=time(NULL);
             q.members[i].wait_time+=difftime(endtime,q.members[i].lastime);
              q.members[i].lastime2=time(NULL);
             kill(q.members[i].pid,SIGCONT);
             ncpu--;
         }
       }
     }
     }
  
}
void remove_process(int pid){
        
        int i;
       for(i=q.low;i<q.high;i++){
         if(q.members[i].pid==pid){
          time_t endtime=time(NULL);
          q.members[i].execution_time=q.members[i].execution_time+difftime(endtime, q.members[i].lastime2);
          q.members[i].finished=1;
          printf("REMOVED SUCCESFULLY\n");
          q.members[i].running=0;
          break;
         }
       }
        
}
