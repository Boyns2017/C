#include "render.h"
#define RECTSIZE 20

int main(int argc, char *argv[])
{
   SDL_Simplewin sw;
   fntrow font[FNTCHARS][FNTHEIGHT];
   Code teletext_cells[HEIGHT][LENGTH];
   if(argc!= FILE_NUMBER)
   {
      ON_ERROR("Wrong number of files uplaoded.\n");
      return 0;
   }
   read_file(argv[1], teletext_cells);
   Neill_SDL_ReadFont(font, "m7fixed.fnt");
   Neill_SDL_Init(&sw);
   SDL_render(&sw, teletext_cells, font);
   do{
      SDL_Delay(MILLISECONDDELAY);
      Neill_SDL_UpdateScreen(&sw);
      Neill_SDL_Events(&sw);
   }while(!sw.finished);
   atexit(SDL_Quit);
   return 0;
}

/* Function: reads in the file. */
int read_file(char* file, Code teletext[HEIGHT][LENGTH])
{
   int i, j , test = 0;
   FILE *fp;
   
   fp = fopen(file, "rb");
   if(fopen(file, "rb")==NULL)
   {
      ON_ERROR("File not located\n");
   }
   for(i=0; i<HEIGHT; i++)
   {
      for(j=0; j<LENGTH; j++)
      {
        test = fread(&(teletext[i][j].hex_code), sizeof(unsigned char), 1, fp);
        if(test != 1)
        {
           ON_ERROR("Error. Wrong number of characters read.\n");
        }
      }
   }
   sort_code(teletext);
   return 1;
}

/* Function: sorts through hex codes and assigns tags. */
int sort_code(Code teletext[HEIGHT][LENGTH])
{
   int i = 0, j = 0;

   for(i=0; i<HEIGHT; i++)
   {
      default_settings(teletext, i);
      for(j=0; j<LENGTH; j++)
      {
          foreground(teletext, i, j);
          background(teletext,i, j);
          held_graphics(teletext, i, j);
          check_for_height_code(teletext, i, j);
          assign_upper_lower_tag(teletext, i, j);
          graphics(teletext,i, j);
          seperated_graphics(teletext, i, j);
          is_control_code(teletext,i, j);
      }
   }
   return 1;
}

/*Function: Sets the default settings */
int default_settings(Code teletext[HEIGHT][LENGTH], int x)
{
   int i = 0;
   while(i!=LENGTH)
   {
      teletext[x][i].foreground_colour = White;
      teletext[x][i].background = Black;
      teletext[x][i].back_colour = Black;
      teletext[x][i].double_height = OFF;
      teletext[x][i].graphics = OFF;
      teletext[x][i].height = OFF;
      teletext[x][i].contiguous_graphics = OFF;
      teletext[x][i].seperated = OFF;
      teletext[x][i].held = OFF;
      i++;
   }
   return 1;
}

/* Function: selects and assigns appropriate foreground colours */
int foreground(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code >= Red && teletext[i][j].hex_code <= White)
   {
      teletext[i][j].foreground_colour = teletext[i][j].hex_code;
   }
   else
   {
       previous_foreground(teletext, i, j);
   }
   return 1;
}
/* Function: upadtes*/
int previous_foreground(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      teletext[i][j].foreground_colour = teletext[i][j-1].foreground_colour;
   }
   return 1;
}

/* Function: selects and assigns appropriate background colours */
int background(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code == NEW_BACKGROUND)
   {
      teletext[i][j].background = NEW_BACKGROUND;
      teletext[i][j].back_colour = teletext[i][j-1].foreground_colour;
   }
   else
   {
      previous_background(teletext, i, j);
   }
   return 1;
}

/* Function: updates*/
int previous_background(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      if(teletext[i][j].hex_code == Black)
      {
         teletext[i][j].back_colour = Black;
      }
      else
      {
         teletext[i][j].background = teletext[i][j-1].background;
         teletext[i][j].back_colour = teletext[i][j-1].back_colour;
      }
   }
   return 1;
}

/* Function: Checks for held graphics. */
int held_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code==HOLD_GRAPHICS)
   {
      teletext[i][j].held = ON;
      teletext[i][j-1].seperated = OFF;
      teletext[i][j].held_colours = teletext[i][j-1].hex_code;
   }
   previous_held_graphics(teletext, i, j);
   return 1;
}

/* Function: upadtes*/
int previous_held_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j-1].held==ON)
   {
      if(j!=0)
      {
         if(teletext[i][j].hex_code == RELEASE_GRAPHICS)
         {
            teletext[i][j].held_colours = OFF;
            return 1;
         }
         teletext[i][j].held_colours = teletext[i][j-1].hex_code;
         teletext[i][j].held = teletext[i][j-1].held;
         teletext[i][j].seperated =  teletext[i][j-1].seperated;
      }
   }
   return 1;
}

/* Function: checks line height */
int check_for_height_code(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code == SINGLE_HEIGHT)
   {
      teletext[i][j].height = OFF;
      teletext[i][j].double_height = OFF;
   }
   else if(teletext[i][j].hex_code == DOUBLE_HEIGHT)
   {
      teletext[i][j].height = ON;
   }
   else
   {
      previous_height(teletext, i, j);
   }
   return 1;
}

/* Function: upadtes*/
int previous_height(Code teletext[HEIGHT][LENGTH], int i, int j)
{
      if(j!=0)
      {
         teletext[i][j].height = teletext[i][j-1].height;
      }
      return 1;
}

/* Function: assigns line half */
int assign_upper_lower_tag(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].height== ON)
   {
      if(teletext[i][j].height == ON && teletext[i-1][j].height == ON)
      {
         teletext[i][j].double_height = LOWER_HALF;
      }
      else
      {
         teletext[i][j].double_height = UPPER_HALF;
      }
   }
   return 1;
}

/* Function: assigns graphic colour. */
int graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code >= Graphics_Red && teletext[i][j].hex_code <= CONTIGUOUS_GRAPHICS)
   {
      teletext[i][j].contiguous_graphics = ON;
      teletext[i][j].graphics = teletext[i][j].hex_code;
   }
   else
   {
       previous_graphics(teletext, i, j);
   }
   return 1;
}

/* Function: upadtes*/
int previous_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      if(teletext[i][j-1].hex_code >= Red && teletext[i][j-1].hex_code <= White )
      {
         teletext[i][j].graphics  = OFF;
      }
      else
      {
         teletext[i][j].graphics = teletext[i][j-1].graphics;
         teletext[i][j].contiguous_graphics = teletext[i][j-1].contiguous_graphics;
      }
   }
   return 1;
}

/* Function: Checks for seperated graphics. COME BACK TO THIS ONE*/
int seperated_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code == SEPERATED_GRAPHICS)
   {
      teletext[i][j].seperated = ON;
      teletext[i][j].contiguous_graphics = ON;
      teletext[i][j].graphics = Graphics_White;
   }
   else
   {
      previous_seperated_graphics(teletext, i, j);
   }
   return 1;
}

/* Function: upadtes*/
int previous_seperated_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      teletext[i][j].seperated = teletext[i][j-1].seperated;
   }
   return 1;
}

/* Function: checks whether the hex code is a command code */
int is_control_code(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code < NON_COMMAND_CODES)
   {
      teletext[i][j].is_ctr_code = ON;
   }
   else
   {
      teletext[i][j].is_ctr_code = OFF;
   }
   return 1;
}

/* Function: executes implementation */
int SDL_render(SDL_Simplewin *sw, Code teletext[HEIGHT][LENGTH], fntrow font[FNTCHARS][FNTHEIGHT])
{
   int i, j;

   for(i = 0; i<HEIGHT; i++)
   {
      for(j = 0; j <LENGTH; j++)
      {
         if(teletext[i][j].hex_code<BLAST_THROUGH_MIN || teletext[i][j].hex_code>BLAST_THROUGH_MAX || teletext[i][j].held==ON)
         {
            sixel_subtraction(teletext, i, j);
            decode_sixels(sw, teletext, i, j);
         }
         my_SDL_DrawChar(sw, font, teletext, i, j, j*SCALE_X ,i*SCALE_Y);
      }
   }
   return 1;
}

/* Function: decodes the temp_sixel value */
int decode_sixels(SDL_Simplewin *sw, Code teletext[HEIGHT][LENGTH], int i, int j)
{
   back_space_render(sw, teletext, i,  j);

   if(teletext[i][j].sixel.block_6 == ON)
   {
      fill_sixel(teletext, i, j, sw, j*SCALE_X+SIXEL_WIDTH, i*SCALE_Y+(STRETCH_CONSTANT*SIXEL_HEIGHT));
   }
   if(teletext[i][j].sixel.block_5 == ON)
   {
      fill_sixel(teletext, i, j, sw, j*SCALE_X, i*SCALE_Y+(STRETCH_CONSTANT*SIXEL_HEIGHT));
   }
   if(teletext[i][j].sixel.block_4 == ON)
   {
     fill_sixel(teletext, i, j, sw,  j*SCALE_X+SIXEL_WIDTH, i*SCALE_Y+SIXEL_HEIGHT);
   }
   if(teletext[i][j].sixel.block_3 == ON)
   {
      fill_sixel(teletext, i, j, sw, j*SCALE_X ,i*SCALE_Y+SIXEL_HEIGHT);
   }
   if(teletext[i][j].sixel.block_2 == ON)
   {
      fill_sixel(teletext, i, j, sw, j*SCALE_X+SIXEL_WIDTH, i*SCALE_Y);
   }
   if(teletext[i][j].sixel.block_1 == ON)
   {
      fill_sixel(teletext, i, j, sw, j*SCALE_X, i*SCALE_Y);
   }
   return 1;
}

/* Function: fills background in.*/
int back_space_render(SDL_Simplewin *sw, Code teletext[HEIGHT][LENGTH], int i, int j)
{
   int h, b;
   if(teletext[i][j].back_colour == White)
   {
      for(h = 0; (int)h < FNTHEIGHT; h++)
      {
         for(b = 0; b < (int)FNTWIDTH; b++)
         {
             colour_change(sw, teletext[i][j].back_colour);
             SDL_RenderDrawPoint(sw->renderer, b+j*SCALE_X,  h+i*SCALE_Y);
         }
      }
   }
   return 1;
}

/* Function: fills the sixels */
int fill_sixel(Code teletext[HEIGHT][LENGTH], int i, int j, SDL_Simplewin *sw, int x, int y)
{
   SDL_Rect pixel;
   int height = SIXEL_HEIGHT, width = SIXEL_WIDTH;
   if(teletext[i][j].contiguous_graphics == ON || teletext[i][j].held == ON)
   {
      height = SIXEL_HEIGHT;
      width = SIXEL_WIDTH+CONTIGUOUS_GRAPHICS_CONSTANT;
   }
   if(teletext[i][j].seperated == ON)
   {
      height = SIXEL_HEIGHT-STRETCH_CONSTANT;
      width = SIXEL_WIDTH-STRETCH_CONSTANT;
   }
   pixel.x = x;
   pixel.y = y;
   pixel.w = width;
   pixel.h = height;
   colour_change(sw, teletext[i][j].graphics);
   if(SDL_RenderFillRect(sw->renderer, &pixel)!=0)
   {
      fprintf(stderr, "Unable to fill sixel. %s.", SDL_GetError());
      SDL_Quit();
      exit(1);
   }
   return 1;
}

/* Function: colour changes.*/
int colour_change(SDL_Simplewin *sw, unsigned char colour)
{
   if(colour>= Graphics_Red && colour<= Graphics_Black)
   {
      colour = colour-GRAPHICS_TO_COLOUR;
   }
   if(colour!=0)
   {
      switch(colour)
      {
         case Red   : Neill_SDL_SetDrawColour(sw, COLOUR_MAX, 0, 0);
                      break;

         case Green : Neill_SDL_SetDrawColour(sw, 0, COLOUR_MAX, 0);
                      break;

         case Yellow: Neill_SDL_SetDrawColour(sw, COLOUR_MAX, COLOUR_MAX, 0);
                      break;

         case Blue  : Neill_SDL_SetDrawColour(sw, 0, 0, COLOUR_MAX);
                      break;

         case Magenta:Neill_SDL_SetDrawColour(sw, COLOUR_MAX, 0, COLOUR_MAX);
                      break;

         case Cyan :  Neill_SDL_SetDrawColour(sw, 0, COLOUR_MAX, COLOUR_MAX);
                      break;

         case White : Neill_SDL_SetDrawColour(sw, COLOUR_MAX, COLOUR_MAX, COLOUR_MAX);
                      break;
         default : ON_ERROR("Error! Operator is not correct.\n");
      }
   }
   return 1;
}

/* Function subtracts 160 from hex value to evaluate sixel block state */
int sixel_subtraction(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   unsigned char temp_sixel = 0;
   if(teletext[i][j].contiguous_graphics!=OFF || teletext[i][j].held==ON)
   {
      temp_sixel = teletext[i][j].hex_code;
      sixel_switch(teletext, i, j, temp_sixel);
   }
   return 1;
}

/* Function : Held graphics facilitator */
int sixel_dividend_value(Code teletext[HEIGHT][LENGTH], int i, int j, int *sixel_dividend, int temp_sixel)
{
   if(teletext[i][j].held!=ON)
   {
      *sixel_dividend = temp_sixel - SIXEL_CONVERSION;
   }
   else
   {
      *sixel_dividend = teletext[i][j].hex_code;
   }
   return 1;
}

/* Function: determines which sixels are ON/OFF*/
int sixel_switch(Code teletext[HEIGHT][LENGTH], int i, int j, int temp_sixel)
{
   int sixel_dividend = 0;

   sixel_dividend_value(teletext, i, j, &sixel_dividend, temp_sixel);

   if(sixel_dividend >= SIXEL_BOTTOM_RIGHT)
   {
      teletext[i][j].sixel.block_6 = ON;
      sixel_dividend = sixel_dividend - SIXEL_BOTTOM_RIGHT;
   }
   else
   {
      teletext[i][j].sixel.block_6 = OFF;
   }

   if(sixel_dividend >= SIXEL_BOTTOM_LEFT)
   {
      teletext[i][j].sixel.block_5 = ON;
      sixel_dividend = sixel_dividend-SIXEL_BOTTOM_LEFT;
   }
   else
   {
      teletext[i][j].sixel.block_5 = OFF;
   }

   if(sixel_dividend >= SIXEL_MIDDLE_RIGHT)
   {
      teletext[i][j].sixel.block_4 = ON;
      sixel_dividend = sixel_dividend - SIXEL_MIDDLE_RIGHT;
   }
   else
   {
      teletext[i][j].sixel.block_4 = OFF;
   }

   if(sixel_dividend >= SIXEL_MIDDLE_LEFT)
   {
      teletext[i][j].sixel.block_3 = ON;
      sixel_dividend = sixel_dividend-SIXEL_MIDDLE_LEFT;
   }
   else
   {
      teletext[i][j].sixel.block_3 = OFF;
   }
   if(sixel_dividend >= SIXEL_TOP_RIGHT)
   {
      teletext[i][j].sixel.block_2 = ON;
      sixel_dividend = sixel_dividend-SIXEL_TOP_RIGHT;
   }
   else
   {
      teletext[i][j].sixel.block_2 = OFF;
   }

   if(sixel_dividend >= SIXEL_TOP_LEFT)
   {
      teletext[i][j].sixel.block_1 = ON;

   }
   else
   {
      teletext[i][j].sixel.block_1 = OFF;
   }
   return 1;
}

/* Function: executes change in height constants*/
int implement_double_height(Code teletext[HEIGHT][LENGTH], int i, int j, int *pixel_start, int *max_pixel_height, int *stretch_value)
{
   if(teletext[i][j].height==ON)
   {
      *stretch_value = STRETCH_CONSTANT;
      if(teletext[i][j].double_height == STRETCH_CONSTANT)
      {
         *max_pixel_height = STRETCH_CONSTANT;
         *pixel_start = FNTHEIGHT;
      }
   }
   return 1;
}

/* Function: If graphics is on assigns blast through colour */
int reset_graph(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].contiguous_graphics==ON)
   {
      if(teletext[i][j].hex_code>=BLAST_THROUGH_MIN && teletext[i][j].hex_code<=BLAST_THROUGH_MAX)
      {
         teletext[i][j].foreground_colour = teletext[i][j].graphics-GRAPHICS_TO_COLOUR;
      }
   }
   return 1;
}

/* Function: executes height*/
int convert_to_ASCII(Code teletext[HEIGHT][LENGTH], int i, int j, unsigned char *alphanumeric)
{
   if(teletext[i][j].hex_code<CONVERT_ASCII)
   {
      *alphanumeric = teletext[i][j].hex_code;
   }
   else
   {
      if(teletext[i][j].is_ctr_code == ON)
      {
         *alphanumeric = NON_COMMAND_CODES-CONVERT_ASCII;
      }
      else
      {
         *alphanumeric = teletext[i][j].hex_code-CONVERT_ASCII;
      }
   }
   return 1;
}

/* Function: puts to screen*/
int my_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Code teletext[HEIGHT][LENGTH], int i, int j, int ox, int oy)
{
   unsigned x, y;
   unsigned char alphanumeric;
   int stretch_value = 1;
   int pixel_start = 0;
   int max_pixel_height = 1;

   reset_graph(teletext, i, j);
   convert_to_ASCII(teletext, i, j, &alphanumeric);
   implement_double_height(teletext, i, j, &pixel_start, &max_pixel_height, &stretch_value);
   if(teletext[i][j].contiguous_graphics==OFF|| (teletext[i][j].hex_code>=BLAST_THROUGH_MIN && teletext[i][j].hex_code<=BLAST_THROUGH_MAX))
   {
      for(y = pixel_start; (int)y < FNTHEIGHT*max_pixel_height; y++)
      {
         for(x = 0; x < FNTWIDTH; x++)
         {
           SDL_draw_wrapper(sw, fontdata, teletext, x, y, i, j, ox, oy, stretch_value, pixel_start, alphanumeric);
         }
      }
   }
   return 1;
}

SDL_draw_wrapper(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Code teletext[HEIGHT][LENGTH], unsigned x, unsigned y, int i, int j, int ox, int oy, int stretch_value, int pixel_start, int alphanumeric)
{
  if(fontdata[alphanumeric-FNT1STCHAR][y/stretch_value] >> (FNTWIDTH - 1 - x)& 1)
  {
     colour_change(sw, teletext[i][j].foreground_colour);
     SDL_RenderDrawPoint(sw->renderer, x+ox, y+oy);

  }
  else
  {
     if(teletext[i][j].background==NEW_BACKGROUND)
     {
        colour_change(sw, teletext[i][j].back_colour);
        SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy-pixel_start);
     }
     else
     {
        Neill_SDL_SetDrawColour(sw, 0, 0, 0);
     }
  }
  SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy-pixel_start);
  return 1;
}
