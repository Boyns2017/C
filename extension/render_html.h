#include <stdio.h>
#include <stdlib.h>
#include "neillsdl2.h"

#define ON 1
#define OFF 0
#define LENGTH 40
#define SHORT_CONSTANT 10
#define MEDIUM_CONSTANT 30
#define LONG_CONSTANT 90
#define HEIGHT 25
#define FILE_NUMBER 2
#define UPPER_HALF 1
#define LOWER_HALF 2
#define SINGLE_HEIGHT 0x8c
#define DOUBLE_HEIGHT 0x8d
#define NEW_BACKGROUND 0x9d
#define SEPERATED_GRAPHICS 0x9A
#define CONTIGUOUS_GRAPHICS 0x99
#define CONTIGUOUS_GRAPHICS_CONSTANT 4
#define SEPERATED_GRAPHICS 0x9A
#define HOLD_GRAPHICS 0x9E
#define RELEASE_GRAPHICS 0x9F
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)
#define SIXEL_TOP_LEFT 1
#define SIXEL_TOP_RIGHT 2
#define SIXEL_MIDDLE_LEFT 4
#define SIXEL_MIDDLE_RIGHT 8
#define SIXEL_BOTTOM_LEFT 16
#define SIXEL_BOTTOM_RIGHT 64
#define SIXEL_CONVERSION 160
#define BLAST_THROUGH_MIN 0xC0
#define BLAST_THROUGH_MAX 0xDF
#define GRAPHICS_TO_COLOUR 16
#define CONVERT_ASCII 128
#define NON_COMMAND_CODES 0xa0
#define UNUSED 0xFF

struct sixels{
      int block_1;
      int block_2;
      int block_3;
      int block_4;
      int block_5;
      int block_6;
};

struct hexcode{
   unsigned char hex_code;
   unsigned char foreground_colour;
   unsigned char back_colour;
   unsigned char graphics;
   unsigned char height;
   unsigned char held_colours;
   int double_height; /* 0 is off 1 is top 2 is bottom */
   int is_ctr_code;
   int background;
   int contiguous_graphics;
   int seperated;
   int held;
   int held_colour;
   struct sixels sixel;
};
typedef struct hexcode Code;

typedef enum ColourCode {
   Red = 0x81,
   Green = 0x82,
   Yellow = 0x83,
   Blue = 0x84,
   Magenta = 0x85,
   Cyan = 0x86,
   White= 0x87,
   Black = 0x9c
} colourCode;

typedef enum GraphicsCode {
   Graphics_Red = 0x91,
   Graphics_Green = 0x92,
   Graphics_Yellow = 0x93,
   Graphics_Blue = 0x94,
   Graphics_Magenta = 0x95,
   Graphics_Cyan = 0x96,
   Graphics_White= 0x97,
   Graphics_Black = 0x9c
} Graphics_Code;

int top_sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file, int *sixel_dividend);

int middle_sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file, int *sixel_dividend);

int bottom_sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file, int *sixel_dividend);

int previous_seperated_graphics(Code teletext[HEIGHT][LENGTH], int i, int j);

int previous_foreground(Code teletext[HEIGHT][LENGTH], int i, int j);

int previous_background(Code teletext[HEIGHT][LENGTH], int i, int j);

int previous_held_graphics(Code teletext[HEIGHT][LENGTH], int i, int j);

int previous_height(Code teletext[HEIGHT][LENGTH], int i, int j);

int previous_graphics(Code teletext[HEIGHT][LENGTH], int i, int j);

int rgb_value(unsigned char colour, char colour_string[MEDIUM_CONSTANT]);

int sixel_build(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i);

int create_html_file(Code teletext[HEIGHT][LENGTH]);

int initiliase_the_file(FILE * file);

int size(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i);

int meaning_code(Code teletext[HEIGHT][LENGTH], FILE *file, int j, int i);

int empty_space(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i);

int read_file(char* file, Code teletext[HEIGHT][LENGTH]);

int table_generation(Code teletext[HEIGHT][LENGTH], FILE *file);

int characters(Code teletext[HEIGHT][LENGTH], FILE *file, int j, int i);

int colours(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i);

int background_render(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i);

int sort_code(Code teletext[HEIGHT][LENGTH]);

int foreground(Code teletext[HEIGHT][LENGTH], int i, int j);

int background(Code teletext[HEIGHT][LENGTH], int i, int j);

int graphics(Code teletext[HEIGHT][LENGTH], int i, int j);

int sixel_subtraction(Code teletext[HEIGHT][LENGTH], int i, int j,  FILE *fp);

int assign_upper_lower_tag(Code teletext[HEIGHT][LENGTH], int i, int j);

int check_for_height_code(Code teletext[HEIGHT][LENGTH], int i, int j);

int default_settings(Code teletext[HEIGHT][LENGTH], int j);

int sixel_switch(Code teletext[HEIGHT][LENGTH], int i, int j, int temp_sixel, FILE *fp);

int seperated_graphics(Code teletext[HEIGHT][LENGTH], int i, int j);

int held_graphics(Code teletext[HEIGHT][LENGTH], int i, int j);

int reset_graph(Code teletext[HEIGHT][LENGTH], int i, int j);

int sixel_dividend_value(Code teletext[HEIGHT][LENGTH], int i, int j, int *sixel_dividend, int temp_sixel);

