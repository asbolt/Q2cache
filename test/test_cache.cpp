#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>

#include "twoq_cache.hpp"

struct TestCase {
    size_t cache_size;
    std::vector<int> requests;
    int expected_hits;
};


void run_test(const TestCase& test) {
    TwoQCache<int> cache(test.cache_size);
    int hits = 0;
    
    for (int page : test.requests) {
        if (cache.look_update(page, [](int key) { return key; })) {
            hits++;
        }
    }
    
    assert(hits == test.expected_hits);
}

void run_all_tests() {
    std::vector<TestCase> tests = {
        {
            3,
            {1, 2, 1, 3, 2},
            2
        },
        {
            3,
            {1,2,3,1,2,3,1,2,3},
            6
        },
        {
            5,
            {2,3,5,2,8,1,3,9,2,4,6,2,7,3,1,5,2,8,4,6},
            6
        },
        {
            10,
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6, 7, 6, 8, 6, 9, 6, 10, 6, 1, 6, 2, 7, 3, 8, 4, 9, 5, 10, 11, 12, 13, 14, 15, 1, 2, 6, 7},
            30
        },
        {
            4,
            {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4},
            6
        }
    };
    
    std::cout << "=========================================" << std::endl;
    
    for (const auto& test : tests) {
        run_test(test);
    }
    
    std::cout << "Все тесты пройдены" << std::endl;
    std::cout << "=========================================" << std::endl;
}

int main() {
    try {
        run_all_tests();
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Тест упал с ошибкой: " << e.what() << std::endl;
        return 1;
    }
}