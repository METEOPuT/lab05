#include <gtest/gtest.h>
#include "MockAccount.h"

TEST(AccountTest, ChangeBalance_IncreasesBalance) {
    MockAccount acc(1, 0);

    EXPECT_CALL(acc, Lock())
        .Times(1);
    EXPECT_CALL(acc, ChangeBalance(100))
        .Times(1);
    EXPECT_CALL(acc, GetBalance())
        .Times(1)
        .WillOnce(::testing::Return(100));
    EXPECT_CALL(acc, Unlock())
        .Times(1);

    acc.Lock();
    acc.ChangeBalance(100);
    EXPECT_EQ(acc.GetBalance(), 100);
    acc.Unlock();
}

TEST(AccountTest, ChangeBalance_DecreasesBalance) {
    MockAccount acc(1, 100);

    EXPECT_CALL(acc, Lock()).Times(1);
    EXPECT_CALL(acc, ChangeBalance(-30)).Times(1);
    EXPECT_CALL(acc, GetBalance())
        .Times(1)
        .WillOnce(::testing::Return(70));
    EXPECT_CALL(acc, Unlock()).Times(1);

    acc.Lock();
    acc.ChangeBalance(-30);
    EXPECT_EQ(acc.GetBalance(), 70);
    acc.Unlock();
}

TEST(AccountTest, LockAndUnlockCalled) {
    MockAccount acc(2, 50);

    EXPECT_CALL(acc, Lock()).Times(1);
    EXPECT_CALL(acc, Unlock()).Times(1);

    acc.Lock();
    acc.Unlock();
}
