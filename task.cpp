#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

// Function to fill a vector with random integers
void randomVector(vector<int>& vec, size_t size) {
    for (size_t i = 0; i < size; i++) {
        vec[i] = rand() % 100;
    }
}

// Function to add corresponding elements of v1 and v2 in parallel
void addArrays(const vector<int>& v1, const vector<int>& v2, vector<int>& v3, size_t start, size_t end) {
    for (size_t i = start; i < end; i++) {
        v3[i] = v1[i] + v2[i];
    }
}

int main() {
    unsigned long size = 100000000;
    srand(time(0));

    // Create vectors for arrays v1, v2, and v3
    vector<int> v1(size);
    vector<int> v2(size);
    vector<int> v3(size);

    // Fill v1 and v2 with random integers
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now();

    // Define the number of threads to use
    size_t num_threads = thread::hardware_concurrency();
    vector<thread> threads;

    // Split the work among threads
    size_t chunk_size = size / num_threads;
    size_t start_index = 0;
    size_t end_index = 0;

    for (size_t i = 0; i < num_threads; i++) {
        end_index = start_index + chunk_size;
        if (i == num_threads - 1) {
            // Adjust the end index for the last thread to include any remaining elements
            end_index = size;
        }
        threads.push_back(thread(addArrays, ref(v1), ref(v2), ref(v3), start_index, end_index));
        start_index = end_index;
    }

    // Join the threads
    for (auto& t : threads) {
        t.join();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}
