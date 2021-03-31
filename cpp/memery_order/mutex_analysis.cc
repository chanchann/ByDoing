#include <mutex>

// -stdlib=libc++ -O2 -Wall -Wextra -std=c++11

extern ::std::mutex mutex;

extern double dvalue;
extern int ivalue;

bool set_values(double dv, int iv) {
    ::std::lock_guard<::std::mutex> lock(::mutex);
    ::dvalue = dv;
    ::ivalue = iv;
    return true;
}