#include <memory>
#include <sys/wait.h>
#include <stdexcept>

class Child_Handler {
    /*  take child pid number and wait for it, or pass a handler to a new process */

    pid_t pid_c_;

    public:
    explicit Child_Handler(pid_t p) : pid_c_(pid_t(p)) { }
    
    Child_Handler(Child_Handler&& m) noexcept : pid_c_(std::move(m.pid_c_)) { }

    ~Child_Handler() {
        if (pid_c_) {
            int status;

            waitpid(*pid_c_, &status, 0);
        }
    }   

    Child_Handler& operator=(Child_Handler&& m) noexcept {
        pid_c_ = std::move(m.pid_c_);
        
        return *this;
    }

    Child_Handler(const Child_Handler&) = delete;

    Child_Handler& operator=(const Child_Handler&) = delete;
};