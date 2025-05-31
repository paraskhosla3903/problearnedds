#include "pds/cuckooFilter/cuckooFilter.h"
#include <iostream>
#include <vector>
#include <string>

using namespace pds::bloomFilter;

int main() {
    CuckooFilter<std::string> filter;
    filter.init(64, 4, 8, 500); // small table to visualize easily

    std::vector<std::string> items = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon"
    };

    std::cout << "=== INSERTION ===\n";
    for (const auto& item : items) {
        bool inserted = filter.insert(item);
        std::cout << "Inserted: " << item << " => " << (inserted ? "Success" : "Failed") << "\n";
    }

    std::cout << "\n=== QUERY ===\n";
    for (const auto& item : items) {
        auto result = filter.query(item);
        if (result.has_value()) {
            std::cout << "Queried: " << item << " => " << (result.value() ? "Possibly Present" : "Definitely Not Present") << "\n";
        } else {
            std::cout << "Queried: " << item << " => Query Error\n";
        }
    }

    std::cout << "\n=== QUERY NON-EXISTENT ===\n";
    std::vector<std::string> others = {"mango", "nectarine", "orange"};
    for (const auto& item : others) {
        auto result = filter.query(item);
        if (result.has_value()) {
            std::cout << "Queried: " << item << " => " << (result.value() ? "Possibly Present" : "Definitely Not Present") << "\n";
        } else {
            std::cout << "Queried: " << item << " => Query Error\n";
        }
    }

    std::cout << "\n=== ERASE ===\n";
    for (const auto& item : {"banana", "cherry", "kiwi"}) {
        bool erased = filter.erase(item);
        std::cout << "Erased: " << item << " => " << (erased ? "Success" : "Failed") << "\n";
    }

    std::cout << "\n=== RE-QUERY AFTER ERASE ===\n";
    for (const auto& item : items) {
        auto result = filter.query(item);
        if (result.has_value()) {
            std::cout << "Queried: " << item << " => " << (result.value() ? "Possibly Present" : "Definitely Not Present") << "\n";
        } else {
            std::cout << "Queried: " << item << " => Query Error\n";
        }
    }

    std::cout << "\nFilter Size: " << filter.getSize() << "\n";
    std::cout << "Filter is empty: " << (filter.isEmpty() ? "Yes" : "No") << "\n";

    return 0;
}