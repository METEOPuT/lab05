#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include "MockAccount.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::InSequence;

TEST(Transaction, construnct_and_positive) {
    Transaction first;
    EXPECT_EQ(first.fee(), 1);

    Account Petya(0, 6132);
    Account Katya(1, 2133);

    first.set_fee(32);
    EXPECT_EQ(first.fee(), 32);

    EXPECT_TRUE(first.Make(Petya, Katya, 100));
    EXPECT_EQ(Katya.GetBalance(), 2233);
    EXPECT_EQ(Petya.GetBalance(), 6000);

}

TEST(Transaction, negative) {
    Transaction second;
    second.set_fee(51);
    Account Roma(0, 10);
    Account Misha(1, 1000);

    EXPECT_THROW(second.Make(Misha, Misha, 0), std::logic_error);

    EXPECT_THROW(second.Make(Misha, Roma, -100), std::invalid_argument);

    EXPECT_THROW(second.Make(Misha, Roma, 50), std::logic_error);

    EXPECT_FALSE(second.Make(Misha, Roma, 100));

    second.set_fee(10);

    EXPECT_FALSE(second.Make(Roma, Misha, 100));

}
