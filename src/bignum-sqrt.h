
#ifndef _BIGNUM_SQRT_H_
#define _BIGNUM_SQRT_H_

#include <openssl/bn.h>


// 使用牛顿迭代法对大整数开平方
BIGNUM* BN_sqrt_by_newton(BIGNUM* in, const BIGNUM* x);

// 使用二分法对大整数开平方
BIGNUM* BN_sqrt_by_search(BIGNUM* in, const BIGNUM* x);

// 逐比特验证法对大整数开平方
BIGNUM* BN_sqrt_by_bit(BIGNUM* in, const BIGNUM* x);


#endif //_BIGNUM_SQRT_H_