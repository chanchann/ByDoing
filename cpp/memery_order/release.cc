#include <atomic>

extern double dvalue;
extern ::std::atomic<int> ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;
    ::ivalue.store(iv, ::std::memory_order_release);
    return true;
}