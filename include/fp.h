#ifndef FP_H
#define FP_H

#define FP_VERBOSE

enum float_type {
    FP_NORMAL,
    FP_SUBNORM,
    FP_ZERO,
    FP_INF,
    FP_NAN
};

enum float_type fp_classify(float f);
float fp_add(float fa, float fb);
float fp_minus(float f);
float fp_sub(float fa, float fb);
float fp_mul(float fa, float fb);
#endif