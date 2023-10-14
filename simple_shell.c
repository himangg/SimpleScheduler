#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>// Include for time function 
#include <sys/wait.h> // Include for wait function
#include "helper.h"  
int NCPU=0;
queue q;
int create_process_and_run(char *command) {
    // Append a newline character to the command
    size_t len = strlen(command);// this doesnt include the null pointing character 
    if (len == 0 || command[len - 1] != '\n') {
        strcat(command, "\n");
    }
    int priority=1;
    int i;
    char priority_str[2]; // Assuming priority is a single digit, so size 2 including null terminator
    if (command[strlen(command)-3]==' ') {
        priority_str[0] = command[strlen(command)-2];
        priority_str[1] = '\0'; // Null-terminate the string
        priority = atoi(priority_str);
    }

    if(command[0]=='s' && command[1]=='u'){
    int status = fork();
    int pid=status;
    if(pid>0){
       add_process(pid,priority,command);// added into queue of parent process 
       kill(pid,SIGSTOP);//pausing the child process iff it is sumbit otherwise it is aforeground process 
       
    }
    if (status < 0) {
         
        printf("SOME ERROR IS THERE\n");
        exit(1); // Exit with an error code
    }if (status == 0) {
        // Child process
        
        // child process has started        
        char* command_2=(char*)malloc(strlen(command)-1);// command_2 will just include the string without any \0 or '\n'
        int i;
        for(i=0;i<strlen(command)-1;i++)command_2[i]=command[i];
        strtok(command_2," ");
       char* command_3= strtok(NULL," ");
       char* command_4=strtok(NULL," ");

       if(command_4!=NULL){// priority is given as well 
          char *argv[]={"/bin/sh","-c",command_3,NULL};
       execvp("/bin/sh",argv);
         printf("ERROR");// it returned
         exit(1); // Exit with an error code
       
       }
       else{
       char *argv[]={"/bin/sh","-c",command_3,NULL};
       execvp("/bin/sh",argv);
        
        printf("ERROR WHILE OPENING THE NEW PROCESS THROUGH THE EXECVP\n");
        exit(1); // Exit with an error code
        }
        
    } if(status>0) {
        
        // at this i need to start the timer for the waititng time for the process with pid=pid
        //wait(NULL); // Wait for the child process to complete
        // at this point the child process has finished, so remove the process 
        //remove_process(pid);
        
    }
    }
    else{
    // for commands like ls, pwd,etc regular commands on terminal
      int status = fork();
       if (status < 0) {
        printf("SOME ERROR IS THERE\n");
        exit(1); // Exit with an error code
    }if (status == 0) {
        // Child process
        
        // child process has started 
        if(command[0]!='s' ){
           char *argv[]={"/bin/sh","-c",command,NULL};
           execvp("/bin/sh",argv);
           printf("ERROR");
           exit(1);
        }
        }
        if(status >0){
        wait(NULL);

        }
    }
    
   
    return 0; 
}


// Function to launch a command
int launch(char *command) {
    int status;
    char * command_copy=NULL;
    command_copy=(char*)malloc(strlen(command)+1);
    strcpy(command_copy,command);
    char *possible_commands[] = {"ls", "echo", "wc", "cat", "uniq", "sort", "grep", "kill","ps","vi","history","chmod","cwd","pwd","clear"};
    int size = sizeof(possible_commands) / sizeof(possible_commands[0]);
    char *token = strtok(command, " \n"); // Tokenize by space
    if (token == NULL) {
        printf("NO COMMAND ENTERED");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (strcmp(possible_commands[i], token) == 0) { // Using strcmp for string comparison
            break;
        }
    }
    
    status = create_process_and_run(command_copy);
    
    free(command_copy);
    return status;
}
//Function for taking input of the command
void shell_loop() {
     int status;
    do { 
         
        
        char *command = NULL;

        printf("\033[1;32mmy@command:~$\033[0m"); 
         fflush(stdout);
        size_t size = 0;
        int bytes_read=getline(&command, &size, stdin);
        // command is null terminated and contains the '\n'
        if(bytes_read==-1){
         printf("%s","ERROR IN READING IN THE INPUT");
         return ;
        }
        int i;
        int ok = 1;
        for (i = 0; i < (int)strlen(command); i++) {
            if (command[i] == '"' || command[i] == '\\' || command[i] == '\'') {
                printf("USER NOT ALLOWED TO GIVE \\ or quotes\n");
                ok = 0;
                break;
            }
        }

        if (ok == 1 ) {
            status = launch(command);
        }
        
        free(command);
    } while (status);
}
void signal_handler(){
   schedule_process();
}
void handler_child(){
   int status;
    pid_t child_pid;

    // Reap terminated child processes
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
          remove_process(child_pid);
       
    }
}
void ctrlCHandler(int signum) {
    
    int i;
    for(i=q.low;i<q.high;i++){
     
     char output1[]=("\nTHE PID OF THE PROCESS IS: ");
      int bytes_tot=write(1,output1,strlen(output1));
      if(bytes_tot==-1){
      printf("FAILED TO WRITE INTO THE MEMORY");
      return;
      }
      char str0[100];
      sprintf(str0, "%d", q.members[i].pid); 
      bytes_tot=write(1,str0,strlen(str0));
      if(bytes_tot==-1){
        printf("FAILED TO WRITE THE PID TABLE");
        return;
      }

      char output2[]=("\nTHE NAME OF THE PROCESS IS :");
      bytes_tot=write(1,output2,strlen(output2));
      if(bytes_tot==-1){
        printf("FAILED TO WRITE THE MESSAGE FOR THE NAME OF THE PROCESS");
      return;
      }
      bytes_tot=write(1,q.members[i].command,strlen(q.members[i].command));
      if((bytes_tot)==-1){
       printf("FAILED TO WRITE THE NAME");
       return;
      }

      char output3[]=("THE EXECTUTION TIME OF THE PROCESS IS(IN SECONDS) :");
      bytes_tot=write(1,output3,strlen(output3));
      if(bytes_tot==-1){
        printf("ERROR IN PRINTING EXECUTION TIME OF THE PROCESS");
        return;
      }
      char str1[100]; 
        sprintf(str1, "%f", q.members[i].execution_time); 
      bytes_tot=write(1,str1,strlen(str1));
      if(bytes_tot==-1){
       printf("ERROR IN PRINTING THE THE ACTUAL EXECUTION TIME OF THE PROCESS");
       return;
      }

      char output4[]=("\nTHE WAITING TIME OF THE PROCESS IS(IN SECONDS) :");
      bytes_tot=write(1,output4,strlen(output4));
      if(bytes_tot==-1){
        printf("ERROR IN PRINTING WAITING TIME OF THE PROCESS");
        return;
      }
        char str2[100]; 
        sprintf(str2, "%f", q.members[i].wait_time); 
      bytes_tot=write(1,str2,strlen(str2));
      if(bytes_tot==-1){
       printf("ERROR IN PRINTING THE THE ACTUAL WAITING TIME OF THE PROCESS");
       return;
      }

      char output5[]=("\n");
      bytes_tot=write(1,output5,strlen(output5));
      if(bytes_tot==-1){
        printf("ERROR IN WRITING THE BACKLASH N");
        return;
      }      
    }

    double totalTime[4];
    double totalCount[4];
    for(i=0;i<4;i++)totalTime[i]=0;
    for(i=0;i<4;i++)totalCount[i]=0;
    for(i=q.low;i<q.high;i++){
            totalTime[q.members[i].priority-1]+=q.members[i].execution_time;
            totalCount[q.members[i].priority-1]++;
    }

    char output10[]=("\nAVERAGE EXECUTION TIME OF PRIORITY 1(IN SECONDS): ");
      int bytes_tot=write(1,output10,strlen(output10));
      if(bytes_tot==-1){
      printf("FAILED TO WRITE INTO THE MEMORY");
      return;
      }
      if(totalCount[0]!=0){
      char str10[100]; 
        sprintf(str10, "%f", (totalTime[0]/totalCount[0]));
      bytes_tot=write(1,str10,strlen(str10));
      if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 1");
        return;
      }
      }
      if(totalCount[0]==0){
           char *zero ="0.0";
         bytes_tot=write(1,zero,strlen(zero));
         if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 1");
        return;
      }
      }
    
    char output20[]=("\nAVERAGE EXECUTION TIME OF PRIORITY 2(IN SECONDS): ");
      bytes_tot=write(1,output20,strlen(output20));
      if(bytes_tot==-1){
      printf("FAILED TO WRITE INTO THE MEMORY");
      return;
      }
      char str20[100]; 
      if(totalCount[1]!=0){
        sprintf(str20, "%f", (totalTime[1]/totalCount[1]));
        bytes_tot=write(1,str20,strlen(str20));      
        if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 2");
        return;
      }
      }
      if(totalCount[1]==0){
             char *zero ="0.0";
         bytes_tot=write(1,zero,strlen(zero));
         if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 2");
        return;
      }
    }
    char output30[]=("\nAVERAGE EXECUTION TIME OF PRIORITY 3 (IN SECONDS): ");
      bytes_tot=write(1,output30,strlen(output30));
      if(bytes_tot==-1){
      printf("FAILED TO WRITE INTO THE MEMORY");
      return;
      }
      if(totalCount[2]!=0){
        char str30[100]; 
        sprintf(str30, "%f", (totalTime[2]/totalCount[2]));
        bytes_tot=write(1,str30,strlen(str30));        
        if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 3");
        return;
      }
      }
     if(totalCount[2]==0){
              char *zero ="0.0";
         bytes_tot=write(1,zero,strlen(zero));
         if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 3");
        return;
      }
     }
     

    char output40[]=("\nAVERAGE EXECUTION TIME OF PRIORITY 4(IN SECONDS): ");
      bytes_tot=write(1,output40,strlen(output40));
      if(bytes_tot==-1){
      printf("FAILED TO WRITE INTO THE MEMORY");
      return;
      }
      if(totalCount[3]!=0){
        char str40[100]; 
        sprintf(str40, "%f", (totalTime[3]/totalCount[3]));
        bytes_tot=write(1,str40,strlen(str40));        
        if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 4");
        return;
      }
      }
      if(totalCount[3]==0){
               char *zero ="0.0";
         bytes_tot=write(1,zero,strlen(zero));
         if(bytes_tot==-1){
        printf("FAILED TO WRITE THE EXECUTION TIME OF PRIORITY 3");
        return;
      }
      }

    exit(0);
}
int main(int argc, char** argv) {
   //member membrs[1000];
   //queue q={0,0,membrs[1000]};
   q.low=0;
   q.high=0;    
    if(argc!=3){
      // this is an error case
      printf("WRONG PARAMETRES GIVEN");
      return -1;
    }
    NCPU=5;
   NCPU = atoi(argv[1]);// no of cpu 
    
    int TSLICE = atoi(argv[2]);// time slice for round robin
    int status1=fork();
     if(status1<0){
       printf("ERROR IN FORKING THE PROCESS FOR THE SCHEDULAR");
     }
     else if(status1==0){
        //daemon schedular process  to generate signals
        //runs in background, did not create a grand child because 
        while(1){
         //generate signals after fixed intervals; 
          //alarm(TSLICE);
          sleep(TSLICE);
          kill(getppid(), SIGUSR1);// signal is sent to the parent process after every TSLICE
          
        }
     }
     else{
    signal(SIGUSR1, signal_handler);
    signal(SIGCHLD,handler_child);
    signal(SIGINT,ctrlCHandler);
    while (1) {
        shell_loop();
    }
    //wait(NULL);
    
    }
     
    return 0;
}
