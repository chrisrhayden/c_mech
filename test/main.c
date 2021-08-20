#include "./font/test_font.h"

int main() {
    bool success = false;

    success = test_font();

    if (success == true) {
        return 0;
    } else {
        return 1;
    }
}
