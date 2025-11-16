#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <list>
#include <deque>
#include <unordered_map>
#include <vector>

template <typename T, typename KeyT = int> 
class IdealCache {
private:
    size_t capacity_;      
    
    std::list<std::pair<KeyT, T>> cache_;    
    
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    std::unordered_map<KeyT, std::deque<size_t>> requests_;

public:
    IdealCache(size_t capacity, std::vector<KeyT> requests) : capacity_(capacity) {
        for (size_t i = 0; i < requests.size(); i++)
        {
            requests_[requests[i]].push_back(i);
        }
    }

    void put(const KeyT& key, const T& value) {
        auto it = requests_.find(key);
        if (it == requests_.end())
            return;

        auto cache_it = hash_.find(key);
        if (cache_it != hash_.end()) {
            cache_it->second->second = value;
            return;
        }

        if (cache_.size() == capacity_)
        {
            if (!erase(key))
                return;
        }

        cache_.push_back({key, value});
        hash_[key] = --cache_.end();
    }

    bool get(const KeyT& key, T& value) {
        requests_[key].pop_front();
        if (requests_[key].empty())
            requests_.erase(key);

        auto it = hash_.find(key);
        if (it != hash_.end()) {
            value = it->second->second;
            return true;
        }

        return false;
    }

    size_t size() const {
        return cache_.size();
    }

    size_t capacity() const {
        return capacity_;
    }

    bool contains(const KeyT& key) const {
        return hash_.find(key) != hash_.end();
    }

    bool empty() const {
        return cache_.empty();
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
    bool erase (const KeyT& key)
    {
        KeyT key_to_remove = cache_.begin()->first;
        bool find_to_remove = false;
        size_t remove_position = requests_[key].front();

        for (auto& pair : cache_)
        {
            KeyT cur_key = pair.first;
            if (requests_[cur_key].empty())
            {
                key_to_remove = cur_key;
                find_to_remove = true;
                break;
            }
            size_t position = requests_[cur_key].front();
            if (position > remove_position)
            {
                key_to_remove = cur_key;
                remove_position = position;
                find_to_remove = true;
            }
        }
        if (!find_to_remove)
            return false;

        auto to_remove = hash_.find(key_to_remove);
        cache_.erase(to_remove->second);
        hash_.erase(key_to_remove);
        return true;
    }
};

#endif