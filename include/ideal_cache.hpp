#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <vector>
#include <unordered_set>

template <typename KeyT>
int ideal_cache_hits(size_t cache_size, const std::vector<KeyT>& requests) {
    std::unordered_set<KeyT> cache;
    int hits = 0;

    for (size_t i = 0; i < requests.size(); ++i) {
        KeyT key = requests[i];
        
        if (cache.find(key) != cache.end()) {
            hits++;
            continue;
        }
        
        if (cache.size() == cache_size) {
            KeyT to_remove;
            size_t farthest_use = 0;
            bool found_unused = false;
            
            for (KeyT cached_key : cache) {
                size_t next_use = requests.size();
                for (size_t j = i + 1; j < requests.size(); ++j) {
                    if (requests[j] == cached_key) {
                        next_use = j;
                        break;
                    }
                }
                
                if (next_use == requests.size()) {
                    to_remove = cached_key;
                    found_unused = true;
                    break;
                }
                
                if (next_use > farthest_use) {
                    farthest_use = next_use;
                    to_remove = cached_key;
                }
            }
            
            cache.erase(to_remove);
        }
        
        cache.insert(key);
    }
    
    return hits;
}

#endif