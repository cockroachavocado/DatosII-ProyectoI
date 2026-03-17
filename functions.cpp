#include "PagedArray.h"
#include <fstream>
#include <iostream>
#include <string>

bool summary(const std::string& destiny, auto time, string algorithm, int pFaults, int pHits)
{
    std::ofstream archivo(destiny);

    if (!archivo.is_open()) return false;

    archivo << "El algoritmo duró " << time.count() << " segundos\n";
    archivo << "Se utilizó el algoritmo " << algorithm << '\n';
    archivo << "Hubo " << pFaults << " page faults, y hubo " << pHits << " page hits";

    archivo.close();
    return true;
}

void binarioATxt(const std::string& binPath, const std::string& txtPath) {

    std::ifstream bin(binPath, std::ios::binary);
    std::ofstream txt(txtPath);

    if (!bin || !txt) {
        std::cout << "Error abriendo archivos\n";
        return;
    }

    const size_t BUFFER_SIZE = 8192;
    int* buffer = new int[BUFFER_SIZE];

    bool primero = true;

    while (bin.read(reinterpret_cast<char*>(buffer), BUFFER_SIZE * sizeof(int))
           || bin.gcount() > 0) {

        size_t leidos = bin.gcount() / sizeof(int);

        for (size_t i = 0; i < leidos; i++) {
            if (!primero) txt << ",";
            txt << buffer[i];
            primero = false;
        }
           }

    delete[] buffer;
}

bool isValidInt(const std::string& s) {
    if (s.empty()) return false;

    int i = 0;
    if (s[0] == '-' || s[0] == '+') i = 1;

    for (; i < s.size(); i++) {
        if (!isdigit(s[i])) return false;
    }

    return true;
}

void selectionSort(PagedArray& v, int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[minIndex]) {
                minIndex = j;
            }
        }

        int temp = v[i];
        v[i] = v[minIndex];
        v[minIndex] = temp;
    }
}

void bubbleSort(PagedArray& v, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                int temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

void insertionSort(PagedArray& v, int n) {
    for (int i = 1; i < n; i++) {
        int key = v[i];
        int j = i - 1;

        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            j--;
        }

        v[j + 1] = key;
    }
}

int partition(PagedArray& v, int low, int high)
{
    int mid = (low + high) / 2;
    int pivot = v[mid];

    int temp = v[mid];
    v[mid] = v[high];
    v[high] = temp;

    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (v[j] < pivot)
        {
            i++;
            int temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }

    temp = v[i + 1];
    v[i + 1] = v[high];
    v[high] = temp;

    return i + 1;
}

void quickSort(PagedArray& v, int low, int high) {
    if (low < high) {
        int pi = partition(v, low, high);

        quickSort(v, low, pi - 1);
        quickSort(v, pi + 1, high);
    }
}

void shellSort(PagedArray& v, int size)
{
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            int temp = v[i];
            int j = i;

            while (j >= gap && v[j - gap] > temp) {
                v[j] = v[j - gap];
                j -= gap;
            }

            v[j] = temp;
        }
    }
}