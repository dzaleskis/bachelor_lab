#pragma once

template <typename T>
struct Statistics {
    T median;
    T iqr;
    double outliers;
};

template <typename T>
Statistics<T> calculate_statistics(std::vector<T>& sample) {
    std::sort(sample.begin(), sample.end());

    auto sample_size = sample.size();
    auto quartile_size = sample_size / 4;

    auto min = sample.front();
    auto max = sample.back();
    auto median = sample[sample_size / 2];

    // quarter of the way through the sample
    auto q1 = sample[quartile_size];
    // three quarters of the way through the sample
    auto q3 = sample[sample_size - quartile_size];
    auto iqr = q3 - q1;

    auto lower_outlier_range = q1 - (iqr * 1.5);
    auto upper_outlier_range = q3 + (iqr * 1.5);

    auto outlier_count = std::accumulate(sample.begin(), sample.end(), 0, [&](int acc, auto c) {
        if (c < lower_outlier_range || c > upper_outlier_range) {
            return acc + 1;
        }

        return acc;
    });

    auto outliers = double(outlier_count) / sample_size;

    return { median, iqr, outliers };
}

template <typename T>
T calculate_median(std::vector<T>& sample) {
    std::sort(sample.begin(), sample.end());

    auto sample_size = sample.size();
    auto median = sample[sample_size / 2];

    return median;
}

template <typename T>
double calculate_average(std::vector<T>& sample) {
    std::sort(sample.begin(), sample.end());

    auto sample_size = sample.size();
    auto sum = std::accumulate(sample.begin(), sample.end(), 0);

    return (double)sum / sample_size;
}