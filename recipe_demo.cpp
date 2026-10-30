#include <iostream>
#include <memory>
#include <vector>
#include "recipe.h"

// Статичний об'єкт: ctor до main, dtor після main.
static Recipe gStatic{"Борщ", "українська", 120, 4};

int main() {
    std::clog << "--- початок main ---\n";

    Recipe onStack{"Паста", "італійська", 25, 2};

    {
        Recipe inner{"Суші", "японська", 60, 3};
    }

    auto leaked = std::make_unique<Recipe>("Забутий рецепт", "французька", 45, 2);
    (void)leaked;

    auto owned = std::make_unique<Recipe>("Керований", "іспанська", 30, 4);

    std::vector<Recipe> book;
    book.reserve(2);
    book.emplace_back("Млинці", "українська", 20, 6);

    std::clog << "--- кінець main ---\n";
    return 0;
}