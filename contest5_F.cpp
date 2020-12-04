#include <iostream>
#include <vector>
#include <limits.h>


const long long MIN_INFINITY = std::numeric_limits<long long>::min();

struct Node {
long long max_value;
size_t count_of_max;

Node() {
    max_value = MIN_INFINITY;
    count_of_max = 0;
}

Node(Node a, Node b) {
    max_value = std::max(a.max_value, b.max_value);
    count_of_max = (a.max_value > b.max_value ? a.count_of_max : b.count_of_max);
    if (a.max_value == b.max_value) {
        count_of_max = a.count_of_max + b.count_of_max;
    }
}
};

template <typename T>
class Segment_tree{
std::vector<Node> tree;
public:
    Segment_tree(const std::vector<T> &values) {
        tree.resize(4 * values.size());
        build(values, 1, 0, values.size() - 1);
    }
    Node get(size_t vertex, size_t tree_left, size_t tree_right,
             size_t left, size_t right) {
        if (left > right) {
            return Node();
        }
        if (left == tree_left && right == tree_right) {
            return tree[vertex];
        }
        size_t mid = (tree_left + tree_right) / 2;
        return Node(get(vertex * 2, tree_left, mid, left, std::min(right, mid)),
                    get(vertex * 2 + 1, mid + 1, tree_right, std::max(left, mid + 1), right));

    }
    void build(const std::vector<T> &values, size_t vertex, size_t left, size_t right) {
        if (left == right) {
            tree[vertex].max_value = values[left];
            tree[vertex].count_of_max = 1;
        } else {
            size_t mid = (left + right) / 2;
            build(values, vertex * 2, left, mid);
            build(values, vertex * 2 + 1, mid + 1, right);
            tree[vertex] = Node(tree[vertex * 2], tree[vertex * 2 + 1]);
        }
    }
};

int main() {
    size_t number_of_soldiers;
    std::cin >> number_of_soldiers;
    std::vector<int> number_of_awards(number_of_soldiers);
    for (size_t i = 0; i < number_of_soldiers; ++i) {
        std::cin >> number_of_awards[i];
    }
    Segment_tree tree(number_of_awards);
    size_t number_of_queries, left_border, right_border;
    std::cin >> number_of_queries;
    for (size_t i = 0; i < number_of_queries; ++i) {
        std::cin >> left_border >> right_border;
        std::cout << tree.get(1, 0, number_of_soldiers - 1, left_border - 1, right_border - 1).max_value << ' ';
        std::cout << tree.get(1, 0, number_of_soldiers - 1, left_border - 1, right_border - 1).count_of_max << '\n';
    }
    return 0;
}
