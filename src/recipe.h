#pragma once

#include <string>

// Інваріант Recipe:
//  1. name_ не порожній;
//  2. cuisine_ не порожній;
//  3. minutes_ у діапазоні [1, 1440] (від 1 хвилини до 24 годин);
//  4. servings_ >= 1.
class Recipe {
 public:
  // 1. Конструктор за замовчуванням — порожній, але коректний об'єкт.
  //    Делегує повному, щоб інкрементувати gLive і пройти перевірки.
  Recipe();

  // 2. Повний конструктор — усі параметри.
  //    cuisine_ — const, тому ініціалізація ТІЛЬКИ через список ініціалізації.
  Recipe(std::string name,
         std::string cuisine,
         int minutes,
         int servings);

  // 3. Скорочений конструктор з одним параметром — explicit, делегує повному.
  explicit Recipe(std::string name);

  // 4. Явний деструктор — декрементує gLive і логує в std::clog.
  ~Recipe();

  // Копіювання заборонено (як у Book: RAII, лічильник, унікальність).
  // Переміщення дозволено (для std::vector<Recipe>).
  Recipe(const Recipe&) = delete;
  Recipe& operator=(const Recipe&) = delete;
  Recipe(Recipe&& other) noexcept;
  Recipe& operator=(Recipe&& other) = delete; 

  // --- Гетери ---
  const std::string& getName() const;
  const std::string& getCuisine() const;
  int getMinutes() const;
  int getServings() const;

  // --- Обчислювальні методи ---
  bool isQuick() const;                 // <= 30 хвилин
  bool servesAtLeast(int n) const;      // порцій >= n
  int totalMinutesFor(int portions) const; // пропорційний час

  // --- Операції предметної області ---
  bool addServings(int n);   // додати n порцій (n > 0)
  bool reduceServings(int n); // зменшити на n (не нижче 1)

  // --- Перевірка інваріанта ---
  bool checkInvariant() const;

 private:
  // Поле з ініціалізатором за замовчуванням:
  int servings_ = 1;

  // const-поле: після створення кухня не змінюється → список ініціалізації обов'язковий
  const std::string cuisine_;

  std::string name_ = "(без назви)";
  int minutes_ = 30;

  static bool isValidMinutes(int m);
};