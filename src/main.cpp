#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include "recipe.h"
#include "cook.h"

void printRecipe(const Recipe &r)
{
    std::cout << "Назва: " << r.getName()
              << " | Кухня: " << r.getCuisine()
              << " | Час: " << r.getMinutes() << " хв"
              << " | Порцій: " << r.getServings()
              << " | Швидкий (<=30 хв)? " << (r.isQuick() ? "так" : "ні")
              << " | На 4+ порції? " << (r.servesAtLeast(4) ? "так" : "ні")
              << std::endl;
}

int main()
{
    std::cout << "==========================================\n";
    std::cout << "       ТЕСТУВАННЯ КЛАСУ RECIPE            \n";
    std::cout << "==========================================\n\n";

    // --- 1. Тест конструкторів та інваріанта за замовчуванням ---
    std::cout << "--- 1. Конструктори за замовчуванням та скорочений ---\n";
    Recipe defaultRecipe;
    assert(defaultRecipe.checkInvariant());
    printRecipe(defaultRecipe);

    Recipe shortRecipe("Еспресо");
    assert(shortRecipe.checkInvariant());
    assert(shortRecipe.getName() == "Еспресо");
    assert(shortRecipe.getCuisine() == "(невідома кухня)");
    printRecipe(shortRecipe);
    std::cout << "\n";

    // --- 2. Тест повного конструктора та гетерів ---
    std::cout << "--- 2. Повний конструктор ---\n";
    Recipe borsch("Борщ український", "Українська", 90, 6);
    assert(borsch.checkInvariant());
    assert(borsch.getName() == "Борщ український");
    assert(borsch.getCuisine() == "Українська");
    assert(borsch.getMinutes() == 90);
    assert(borsch.getServings() == 6);
    printRecipe(borsch);
    std::cout << "\n";

    // --- 3. Тест валідації інваріанта та винятків ---
    std::cout << "--- 3. Перевірка валідації інваріанта (винятки) ---\n";
    try
    {
        Recipe badName("", "Італійська", 20, 2);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "invalid_argument: " << e.what() << '\n';
    }
    catch (const std::exception &e)
    {
        std::cout << "std::exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cout << "unknown exception\n";
    }
    try
    {
        Recipe badCuisine("Паста", "", 20, 2);
        assert(false && "Мало бути кинуто std::invalid_argument для порожньої кухні");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Очікувано перехоплено помилку: " << e.what() << '\n';
    }

    try
    {
        Recipe badMinutes("Швидка страва", "Фастфуд", 0, 1);
        assert(false && "Мало бути кинуто std::out_of_range для хвилин <= 0");
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Очікувано перехоплено помилку: " << e.what() << '\n';
    }

    try
    {
        Recipe badServings("Омлет", "Французька", 10, 0);
        assert(false && "Мало бути кинуто std::invalid_argument для порцій < 1");
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Очікувано перехоплено помилку: " << e.what() << '\n';
    }
    std::cout << "\n";

    // --- 4. Тест обчислювальних методів та операцій зі зміною порцій ---
    std::cout << "--- 4. Обчислювальні методи та зміна порцій ---\n";
    Recipe pizza("Піца Маргарита", "Італійська", 25, 2);
    assert(pizza.isQuick() == true);
    assert(borsch.isQuick() == false);

    // totalMinutesFor: округлення вгору ((minutes * portions + servings - 1) / servings)
    // 2 порції робляться за 25 хв -> 5 порцій: (25 * 5 + 2 - 1) / 2 = 126 / 2 = 63 хв
    assert(pizza.totalMinutesFor(5) == 63);
    assert(pizza.totalMinutesFor(0) == 0);
    assert(pizza.totalMinutesFor(-3) == 0);
    std::cout << "Час на 5 порцій піци (вихідно 2 порції/25 хв): "
              << pizza.totalMinutesFor(5) << " хв (очікується 63)\n";

    // addServings
    assert(pizza.addServings(0) == false);
    assert(pizza.addServings(-2) == false);
    assert(pizza.addServings(2) == true);
    assert(pizza.getServings() == 4);
    assert(pizza.checkInvariant());
    std::cout << "Після addServings(2): порцій = " << pizza.getServings() << '\n';

    // reduceServings
    assert(pizza.reduceServings(0) == false);
    assert(pizza.reduceServings(-1) == false);
    assert(pizza.reduceServings(4) == false); // не можна зробити < 1 порції
    assert(pizza.reduceServings(2) == true);
    assert(pizza.getServings() == 2);
    assert(pizza.checkInvariant());
    std::cout << "Після reduceServings(2): порцій = " << pizza.getServings() << '\n';
    std::cout << "\n";

    // --- 5. Тест семантики переміщення (Move Constructor) ---
    std::cout << "--- 5. Переміщення об'єктів ---\n";
    {
        Recipe src("Вареники з вишнею", "Українська", 45, 4);
        printRecipe(src);

        Recipe dst = std::move(src); // Move-конструктор
        std::cout << "Після move-конструктора:\n";
        std::cout << "dst -> ";
        printRecipe(dst);
        std::cout << "src (moved-from) -> ";
        printRecipe(src);

        // Перевірка інваріантів цільового та спустошеного об'єктів
        assert(dst.checkInvariant());
        assert(dst.getName() == "Вареники з вишнею");
        assert(src.getName() == "(moved-from)");
        assert(src.checkInvariant());
    }
    std::cout << "\n";

    // --- 6. Тест колекції у std::vector та взаємодії з Cook ---
    std::cout << "--- 6. Робота в контейнері std::vector та клас Cook ---\n";
    Cook chef("Гордон Рамзі", "Висока кухня");
    assert(chef.getName() == "Гордон Рамзі");
    assert(chef.getSpecialty() == "Висока кухня");
    assert(chef.recipeCount() == 0);

    // Додавання об'єктів за допомогою move
    chef.addRecipe(Recipe("Яловичина Веллінгтон", "Британська", 120, 4));
    chef.addRecipe(Recipe("Різотто", "Італійська", 25, 2));
    chef.addRecipe(Recipe("Яблучний пиріг", "Французька", 50, 6));

    std::cout << "Кухар: " << chef.getName()
              << " (" << chef.getSpecialty() << ")\n";
    std::cout << "Кількість рецептів у базі: " << chef.recipeCount() << '\n';
    assert(chef.recipeCount() == 3);
    std::cout << "\n";

    std::cout << "==========================================\n";
    std::cout << "УСІ ТЕСТИ ВИКОНАНО УСПІШНО!\n";
    std::cout << "==========================================\n";

    return 0;
}