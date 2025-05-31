#include "pds/countingBloomFilter/countingBloomFilter.h"
#include <iostream>
#include <vector>

using namespace pds::bloomFilter;

int main()
{
    CountingBloomFilter<std::string> cbf;

    // Initialize with 3 hash functions
    cbf.init(3);

    std::vector<std::string> items = {
        "apple", "banana", "cherry", "date", "fig"
    };

    // Insert items
    std::cout << "\n=== INSERTING ITEMS ===\n";
    for (const auto& item : items) {
        cbf.insert(item);
    }

    // Query inserted items (should be likely present)
    std::cout << "\n=== QUERYING INSERTED ITEMS ===\n";
    for (const auto& item : items) {
        auto result = cbf.query(item);
        if (result.has_value()) {
            std::cout << "Possibly found '" << item 
                      << "' with false positive probability: " 
                      << result.value() * 100 << "%\n";
        } else {
            std::cout << "Definitely not found: '" << item << "'\n";
        }
    }

    // Query non-inserted item
    std::cout << "\n=== QUERYING NON-INSERTED ITEMS ===\n";
    std::vector<std::string> nonItems = { "grape", "kiwi", "mango" };
    for (const auto& item : nonItems) {
        auto result = cbf.query(item);
        if (result.has_value()) {
            std::cout << "[False Positive?] '" << item 
                      << "' may be present. Probability: "
                      << result.value() * 100 << "%\n";
        } else {
            std::cout << "Definitely not found: '" << item << "'\n";
        }
    }

    // Erase some items
    std::cout << "\n=== ERASING ITEMS ===\n";
    cbf.erase("banana");
    cbf.erase("cherry");
    cbf.erase("grape");  // Not inserted — should be a miss

    // Query again after erase
    std::cout << "\n=== QUERYING AFTER ERASE ===\n";
    for (const auto& item : items) {
        auto result = cbf.query(item);
        if (result.has_value()) {
            std::cout << "Possibly found '" << item 
                      << "' with false positive probability: " 
                      << result.value() * 100 << "%\n";
        } else {
            std::cout << "Definitely not found: '" << item << "'\n";
        }
    }

    std::cout << "\n=== FINISHED TESTING ===\n";
    return 0;
}