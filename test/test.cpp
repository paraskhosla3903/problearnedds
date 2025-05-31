#include "../include/pds/pds.h"

using namespace pds::hashTable;

int main() {
    OpenAddressingHashTable<std::string, std::string> table;
    table.init(128);

    table.insert("apple", "fruit");
    table.insert("carrot", "vegetable");
    table.insert("banana", "fruit");

    auto result = table.query("apple");
    if (result) std::cout << "Query result: " << *result << "\n";

    table.erase("carrot");

    std::cout << "Contains banana? " << table.contains("banana") << "\n";

    return 0;
}