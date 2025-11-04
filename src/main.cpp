#include <iostream>
#include <vector>

#include "twoq_cache.hpp"

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

    std::cout << twoq_hits << std::endl;
    
    return 0;
}