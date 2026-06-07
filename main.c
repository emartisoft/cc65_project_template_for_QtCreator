#include <c64.h>
#include <conio.h>

int main(void) {
    clrscr();
    bordercolor(COLOR_BLUE);
    bgcolor(COLOR_BLUE);
    textcolor(COLOR_WHITE);
    cputsxy(0, 0, "Hello, C64!");
    return 0;
}
