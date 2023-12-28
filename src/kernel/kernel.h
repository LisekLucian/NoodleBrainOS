#ifndef KERNEL_H
#define KERNEL_H

// Define size_t
typedef unsigned int size_t;

void k_clear_screen(unsigned char background_color);
void k_set_color(unsigned char forecolor, unsigned char backcolor);
void k_printf(const char* message, unsigned char color);

void read_line(char* buffer, unsigned int buffer_size);

// Custom string functions
int k_strcmp(const char* str1, const char* str2);
int k_strncmp(const char* str1, const char* str2, size_t n);
size_t k_strlen(const char* str);

#endif /* KERNEL_H */
