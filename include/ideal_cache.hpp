#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <vector>
#include <unordered_set>

template <typename KeyT>
int ideal_cache_hits(size_t cache_size, const std::vector<KeyT>& requests) {
    
    std::unordered_set<KeyT> cache;
    int hits = 0;
    
    std::unordered_map<KeyT, std::vector<size_t>> occurrences;
    for (size_t i = 0; i < requests.size(); ++i) {
        occurrences[requests[i]].push_back(i);
    }
    
    std::unordered_map<KeyT, size_t> ptr;
    for (auto& p : occurrences) {
        ptr[p.first] = 0;
    }
    
    for (size_t i = 0; i < requests.size(); ++i) {
        KeyT key = requests[i];
        
        if (ptr[key] < occurrences[key].size() && occurrences[key][ptr[key]] == i) {
            ptr[key]++;
        }
        
        if (cache.count(key)) {
            hits++;
            continue;
        }
        
        if (ptr[key] == occurrences[key].size()) {
            continue;
        }
        
        if (cache.size() < cache_size) {
            cache.insert(key);
            continue;
        }
        
        KeyT to_remove;
        size_t farthest_use = 0;
        bool found_unused = false;
        
        for (const KeyT& k : cache) {
            if (ptr[k] == occurrences[k].size()) {
                to_remove = k;
                found_unused = true;
                break;
            }
            size_t next_use = occurrences[k][ptr[k]];
            if (next_use > farthest_use) {
                farthest_use = next_use;
                to_remove = k;
            }
        }
        
        cache.erase(to_remove);
        cache.insert(key);
    }
    
    return hits;
}

#endif