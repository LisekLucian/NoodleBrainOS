// string.c

#include "../kernel/kernel.h"

int strcmp(const char* str1, const char* str2) {
    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }

    return (*str1 - *str2);
}

int strncmp(const char* str1, const char* str2, size_t n) {
    while (n > 0 && *str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
        n--;
    }

    return (n == 0 ? 0 : *str1 - *str2);
}
