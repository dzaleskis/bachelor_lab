#pragma once

namespace utils {
    template<typename T>
    int merge_sort_with_inversions(std::vector<T> &data) {
        int n = size(data);
        int middle = n / 2;
        int inversions = 0;

        if (n <= 1) {
            return inversions;
        }

        std::vector<T> left(data.begin(), data.begin() + middle);
        std::vector<T> right(data.begin() + middle, data.end());

        inversions += merge_sort_with_inversions(left);
        inversions += merge_sort_with_inversions(right);
        inversions += merge(left, right, data);

        return inversions;
    }

    template<typename T>
    int merge(std::vector<T> &a, std::vector<T> &b, std::vector<T> &c) {
        int inversions = 0;

        int i = 0;
        int j = 0;
        int n = a.size();
        int m = b.size();

        for (int k = 0; k < c.size(); k++) {
            if (i < n) {
                if (j < m) {
                    if (a[i] <= b[j]) {
                        c[k] = a[i++];
                    } else {
                        c[k] = b[j++];
                        inversions += (n - i);
                    }
                } else {
                    c[k] = a[i++];
                }
            } else {
                c[k] = b[j++];
            }
        }

        return inversions;
    }
}