#include <kernel/time.h>
#include "private/_time.h"

static ktime_t boot_time_ns;
ktime_t __ktime_ticks = 0;

void time_init(void)
{
#ifdef __cplusplus
    boot_time_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count();
#else
    boot_time_ns = 0;
#endif
}

ktime_t ktime_now(void)
{
#ifdef __cplusplus
    ktime_t now =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch())
            .count();

    return now - boot_time_ns;
#else
    return 0;
#endif
}

void time_tick(void)
{
    __ktime_ticks++;
}

ktime_t ktime_ticks(void)
{
    return __ktime_ticks;
}