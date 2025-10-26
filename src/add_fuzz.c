#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fp.h"
#include "fptest.h"
#include "time.h"

// #define ONESHOT

int main(int argc, char** argv) {
    srand(time(NULL));

    size_t count = 0;
    while(1) {
        // Small wrinkle here - on modern systems, rand produces a postive
        // signed integer for some reason, so it's 1 bit short of a full 32.
        uint32_t f_bits = rand() << 31 | rand();
        uint32_t g_bits = rand() << 31 | rand();
        float f = *(float*) &f_bits, g = *(float*) &g_bits;
        // Not dealing with complicated stuff for now
        if(fp_classify(f) != FP_NORMAL || fp_classify(g) != FP_NORMAL) continue;
#ifdef FP_VERBOSE
        printf("=====\n");
#endif
        test_add(f_bits, g_bits);

        count++;
        if(count % 1000000 == 0) {
            printf("%zu iterations\n", count);
        }
#ifdef ONESHOT
        break;
#endif
    }
}