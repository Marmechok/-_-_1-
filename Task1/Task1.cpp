#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <algorithm> 
#include <future>    

using std::cin;
using std::cout;
using std::vector;

vector<int> generateArray(int n, int minV, int maxV) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(minV, maxV);

    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = dist(gen);
    return a;
}

void printArray(const vector<int>& a, int firstCount = 30) {
    int limit = (a.size() < (size_t)firstCount) ? (int)a.size() : firstCount;
    for (int i = 0; i < limit; i++) cout << a[i] << " ";
    if ((int)a.size() > firstCount) cout << "...";
    cout << "\n";
}

void insertionSort(vector<int>& a) {
    for (int i = 1; i < (int)a.size(); i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void quickSortRec(vector<int>& a, int l, int r) {
    int i = l, j = r;
    int pivot = a[l + (r - l) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            std::swap(a[i], a[j]);
            i++; j--;
        }
    }
    if (l < j) quickSortRec(a, l, j);
    if (i < r) quickSortRec(a, i, r);
}

void quickSort(vector<int>& a) {
    if (!a.empty()) quickSortRec(a, 0, (int)a.size() - 1);
}

int binarySearch(const vector<int>& a, int key) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == key) return m;
        if (a[m] < key) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

long long measureSortUs(void (*sortFunc)(vector<int>&), const vector<int>& base) {
    vector<int> tmp = base;
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(tmp);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

long long measureTwoSortsSyncUs(const vector<int>& base) {
    auto a1 = base;
    auto a2 = base;

    auto start = std::chrono::high_resolution_clock::now();
    insertionSort(a1);
    quickSort(a2);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

long long measureTwoSortsAsyncUs(const vector<int>& base) {
    auto start = std::chrono::high_resolution_clock::now();

    auto f1 = std::async(std::launch::async, [&base]() {
        auto a1 = base;
        insertionSort(a1);
        });

    auto f2 = std::async(std::launch::async, [&base]() {
        auto a2 = base;
        quickSort(a2);
        });

    f1.get();
    f2.get();

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    vector<int> ns = { 1000, 2000, 5000, 10000, 20000 };

    std::ofstream fout("results.csv");
    fout << "n,Insertion_us,Quick_us,SyncTotal_us,AsyncTotal_us\n";

    cout << "Benchmark (time in microseconds)\n";
    cout << "n\tInsertion\tQuick\tSyncTotal\tAsyncTotal\n";

    for (int size : ns) {
        auto base = generateArray(size, -100000, 100000);

        long long t1 = measureSortUs(insertionSort, base);
        long long t2 = measureSortUs(quickSort, base);
        long long syncTotal = measureTwoSortsSyncUs(base);
        long long asyncTotal = measureTwoSortsAsyncUs(base);

        cout << size << "\t" << t1 << "\t\t" << t2
            << "\t" << syncTotal << "\t\t" << asyncTotal << "\n";

        fout << size << "," << t1 << "," << t2 << "," << syncTotal << "," << asyncTotal << "\n";
    }

    fout.close();
    cout << "\nSaved to results.csv\n\n";

    int n;
    cout << "n = ";
    cin >> n;

    auto a = generateArray(n, -50, 50);

    cout << "Original: ";
    printArray(a);

  
    insertionSort(a);

    cout << "Sorted:   ";
    printArray(a);

    int key;
    cout << "Enter key to find: ";
    cin >> key;

    int idx = binarySearch(a, key);
    if (idx == -1) cout << "Not found\n";
    else cout << "Found at index: " << idx << "\n";

    return 0;
}