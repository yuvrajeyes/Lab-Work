#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

bool containsHighPriority(int* high_priority, int k, int start, int end) {
    for (int i = 0; i < k; i++) {
        if (high_priority[i] >= start && high_priority[i] <= end) return 1;
    }
    return 0;
}

long helper(int start, int end, int* high_priority, int k, int normal_time, int priority_time) {
    int l = end - start + 1;
    if (l == 1) {
        if (containsHighPriority(high_priority, k, start, end)) return priority_time;
        else return normal_time;
    }

    bool hasHighPriority = containsHighPriority(high_priority, k, start, end);
    long timeIfNoSplit = hasHighPriority ? l * priority_time * k : normal_time;

    int mid = (start + end) / 2;
    long left = helper(start, mid, high_priority, k, normal_time, priority_time);
    long right = helper(mid + 1, end, high_priority, k, normal_time, priority_time);
    long timeIfSplit = left + right;

    return timeIfNoSplit < timeIfSplit ? timeIfNoSplit : timeIfSplit;
}

long getMinimumTime(int n, int k, int* high_priority, int normal_time, int priority_time) {
    return helper(0, n - 1, high_priority, k, normal_time, priority_time);
}

int main() {
    // Example input
    int n = 4;
    int high_priority[] = {1}; // indices are zero-based in C
    int k = 1;
    int normal_time = 2;
    int priority_time = 2;

    long result = getMinimumTime(n, k, high_priority, normal_time, priority_time);
    printf("Minimum time required: %ld\n", result);

    return 0;
}
