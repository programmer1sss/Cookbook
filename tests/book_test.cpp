#include <gtest/gtest.h>
#include "book.h"

// 1. Успішна зміна поля
TEST(BookTest, SuccessfulAddCopies) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 2);

  EXPECT_TRUE(b.addCopies(3));
  EXPECT_EQ(b.getTotalCopies(), 5);
  EXPECT_EQ(b.getAvailableCopies(), 5);
  EXPECT_EQ(b.lentOut(), 0);
  EXPECT_TRUE(b.checkInvariant());
}

// 2. Відхилення некоректного значення
TEST(BookTest, RejectsInvalidAddCopies) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 2);

  EXPECT_FALSE(b.addCopies(0));
  EXPECT_FALSE(b.addCopies(-5));
  EXPECT_TRUE(b.checkInvariant());
}

// 3. Незмінність стану після відхилення
TEST(BookTest, RejectedOperationKeepsState) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 2);

  int totalBefore = b.getTotalCopies();
  int availableBefore = b.getAvailableCopies();

  EXPECT_FALSE(b.addCopies(-1));
  EXPECT_EQ(b.getTotalCopies(), totalBefore);
  EXPECT_EQ(b.getAvailableCopies(), availableBefore);
  EXPECT_TRUE(b.checkInvariant());
}

// 4. Видача останнього примірника
TEST(BookTest, LendLastCopy) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 1);

  EXPECT_TRUE(b.lend());
  EXPECT_EQ(b.getAvailableCopies(), 0);
  EXPECT_EQ(b.lentOut(), 1);
  EXPECT_FALSE(b.isAvailable());
  EXPECT_TRUE(b.checkInvariant());
}

// 5. Спроба видати книжку, якої немає
TEST(BookTest, LendUnavailableBook) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 1);

  EXPECT_TRUE(b.lend());
  EXPECT_FALSE(b.lend());
  EXPECT_EQ(b.getAvailableCopies(), 0);
  EXPECT_EQ(b.lentOut(), 1);
  EXPECT_TRUE(b.checkInvariant());
}

// 6. Повернення понад наявну кількість
TEST(BookTest, ReturnMoreThanPossible) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 3);

  EXPECT_TRUE(b.lend());
  EXPECT_TRUE(b.returnCopy());
  EXPECT_FALSE(b.returnCopy());
  EXPECT_EQ(b.getAvailableCopies(), 3);
  EXPECT_EQ(b.getTotalCopies(), 3);
  EXPECT_TRUE(b.checkInvariant());
}

// 7. Обчислювальні методи
TEST(BookTest, ComputedMethods) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 3);

  EXPECT_EQ(b.lentOut(), 0);
  EXPECT_TRUE(b.isAvailable());

  b.lend();
  b.lend();
  EXPECT_EQ(b.lentOut(), 2);
  EXPECT_EQ(b.getAvailableCopies(), 1);
  EXPECT_TRUE(b.isAvailable());

  b.lend();
  EXPECT_EQ(b.lentOut(), 3);
  EXPECT_FALSE(b.isAvailable());
  EXPECT_TRUE(b.checkInvariant());
}

// 8. Інваріант зберігається після довільної послідовності операцій
TEST(BookTest, InvariantHoldsAfterOperations) {
  Book b("Кобзар", "Тарас Шевченко", "9789660335809", 1840, 2);

  b.lend();
  b.returnCopy();
  b.addCopies(1);
  b.lend();
  b.lend();
  b.lend();
  b.addCopies(0);
  b.addCopies(-5);

  EXPECT_TRUE(b.checkInvariant());
}
