#include <atomic>

extern double dvalue;
extern ::std::atomic<double*> ivalue;

bool set_values(double dv, int iv) {
    ::dvalue = dv;
    ::ivalue.store(&dvalue, ::std::memory_order_release);
    return true;
}

int get_value(double& dv) {
    double* pv = ::ivalue.load(::std::memory_order_consume);
    dv = *pv;
    return 0;
}