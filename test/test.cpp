#include "bignum-sqrt.h"
#include <openssl/rand.h>
#include <gtest/gtest.h>
#include <chrono>
#include <ctime> 


void print_hex(uint8_t* buf, int size)
{
	if (!buf || size <= 0) {
		return;
	}

	for (int i = 0; i < size; i++) {
		printf("%02X", buf[i]);
	}
	printf("\n");
}

long test_long_sqrt(int type, long x) 
{
    int r = 0;
	BIGNUM* bn = BN_new();
	BIGNUM* sqrt = BN_new();
	BN_CTX* ctx = BN_CTX_new();

    BN_zero(sqrt);
	BN_set_word(bn, x);

    switch(type) {
    case 1:
        BN_sqrt_by_newton(sqrt, bn);
        break;
    case 2:
        BN_sqrt_by_search(sqrt, bn);
        break;
    case 3:
    default:
        BN_sqrt_by_bit(sqrt, bn);
        break;
    }

    r = BN_get_word(sqrt);

	if (bn) {
		BN_free(bn);
		bn = nullptr;
	}
	if (sqrt) {
		BN_free(sqrt);
		sqrt = nullptr;
	}
	if (ctx) {
		BN_CTX_free(ctx);
		ctx = nullptr;
	}

    return r;
}

void test_bignum_sqrt(int bits)
{
	int x_bytes = 0;
	int ret_len = 0;
	uint8_t* rnd = nullptr;
	uint8_t* ret = nullptr;
	uint8_t* sqrt = nullptr;
	BIGNUM* bn_x = BN_new();
	BIGNUM* bn_sqrt = BN_new();
	std::chrono::system_clock::time_point start, end;
	std::chrono::duration<float> elapsed_seconds;
	float total_time;

	x_bytes = bits / 8;
	rnd = new uint8_t[x_bytes];
	ret = new uint8_t[x_bytes];
	sqrt = new uint8_t[x_bytes];
	memset(rnd, 0, x_bytes);
	memset(ret, 0, x_bytes);
	memset(sqrt, 0, x_bytes);

	if (RAND_bytes(rnd, x_bytes) <= 0) {
		printf("RAND_bytes() failed!\n");
		goto err;
	}
	if (!BN_bin2bn(rnd, x_bytes, bn_x)) {
		printf("BN_bin2bn() failed!\n");
		goto err;
	}
	printf("Big number: 0x");
	print_hex(rnd, x_bytes);
	printf("\n");

    // newton method
    printf("--->Test sqrt of %d bits by newton method...\n", bits);
	start = std::chrono::system_clock::now();
    BN_sqrt_by_newton(bn_sqrt, bn_x);
	end = std::chrono::system_clock::now();
    //
	if ((ret_len = BN_bn2bin(bn_sqrt, ret)) <= 0) {
		printf("BN_sqrt() failed!\n");
		goto err;
	}
	memcpy(sqrt + (x_bytes - ret_len), ret, ret_len);
	printf("squre root: 0x");
	print_hex(sqrt, x_bytes);
    //
	elapsed_seconds = end - start;
	total_time = elapsed_seconds.count();
	printf("Time(s): %f\n", total_time);
	printf("\n");

    // binary search method
    printf("--->Test sqrt of %d bits by binary search method...\n", bits);
	start = std::chrono::system_clock::now();
    BN_sqrt_by_search(bn_sqrt, bn_x);
	end = std::chrono::system_clock::now();
    //
	if ((ret_len = BN_bn2bin(bn_sqrt, ret)) <= 0) {
		printf("BN_sqrt() failed!\n");
		goto err;
	}
	memcpy(sqrt + (x_bytes - ret_len), ret, ret_len);
	printf("squre root: 0x");
	print_hex(sqrt, x_bytes);
    //
	elapsed_seconds = end - start;
	total_time = elapsed_seconds.count();
	printf("Time(s): %f\n", total_time);
	printf("\n");

    // bit-by-bit method
    printf("--->Test sqrt of %d bits by bit-by-bit method...\n", bits);
	start = std::chrono::system_clock::now();
    BN_sqrt_by_bit(bn_sqrt, bn_x);
	end = std::chrono::system_clock::now();
    //
	if ((ret_len = BN_bn2bin(bn_sqrt, ret)) <= 0) {
		printf("BN_sqrt() failed!\n");
		goto err;
	}
	memcpy(sqrt + (x_bytes - ret_len), ret, ret_len);
	printf("squre root: 0x");
	print_hex(sqrt, x_bytes);
    //
	elapsed_seconds = end - start;
	total_time = elapsed_seconds.count();
	printf("Time(s): %f\n", total_time);
	printf("\n");

err:
	if (bn_x) {
		BN_free(bn_x);
		bn_x = nullptr;
	}
	if (bn_sqrt) {
		BN_free(bn_sqrt);
		bn_sqrt = nullptr;
	}
}

TEST(Bignum_Sqrt, LongSqrt)
{
    int ret = 0;

    ret = test_long_sqrt(1, 16);
    EXPECT_TRUE(ret == 4);
    ret = test_long_sqrt(2, 16);
    EXPECT_TRUE(ret == 4);
    ret = test_long_sqrt(3, 16);
    EXPECT_TRUE(ret == 4);
    //
    ret = test_long_sqrt(1, 256);
    EXPECT_TRUE(ret == 16);
    ret = test_long_sqrt(2, 256);
    EXPECT_TRUE(ret == 16);
    ret = test_long_sqrt(3, 256);
    EXPECT_TRUE(ret == 16);
}

TEST(Bignum_Sqrt, BignumSqrt)
{
    test_bignum_sqrt(1024);
    test_bignum_sqrt(2048);
    test_bignum_sqrt(4096);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();

    return ret;
}