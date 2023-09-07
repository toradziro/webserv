#include <SecureMemset.h>

void secure_zero(void *s, size_t n) {
    volatile char *p = (char*)s;
    while(n--) {
        *p++ = 0;
    }
}