#pragma once

#include "Transaction.h"
#include "gmock/gmock.h"

class MockTransaction : public Transaction {
 public:
  MOCK_METHOD(void, Credit, (Account& account, int sum), (override));
  MOCK_METHOD(bool, Debit, (Account& account, int sum), (override));
  MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};
