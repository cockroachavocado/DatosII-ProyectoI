#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {

    std::string size;
    std::string outputPath;

	srand(time(0));

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-size" && i + 1 < argc) {
            size = argv[++i];
            if (size != "SMALL" && size != "MEDIUM" && size != "LARGE") {
                std::cout << "Ingrese como argumento de size SMALL, MEDIUM o LARGE";
                return 0;
            }
        }
        else if (arg == "-output" && i + 1 < argc) {
            outputPath = argv[++i];
        }
    }

    std::ofstream file(outputPath, std::ios::binary);

    if (!file) {
        std::cerr << "Error, dirección no válida\n";
        return 1;
    }

	int enteros;

    if (size == "SMALL") enteros = 134217728;
    else if (size == "MEDIUM") enteros = 268435456;
    else enteros = 536870912;

    const int num = 1000000;
    int buffer[num];
	const int bloques = enteros / num;
	const int resto = enteros % num;

    for (int n = 0; n < bloques; n++) {
        for (int i = 0; i < 74982; i++) {
            buffer[i] = rand() % 100;
        }
        file.write(reinterpret_cast<char*>(buffer), sizeof(buffer));
    }

	if (resto > 0) {
        for (int i = 0; i < resto; i++) {
            buffer[i] = rand() % 100;
        }
        file.write((char*)buffer, resto * sizeof(int));
    }

    file.close();

    return 0;
}