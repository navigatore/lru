#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <optional>

class LRUCache {
public:
  LRUCache(size_t max_size) : max_size(max_size) {}

  std::optional<int> get(int key) {
    if (!lookup.contains(key)) {
      return {};
    }
    auto elemIt = lookup[key];
    moveElemAtBeginning(elemIt);
    return elemIt->second;
  }

  void insert(int key, int value) {
    if (!lookup.contains(key)) {
      values.emplace_front(key, value);
      lookup[key] = begin(values);

      if (lookup.size() > max_size) {
        auto oldest_key = values.back().first;
        lookup.erase(oldest_key);
        values.pop_back();
      }
    } else {
      auto elemIt = lookup[key];
      moveElemAtBeginning(elemIt);
      elemIt->second = value;
    }
  }

private:
  typedef std::list<std::pair<int, int>> ListType;

  void moveElemAtBeginning(ListType::iterator elemIt) {
    if (elemIt != begin(values)) {
      values.splice(begin(values), values, elemIt, next(elemIt));
    }
  }

  size_t max_size;
  ListType values{};
  std::unordered_map<int, ListType::iterator> lookup{};
};

int main() {
  LRUCache lru(2);
  lru.insert(1, 2);
  lru.insert(1, 3);
  lru.insert(2, 55);
  assert((lru.get(1)));
  assert((lru.get(2)));
  assert((!lru.get(3)));
  assert((lru.get(1) == 3));
  assert((lru.get(2) == 55));
  lru.get(1);
  lru.insert(5, 32);
  assert((!lru.get(2)));
  assert((lru.get(1) == 3));
  std::cout << "OK" << std::endl;
}
