#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fptest.h"

int main(int argc, char** argv) {
    // A collection of the most obnoxious operations I've found

    // haha funny
    test_add(0x69000420, 0xdeadbeef);

    // Result zero
    test_sub(0x77777777, 0x77777777);

    // Rounding causes mantissa overflow, causing an additional shift
    test_add(0x5b819c6c, 0x5df7e639);
    // Rounding causes mantissa overflow, causing an additional shift which
    // overflows to infinity (somehow this one works correctly by default?)
    test_add(0x7F7FFFFF, 0x73000000);

    // Exponent underflow producing subnormal
    test_sub(0x01eb116c, 0x01ec4b8d);

    // Sum of subnormals
    test_add(0x0048d5fb, 0x087d577d);

    // Sum producing -inf
    test_add(0xfe976619, 0xff5ab913);

    // probably both to do with the input being infinity

    // inf (7f800000) + -1.06525e-37 (8210fe7d)
    // Expected inf (7f800000), got -1.03586e-37 (820cfe7d)

    // inf (7f800000) - 8.92134e+32 (762ff13d)
    // Expected inf (7f800000), got -8.92134e+32 (f62ff13d)
}