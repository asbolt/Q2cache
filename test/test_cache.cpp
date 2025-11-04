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
            10,
            {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
            5
        },
        {
            8,
            {1,2,3,4,5,6,7,8, 1,2,3,4,9,10, 1,2,11,12, 1,2},
            8
        },
        {
            6,
            {1,2,3,4,5,6, 1,2,7,8, 1,2,9,10, 1,2,11,12},
            6
        },
        {
            10,
            {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, 1,2,3,4,5},
            0
        },
        {
            12,
            {1,2,3,4,5,6,7,8,9,10,11,12, 1,2,3,4,13,14,15,16, 1,2,3,4},
            8
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