/*
WARNING: THE BASE OF THIS KERNEL AND OS WAS "CHOACURYOS" BY PINECONIUM!

█░░░█ █▀▀ █░░ █▀▀ █▀▀█ █▀▄▀█ █▀▀ 　 ▀▀█▀▀ █▀▀█  
█▄█▄█ █▀▀ █░░ █░░ █░░█ █░▀░█ █▀▀ 　 ░░█░░ █░░█  
░▀░▀░ ▀▀▀ ▀▀▀ ▀▀▀ ▀▀▀▀ ▀░░░▀ ▀▀▀ 　 ░░▀░░ ▀▀▀▀ █ █ █

████████╗██╗░░██╗███████╗
╚══██╔══╝██║░░██║██╔════╝
░░░██║░░░███████║█████╗░░
░░░██║░░░██╔══██║██╔══╝░░
░░░██║░░░██║░░██║███████╗
░░░╚═╝░░░╚═╝░░╚═╝╚══════╝

███╗░░██╗░█████╗░░█████╗░██████╗░██╗░░░░░███████╗██████╗░██████╗░░█████╗░██╗███╗░░██╗░█████╗░░██████╗  
████╗░██║██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔════╝██╔══██╗██╔══██╗██╔══██╗██║████╗░██║██╔══██╗██╔════╝  
██╔██╗██║██║░░██║██║░░██║██║░░██║██║░░░░░█████╗░░██████╦╝██████╔╝███████║██║██╔██╗██║██║░░██║╚█████╗░  
██║╚████║██║░░██║██║░░██║██║░░██║██║░░░░░██╔══╝░░██╔══██╗██╔══██╗██╔══██║██║██║╚████║██║░░██║░╚═══██╗  
██║░╚███║╚█████╔╝╚█████╔╝██████╔╝███████╗███████╗██████╦╝██║░░██║██║░░██║██║██║░╚███║╚█████╔╝██████╔╝  
╚═╝░░╚══╝░╚════╝░░╚════╝░╚═════╝░╚══════╝╚══════╝╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝╚═╝╚═╝░░╚══╝░╚════╝░╚═════╝░  

██╗░░██╗███████╗██████╗░███╗░░██╗███████╗██╗░░░░░██╗
██║░██╔╝██╔════╝██╔══██╗████╗░██║██╔════╝██║░░░░░██║
█████═╝░█████╗░░██████╔╝██╔██╗██║█████╗░░██║░░░░░██║
██╔═██╗░██╔══╝░░██╔══██╗██║╚████║██╔══╝░░██║░░░░░╚═╝
██║░╚██╗███████╗██║░░██║██║░╚███║███████╗███████╗██╗
╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝╚═╝░░╚══╝╚══════╝╚══════╝╚═╝

*/
#include "../drivers/ports.h"
#include "../drivers/idt.h"
#include "../drivers/types.h"
#include "../kernel/kernel.h"


// Define necessary types 
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
volatile uint16_t* vidptr = (uint16_t*)0xB8000;

// VGA fefines
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define VGA_FRAME_BUFFER 0xA0000

// Code Page 437 stuff for my ascii doings
#define VERTICAL_LINE       '\xB3'
#define HORIZONTAL_LINE     '\xC4'
#define TOP_LEFT_CORNER     '\xDA'
#define TOP_RIGHT_CORNER    '\xBF'
#define BOTTOM_LEFT_CORNER  '\xC0'
#define BOTTOM_RIGHT_CORNER '\xD9'
#define T_JUNCTION          '\xC3'
#define L_JUNCTION          '\xB4'
#define R_JUNCTION          '\xC5'
#define B_JUNCTION          '\xC1'
#define CROSS               '\xC2'

// VGA colors
#define VGA_COLOR_BLACK        0
#define VGA_COLOR_BLUE 0x1
#define VGA_COLOR_GREEN        2
#define VGA_COLOR_CYAN         3
#define VGA_COLOR_RED          4
#define VGA_COLOR_MAGENTA      5
#define VGA_COLOR_BROWN        6
#define VGA_COLOR_LIGHT_GREY   7
#define VGA_COLOR_DARK_GREY    8
#define VGA_COLOR_LIGHT_BLUE   9
#define VGA_COLOR_LIGHT_GREEN  10
#define VGA_COLOR_LIGHT_CYAN   11
#define VGA_COLOR_LIGHT_RED    12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN  14
#define VGA_COLOR_WHITE        15


















// Function to clear the screen, also used to color the kernel
void k_clear_screen(unsigned char background_color) {

    char* video_memory = (char*)0xb8000;


    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = background_color; 
    }
}


void k_printf(const char* message, unsigned char color)
{

    char* video_memory = (char*)0xb8000;


    int position = 0;
    port_byte_out(0x3d4, 14);
    position = port_byte_in(0x3d5);
    position = position << 8;

    port_byte_out(0x3d4, 15);
    position += port_byte_in(0x3d5);

    int offset_from_vga = position * 2;


    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n') {

            offset_from_vga += 80 * 2 - (offset_from_vga % (80 * 2));
        } else {
            video_memory[offset_from_vga] = message[i];
            video_memory[offset_from_vga + 1] = color; 
            offset_from_vga += 2; 
        }
    }


    position = offset_from_vga / 2;
    port_byte_out(0x3d4, 14);
    port_byte_out(0x3d5, position >> 8);
    port_byte_out(0x3d4, 15);
    port_byte_out(0x3d5, position);
}



















// kernel part of the kernel
void k_main() 
{
	
    k_clear_screen(VGA_COLOR_LIGHT_BLUE); //For some reason this is the only way i could figure this stuff out.

    #define VGA_COLOR_YELLOW VGA_COLOR_LIGHT_BROWN

    // ASCII art
    k_printf("\n\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xDA\xBF\xC2\xC2\xC2\xDA\xBF\n", VGA_COLOR_YELLOW);
    k_printf("\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xC2\xb3\xb3\xb3\xC2\xC2\xb3\xb3\n", VGA_COLOR_YELLOW);
    k_printf("\xDA\xc4\xBF\xDA\xc4\xc4\xC4\xc4\xC4\xd9\xb3\xb3\xDA\xc4\xc4\xC2\xC0\xc4\xC4\xC4\xc4\xC4\xBF\n", VGA_COLOR_YELLOW);
    k_printf("\xB3\xDA\xBF\xC2\xDA\xBF\xb3\xDA\xBF\xb3\xDA\xBF\xb3\xb3\xb3\xb3\xC4\xC2\xDA\xBF\xb3\xDA\xC2\xDA\xBF\xC2\xC2\xDA\xBF\xBF\n", VGA_COLOR_YELLOW);
    k_printf("\xB3\xb3\xb3\xb3\xC0\xd9\xb3\xC0\xd9\xb3\xC0\xd9\xb3\xC0\xC2\xb3\xc4\xC2\xC0\xd9\xb3\xb3\xb3\xDA\xBF\xb3\xb3\xb3\xb3\xb3\n", VGA_COLOR_YELLOW);
    k_printf("\xC0\xd9\xC0\xC0\xc4\xc4\xC4\xc4\xc4\xC4\xc4\xc4\xC4\xc4\xC4\xc4\xc4\xC4\xc4\xc4\xC4\xd9\xC0\xd9\xC0\xC0\xC0\xd9\xC0\xd9\n", VGA_COLOR_YELLOW);
    // End of ASCII art

    k_printf("Welcome to the NoodleBrain Kernel: Where Spaghetti Code Meets\n  Alphabet Soup Logic!\n\n", VGA_COLOR_YELLOW);
    k_printf("NoodleBrain OS Version 404: Lost in the Sauce Edition\n\n", VGA_COLOR_YELLOW);  
    k_printf("Copyright (C)  2023 Silly Noodle Coherence Consortium.\n    All rights entangled, knotted, and simmering in a pot of absurdity.\n\n", VGA_COLOR_YELLOW);
    k_printf("Life is a noodle: sometimes twisted, often saucy,\n but always best enjoyed with a splash of absurdity and a pinch of silliness.\n\n", VGA_COLOR_YELLOW);

    #undef VGA_COLOR_YELLOW
}


/* thanks choacury guy for some of the code */

