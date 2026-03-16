#include <iostream>
#include <string>
#include <fstream>
#include <random>

int main(int argc, char* argv[]) {
    std::string size;
    std::string outputPath;

    int enteros = 0;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-size" && i + 1 < argc) {
            size = argv[++i];
            if (size == "SMALL") enteros = 67108864;
            else if (size == "MEDIUM") enteros = 134217728;
            else if (size == "LARGE") enteros = 268435456;
            else
            {
                std::cout << "Error, tamaño no válido";
                return 0;
            }
        }
        else if (arg == "-output" && i + 1 < argc) {
            outputPath = argv[++i];
        }
    }

    std::ofstream file(outputPath, std::ios::binary);

    if (size.empty()) {
        std::cout << "Error, debe indicar -size\n";
        return 0;
    }

    if (outputPath.empty()) {
        std::cout << "Error, debe indicar -output\n";
        return 0;
    }

    if (!file) {
        std::cout << "Error, dirección no válida\n";
        return 0;
    }

    const int num = 1000000;
    int* buffer = new int[num]; // Utilizo un buffer para la eficiencia

    std::mt19937 rng(std::random_device{}()); // Función random
    std::uniform_int_distribution<int> dist(0, 2147483647); // Puede albergar todos los números desde 0 hasta el máximo de int

	const int block = enteros / num;
	const int remainder = enteros % num;

    for (int n = 0; n < block; n++) {
        for (int i = 0; i < num; i++) {
            buffer[i] = dist(rng);
        }
        file.write(reinterpret_cast<char*>(buffer), num * sizeof(int));
    }

	if (remainder > 0) { // Esto es porque la división no es exacta
        for (int i = 0; i < remainder; i++) {
            buffer[i] = dist(rng);
        }
	    file.write(reinterpret_cast<char*>(buffer), remainder * sizeof(int));
	}

    delete[] buffer;

    file.close();

    return 0;
}