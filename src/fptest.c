#include "fptest.h"

#include <stdlib.h>
#include <stdio.h>

#include "fp.h"

void test_add(uint32_t f_bits, uint32_t g_bits) {
    float f = *(float*) &f_bits, g = *(float*) &g_bits;
    float h_exp = f+g;
    float h = fp_add(f, g);
    uint32_t h_exp_bits = *(uint32_t*) &h_exp;
    uint32_t h_bits = *(uint32_t*) &h;
    if(h_bits != h_exp_bits) {
        printf("%g (%08x) + %g (%08x)\n", f, f_bits, g, g_bits);
        printf("Expected %g (%08x), got %g (%08x)\n", h_exp, h_exp_bits, h, h_bits);
        exit(-1);
    }
}

void test_sub(uint32_t f_bits, uint32_t g_bits) {
    float f = *(float*) &f_bits, g = *(float*) &g_bits;
    float h_exp = f-g;
    float h = fp_sub(f, g);
    uint32_t h_exp_bits = *(uint32_t*) &h_exp;
    uint32_t h_bits = *(uint32_t*) &h;
    if(h_bits != h_exp_bits) {
        printf("%g (%08x) - %g (%08x)\n", f, f_bits, g, g_bits);
        printf("Expected %g (%08x), got %g (%08x)\n", h_exp, h_exp_bits, h, h_bits);
        exit(-1);
    }
}