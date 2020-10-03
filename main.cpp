#include <iostream>
#include <string>
#include <vector>

struct Student{
    std::string name_surname = " ";
    size_t sum = 0;
};

template<class T>
void push_in_result(std::vector<T> &result, T value, size_t &end_, size_t &start) {
    result.push_back(value);
    ++end_;
    ++start;
}

template<class T>
void Merge_(std::vector<T> &array_, size_t beg_a, size_t mid, size_t end_a) {

    size_t start = beg_a;
    size_t middle = mid;
    size_t end_ = beg_a;

    std::vector<T> result;

    while (start < mid && middle <= end_a) {
        if (array_[start].sum >= array_[middle].sum) {
            push_in_result(result, array_[start], end_, start);
        }
        else {
            push_in_result(result, array_[middle], end_, middle);
        }
    }
    while (start < mid) {
        push_in_result(result, array_[start], end_, start);
    }
    while (middle <= end_a) {
        push_in_result(result, array_[middle], end_, middle);
    }
    swap_ranges(result.begin(), result.begin() + result.size(), array_.begin() + beg_a);
}

template<class T>
void sort_descending(std::vector<T> &array_, size_t l, size_t r) {
    size_t mid = (l + r) / 2;
    if (l < mid) sort_descending(array_, l, mid);
    if (mid + 1  < r) sort_descending(array_, mid + 1, r);
    Merge_(array_, l, mid + 1, r);

}


int main() {
    size_t num_of_stud;
    std::cin >> num_of_stud;
    std::vector<Student> stud_grades(num_of_stud);

    for (size_t i = 0; i < num_of_stud; ++i){

        std::string temp_str_name, temp_str_surname;
        std::cin >> temp_str_name >> temp_str_surname;

        stud_grades[i].name_surname = temp_str_name + " " + temp_str_surname;
        int inf, rus, math;
        std::cin >> inf >> math >> rus;
        stud_grades[i].sum = inf + rus + math;
    }

    sort_descending(stud_grades, 0, num_of_stud - 1);
    for (size_t i = 0; i < num_of_stud; ++i) {
        std::cout << stud_grades[i].name_surname << '\n';
    }

    return 0;
}
