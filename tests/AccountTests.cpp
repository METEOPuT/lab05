#include <gtest/gtest.h>
#include "Account.h"

TEST(AccountTest, ChangeBalanceIncreasesBalance) {
    Account acc(1, 0);
    acc.Lock();
    acc.ChangeBalance(100);
    EXPECT_EQ(acc.GetBalance(), 100);
    acc.Unlock();
}

TEST(AccountTest, ChangeBalanceDecreasesBalance) {
    Account acc(1, 0);
    acc.Lock();
    acc.ChangeBalance(100);
    acc.ChangeBalance(-30);
    EXPECT_EQ(acc.GetBalance(), 70);
    acc.Unlock();

}
