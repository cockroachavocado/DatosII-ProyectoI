#ifndef DATOSII_PROYECTOI_PAGEDARRAY_H
#define DATOSII_PROYECTOI_PAGEDARRAY_H
#include <fstream>
#include <string>

class PagedArray
{
private:
    struct Page
    {
        int number;
        int* data;
    };

    std::fstream file;
    int pageSize; // enteros por página
    int pageCount; // páginas en la RAM

    Page* ram;
    int loadedPages;
    int lastChecked;
    int nextReplace;

    int pageFault;
    int pageHit;

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
        }
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        lastChecked = -1;
        nextReplace = 0;
        pageFault = 0;
        pageHit = 0;
    }

    int& operator[](int index)
    {
        int pageNum = index / pageSize;
        int offset = index % pageSize;

        // Cache rápida
        if (lastChecked != -1 && ram[lastChecked].number == pageNum)
        {
            return ram[lastChecked].data[offset];
        }

        // Buscar página en RAM
        for (int i = 0; i < loadedPages; i++)
        {
            if (ram[i].number == pageNum)
            {
                pageHit++;
                lastChecked = i;
                return ram[i].data[offset];
            }
        }

        pageFault++;
        // Si aún hay espacio en RAM
        if (loadedPages < pageCount)
        {
            ram[loadedPages].number = pageNum;
            ram[loadedPages].data = new int[pageSize];

            long pos = (long)pageNum * pageSize * sizeof(int);

            file.seekg(pos, std::ios::beg);
            file.read(reinterpret_cast<char*>(ram[loadedPages].data),
                      pageSize * sizeof(int));

            lastChecked = loadedPages;
            loadedPages++;

            return ram[lastChecked].data[offset];
        }

        // Reemplazo FIFO
        int victim = nextReplace;

        // Guardar página vieja
        long oldPos = (long)ram[victim].number * pageSize * sizeof(int);

        file.seekp(oldPos, std::ios::beg);
        file.write(reinterpret_cast<char*>(ram[victim].data),
                   pageSize * sizeof(int));

        delete[] ram[victim].data;

        // Cargar nueva página
        ram[victim].number = pageNum;
        ram[victim].data = new int[pageSize];

        long pos = (long)pageNum * pageSize * sizeof(int);

        file.seekg(pos, std::ios::beg);
        file.read(reinterpret_cast<char*>(ram[victim].data),
                  pageSize * sizeof(int));

        lastChecked = victim;

        nextReplace = (nextReplace + 1) % pageCount;

        return ram[lastChecked].data[offset];
    }

    int faults()
    {
        return pageFault;
    }

    int hits()
    {
        return pageHit;
    }

    ~PagedArray()
    {
        for (int i = 0; i < loadedPages; i++)
        {
            long pos = (long)ram[i].number * pageSize * sizeof(int);

            file.seekp(pos, std::ios::beg);
            file.write(reinterpret_cast<char*>(ram[i].data),
                       pageSize * sizeof(int));

            delete[] ram[i].data;
        }

        delete[] ram;
        file.close();
    }
};


#endif