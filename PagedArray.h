#ifndef DATOSII_PROYECTOI_PAGEDARRAY_H
#define DATOSII_PROYECTOI_PAGEDARRAY_H
#include <algorithm>
#include <fstream>
#include <string>

class PagedArray
{
private:
    struct Page
    {
        int number;
        int* data;
        bool changed;
    };

    std::fstream file;
    int pageSize; // enteros por página
    int pageCount; // cantidad de páginas en RAM

    Page* ram; // páginas cargadas en RAM
    int loadedPages;
    int nextReplace;

    long fileSize;

    long long pageFault;
    long long pageHit; // Por si llega a ser muy alto, para evitar overflow se utiliza long long

public:
    PagedArray(std::string filename, int pSize, int pCount)
    {
        pageSize = pSize;
        pageCount = pCount;
        loadedPages = 0;
        ram = new Page[pCount];
        for (int i = 0; i < pCount; i++)
        {
            ram[i].number = -1;
            ram[i].data = nullptr;
            ram[i].changed = false;
        }
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        nextReplace = 0;
        pageFault = 0;
        pageHit = 0;
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
    }

    int& operator[](int index)
    {
        int pageNum = index / pageSize;
        int offset = index % pageSize;

        // Buscar en RAM
        for (int i = 0; i < loadedPages; i++)
        {
            if (ram[i].number == pageNum)
            {
                pageHit++;
                ram[i].changed = true;
                return ram[i].data[offset];
            }
        }

        pageFault++;

        int slot;

        if (loadedPages < pageCount)
        {
            slot = loadedPages;
            loadedPages++;
        }
        else
        {
            slot = nextReplace;
            nextReplace = (nextReplace + 1) % pageCount;
        }
        // Escribe si cambió
        if (ram[slot].number != -1 && ram[slot].changed)
        {
            long oldPos = (long)ram[slot].number * pageSize * 4;

            long bytes = pageSize * 4;
            if (oldPos + bytes > fileSize) bytes = fileSize - oldPos;

            file.seekp(oldPos);
            file.write(reinterpret_cast<char*>(ram[slot].data), bytes);

            ram[slot].changed = false;
        }

        delete[] ram[slot].data;
        ram[slot].data = new int[pageSize];

        long pos = (long)pageNum * pageSize * 4;
        long bytes = pageSize * 4;
        if (pos + bytes > fileSize) bytes = fileSize - pos;

        std::fill(ram[slot].data, ram[slot].data + pageSize, 0);
        file.seekg(pos);
        file.read(reinterpret_cast<char*>(ram[slot].data), bytes);

        ram[slot].number = pageNum;
        ram[slot].changed = false;

        return ram[slot].data[offset];
    }

    long long faults()
    {
        return pageFault;
    }

    long long hits()
    {
        return pageHit;
    }

    ~PagedArray()
    {
        for (int i = 0; i < loadedPages; i++)
        {
            if (ram[i].changed)
            {
                long pos = (long)ram[i].number * pageSize * 4;
                long bytes = pageSize * 4;
                if (pos + bytes > fileSize) bytes = fileSize - pos;
                file.seekp(pos, std::ios::beg);
                file.write(reinterpret_cast<char*>(ram[i].data), bytes);
                ram[i].changed = false;
            }

            delete[] ram[i].data;
        }

        delete[] ram;
        file.close();
    }
};


#endif