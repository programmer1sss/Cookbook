#include "book.h"

#include <iostream>    // std::clog
#include <stdexcept>
#include <string>
#include <utility>     // std::move

namespace {
int gLive = 0;
}

// --- Конструктор за замовчуванням: делегує повному ---
Book::Book()
    : Book("(untitled)", "", "", 1450, 0) {}

// --- Повний конструктор: єдине місце, де інкрементується gLive ---
Book::Book(std::string title,
           std::string author,
           std::string isbn,
           int year,
           int totalCopies)
    : title_(std::move(title)),
      author_(std::move(author)),
      isbn_(std::move(isbn)),
      year_(year),
      totalCopies_(totalCopies),
      available_(totalCopies)
{
    // Перевірки інваріанту — у тілі, після ініціалізації полів.
    if (title_.empty()) {
        throw std::invalid_argument("Book: empty title");
    }
    if (year_ < 1450 || year_ > 2026) {
        throw std::out_of_range("Book: year out of range [1450, 2026]");
    }
    if (!isbn_.empty() && !isValidIsbn(isbn_)) {
        throw std::invalid_argument("Book: invalid ISBN (expected 13 digits)");
    }
    if (totalCopies_ < 0) {
        throw std::invalid_argument("Book: totalCopies must be >= 0");
    }

    // Логуємо ТІЛЬКИ після успішної перевірки інваріанту,
    // щоб не було "ctor" для об'єкта, який одразу кине виняток.
    ++gLive;
    std::clog << "ctor  " << title_ << "  live=" << gLive << '\n';
}

// --- Скорочений конструктор: делегує повному ---
Book::Book(std::string title, int year)
    : Book(std::move(title), std::string{}, std::string{}, year, 0) {}

// --- Переміщувальний конструктор ---
// gLive НЕ змінюємо: об'єкт не створюється і не знищується, лише переїжджає.
Book::Book(Book&& other) noexcept
    : title_(std::move(other.title_)),
      author_(std::move(other.author_)),
      isbn_(std::move(other.isbn_)),
      year_(other.year_),
      totalCopies_(other.totalCopies_),
      available_(other.available_)
{
    std::clog << "move  " << title_ << "  live=" << gLive << '\n';

    // moved-from: валідний, але «порожній» стан
    other.title_ = "(moved-from)";
    other.author_.clear();
    other.isbn_.clear();
    other.year_ = 1450;
    other.totalCopies_ = 0;
    other.available_ = 0;
}

// --- Переміщувальне присвоєння ---
Book& Book::operator=(Book&& other) noexcept {
    if (this != &other) {
        title_ = std::move(other.title_);
        author_ = std::move(other.author_);
        isbn_ = std::move(other.isbn_);
        year_ = other.year_;
        totalCopies_ = other.totalCopies_;
        available_ = other.available_;

        other.title_ = "(moved-from)";
        other.author_.clear();
        other.isbn_.clear();
        other.year_ = 1450;
        other.totalCopies_ = 0;
        other.available_ = 0;
    }
    return *this;
}

// --- Деструктор ---
Book::~Book() {
    --gLive;
    std::clog << "dtor  " << title_ << "  live=" << gLive << '\n';
}

// --- Гетери ---
const std::string& Book::getTitle() const { return title_; }
const std::string& Book::getAuthor() const { return author_; }
const std::string& Book::getIsbn() const { return isbn_; }
int Book::getYear() const { return year_; }
int Book::getTotalCopies() const { return totalCopies_; }
int Book::getAvailableCopies() const { return available_; }

// --- Обчислювальні методи ---
int Book::lentOut() const { return totalCopies_ - available_; }
bool Book::isAvailable() const { return available_ > 0; }
bool Book::isOlderThan(int y) const { return year_ < y; }

// --- Операції ---
bool Book::lend() {
    if (available_ <= 0) return false;
    --available_;
    return true;
}

bool Book::returnCopy() {
    if (available_ >= totalCopies_) return false;
    ++available_;
    return true;
}

bool Book::addCopies(int n) {
    if (n <= 0) return false;
    totalCopies_ += n;
    available_ += n;
    return true;
}

// --- Інваріант ---
bool Book::checkInvariant() const {
    return !title_.empty() &&
           year_ >= 1450 && year_ <= 2026 &&
           (isbn_.empty() || isValidIsbn(isbn_)) &&
           totalCopies_ >= 0 &&
           available_ >= 0 && available_ <= totalCopies_;
}

bool Book::isValidIsbn(const std::string& isbn) {
    if (isbn.size() != 13) return false;
    for (char c : isbn) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}