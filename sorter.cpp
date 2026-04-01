#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <chrono> // Para el tiempo
#include <filesystem> // Para copiar archivos

#include "PagedArray.h"
#include "functions.cpp"


int main(int argc, char* argv[])
{
    std::string input;
    std::string output;
    std::string alg;
    std::string pSize;
    std::string pCount;

    int pageSize = -1;
    int pageCount = -1;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-input" && i + 1 < argc) {
            input = argv[++i];
        }
        else if (arg == "-output" && i + 1 < argc) {
            output = argv[++i];
        }
        else if (arg == "-alg" && i + 1 < argc) {
            alg = argv[++i];
            if (alg != "QUICKSORT" && alg != "BUBBLESORT" && alg != "SELECTIONSORT" && alg != "INSERTIONSORT" && alg != "SHELLSORT")
            {
                std::cout << "Error, el algoritmo proporcionado no es valido";
                return 0;
            }
        }
        else if (arg == "-pageSize" && i + 1 < argc) {
            pSize = argv[++i];
            if (isValidInt(pSize)) pageSize = atoi(pSize.c_str());
            if (pageSize < 1)
            {
                std::cout << "Ingrese un entero no negativo para la cantidad de enteros por pagina";
                return 0;
            }
        }
        else if (arg == "-pageCount" && i + 1 < argc) {
            pCount = argv[++i];
            if (isValidInt(pCount)) pageCount = atoi(pCount.c_str());
            if (pageCount < 1)
            {
                std::cout << "Ingrese un entero no negativo para la cantidad de paginas";
                return 0;
            }
        }
    }

    if (!std::filesystem::exists(input))
    {
        std::cout << "Error, el archivo de input no existe\n";
        return 0;
    }

    std::filesystem::path origin = input;

    std::filesystem::path destiny =
        std::filesystem::path(output) /
        (origin.stem().string() + " - copy" + origin.extension().string());

    std::filesystem::copy_file(origin, destiny);

    size_t totalNumbers = std::filesystem::file_size(destiny) / sizeof(int); // Requerido para los algoritmos de ordenamiento

    if (alg == "QUICKSORT")
    {
        PagedArray pags(destiny.string(), pageSize, pageCount);
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(pags, 0, totalNumbers - 1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        long long time = duration.count();
        long long pageFaults = pags.faults();
        long long pageHits = pags.hits();
        summary(origin.stem().string() + " - summary.txt", time, "quick sort", pageFaults, pageHits);
    }
    else if (alg == "BUBBLESORT")
    {
        PagedArray pags(destiny.string(), pageSize, pageCount);
        auto start = std::chrono::high_resolution_clock::now();
        bubbleSort(pags, totalNumbers);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        long long time = duration.count();
        long long pageFaults = pags.faults();
        long long pageHits = pags.hits();
        summary(origin.stem().string() + " - summary.txt", time, "bubble sort", pageFaults, pageHits);
    }
    else if (alg == "SELECTIONSORT")
    {
        PagedArray pags(destiny.string(), pageSize, pageCount);
        auto start = std::chrono::high_resolution_clock::now();
        selectionSort(pags, totalNumbers);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        long long time = duration.count();
        long long pageFaults = pags.faults();
        long long pageHits = pags.hits();
        summary(origin.stem().string() + " - summary.txt", time, "selection sort", pageFaults, pageHits);
    }
    else if (alg == "INSERTIONSORT")
    {
        PagedArray pags(destiny.string(), pageSize, pageCount);
        auto start = std::chrono::high_resolution_clock::now();
        insertionSort(pags, totalNumbers);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        long long time = duration.count();
        long long pageFaults = pags.faults();
        long long pageHits = pags.hits();
        summary(origin.stem().string() + " - summary.txt", time, "insertion sort", pageFaults, pageHits);
    }
    else if (alg == "SHELLSORT")
    {
        PagedArray pags(destiny.string(), pageSize, pageCount);
        auto start = std::chrono::high_resolution_clock::now();
        shellSort(pags, totalNumbers);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        long long time = duration.count();
        long long pageFaults = pags.faults();
        long long pageHits = pags.hits();
        summary(origin.stem().string() + " - summary.txt", time, "shell sort", pageFaults, pageHits);
    }
    binarioATxt(destiny.string(), destiny.stem().string() + ".txt");
}