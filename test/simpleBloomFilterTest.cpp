#include "pds/bloomFilter/simpleBloomFilter.h"

using namespace pds::bloomFilter;

int main() {
    // Create Bloom Filter with default constructor and initialize with 3 hash functions
    SimpleBloomFilter<std::string> filter;
    filter.init(3);

    // Insert elements
    filter.insert("apple");
    filter.insert("banana");
    filter.insert("cherry");

    // Query inserted elements
    filter.query("apple");    // Expected: Likely present
    filter.query("banana");   // Expected: Likely present
    filter.query("cherry");   // Expected: Likely present

    // Query non-inserted element
    filter.query("mango");    // Expected: Possibly absent

    return 0;
}