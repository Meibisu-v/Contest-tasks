#include <iostream>
#include <vector>
#include <cstdlib>
#include <utility>
#include <list>
#include <string>
#include <algorithm>

#define FACTOR 2
#define DEFAULT_SIZE 100


template <typename T>
class hash_table {
private:

    std::vector<std::list<T>> table;
    size_t size_table = 0;;
    size_t elem_count = 0;

public:

    hash_table(size_t size_) {
        table.reserve(size_);
        size_table = size_;
        std::list<T> new_list;
        for (size_t i = 0; i < size_table; ++i) {
            table.push_back(new_list);
        }
    }
    ~hash_table() = default;

    size_t find_index(const T& elem) const {
        return std::hash<T>{}(elem) % size_table;
    }

    void insert_t(const T &elem) {
        if (elem_count + 1 >= size_table) {
            rehash();
        }
        size_t index = find_index(elem);

        ++elem_count;
        table[index].push_back(elem);
    }

    bool erase_t(const T& elem) {
        size_t index = find_index(elem);
        if (table[index].empty() || !find_t(elem)) {
            return false;
        }
        table[index].remove(elem);
        --elem_count;
        return true;
    }

    bool find_t(const T& elem) {
        size_t index = find_index(elem);
        const auto& list_ = table[index];
        const auto& find_it = std::find(list_.begin(), list_.end(), elem);
        return find_it != list_.end();
    }

    void rehash() {
        size_t new_size = size_table * FACTOR;

        std::vector<std::list<T>> new_table;
        new_table.reserve(new_size);
        for (size_t i = 0; i < new_size; ++i) {
            new_table.push_back(std::list<T>());
        }

        for (auto index : table) {
            for (auto elem : index) {
                size_t new_index = std::hash<T>{}(elem) % new_size;
                new_table[new_index].push_back(elem);
            }
        }
        swap_tables(new_table, new_size);
    }

    void swap_tables(std::vector<std::list<T>> new_table, size_t new_size) {
        table.resize(new_size);
        std::swap(table, new_table);
        size_table = new_size;
    }
};



int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    hash_table<std::string> table(DEFAULT_SIZE);
    char sign;
    std::cin >> sign;
    while (sign != '#'){
        std::string value;
        std::cin >> value;

        if (sign == '+'){
            table.insert_t(value);
        }
        if (sign == '?'){
            if (table.find_t(value)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        }
        if (sign == '-'){
            table.erase_t(value);
        }
        std::cin >> sign;
    }
    return 0;
}
