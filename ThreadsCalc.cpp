#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <ctime>
#include <chrono>

class ThreadSumCalculator {
public:
    ThreadSumCalculator(const std::vector<int>& arr, size_t start, size_t end, size_t threadNumber)
        : arr(arr), start(start), end(end), sum(0), threadNumber(threadNumber) {}

    void operator()() {
        sum = calculateSum();
    }

    int getSum() const {
        return sum;
    }

    std::string getThreadResultString() const {
        std::string resultString = "Thread " + std::to_string(threadNumber) + ": ";
        for (size_t i = start; i < end; ++i) {
            resultString += std::to_string(arr[i]);
            if (i != end - 1) {
                resultString += "+";
            }
        }
        resultString += " = " + std::to_string(sum);
        return resultString;
    }

private:
    const std::vector<int>& arr;
    size_t start, end;
    int sum;
    size_t threadNumber;

    int calculateSum() {
        int localSum = 0;
        for (size_t i = start; i < end; ++i) {
            localSum += arr[i];
        }
        return localSum;
    }
};

int main() {
    size_t N, M;
    std::cout << "Enter the size of the array: ";
    std::cin >> N;
    std::cout << "Enter the number of parts: ";
    std::cin >> M;

    std::vector<int> randomArray;
    randomArray.reserve(N);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (size_t i = 0; i < N; ++i) {
        randomArray.push_back(std::rand() % 10 + 1);
    }

    std::vector<std::thread> threads;
    std::vector<ThreadSumCalculator> threadCalculators;
    threadCalculators.reserve(M);

    size_t chunkSize = N / M;
    size_t startIndex = 0;
    for (size_t i = 0; i < M; ++i) {
        size_t endIndex = (i == M - 1) ? N : startIndex + chunkSize;
        threadCalculators.emplace_back(randomArray, startIndex, endIndex, i);
        threads.emplace_back(std::ref(threadCalculators.back()));
        startIndex = endIndex;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    for (std::thread& t : threads) {
        t.join();
    }

    int totalSum = 0;
    for (const ThreadSumCalculator& calculator : threadCalculators) {
        totalSum += calculator.getSum();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    for (const ThreadSumCalculator& calculator : threadCalculators) {
        std::cout << calculator.getThreadResultString() << std::endl;
    }

    std::cout << "Total sum using threads: ";
    for (size_t i = 0; i < threadCalculators.size(); ++i) {
        std::cout << threadCalculators[i].getSum();
        if (i != threadCalculators.size() - 1) {
            std::cout << "+";
        }
    }
    std::cout << " = " << totalSum << std::endl;
    std::cout << "Time taken with threads: " << duration.count() << " microseconds" << std::endl;

    return 0;
}

