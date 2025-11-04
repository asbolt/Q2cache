#include <iostream>
#include <vector>

#include "twoq_cache.hpp"
#include "ideal_cache.hpp"

int main() {
    size_t cache_size, num_requests;
    std::cin >> cache_size >> num_requests;
    
    std::vector<int> requests(num_requests);
    for (size_t i = 0; i < num_requests; ++i) {
        std::cin >> requests[i];
    }
    
    TwoQCache<int> twoq_cache(cache_size);
    int twoq_hits = 0;
    
    for (int page : requests) {
        if (twoq_cache.look_update(page, [](int key) { return key; })) {
            twoq_hits++;
        }
    }
    
    int ideal_hits = ideal_cache_hits(cache_size, requests);
    
    std::cout << "2Q кеш: " << twoq_hits << " попаданий" << std::endl;
    std::cout << "Идеальный кеш: " << ideal_hits << " попаданий" << std::endl;
    
    return 0;
}