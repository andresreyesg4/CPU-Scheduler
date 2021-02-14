/* Do Not Modify This File */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Process Flags */
#define STATE_COUNT         5
#define STATE_CREATED    0x01
#define STATE_READY      0x02
#define STATE_STOPPED    0x04
#define STATE_DEFUNCT    0x08
#define STATE_TERMINATED 0x10

#define PF_COUNT            1
#define PF_SUPERPRIV     0x20

/* Simulation Constants */
#define TIME_STARVATION 5     /* Starvation Time */
#define MAX_COMMAND     255   /* Max Process Command Length */ 
#define MAX_LINE_LEN    512   /* Max Trace File Line Length */

#endif /* CONSTANTS_H */
