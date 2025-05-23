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
    EXPECT_CALL(acc, id()).Times(1).WillOnce(Return(1));
    EXPECT_THROW(tx.Make(acc, acc, 100), std::logic_error);
}

TEST_F(TransactionTest, ThrowsIfNegativeSum) {
    EXPECT_CALL(from, id()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(to, id()).Times(1).WillOnce(Return(2));
    EXPECT_THROW(tx.Make(from, to, -10), std::invalid_argument);
}

TEST_F(TransactionTest, ThrowsIfSumTooSmall) {
    EXPECT_CALL(from, id()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(to, id()).Times(1).WillOnce(Return(2));
    EXPECT_THROW(tx.Make(from, to, 50), std::logic_error);
}

TEST_F(TransactionTest, ReturnsFalseIfFeeTooHigh) {
    EXPECT_CALL(from, id()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(to, id()).Times(1).WillOnce(Return(2));

    EXPECT_FALSE(tx.Make(from, to, 2));
}

TEST_F(TransactionTest, SuccessfulTransaction) {
    InSequence s;

    EXPECT_CALL(from, id()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(to, id()).Times(1).WillOnce(Return(2));

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);

    EXPECT_CALL(to, ChangeBalance(100)).Times(1);

    EXPECT_CALL(to, GetBalance()).Times(1).WillOnce(Return(1000));
    EXPECT_CALL(to, ChangeBalance(-101)).Times(1);

    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    EXPECT_TRUE(tx.Make(from, to, 100));
}

TEST_F(TransactionTest, DebitFailsAndRevertsCredit) {
    InSequence s;

    EXPECT_CALL(from, id()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(to, id()).Times(1).WillOnce(Return(2));

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);

    EXPECT_CALL(to, ChangeBalance(200)).Times(1);

    EXPECT_CALL(to, GetBalance()).Times(1).WillOnce(Return(50));
    EXPECT_CALL(to, ChangeBalance(-200)).Times(1);

    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    EXPECT_FALSE(tx.Make(from, to, 200));
}
