#include <iostream>
#include <vector>

long orderStat(std::vector<long> &arr, int arrLen, int k);

int partition(std::vector<long> &arr, int left, int right);

int main(int argc, char* argv[]) {
    int n = 0, k = 0, A = 0, B = 0, C = 0, a1 = 0, a2 = 0;
    std::vector<long> a;
    std::cin >> n >> k;
    std::cin >> A >> B >> C >> a1 >> a2;
    a.push_back(a1);
    a.push_back(a2);
    for (int i = 2; i < n; ++i) {
        a.push_back(A * a[i - 2] + B * a[i - 1] + C);
    }
    std::cout << orderStat(a, n, k - 1);
}

long orderStat(std::vector<long> &arr, int arrLen, int k) {
    int left = 0, right = arrLen - 1, mid = 0;
    while (mid != k) {
        mid = partition(arr, left, right);
        if (mid > k) {
            right = mid;
        }
        if (mid < k) {
            left = mid + 1;
        }
    }
    return arr[mid];
}

int partition(std::vector<long> &arr, int left, int right) {
    long pivot = arr[left];
    int count = 0;
    for (int i = left + 1; i < right + 1; i++) {
        if (arr[i] <= pivot)
            count++;
    }
    int iPivot = left + count;
    std::swap(arr[iPivot], arr[left]);
    int iLeft = left, iRight = right;
 
    while (iLeft < iPivot && iRight > iPivot) {
        while (arr[iLeft] <= pivot) {
            iLeft++;
        }
        while (arr[iRight] > pivot) {
            iRight--;
        }
        if (iLeft < iPivot && iRight > iPivot) {
            std::swap(arr[iLeft++], arr[iRight--]);
        }
    }
    return iPivot;
}