
/**
 *  user和kernel约定的syscall调用id
 */


#ifndef __SYSCALL_ID_H
#define __SYSCALL_ID_H


#define SYSCALL_WRITE      64
#define SYSCALL_EXIT       93
#define SYSCALL_YIELD     124
#define SYSCALL_GET_TIME  169


/* SYSCALL_WRITE */
#define FD_STDOUT  1


#endif /* __SYSCALL_ID_H */
