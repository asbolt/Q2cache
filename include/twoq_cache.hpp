#ifndef TWOQ_CACHE_HPP
#define TWOQ_CACHE_HPP

#include <list>
#include <unordered_map>

template <typename T, typename KeyT = int> 
class TwoQCache {
private:
    size_t capacity_;      
    size_t A1_capacity_; 
    
    std::list<std::pair<KeyT, T>> A1_in;  
    std::list<std::pair<KeyT, T>> Am;    
    
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    std::unordered_map<KeyT, ListIt> A1_in_hash;
    std::unordered_map<KeyT, ListIt> Am_hash;

public:
    TwoQCache(size_t capacity) : capacity_(capacity), A1_capacity_(capacity <= 4 ? 1 : capacity / 4) {}

    void put(const KeyT& key, const T& value) {
        T temp;
        if (get(key, temp)) {
            Am.front().second = value;
            return;
        }

        if (size() >= capacity_) {
            evict_from_cold();
        }

        A1_in.push_back({key, value});
        A1_in_hash[key] = --A1_in.end();
    }

    bool get(const KeyT& key, T& value) {
        auto it_am = Am_hash.find(key);
        if (it_am != Am_hash.end()) {
            value = it_am->second->second;
            Am.splice(Am.begin(), Am, it_am->second);
            return true;
        }

        auto it_a1 = A1_in_hash.find(key);
        if (it_a1 != A1_in_hash.end()) {
            value = it_a1->second->second;

            A1_in.erase(it_a1->second);
            A1_in_hash.erase(key);

            if (Am.size() >= capacity_ - A1_capacity_) {
                evict_from_hot();
            }

            Am.push_front({key, value});
            Am_hash[key] = Am.begin();

            return true;
        }

        return false;
    }

    size_t size() const {
        return A1_in.size() + Am.size();
    }

    size_t capacity() const {
        return capacity_;
    }

    bool contains(const KeyT& key) const {
        return A1_in_hash.find(key) != A1_in_hash.end() || 
               Am_hash.find(key) != Am_hash.end();
    }

    bool empty() const {
        return A1_in.empty() && Am.empty();
    }

    template <typename F>
    bool look_update(const KeyT& key, F slow_get_page) {
        T value;
        
        if (get(key, value)) {
            return true;
        }
        
        value = slow_get_page(key);
        put(key, value);
        return false; 
    }

private:
    void evict_from_cold() {
        if (!A1_in.empty()) {
            KeyT old_key = A1_in.front().first;
            A1_in_hash.erase(old_key);
            A1_in.pop_front();
        }
    }

    void evict_from_hot() {
        if (!Am.empty()) {
            KeyT old_key = Am.back().first;
            Am_hash.erase(old_key);
            Am.pop_back();
        }
    }
};

#endif