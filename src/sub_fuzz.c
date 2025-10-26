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
        uint32_t f_bits = rand(), g_bits = rand();
        float f = *(float*) &f_bits, g = *(float*) &g_bits;
        // Not dealing with complicated stuff for now
        if(fp_classify(f) == FP_NAN || fp_classify(g) == FP_NAN) continue;
#ifdef FP_VERBOSE
        printf("=====\n");
#endif
        test_sub(f_bits, g_bits);

        count++;
        if(count % 1000000 == 0) {
            printf("%zu iterations\n", count);
        }
#ifdef ONESHOT
        break;
#endif
    }
}