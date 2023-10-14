# Schedular
SimpleScheduler: A Process Scheduler in C from Scratch
In this we have a made a simple schedular. There are two different .c files, simpleshell and simpleschedular.c, which must be compiled together. After running the running executable obtained after compiling, the Number of cpu's available and the time slice for each of the process should be mentioned as a command line parametre. Then, the simple shell will begin,
the user can give any command of type sumbit ./<executable_name>  <priority> , by default if the priority is not provided then its assumed to be 1.The simple shell further forks() the schedular process, which runs in an infinite loops in the background. It is a daemon process, and after every TSLICE interval, it schedules the process by going in the queue, and selecting NCPU proceses,them according to their priority to resume their exectuion from the point at which it was paused. Any process entering for the first time, is paused, and waits for the schedular till the next TSLICE seconds for it to schedule it. There are some basic assumptions like -
1.The scheduling happens only after TSLICE interval, even if some process completed its execution before the expiration of the TSLICE interval, it will wait for the next one, till that the cpu remains idle. So the time quantum expires for all the currently existing jobs at the same time. 
2.The user executable which are provided does not have any command line arguments, blocking calls like sleep, and no pipe.

The simple_schedular.c basically contains functions like add_process() for adding a proccess that is newly entered into the queue. The implementations can support any number of jobs sumbitted by the user at any given time. 


Contributions-
Rishabh Jay- Implemented the functions of the simpleschedular from scratch, and the helper header file, and modified the earlier implemented simple_shell.c to work with simpleschedualar.c.Also, tested the implementations with some slow - to high time taking exectuables, such as counting the number of the primes upto 1e9, and also debugged.

Himang Chandra Garg-Implemented the schedular with the calculation of wait time and execution time in the schedular. Printed the final details of the final details of the process according to their priority.
Handled the error in priority calculations with a few other corrections.
Also tested the code using several testcases and debugged accordingly
