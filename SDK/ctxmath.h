#ifndef CTXMATH_H
#define CTXMATH_H

#include <stdint.h>

int64_t ctxmath_add_i64(int64_t a, int64_t b);
int64_t ctxmath_sub_i64(int64_t a, int64_t b);
int64_t ctxmath_mul_i64(int64_t a, int64_t b);
int64_t ctxmath_div_i64(int64_t a, int64_t b);

uint64_t ctxmath_add_u64(uint64_t a, uint64_t b);
uint64_t ctxmath_sub_u64(uint64_t a, uint64_t b);
uint64_t ctxmath_mul_u64(uint64_t a, uint64_t b);
uint64_t ctxmath_div_u64(uint64_t a, uint64_t b);

int ctxmath_abs_i64(int64_t value);

#endif
