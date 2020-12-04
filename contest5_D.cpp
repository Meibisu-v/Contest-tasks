#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
struct Min {
  T operator()(const T &lhs, const T &rhs) const {
    return std::min(lhs, rhs);
  }
};

template<class T, class Function>
class Sparse_table {
    std::vector<std::vector<T>> sparse_table;
    std::vector<size_t> log_;
    Function func;
public:
    Sparse_table(const std::vector<T> &values) {
        log_.resize(values.size() + 1, 0);
        size_t table_size = values.size();

        for (size_t i = 2; i < table_size; ++i) {
            log_[i] = log_[i / 2] + 1;
        }

        sparse_table.push_back(values);

        for (size_t next_size = 2; next_size <= table_size; next_size *= 2) {
            std::vector<T> next_level(table_size - next_size + 1);
            for (size_t i = 0; i < next_level.size(); ++i) {
                next_level[i] = func(sparse_table.back()[i], sparse_table.back()[i + next_size / 2]);
            }
            sparse_table.push_back(next_level);
        }
    }

    T query(size_t left, size_t right) const {
        size_t border = log_[right - left];
        return func(sparse_table[border][left], sparse_table[border][right - (1u << border) + 1]);
    }

};


template<typename T>
struct make_value {
  static T make_new_value(const T &previous_value) {
    return (23 * previous_value + 21563) % 16714589;
  }
};

int main() {
  size_t number_of_soldiers, queries_count, height_of_first;
  std::cin >> number_of_soldiers >> queries_count >> height_of_first;
  std::vector<size_t> height_of_soldiers(number_of_soldiers);
  height_of_soldiers[0] = height_of_first;
  for (size_t i = 1; i < number_of_soldiers; ++i) {
    height_of_soldiers[i] = make_value<size_t>::make_new_value(height_of_soldiers[i - 1]);
  }
  Sparse_table<size_t, Min<size_t>> sparse_table(height_of_soldiers);
  size_t left, right;
  std::cin >> left >> right;
  size_t answer = 0;
  for (size_t i = 1; i < queries_count; ++i) {
    answer = sparse_table.query(std::min(left - 1, right - 1), std::max(left - 1, right - 1));
    left = (17 * left + 751 + answer + 2 * i) % number_of_soldiers + 1;
    right = (13 * right + 593 + answer + 5 * i) % number_of_soldiers + 1;
  }
  answer = sparse_table.query(std::min(left - 1, right - 1), std::max(left - 1, right - 1));
  std::cout << left << ' ' << right << ' ' << answer;

  return 0;
}
