/* Fill in your Name and GNumber in the following two comment fields
 * Name: Andres Reyes 
 * GNumber: G01162322
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"
#include "scheduler.h"

/* This helper function calls malloc and also checks if the 
 * pointer returns null.
 */
void *call_malloc(size_t size);

/* This add helper function will handle inserting a process into a linked list.
 * Which handles inserting in the beginning, middle and end.
 */
void insert(List *list, Process *process);

/* This removes a process from the list that fits the criteria to be removed. 
 */
Process *delete(List *list);

/* This helper function, will search for a matching pid in the list that is recieved. 
 * Once the process is found, it will be removed and and returned to be added to another list.
 */
Process *search(List *list, int pid);

/* This helper function will be taking care of freeing memory for every process in the list passed in. 
 */
void free_process(Process *head);

/* Called when the program terminates.
 * You may use this to clean up memory or for any other purpose.
 */
void scheduler_exit(Schedule *schedule) {
  return;
}

/* Initialize the Schedule Struct
 * Follow the specification for this function.
 * Returns a pointer to the new Schedule or NULL on any error.
 */
Schedule *scheduler_init() {
	//Initialize the schedule and all of its components.
  Schedule *schedule = (Schedule *)call_malloc(sizeof(Schedule));
	
	//for each list set the head to null and count to 0.
  schedule->ready_list = (List *)call_malloc(sizeof(List));
  schedule->ready_list->head = NULL;
  schedule->ready_list->count = 0;

  schedule->stopped_list = (List *)call_malloc(sizeof(List));
  schedule->stopped_list->head = NULL;
  schedule->stopped_list->count = 0;

  schedule->defunct_list = (List *)call_malloc(sizeof(List));
  schedule->defunct_list->head = NULL;
  schedule->defunct_list->count = 0;
  return schedule;
}

/* Add the process into the appropriate linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_add(Schedule *schedule, Process *process) {

  int sucessful = 0;	
  int flags = process->flags;
	//if flags are in state created. insert in ready list.
  if(flags & STATE_CREATED){
    flags |= STATE_READY;
    flags &= 0xFFE; //turn off state created.
    process->flags = flags;
    insert(schedule->ready_list, process);
	
	//check for state ready.
  }else if(flags & STATE_READY){
    if(process->time_remaining > 0){
      insert(schedule->ready_list, process);
		
		//if the time remaining is 0, then zombie the process.
    }else if(process->time_remaining == 0){
      flags |= STATE_DEFUNCT;
      flags &= 0xFFD;
      process->flags = flags;
      insert(schedule->defunct_list, process);

    }else if(flags & STATE_DEFUNCT){
      insert(schedule->defunct_list, process);
    }else{
      sucessful = -1;
    }
  }
  return sucessful;
}

/* Move the process with matching pid from Ready to Stopped.
 * Change its State to Stopped.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_stop(Schedule *schedule, int pid) {
	//call the search function.
  Process *stopped = search(schedule->ready_list, pid);
  int success = 0;
  int flags = stopped->flags;

  if(stopped == NULL){
    success = -1;
  }else{
		//set flags and insert.
    flags |= STATE_STOPPED;
    flags &= 0xFFD;
    stopped->flags = flags;
    insert(schedule->stopped_list, stopped);

  }
  return success;
}

/* Move the process with matching pid from Stopped to Ready.
 * Change its State to Ready.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_continue(Schedule *schedule, int pid) {
	//call the search function.
  Process *continued = search(schedule->stopped_list, pid);
  int success = 0;
  int flags = continued->flags;

  if(continued == NULL){
    success = -1;
  }else{
		//set flags and insert.
    flags |= STATE_READY;
    flags &= 0xFFB;
    continued->flags = flags;
    insert(schedule->ready_list, continued);
  }
  return success;
}

/* Remove the process with matching pid from Defunct.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int scheduler_reap(Schedule *schedule, int pid) {
	//call the search function and get a pointer back.
  Process *reaped = search(schedule->defunct_list, pid);
  int exit_code = 0;
	
	if(reaped != NULL){
	//set flags.
  reaped->flags |= STATE_TERMINATED;
  exit_code = reaped->flags & 0xFFFFFFE0; //extract exit code.
	exit_code = exit_code >> 6;	//shift left to remove the uneccessary bits.
	free(reaped->command);
  free(reaped);
	}
  return exit_code;
}

/* Create a new Process with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Set the STATE_CREATED flag.
 * If is_sudo, also set the PF_SUPERPRIV flag.
 * Follow the specification for this function.
 * Returns the Process on success or a NULL on any error.
 */
Process *scheduler_generate(char *command, int pid, int time_remaining, int is_sudo) {
  //set a process pointer to allocate memory
	Process *process = (Process *)call_malloc(sizeof(Process));
	//set memory for command then use strcpy to copy the command.
  process->command = (char *)call_malloc(sizeof(strlen(command))+1);
  strcpy(process->command, command);
	//set the process pid to pid.
  process->pid = 0;
	process->pid = pid;
	//initialize the rest.
	process->flags = 0;
	process->time_remaining = 0;
	process->time_last_run = 0;
	
	//check if sudo, then set flags accordingly.
  if(is_sudo){
    process->flags |= (PF_SUPERPRIV | STATE_CREATED);
  }else{
    process->flags |= STATE_CREATED;
  }
	//set the rest.
  process->time_remaining = time_remaining;
  process->time_last_run = clock_get_time();
	process->next = NULL;
  return process;
}

/* Select the next process to run from Ready List.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
Process *scheduler_select(Schedule *schedule) {
  return delete(schedule->ready_list);
}

/* Returns the number of items in a given List
 * Follow the specification for this function.
 * Returns the count of the List, or -1 on any errors.
 */
int scheduler_count(List *ll) {
  int count = 0;
	//if list is null, then there is an error.
  if(ll == NULL){
    count = -1;
  }else {
    count = ll->count;
  }
	return count;
}

/* Completely frees all allocated memory in the scheduler
 * Follow the specification for this function.
 */
void scheduler_free(Schedule *scheduler) {
	if(scheduler != NULL){
		//call the helper method to free the head and processes. Then the list itself.
		free_process(scheduler->ready_list->head);
  	free(scheduler->ready_list);

		free_process(scheduler->stopped_list->head);
  	free(scheduler->stopped_list);

		free_process(scheduler->defunct_list->head);
  	free(scheduler->defunct_list);
		//finally free the scheduler.
  	free(scheduler);
  }
  return;
}

/* Helper function which will handle inserting in the head, middle and end.
 * The insertion will be according to the lowest pid. Ascending order.
*/
void insert(List *list, Process *process){
	//if the list is empty, insert at the end.
  if(list->head == NULL){
    list->head = process;
		(list->count)++;
		return;
	
  }else{
		//if the new process's pid is lower, then update the head.
		if(process->pid < list->head->pid){
			process->next = list->head;
			list->head = process;

		}else{
			//have a temp point at the head's next and a previous point at the head.
    	Process *temp = list->head->next;
			Process *previous = list->head;
			
			//if we find a process whose pid is smaller than the temp, break.
    	while(temp != NULL){
				if(process->pid < temp->pid){
						break;
				}
	      //set the previous to the next, then next to it's next.
				previous = previous->next;
	      temp = temp->next;
	    }
			
			//once out the loop, we set the process's next to the temp, and previous to process.
			process->next = temp;
			previous->next = process;
		}
		(list->count)++;
  }
}

/* Helper function to remove a process from the ready list.
 * The function returns the process with the next pointer to null.
 */
Process *delete(List *list){
	//if the list has an empty head, then there is an error. 
	if(list->head == NULL){
		return NULL;

	}else{
		Process *temp = list->head;	 //temp is used for the while loop only.
		Process *previous = NULL;		//used to point one process before the one to be removed.
		Process *removed = NULL;		//is used to set the process to remove.
		Process *lwst = list->head;	//used to check the head conditions.

		int lowest = temp->time_remaining;	//used to compare the time remaining starting with the head.
		int counter = list->count; 	//used to update the counter of the list.
		
		//first thing to check is if the head is starving.
		if(clock_get_time() - temp->time_last_run >= TIME_STARVATION){
			removed = temp;

		}else{
			//find other process in starvation or with the lowest time remaining. 
			while(temp->next != NULL){
				if(clock_get_time() - temp->next->time_last_run >= TIME_STARVATION){
					previous = temp; 
					removed = temp->next;
					lwst = NULL;		//setting lwst to null will prevent the first if to run, since the head is already checked for starvation.
					break;

				}else if(lowest > temp->next->time_remaining){
					lowest = temp->next->time_remaining;
					removed = temp->next;
					lwst = NULL; 		//since the int lowest is set to the heads time remaining, if the this condition is true, lwst is no longer needed.
					previous = temp; //one previous from removed.
				}
				temp = temp->next;
			}
		}
		
		//conditions to remove at the head.
		if(lwst == list->head ){
			removed = list->head;
			list->head = list->head->next;
			removed->next = NULL;
	
		}else if(removed != NULL && removed->next == NULL && previous != NULL){		//condition to remove at the end.
			previous->next = NULL;
			removed->next = NULL;

		}else if(removed != NULL && removed->next != NULL && previous != NULL){	//condition to remove at the middle.
			previous->next = removed->next;
			removed->next = NULL;
		}else { 	//returns null if error.
			return NULL;
		}

		counter--;
		list->count = counter;
		return removed;
	}
}

/* This helper function, will search for a matching pid in the list that is recieved. 
 * Once the process is found, it will be removed and and returned to be added to another list.
 */
 Process *search(List *list, int pid){
	//set a temp at the head to use in a loop.
  Process *temp = list->head;
	//set a temp2 to use in a loop to point one previous.
	Process *temp2 = list->head;	
	int counter = list->count;

	//find the matching pid.
  while(temp != NULL){
    if(temp->pid == pid){
      break;
    }
    temp = temp->next;
  }
	
	//point to the process previous to the temp.
	while(temp2->next != temp && temp2 != NULL && temp != list->head){
		temp2 = temp2->next;
	}
	
	if(temp != NULL){
	//if the temp is the head, then we update the head.
	if(temp == list->head){
		list->head = temp->next;
		temp->next = NULL;
	//if temp is in the middle of the list.
	}else if(temp->next != NULL){
		temp2->next = temp->next;
		temp->next = NULL;
	//if the temp is at the end.
	}else{
		temp2->next = NULL;
		temp->next = NULL;
	}
	
	counter--;
	list->count = counter;
	}
  return temp; //returns the pointer of the process to be removed in the specified list.
 }

/* This helper function calls free on all the list's processes. 
 * Therefore exterminating every leak possible. 
 */
void free_process(Process *head){
	//set a temporary at the head.
	Process *temp = head;
	//in a loop update the head then free the temp.
	while(temp != NULL){
		head = head->next;
		free(temp->command);
		free(temp);
		temp = head;
	}
}

/* This helper function will be taking care of calling malloc. 
 * it will return the pointer to memory.
 */
void *call_malloc(size_t size){
  void *pointer = malloc(size);
  if(pointer == NULL){
    puts("Memory allocation gone wrong.");
  }

  return pointer;
}
