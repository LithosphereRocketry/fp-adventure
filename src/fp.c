#include "fp.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

struct fp {
    bool sign;
    int8_t exp;
    uint32_t mnt;
};

uint8_t trailing_set_bits(uint64_t val) {
    uint8_t ct = 0;
    while(val) {
        val >>= 1;
        ct++;
    }
    return ct;
}

enum float_type fp_classify(float f) {
    uint32_t f_bits = *(uint32_t*) &f;
    uint8_t exp_bits = f_bits >> 23;
    uint32_t mantissa_bits = f_bits & ((1<<23) - 1);

    if(exp_bits == 0) {
        if(mantissa_bits == 0) {
            return FP_ZERO;
        } else {
            return FP_SUBNORM;
        }
    } else if(exp_bits == 0xFF) {
        if(mantissa_bits == 0) {
            return FP_INF;
        } else {
            return FP_NAN;
        }
    } else {
        return FP_NORMAL;
    }
}

struct fp fp_unpack(float f) {
    struct fp u;
    uint32_t f_bits = *(uint32_t*) &f;
    u.sign = (f_bits & (1<<31)) != 0;
    u.exp = (f_bits >> 23) - 127;
    u.mnt = (f_bits & ((1<<23)-1)) | (1 << 23);
    return u;
}

float fp_pack(struct fp f) {
    uint32_t f_bits = (f.sign ? 1<<31 : 0) | ((f.exp+127) & 0xFF) << 23 | (f.mnt & ((1 << 23) - 1));
    return *(float*) &f_bits;
}

void fp_print(struct fp f) {
    printf("%c %i %x %g", f.sign ? '-' : '+', f.exp, f.mnt, fp_pack(f));
}

float fp_minus(float f) {
    uint32_t bits = (*(uint32_t*) &f) ^ (1 << 31);
    return *(float*) &bits;
}

// For now, we only support the NEAREST rounding mode

float fp_add(float fa, float fb) {
    struct fp a = fp_unpack(fa);
    struct fp b = fp_unpack(fb);

    // Ensure a >= b at least for exponents
    if(a.exp < b.exp) {
        struct fp tmp = a;
        a = b;
        b = tmp;
    }

#ifdef FP_VERBOSE
    fp_print(a);
    printf("\t|\t%08x\n", *(uint32_t*) &fa);
    fp_print(b);
    printf("\t|\t%08x\n", *(uint32_t*) &fb);
#endif

    struct fp res = {false, 0, 0};
    
    uint8_t exp_diff = a.exp - b.exp;
#ifdef FP_VERBOSE
    printf("exponent diff: %hhu\n", exp_diff);
#endif

    int64_t a_aligned = ((int64_t) a.mnt) << 24;
    if(a.sign) a_aligned *= -1;
#ifdef FP_VERBOSE
    printf("a aligned:\t%016lx\n", a_aligned);
#endif
    int64_t b_aligned = exp_diff > 63 ? 0 : (((uint64_t) b.mnt) << 24) >> exp_diff;
    if(b.sign) b_aligned *= -1;
#ifdef FP_VERBOSE
    printf("b aligned:\t%016lx\n", b_aligned);
#endif
    int64_t q_pre_mant = (a_aligned + b_aligned);
    if(q_pre_mant < 0) {
        res.sign = true;
        q_pre_mant *= -1;
    }
#ifdef FP_VERBOSE
    printf("a-aligned sum:\t%016lx\n", q_pre_mant);
#endif
    int32_t offs = trailing_set_bits(q_pre_mant) - 48;
#ifdef FP_VERBOSE
    printf("normalization offset: %hhi\n", offs);
#endif

    if(offs < -24 /* mant zero */ || a.exp + offs <= -127 /* exp underflow */) {
#ifdef FP_VERBOSE
        printf("rounds to zero\n");
#endif
        res.exp = -127; // becomes zero
        res.mnt = 0;
    } else if(a.exp + offs > 127) {
#ifdef FP_VERBOSE
        printf("rounds to infinity\n");
#endif
        res.exp = -128; // infinity
        res.mnt = 0;
    } else {
        res.exp = a.exp + offs;
        res.mnt = q_pre_mant >> (24 + offs);
        uint64_t discarded_bits = q_pre_mant & ((1 << (24 + offs)) - 1);
#ifdef FP_VERBOSE
        printf("new mantissa: %x\n", res.mnt);
        printf("discarded: %lx\n", discarded_bits);
#endif
        if(discarded_bits > (1 << (23 + offs))
        || discarded_bits == (1 << (23 + offs)) && (res.mnt & 1)) { // ties to even
#ifdef FP_VERBOSE
            printf("roundup needed\n");
#endif
            res.mnt ++;
#ifdef FP_VERBOSE
            printf("new mantissa: %x\n", res.mnt);
#endif
            if(res.mnt >= (1<<24)) {
#ifdef FP_VERBOSE
               printf("roundup overflow, shifting again\n");
#endif
                res.mnt >>= 1;
                res.exp ++;
#ifdef FP_VERBOSE
                printf("new mantissa: %x\n", res.mnt);
#endif
            }
        }
#ifdef FP_VERBOSE
        printf("new exponent: %hhi\n", res.exp);
#endif
    }

#ifdef FP_VERBOSE
    fp_print(res); printf("\n");

    printf("Expected: ");
    fp_print(fp_unpack(fa + fb));
    printf("\n");
#endif
    
    return fp_pack(res);
}

float fp_sub(float fa, float fb) {
    return fp_add(fa, fp_minus(fb));
}