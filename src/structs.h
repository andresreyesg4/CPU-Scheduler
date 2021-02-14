/* Do Not Modify this File */

/* Definitions for Structs for this Project */
#ifndef STRUCTS_H
#define STRUCTS_H


/* Process Struct Definition */
typedef struct process_struct {
  char *command;      /* Process Command */
  int pid;            /* Process ID (unique) */
  int flags;          /* Process Flags */
  int time_remaining; /* Time Units Left to Execute */
  int time_last_run;  /* Last Time Executed (or time created) */
  struct process_struct *next; 
} Process;

/* List Struct Definition */
typedef struct list_struct {
  Process *head; /* Singly Linked List */
  int count;     /* Number of items in list */
} List;

/* Schedule Struct Definition */
typedef struct schedule_struct {
  List *ready_list;   /* Ready Processes */
  List *stopped_list; /* Stopped Processes */
  List *defunct_list; /* Defunct Processes */
} Schedule;

#endif /* STRUCTS_H */
