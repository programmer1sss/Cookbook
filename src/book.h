#pragma once

#include <string>

// Інваріант Book:
//  1. title_ не порожній;
//  2. рік у діапазоні [1450, 2026];
//  3. isbn_ порожній або містить рівно 13 цифр;
//  4. totalCopies_ >= 0;
//  5. 0 <= available_ <= totalCopies_.
class Book
{
public:
  // 1. Конструктор за замовчуванням — порожній, але коректний об'єкт.
  //    Реалізований у .cpp (делегує повному), щоб інкрементувати gLive.
  Book();

  // 2. Повний конструктор — усі параметри.
  Book(std::string title,
       std::string author,
       std::string isbn,
       int year,
       int totalCopies);

  // 3. Скорочений конструктор — делегує повному.
  Book(std::string title, int year);

  // 4. Явний деструктор — декрементує gLive і логує в std::clog.
  ~Book();

  // Забороняємо копіювання/переміщення, поки не вирішимо, як їх рахувати.
  // (Інакше gLive розсинхронізується: копія не викличе ctor, але викличе dtor.)
  Book(const Book &) = delete;
  Book &operator=(const Book &) = delete;
  Book(Book &&other) noexcept;
  Book &operator=(Book &&other) noexcept;

  // --- Гетери ---
  const std::string &getTitle() const;
  const std::string &getAuthor() const;
  const std::string &getIsbn() const;
  int getYear() const;
  int getTotalCopies() const;
  int getAvailableCopies() const;

  // --- Обчислювальні методи ---
  int lentOut() const;
  bool isAvailable() const;
  bool isOlderThan(int year) const;

  // --- Операції ---
  bool lend();
  bool returnCopy();
  bool addCopies(int n);

  // --- Перевірка інваріанта ---
  bool checkInvariant() const;

private:
  std::string title_ = "(untitled)";
  std::string author_;
  std::string isbn_;
  int year_ = 1450;
  int totalCopies_ = 0;
  int available_ = 0;

  static bool isValidIsbn(const std::string &isbn);
};