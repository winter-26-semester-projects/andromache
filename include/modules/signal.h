#ifndef SIGNAL_H
#define SIGNAL_H

typedef void (*sighandler_t)(int);

void signal_send(int pid, int signum);
void signal_register(int signum, sighandler_t handler);

#endif
