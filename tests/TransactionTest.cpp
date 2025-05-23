#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include "MockAccount.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::InSequence;

class TransactionTest : public ::testing::Test {
protected:
    Transaction tx;
    MockAccount from{1, 1000};
    MockAccount to{2, 500};
};

TEST_F(TransactionTest, ThrowsIfSameAccount) {
    MockAccount acc(1, 1000);
    EXPECT_THROW(tx.Make(acc, acc, 100), std::logic_error);
}

TEST_F(TransactionTest, ThrowsIfNegativeSum) {
    EXPECT_THROW(tx.Make(from, to, -10), std::invalid_argument);
}

TEST_F(TransactionTest, ThrowsIfSumTooSmall) {
    EXPECT_THROW(tx.Make(from, to, 50), std::logic_error);
}

TEST_F(TransactionTest, ReturnsFalseIfFeeTooHigh) {
    tx.set_fee(51);
    EXPECT_FALSE(tx.Make(from, to, 101));
}

TEST_F(TransactionTest, SuccessfulTransaction) {
    InSequence s;

    EXPECT_CALL(from, Lock());
    EXPECT_CALL(to, Lock());

    EXPECT_CALL(to, ChangeBalance(100));

    EXPECT_CALL(from, GetBalance()).WillOnce(Return(1000));
    EXPECT_CALL(from, ChangeBalance(-101));

    EXPECT_CALL(from, GetBalance()).WillOnce(Return(899));
    EXPECT_CALL(to, GetBalance()).WillOnce(Return(1100));

    EXPECT_TRUE(tx.Make(from, to, 100));
}

TEST_F(TransactionTest, DebitFailsAndRevertsCredit) {
    InSequence s;

    EXPECT_CALL(from, Lock());
    EXPECT_CALL(to, Lock());

    EXPECT_CALL(to, ChangeBalance(200));

    EXPECT_CALL(from, GetBalance()).WillOnce(Return(1000));
    EXPECT_CALL(from, GetBalance()).WillOnce(Return(199));

    EXPECT_TRUE(tx.Make(from, to, 200));
}
