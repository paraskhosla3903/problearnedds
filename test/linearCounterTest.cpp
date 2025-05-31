#include "pds/linearCounter/linearCounter.h"
#include <string>

using namespace pds::bloomFilter;

int main()
{
    LinearCounter<std::string> counter;
    counter.init(1024);  // Initialize with 1024-bit bitmap

    counter.insert("apple");
    counter.insert("banana");
    counter.insert("cherry");
    counter.insert("apple");   // duplicate
    counter.insert("banana");  // duplicate
    counter.insert("date");

    auto estimated = counter.estimate().value();
    auto actual = counter.getSize();

    std::cout << "\nEstimated unique elements: " << estimated << "\n";
    std::cout << "Actual inserted elements:   " << actual << "\n";

    return 0;
}