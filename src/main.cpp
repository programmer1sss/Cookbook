#include <iostream>
#include <vector>
#include <cassert>
#include "book.h"

void print(const Book &b)
{
    std::cout << "Назва: " << b.getTitle()
              << " | Автор: " << b.getAuthor()
              << " | ISBN: " << b.getIsbn()
              << " | Рік: " << b.getYear()
              << " | Усього: " << b.getTotalCopies()
              << " | Доступно: " << b.getAvailableCopies()
              << " | Видано: " << b.lentOut()
              << " | Є в наявності? "
              << (b.isAvailable() ? "так" : "ні")
              << " | Старша за 2000? "
              << (b.isOlderThan(2000) ? "так" : "ні")
              << std::endl;
}

int main()
{
    std::cout << "Каталог книг" << std::endl;
    std::cout << "------------" << std::endl;

    std::vector<Book> catalog;
    catalog.reserve(4); // щоб не було реаллокацій
    catalog.emplace_back("1984", "George Orwell", "9780451524935", 1949, 3);
    catalog.emplace_back("The Hobbit", "J.R.R. Tolkien", "9780547928227", 1937, 2);
    catalog.emplace_back("Clean Code", "Robert C. Martin", "9780132350884", 2008, 1);
    catalog.emplace_back("The Pragmatic Programmer", "Andrew Hunt", "9780201616224", 1999, 4);

    for (const auto &book : catalog)
    {
        print(book);
    }

    std::cout << "\n--- Демонстрація операцій на першій книзі ---" << std::endl;
    Book &b = catalog[0];
    print(b);

    std::cout << "\nlend() -> " << (b.lend() ? "true" : "false") << std::endl;
    print(b);

    std::cout << "\nlend() -> " << (b.lend() ? "true" : "false") << std::endl;
    print(b);

    std::cout << "\nlend() -> " << (b.lend() ? "true" : "false") << std::endl;
    print(b);

    // Усі три примірники видані — наступний lend() має повернути false
    std::cout << "\nlend() (немає що видавати) -> "
              << (b.lend() ? "true" : "false") << std::endl;

    std::cout << "\nreturnCopy() -> " << (b.returnCopy() ? "true" : "false") << std::endl;
    print(b);

    std::cout << "\naddCopies(2) -> " << (b.addCopies(2) ? "true" : "false") << std::endl;
    print(b);

    std::cout << "\naddCopies(0) (некоректно) -> "
              << (b.addCopies(0) ? "true" : "false") << std::endl;
    print(b);

    // Фінальна перевірка: після всіх операцій інваріант зберігся
    assert(b.checkInvariant());

    return 0;
}



