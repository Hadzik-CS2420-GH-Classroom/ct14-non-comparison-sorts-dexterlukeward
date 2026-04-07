#include "NonComparisonSorts.h"
#include <iostream>
#include <algorithm>

// ---------------------------------------------------------------------------
// Counting Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #1 -- Steps 1a, 1b, 1c (Count Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #2 -- Step 2 (Placement Phase)
//
void counting_sort(std::vector<int>& data) {
    // TODO: Implement counting sort
    //   1. Find the min and max values
    //   2. Create a count array of size (max - min + 1)
    //   3. Count occurrences of each value
    //   4. Overwrite data with sorted values using the counts
    if (data.empty()) return;

    int minv = *std::min_element(data.begin(), data.end());
    int maxv = *std::max_element(data.begin(), data.end());
    int range = maxv - minv + 1;

    std::vector<int> counts(range, 0);
    for (int v : data) counts[v - minv]++;

    size_t idx = 0;
    for (int i = 0; i < range; ++i) {
        while (counts[i]-- > 0) {
            data[idx++] = i + minv;
        }
    }
}

// ---------------------------------------------------------------------------
// Bucket Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #3 -- Steps 1a, 1b (Distribution Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #4 -- Steps 2-3 (Sort + Concatenate)
//
void bucket_sort(std::vector<int>& data, int num_buckets) {
    // TODO: Implement bucket sort
    //   1. Find the min and max values to determine bucket ranges
    //   2. Create num_buckets empty buckets (vectors)
    //   3. Distribute each element into its bucket
    //   4. Sort each bucket (use insertion sort or std::sort)
    //   5. Concatenate all buckets back into data
    if (data.empty() || num_buckets <= 0) return;

    int minv = *std::min_element(data.begin(), data.end());
    int maxv = *std::max_element(data.begin(), data.end());
    int range = maxv - minv + 1;

    std::vector<std::vector<int>> buckets(static_cast<size_t>(num_buckets));

    for (int v : data) {
        // map value to bucket index in [0, num_buckets-1]
        int idx = static_cast<int>((static_cast<long long>(v - minv) * num_buckets) / range);
        if (idx < 0) idx = 0;
        if (idx >= num_buckets) idx = num_buckets - 1;
        buckets[idx].push_back(v);
    }

    data.clear();
    for (auto& b : buckets) {
        std::sort(b.begin(), b.end());
        data.insert(data.end(), b.begin(), b.end());
    }
}

// ---------------------------------------------------------------------------
// Radix Sort (LSD)
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #5 -- Step 0 + Pass 1 Detail
// ? SEE DIAGRAM: cpp_diagrams.md #6 -- Passes 2-3 (Stability in Action)
//
void radix_sort(std::vector<int>& data) {
    // TODO: Implement radix sort (LSD)
    //   1. Find the maximum value to determine the number of digits
    //   2. For each digit position (ones, tens, hundreds, ...):
    //      a. Use counting sort on that digit
    //   3. After all digit passes, data is sorted
    if (data.empty()) return;

    // Separate negatives and non-negatives
    std::vector<int> positives;
    std::vector<int> negatives_abs; // store absolute values of negatives
    positives.reserve(data.size());
    negatives_abs.reserve(data.size());

    for (int v : data) {
        if (v < 0) negatives_abs.push_back(-v);
        else positives.push_back(v);
    }

    auto lsd_radix = [](std::vector<int>& arr) {
        if (arr.empty()) return;
        int maxv = *std::max_element(arr.begin(), arr.end());
        for (int exp = 1; maxv / exp > 0; exp *= 10) {
            std::vector<int> output(arr.size());
            std::vector<int> count(10, 0);
            for (int v : arr) count[(v / exp) % 10]++;
            for (int i = 1; i < 10; ++i) count[i] += count[i - 1];
            for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
                int d = (arr[i] / exp) % 10;
                output[--count[d]] = arr[i];
            }
            arr.swap(output);
        }
    };

    lsd_radix(positives);
    lsd_radix(negatives_abs);

    // Reconstruct data: negatives (in reversed absolute order, negated) then positives
    std::vector<int> result;
    result.reserve(data.size());
    for (auto it = negatives_abs.rbegin(); it != negatives_abs.rend(); ++it) result.push_back(-*it);
    for (int v : positives) result.push_back(v);

    data.swap(result);
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------
void print_vector(const std::vector<int>& data, const std::string& label) {
    if (!label.empty()) std::cout << label << ": ";
    std::cout << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if (i + 1 < data.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
