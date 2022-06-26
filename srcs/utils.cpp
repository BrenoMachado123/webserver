#include "utils.hpp"


std::string & strtrim(std::string & str, const char * separators) {
    str.erase(str.find_last_not_of(separators) + 1);
    str.erase(0, str.find_first_not_of(separators));
    return (str);
}

static uint64_t    gettimeofday_ms(void)
{
    static struct timeval   tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t    timestamp_in_ms(void)
{
    static uint64_t start = 0;

    if (start == 0)
        start = gettimeofday_ms();
    return (gettimeofday_ms() - start);
}
