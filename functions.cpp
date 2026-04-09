#include "PagedArray.h"
#include <fstream>
#include <iostream>

bool summary(const std::string& destiny, const long long time, const std::string& algorithm, const long long pFaults, const long long pHits)
{
    std::ofstream archivo(destiny);

    if (!archivo.is_open()) return false;

    archivo << "El algoritmo duró " << time << " milisegundos\n";
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

void insertionSort(PagedArray& v, int low, int high) {
    for (int i = low + 1; i <= high; i++)
    {
        int key = v[i];  // 1 acceso
        int j = i - 1;

        while (j >= low)
        {
            int val = v[j];  // cacheo
            if (val <= key) break;

            v[j + 1] = val;
            j--;
        }

        v[j + 1] = key;
    }
}

void quickSort(PagedArray &arr, int low, int high) {
    if (low < high) {
        int pivot = low;
        int pivotValue = arr[pivot];

        int i = low;
        int j = high;

        while (i < j) {
            while (i <= high && arr[i] <= pivotValue)
                i++;

            while (j >= low && arr[j] > pivotValue)
                j--;

            if (i < j) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        int temp = arr[j];
        arr[j] = arr[pivot];
        arr[pivot] = temp;

        quickSort(arr, low, j - 1);
        quickSort(arr, j + 1, high);
    }
}

void shellSort(PagedArray& v, int n)
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = v[i];
            int j = i;

            while (j >= gap)
            {
                int prev = v[j - gap];

                if (prev <= temp)
                    break;

                v[j] = prev;
                j -= gap;
            }

            v[j] = temp;
        }
    }
}

void count_sort_byte(PagedArray &arr, int n, int shift)
{
    int count[256] = {0};

    // Contar frecuencias
    for (int i = 0; i < n; i++) {
        int val = arr[i];
        int byte = (val >> shift) & 0xFF;
        count[byte]++;
    }

    // Prefix sum
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    int* output = new int[n];

    // Construcción (de atrás hacia adelante)
    for (int i = n - 1; i >= 0; i--) {
        int val = arr[i];
        int byte = (val >> shift) & 0xFF;
        output[--count[byte]] = val;
    }

    // Copiar de vuelta
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    delete[] output;
}

void radixSort(PagedArray &arr, int n)
{
    // 4 pasadas (32 bits / 8 bits)
    for (int shift = 0; shift < 32; shift += 8)
    {
        count_sort_byte(arr, n, shift);
    }
}