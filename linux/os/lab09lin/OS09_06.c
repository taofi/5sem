#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");
    wchar_t c;
    int rowNumber = 0;
    FILE* in = fopen("./OS09_01.txt", "r, ccs=UTF-8");
    FILE* out;
    int num;
    bool yaBool = false;

    if (argc != 2) {
        wprintf(L"Please provide an integer parameter.\n");
        exit(1);
    }
    num = atoi(argv[1]);

    if (num % 2 != 0) {
        out = fopen("./OS09_06_1.txt", "w, ccs=UTF-8");
    }
    else {
        out = fopen("./OS09_06_2.txt", "w, ccs=UTF-8");
        yaBool = true;
    }

    while ((c = fgetwc(in)) != WEOF) {
        if (!yaBool)
            fputwc(c, out);
        if (c == L'\n')
            yaBool = !yaBool;
    }

    fclose(out);
    fclose(in);
    return 0;
}