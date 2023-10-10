#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>// Include for time function 
#include <sys/wait.h> // Include for wait function
#include "helper.h"
char* history[10000];
char* pid_table[10000];
int size1=0;
char* time_table[10000];
char* diff_table[10000];
int diff_sz=0;
int time_sz=0;
int size_pid=0;   
int NCPU=0;
queue q;
int create_process_and_run(char *command) {
    // Append a newline character to the command
    size_t len = strlen(command);// this doesnt include the null pointing character 
    if (len == 0 || command[len - 1] != '\n') {
        strcat(command, "\n");
    }
 //Measuring start time of the process
    clock_t start_time = clock();
   char pid_str[10000];
   int p_pid = getpid();
   snprintf(pid_str, sizeof(pid_str), "%d", p_pid);

// Allocate memory for pid_table[size_pid] and copy pid_str
   pid_table[size_pid] = (char *)malloc(strlen(pid_str) + 1);
   strcpy(pid_table[size_pid], pid_str);
   size_pid++;
   time_t my_time = time(NULL);
    char * time_str = ctime(&my_time);
    time_str[strlen(time_str)-1] = '\0';
    time_table[time_sz]=(char*)malloc(strlen(time_str)+1);
    strcpy(time_table[time_sz],time_str);
    
    time_sz++;
// Allocate memory for the time string in time_table  

    int status = fork();
    int pid=status;
    int priority=1;
    int i;
    /*for(i=0;i<strlen(command);i++){
     if(command[i]<='4' && command[i]>='1')priority=atoi(command[i]);
    }*/
    char priority_str[2]; // Assuming priority is a single digit, so size 2 including null terminator
for (i = 0; i < strlen(command); i++) {
    if (command[i] >= '1' && command[i] <= '4') {
        priority_str[0] = command[i];
        priority_str[1] = '\0'; // Null-terminate the string
        priority = atoi(priority_str);
        break;
    }
}

    if(pid>0){
       kill(pid,SIGSTOP);//pausing the child process
       
    }
    if (status < 0) {
        printf("SOME ERROR IS THERE\n");
        exit(1); // Exit with an error code
    }if (status == 0) {
        // Child process
        // child process has started 
        clock_t
        char* command_2=(char*)malloc(strlen(command)-1);// command_2 will just include the string without any \0 or '\n'
        int i;
        for(i=0;i<strlen(command)-1;i++)command_2[i]=command[i];
        strtok(command_2," ");
        printf("%s","COMMAND_2 AFTER STRTOK IS");
        printf("%s",command_2);
       char* command_3= strtok(NULL," ");
       printf("COMMAND_3 is");
       printf("%s",command_3);
       char* command_4=strtok(NULL," ");
       printf("COMMAND_4 is");
       printf("%s",command_4);
        printf("COMMAND_3 is");
       printf("%s",command_3);
       
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
        add_process(pid,priority);
        // at this i need to start the timer for the waititng time for the process with pid=pid
        wait(NULL); // Wait for the child process to complete
        // at this point the child process has finished, so remove the process 
        remove_process(pid);
        
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

        size_t size = 0;
        int bytes_read=getline(&command, &size, stdin);
        // command is null terminated and contains the '\n'
        if(bytes_read==-1){
         printf("%s","ERROR IN READING IN THE INPUT");
         return ;
        }
        history[size1]=(char*)malloc(strlen(command)+1);//strlen() doesnt give the null terminated character
        strcpy(history[size1],command);
        size1++;
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
void handle_alarm(){
  // call schedular 
  schedule_process();
}
int main(int argc, char** argv) {
   member membrs[1000];
   queue q={0,0,membrs[1000]};
   q.low=0;
   q.high=0;
   struct itimerval timer;
    signal(SIGALRM, handle_alarm);// handler for timer interrupt
    if(argc!=3){
      // this is an error case
      printf("WRONG PARAMETRES GIVEN");
      return -1;
    }
    NCPU=5;
   NCPU = atoi(argv[1]);// no of cpu 
  
    int TSLICE = atoi(argv[2]);// time slice for round robin
   
    timer.it_interval.tv_sec = TSLICE;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = TSLICE;
    timer.it_value.tv_usec = 0;
    signal(SIGALRM, handle_alarm);
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error setting timer");
        return 1;
    }

    while (1) {
        shell_loop();
    }
    return 0;
}

