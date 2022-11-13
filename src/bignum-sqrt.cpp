#include "bignum-sqrt.h"

// 使用牛顿迭代法对大整数开平方
BIGNUM* BN_sqrt_by_newton(BIGNUM* r, const BIGNUM* x)
{
	BIGNUM* tmp = BN_new();
    BN_CTX* ctx = BN_CTX_new();

    if (!r) {
        r = BN_new();
    }

	// r = x
	BN_copy(r, x);

	// tmp = r^2
	BN_sqr(tmp, r, ctx);

	// r^2 > x
	while (BN_cmp(tmp, x) == 1) {
		// r = (r + x / r) / 2;
		BN_div(tmp, nullptr, x, r, ctx);
		BN_add(r, r, tmp);
		BN_div_word(r, 2);

		// tmp = r^2
		BN_sqr(tmp, r, ctx);
	}

    if (tmp) {
	    BN_free(tmp);
	    tmp = nullptr;
    }
    if (ctx) {
        BN_CTX_free(ctx);
        ctx = nullptr;
    }

	return r;
}

// 使用二分法对大整数开平方
BIGNUM* BN_sqrt_by_search(BIGNUM* r, const BIGNUM* x)
{
	BIGNUM* sum_n = nullptr;
	BIGNUM* n = nullptr;
	BIGNUM* top = nullptr;
	BIGNUM* bottom = nullptr;
	BIGNUM* tmp = nullptr;
    BN_CTX* ctx = nullptr;

    if (!(ctx = BN_CTX_new())) {
        return nullptr;
    }
    BN_CTX_start(ctx);

    if (!(sum_n = BN_CTX_get(ctx)) ||
        !(n = BN_CTX_get(ctx)) ||
        !(top = BN_CTX_get(ctx)) ||
        !(bottom = BN_CTX_get(ctx)) ||
        !(tmp = BN_CTX_get(ctx))) {
        return nullptr;
    }

    if (!r) {
        r = BN_new();
    }

	BN_set_word(sum_n, 0);
	BN_set_word(bottom, 0);

	BN_copy(n, x);
	BN_rshift(n, n, 1);
	BN_copy(top, x);

	for (;;) {
		BN_mul(sum_n, n, n, ctx);
		if (BN_cmp(sum_n, x) < 0) {
			BN_copy(bottom, n);
			BN_sub(tmp, top, bottom);
			BN_rshift(tmp, tmp, 1);
			BN_add(n, n, tmp);
			if (BN_cmp(n, bottom) == 0) {
				BN_copy(r, n);
				goto err;
			}
		}
		else if (BN_cmp(sum_n, x) > 0) {
			BN_copy(top, n);
			BN_sub(tmp, top, bottom);
			BN_rshift(tmp, tmp, 1);
			BN_sub(n, n, tmp);
			if (BN_cmp(n, top) == 0) {
				BN_sub_word(n, 1);
				BN_copy(r, n);
				goto err;
			}
		}
		else {
			BN_copy(r, n);
			goto err;
		}
	}

err:
    if (ctx) {
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
        ctx = nullptr;
    }

	return r;
}

// 逐比特验证法对大整数开平方
BIGNUM* BN_sqrt_by_bit(BIGNUM* r, const BIGNUM* x)
{
	int shift = 0;
	BN_CTX* ctx = nullptr;
	BIGNUM* sqrt = BN_new();
	BIGNUM* sqrt2 = BN_new();
	BIGNUM* tmp = BN_new();
	BIGNUM* tmp_x = BN_new();
	BIGNUM* mask = BN_new();
	BIGNUM* BN_ONE = BN_new();

	if (!(ctx = BN_CTX_new())) {
		goto err;
	}
	BN_CTX_start(ctx);
	if (!(sqrt = BN_CTX_get(ctx)) ||
		!(sqrt2 = BN_CTX_get(ctx)) ||
		!(tmp = BN_CTX_get(ctx)) ||
		!(tmp_x = BN_CTX_get(ctx)) ||
		!(mask = BN_CTX_get(ctx)) ||
		!(BN_ONE = BN_CTX_get(ctx))) {
		goto err;
	}
    
    if (!r) {
        r = BN_new();
    }

	shift = BN_num_bits(x) / 2;

	BN_one(BN_ONE);
	BN_zero(sqrt);
	BN_zero(sqrt2);
	BN_zero(tmp);
	BN_copy(tmp_x, x);

	while (shift >= 0) {
		BN_lshift(mask, BN_ONE, shift);
		//
		BN_lshift(tmp, sqrt, 1);
		BN_add(tmp, tmp, mask);
		BN_lshift(sqrt2, tmp, shift);
		//
		if (BN_cmp(sqrt2, tmp_x) <= 0) {
			BN_add(sqrt, sqrt, mask);
			BN_sub(tmp_x, tmp_x, sqrt2);
		}
		shift--;
	}

	BN_copy(r, sqrt);
err:
	if (ctx) {
		BN_CTX_end(ctx);
		BN_CTX_free(ctx);
		ctx = nullptr;
	}
	return r;
}