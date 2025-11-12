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
    TwoQCache(size_t capacity) : capacity_(capacity), A1_capacity_(capacity / 4) {}

    void put(const KeyT& key, const T& value) {
        if (size() >= capacity_) {
            evict();
        }

        A1_in.push_back({key, value});
        A1_in_hash[key] = --A1_in.end();
    }

    bool get(const KeyT& key, T& value) {
        auto it = Am_hash.find(key);
        if (it != Am_hash.end()) {
            value = it->second->second;
            Am.splice(Am.begin(), Am, it->second); 
            return true;
        }

        it = A1_in_hash.find(key);
        if (it != A1_in_hash.end()) {
            value = it->second->second;
            promote_from_A1_to_Am(key, value);
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
    void promote_from_A1_to_Am(const KeyT& key, const T& value) {
        auto it = A1_in_hash.find(key);
        if (it != A1_in_hash.end()) {
            A1_in.erase(it->second);
            A1_in_hash.erase(key);
        }

        if (Am.size() >= capacity_ - A1_capacity_) {
            KeyT old_key = Am.back().first;
            Am_hash.erase(old_key);
            Am.pop_back();
        }

        Am.push_front({key, value});
        Am_hash[key] = Am.begin();
    }

    void evict() {
        KeyT old_key = A1_in.front().first;
        A1_in_hash.erase(old_key);
        A1_in.pop_front();
    }
};

#endif