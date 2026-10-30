#include <iostream>
#include <memory>
#include <vector>
#include "book.h"

static Book gStatic{"Статичний", 1900};

int main() {
    std::clog << "--- початок main ---\n";

    Book onStack{"Автоматичний", 2020};

    {
        Book inner{"Вкладений", 2021};
    }

    // Було: Book* leaked = new Book{"Забутий", 2022};
    // Стало: RAII — без витоку
    auto leaked = std::make_unique<Book>("Забутий", 2022);
    (void)leaked;   // запобіжник від -Wunused-variable (хоча unique_ptr не потребує)

    auto owned = std::make_unique<Book>("Керований", 2023);

    std::vector<Book> shelf;
    shelf.reserve(2);
    shelf.emplace_back("У векторі", 2024);

    std::clog << "--- кінець main ---\n";
    return 0;
}