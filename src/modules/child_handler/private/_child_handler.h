#ifndef _CHILD_HANDLER_H_
#define _CHILD_HANDLER_H_

typedef int pid_t;

#ifdef __cplusplus
extern "C" {
#endif

#define WNOHANG 0
#define WEXITSTATUS(status) ((status) & 0xFF)
#define WIFEXITED(status) 1

struct child_handler_internal {
    pid_t pid;
    int status;
    int flags;
    int alive;
};

pid_t waitpid(pid_t pid, int* status, int options);

#ifdef __cplusplus
}
#endif

#endif /* _CHILD_HANDLER_H_ */
