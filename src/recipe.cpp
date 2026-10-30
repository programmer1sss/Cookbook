#include "recipe.h"

#include <iostream>    // std::clog
#include <stdexcept>
#include <string>
#include <utility>     // std::move

namespace {
int gLive = 0;   // лічильник живих Recipe
}

// --- Конструктор за замовчуванням: делегує повному ---
Recipe::Recipe()
    : Recipe("(без назви)", "(невідома кухня)", 30, 1) {}

// --- Повний конструктор ---
// cuisine_ — const, тому ініціалізується ТІЛЬКИ в списку (присвоєння в тілі — помилка).
Recipe::Recipe(std::string name,
               std::string cuisine,
               int minutes,
               int servings)
    : servings_(servings),               // поле з ініціалізатором за замовчуванням — перекриваємо
      cuisine_(std::move(cuisine)),      // const-поле — обов'язково в списку
      name_(std::move(name)),
      minutes_(minutes)
{
    // Перевірки інваріанту — у тілі, після ініціалізації.
    if (name_.empty()) {
        throw std::invalid_argument("Recipe: empty name");
    }
    if (cuisine_.empty()) {
        throw std::invalid_argument("Recipe: empty cuisine");
    }
    if (!isValidMinutes(minutes_)) {
        throw std::out_of_range("Recipe: minutes out of range [1, 1440]");
    }
    if (servings_ < 1) {
        throw std::invalid_argument("Recipe: servings must be >= 1");
    }

    ++gLive;
    std::clog << "ctor  " << name_ << " (" << cuisine_ << ", "
              << minutes_ << "min, " << servings_ << " serv)  live=" << gLive << '\n';
}

// --- Скорочений конструктор: explicit, делегує повному ---
Recipe::Recipe(std::string name)
    : Recipe(std::move(name), "(невідома кухня)", 30, 1) {}




// --- Переміщувальне присвоєння ---
// cuisine_ — const, тому НЕ МОЖНА присвоїти. Тому move-assignment або = delete,
// або реалізується через placement new (складно). Для лабораторної — = delete.
// Але тоді std::vector<Recipe> не зможе робити erase/insert у середині (нам не треба).
// Див. коментар у recipe.h — там operator= теж оголошений. Варіанти:
//   (a) = delete для move-assignment (copy теж delete) → vector::reserve/push_back працює,
//       бо йому достатньо move-ctor;
//   (b) реалізувати через std::destroy_at + placement new — overkill.

Recipe::Recipe(Recipe&& other) noexcept
    : servings_(other.servings_),
      cuisine_(other.cuisine_),          // const — копіюємо (move неможливий)
      name_(std::move(other.name_)),
      minutes_(other.minutes_)
{
    std::clog << "move  " << name_ << "  live=" << gLive << '\n';

    other.servings_ = 1;
    other.name_ = "(moved-from)";
    other.minutes_ = 30;
    // cuisine_ — const, залишаємо як є

    }


// --- Деструктор ---
Recipe::~Recipe() {
    --gLive;
    std::clog << "dtor  " << name_ << "  live=" << gLive << '\n';
}

// --- Гетери ---
const std::string& Recipe::getName() const { return name_; }
const std::string& Recipe::getCuisine() const { return cuisine_; }
int Recipe::getMinutes() const { return minutes_; }
int Recipe::getServings() const { return servings_; }

// --- Обчислювальні методи ---
bool Recipe::isQuick() const { return minutes_ <= 30; }

bool Recipe::servesAtLeast(int n) const { return servings_ >= n; }

int Recipe::totalMinutesFor(int portions) const {
    if (portions <= 0) return 0;
    // пропорційно: якщо 4 порції за 40 хв, то 8 порцій — 80 хв
    return (minutes_ * portions + servings_ - 1) / servings_;
}

// --- Операції ---
bool Recipe::addServings(int n) {
    if (n <= 0) return false;
    servings_ += n;
    return true;
}

bool Recipe::reduceServings(int n) {
    if (n <= 0) return false;
    if (servings_ - n < 1) return false;   // не можна зменшити нижче 1
    servings_ -= n;
    return true;
}

// --- Інваріант ---
bool Recipe::checkInvariant() const {
    return !name_.empty() &&
           !cuisine_.empty() &&
           isValidMinutes(minutes_) &&
           servings_ >= 1;
}

bool Recipe::isValidMinutes(int m) {
    return m >= 1 && m <= 1440;
}