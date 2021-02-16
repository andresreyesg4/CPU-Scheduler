# CPU-Scheduler
This project simulates a CPU-Scheduler, specifically the Earliest Deadline First. This alrogithm is preemptive which means that it will make a new decision
everytime there is an interrupt for Created, I/O/, ready, Stopped, and Terminated. This project reflects on how the CPU selects a new process to run for 
one unit time.

## Description
This project was completed in my Systems Programming course with profesor Kevin Andrea. The <scheduler.c> is the only file I worked on. That source file contains
the heart of the Earliest Deadline First algorithm. There are three different structs that I use to construct the scheduling algorithm. The structs include a
Process struct, List struct, and a Scheduler Struct. The Process could be seen as a Node for a linked list. The list will hold the head and the count of the linked
list. The scuedhuler holds three different Linked lists which are the Ready List, Defunct List, and the Stopeed List. The ready list and the stopped list use
different algorithms to select the next process to run on the CPU. 

**Problem Background** 
A major feature of most operating systems is to allow multple processes (running programs) to run on a single CPU.
The OS manages processes by putting them into one of several scheduler lists, which are implemented as singly linked lists. 

For each time unit on the computer, the OS chooses the next process to run form the ready list, which contains all 
ready processes. This process will run for a set amount of time, which uses a hardware timer to keep track of.

When the timer expires, if the running process did not finish, it will go back to the ready list to get scheduled again. A process may be stopped which will have to be moved to the stopped list and will wait to be continued by
the scheduler. 

When a process finish its time execution time then it will be moved to the defunct list which will be safely removed and reaped by the scheduler. It will become a zombie process. 

## Project Overview
This project mplements several operations related to the CPU scheduler. This project requires the implementation
and use of three linked lists to create, store process structs, to move process structs between lists, to 
add/search/remove preocess structs from the lists, and to perform basic bitwise operations to manage flags and 
combined data. 

**scheduler.c** will work with pre-written files from an OS simulator to implement several of these operations. 
The structs for these lists are all defined in **structs.h**.

**Process Flags**
The process structs maintain their current state using the flags field. This is a 32-bit int that contains peices of informaiton, which have been combiend together using bitwise operations. 

### Bit 0 is 1-bit flag saying if the proces was run with super user privileges.** 
  *S = sudo

**Bits 1-6 represents the current state of the process.** These are stored as 1-bit values (0 for No, 1 for yes).
**Note: only one of these will be set to 1 at any time. All of the others must be 0!**.

1. C = created     
1. R = ready     
1. U = running     
1. T = stopped     
1. X = terminated    
1. D = defuncted
