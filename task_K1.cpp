#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>

using namespace std;

const int maxn = 100000;


template<class T, class Hash = std::hash<T>>
class DSU {
public:
    DSU(std::vector<T> _v) {
        for (const T &elem : _v) {
            size[elem] = 1;
            prev[elem] = elem;
        }
    }

    DSU(T start, T finish) {
        for (; start <= finish; start++) {
            size[start] = 1;
            prev[start] = start;
        }
    }

    void union_sets(const T &first, const T &second) {
        T first_root = find_set(first);
        T second_root = find_set(second);
        if (size[second_root] > size[first_root]) {
            T tmp = first_root;
            first_root = second_root;
            second_root = tmp;
        }
        prev[second_root] = first_root;
        size[first_root] += size[second_root];

    }

    size_t get_set_size(const T &elem) {
        return size[find_set(elem)];
    }

    bool in_same_set(const T &first, const T &second) {
        return find_set(first) == find_set(second);
    }


    T find_set(const T &elem) {
        if (elem == prev[elem]) {
            return elem;
        }
        return prev[elem] = find_set(prev[elem]);
    }
protected:
    std::unordered_map<T, size_t, Hash> size;
    std::unordered_map<T, T, Hash> prev;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t n, m;
    cin >> n >> m;
    std::vector<size_t> tmp(n + 1);
    for (size_t i = 0; i <= n; ++i) {
        tmp[i] = i;
    }
    DSU<size_t> parent(tmp);
    int result = 0;
    for (size_t i = 0; i < m; i++) {
        size_t vertex1, vertex2, w;
        cin >> vertex1 >> vertex2 >> w;
        size_t set_num1 = parent.find_set(--vertex1);
        size_t set_num2 = parent.find_set(--vertex2);
        if ( set_num1 != set_num2) {
            result += w;
            parent.union_sets(vertex1, vertex2);
        }
    }
    cout << result;
    return 0;
}
