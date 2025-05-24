#include <gtest/gtest.h>
#include "MockAccount.h"

TEST(Account, balance_positive) {
    Account acc1(0, 1000);
    EXPECT_EQ(acc1.GetBalance(), 1000);

    acc1.Lock();
    EXPECT_NO_THROW(acc1.ChangeBalance(100));

    EXPECT_EQ(acc1.GetBalance(), 1100);
}

TEST(Accout, balance_negative) {
    Account Vasya(1, 100);

    EXPECT_THROW(Vasya.ChangeBalance(100), std::runtime_error);

    Vasya.Lock();
    EXPECT_ANY_THROW(Vasya.Lock());
}

TEST(Account, Locker) {
    MockAccount acc(0, 1111);
	EXPECT_CALL(acc, Lock()).Times(2);
	EXPECT_CALL(acc, Unlock()).Times(1);
	acc.Lock();
	acc.Lock();
	acc.Unlock();
}

