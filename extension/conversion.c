#include <stdio.h>
#include <stdlib.h>
#include "render_html.h"
#include <assert.h>
#define RECTSIZE 20

int main(int argc, char *argv[])
{
   Code teletext[HEIGHT][LENGTH];

   if(argc!= FILE_NUMBER)
   {
      ON_ERROR("Wrong number of files uplaoded.\n");
      return 0;
   }
   assert(read_file(argv[1], teletext));
   assert(create_html_file(teletext));
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
        assert(test);
        if(test != 1)
        {
           ON_ERROR("Error. Wrong number of characters read.\n");
        }
      }
   }
   assert(sort_code(teletext));
   return 1;
}

/* Function : sets structure members to default values */
int default_settings(Code teletext[HEIGHT][LENGTH], int x)
{
   int i = 0;
   while(i!=LENGTH)
   {
      teletext[x][i].foreground_colour = White;
      assert(teletext[x][i].foreground_colour == White);

      teletext[x][i].background = Black;
      assert(teletext[x][i].background == Black);

      teletext[x][i].back_colour = Black;
      assert(teletext[x][i].back_colour == Black);

      teletext[x][i].double_height = OFF;
      assert(teletext[x][i].double_height == OFF);

      teletext[x][i].graphics = OFF;
      assert(teletext[x][i].graphics == OFF);

      teletext[x][i].height = OFF;
      assert(teletext[x][i].height == OFF);

      teletext[x][i].contiguous_graphics = OFF;
      assert(teletext[x][i].contiguous_graphics == OFF);

      teletext[x][i].seperated = OFF;
      assert(teletext[x][i].seperated == OFF);

      teletext[x][i].held = OFF;
      assert(teletext[x][i].held == OFF);

      i++;
   }
   return 1;
}

/* Function: sorts through hex codes and assigns appropriate tags. */
int sort_code(Code teletext[HEIGHT][LENGTH])
{
   int i = 0, j = 0;

   for(i=0; i<HEIGHT; i++)
   {
      assert(default_settings(teletext, i));
      for(j=0; j<LENGTH; j++)
      {
         assert(foreground(teletext, i, j));
         assert(background(teletext,i, j));
         assert(check_for_height_code(teletext, i, j));
         assert(assign_upper_lower_tag(teletext, i, j));
         assert(held_graphics(teletext, i, j));
         assert(graphics(teletext,i, j));
         assert(seperated_graphics(teletext, i, j));
      }
   }
   return 1;
}

/* Function: selects and assigns appropriate foreground colours */
int foreground(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code >= Red && teletext[i][j].hex_code <= White)
   {
      teletext[i][j].foreground_colour = teletext[i][j].hex_code;
      assert(teletext[i][j].foreground_colour == teletext[i][j].hex_code);
   }
   else
   {
      assert(previous_foreground(teletext, i, j));
   }
   return 1;
}

/* Function: updates*/
int previous_foreground(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      teletext[i][j].foreground_colour = teletext[i][j-1].foreground_colour;
      assert(teletext[i][j].foreground_colour == teletext[i][j-1].foreground_colour);
   }
   return 1;
}

/* Function: selects and assigns appropriate background colours */
int background(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code == NEW_BACKGROUND)
   {
         teletext[i][j].background = NEW_BACKGROUND;
         assert(teletext[i][j].background == NEW_BACKGROUND);
         teletext[i][j].back_colour = teletext[i][j-1].foreground_colour;
         assert(teletext[i][j].back_colour == teletext[i][j-1].foreground_colour);
   }
   else
   {
      assert(previous_background(teletext, i, j));
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
         assert(teletext[i][j].back_colour == Black);
      }
      else
      {
         teletext[i][j].background = teletext[i][j-1].background;
         assert(teletext[i][j].background == teletext[i][j-1].background);
         teletext[i][j].back_colour = teletext[i][j-1].back_colour;
         assert(teletext[i][j].back_colour == teletext[i][j-1].back_colour);
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
      assert(teletext[i][j].height == OFF);
      teletext[i][j].double_height = OFF;
      assert(teletext[i][j].double_height == OFF);
   }
   else if(teletext[i][j].hex_code == DOUBLE_HEIGHT)
   {
      teletext[i][j].height = ON;
      assert(teletext[i][j].height == ON);
   }
   else
   {
      if(j!=0)
      {
         teletext[i][j].height = teletext[i][j-1].height;
         assert(teletext[i][j].height == teletext[i][j-1].height);
      }
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
         assert(teletext[i][j].double_height == LOWER_HALF);
      }
      else
      {
         teletext[i][j].double_height = UPPER_HALF;
         assert(teletext[i][j].double_height == UPPER_HALF);
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
      assert(teletext[i][j].held == ON);
      teletext[i][j-1].seperated = OFF;
      assert(teletext[i][j-1].seperated == OFF);
      teletext[i][j].held_colours = teletext[i][j-1].hex_code;
      assert(teletext[i][j].held_colours == teletext[i][j-1].hex_code);
   }
   assert(previous_held_graphics(teletext, i, j));
   return 1;
}

/* Function: updates*/
int previous_held_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j-1].held==ON)
   {
      if(j!=0)
      {
         if(teletext[i][j].hex_code == RELEASE_GRAPHICS)
         {
            teletext[i][j].held_colours = OFF;
            assert(teletext[i][j].held_colours == OFF);
            return 1;
         }
         teletext[i][j].held_colours = teletext[i][j-1].hex_code;
         assert(teletext[i][j].held_colours == teletext[i][j-1].hex_code);
         teletext[i][j].held = teletext[i][j-1].held;
         assert(teletext[i][j].held == teletext[i][j-1].held);
         teletext[i][j].seperated =  teletext[i][j-1].seperated;
         assert(teletext[i][j].seperated ==  teletext[i][j-1].seperated);
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
      assert(teletext[i][j].contiguous_graphics == ON);
      teletext[i][j].graphics = teletext[i][j].hex_code;
      assert(teletext[i][j].graphics == teletext[i][j].hex_code);
   }
   else
   {
      assert(previous_graphics(teletext, i, j));
   }
   return 1;
}

/* Function: updates*/
int previous_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      if(teletext[i][j-1].hex_code >= Red && teletext[i][j-1].hex_code <= White )
      {
         teletext[i][j].graphics  = OFF;
         assert(teletext[i][j].graphics  == OFF);
      }
      else
      {
         teletext[i][j].graphics = teletext[i][j-1].graphics;
         assert(teletext[i][j].graphics == teletext[i][j-1].graphics);
         teletext[i][j].contiguous_graphics = teletext[i][j-1].contiguous_graphics;
         assert(teletext[i][j].contiguous_graphics == teletext[i][j-1].contiguous_graphics);
      }
   }
   return 1;
}

/* Function: checks and assigns seperated graphics */
int seperated_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(teletext[i][j].hex_code == SEPERATED_GRAPHICS)
   {
      teletext[i][j].seperated = ON;
      assert(teletext[i][j].seperated == ON);
      teletext[i][j].contiguous_graphics = ON;
      assert(teletext[i][j].contiguous_graphics == ON);
      teletext[i][j].graphics = Graphics_White;
      assert(teletext[i][j].graphics == Graphics_White);
   }
   else
   {
      assert(previous_seperated_graphics(teletext, i, j));
   }
   return 1;
}

/* Function: updates*/
int previous_seperated_graphics(Code teletext[HEIGHT][LENGTH], int i, int j)
{
   if(j!=0)
   {
      teletext[i][j].seperated = teletext[i][j-1].seperated;
      assert(teletext[i][j].seperated == teletext[i][j-1].seperated);
   }
   return 1;
}

/* html begins */

/* Function: writes file. */
int create_html_file(Code teletext[HEIGHT][LENGTH])
{
   FILE *file;
   char string2[MEDIUM_CONSTANT] = "</head>";
   file = fopen("file.html", "w");
   if(file == NULL)
   {
      ON_ERROR("File not located\n");
   }
   assert(initiliase_the_file(file));
   assert(table_generation(teletext, file));
   fprintf(file, "\n%s", string2);

   return 1;
}

/* Function: Standard html tag */
int initiliase_the_file(FILE * file)
{
   char init[LONG_CONSTANT] = "<!doctype html>\n<head>\n<body BGCOLOR = \"Black\" TEXT = \"White\">";
   char style[LONG_CONSTANT] = "<table width: SHORT_CONSTANT0; cellspacing = \"0\"cellpadding =\"0\" id =\"table1\">";
   fprintf(file, "%s\n", init);
   fprintf(file, "%s\n\n", style);

   return 1;
}

/*Function: generate a 2D table\n*/
int table_generation(Code teletext[HEIGHT][LENGTH], FILE * file)
{
   char colour[MEDIUM_CONSTANT] = "<FONT ";
   char colour_end[MEDIUM_CONSTANT] = "</FONT>";
   char string2[SHORT_CONSTANT] = "</table>";
   char row[SHORT_CONSTANT] = "<tr>";
   char data_close[SHORT_CONSTANT] = ">";
   char data_end[SHORT_CONSTANT] = "</td>";
   char row_end[SHORT_CONSTANT] ="</tr>";
   char column_end[SHORT_CONSTANT] = "</th>";
   int j, i;

   for(j=0; j<HEIGHT; j++)
   {
      fprintf(file, "%s\n",row);
      for(i=0; i<LENGTH; i++)
      {
            assert(background_render(teletext, file, j, i));
            fprintf(file, "\t%s", data_close);
            assert(sixel_build(teletext, file, j, i));
            fprintf(file, "%s", colour);
            assert(size(teletext, file, j, i));
            assert(reset_graph(teletext, j, i));
            assert(colours(teletext, file, j, i));
            assert(meaning_code(teletext, file, j, i));
            fprintf(file, "%s", colour_end);
            fprintf(file, "\t%s", data_end);
            fprintf(file, "\t%s\n", column_end);
      }
      fprintf(file, "%s\n",row_end);
   }
   fprintf(file, "%s", string2);
   return 1;
}
int sixel_build(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i)
{
  if(teletext[j][i].hex_code<BLAST_THROUGH_MIN || teletext[j][i].hex_code>BLAST_THROUGH_MAX)
  {
      if(teletext[j][i].contiguous_graphics==ON)
      {
         assert(sixel_subtraction(teletext, j, i, file));
      }
  }
  return 1;
}

/* Function subtracts 160 from hex value to evaluate sixel block state */
int sixel_subtraction(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file)
{
   unsigned char temp_sixel = 0;
   if(teletext[j][i].contiguous_graphics!=OFF && teletext[j][i].held==OFF)
   {
         temp_sixel = teletext[j][i].hex_code;
         assert(temp_sixel == teletext[j][i].hex_code);
         assert(sixel_switch(teletext, j, i, temp_sixel, file));
   }
   return 1;
}

int rgb_value(unsigned char colour, char colour_string[MEDIUM_CONSTANT])
{
   int temp = 0;
   if(colour!=0)
   {
      if(colour>= Graphics_Red && colour<= Graphics_Black)
      {
         colour = colour-GRAPHICS_TO_COLOUR;
         temp = colour;
         assert(colour == temp);
      }
      switch(colour)
      {
         case Red   : strcpy(colour_string, "rgb(255, 0, 0)\">");
                      break;

         case Green : strcpy(colour_string, "rgb(0, 255, 0)\">");
                      break;

         case Yellow: strcpy(colour_string, "rgb(255, 255, 0)\">");
                      break;

         case Blue  : strcpy(colour_string, "rgb(255, 0, 255)\">");
                      break;

         case Magenta: strcpy(colour_string, "rgb(255, 0, 255)\">");
                      break;

         case Cyan :  strcpy(colour_string, "rgb(0, 255, 255)\">");
                      break;

         case White : strcpy(colour_string, "rgb(255, 255, 255)\">");
                      break;

         default : ON_ERROR("Error! Operator is not correct.\n");
      }
   }
   return 1;
}

/* Function: determines which sixels are ON/OFF*/
int sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, int temp_sixel, FILE *file)
{
   int sixel_dividend = 0;
   char string1[LONG_CONSTANT] = "<svg width = \"16\" height = \"18\">";
   char string3[SHORT_CONSTANT] = "</svg>";

   assert(sixel_dividend_value(teletext, j, i, &sixel_dividend, temp_sixel));
   fprintf(file,"%s",string1);
   assert(bottom_sixel_switch(teletext, j, i, file, &sixel_dividend));
   assert(middle_sixel_switch(teletext, j, i, file, &sixel_dividend));
   assert(top_sixel_switch(teletext, j, i, file, &sixel_dividend));
   fprintf(file,"%s", string3);
   return 1;
}

int bottom_sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file, int *sixel_dividend)
{
   char bottom_right[LONG_CONSTANT] = "<rect width=\"8\" height=\"6\" x=\"8\" y=\"12\" style=\"fill:";
   char bottom_left[LONG_CONSTANT] = "<rect width=\"8\" height=\"6\" x=\"0\" y=\"12\" style=\"fill:";
   char string_colour[MEDIUM_CONSTANT];
   int temp;

   if(*sixel_dividend >= SIXEL_BOTTOM_RIGHT)
   {
      assert(rgb_value(teletext[j][i].graphics, string_colour));
      teletext[j][i].sixel.block_6 = ON;
      assert(teletext[j][i].sixel.block_6 == ON);
      temp = *sixel_dividend - SIXEL_BOTTOM_RIGHT;
      fprintf(file,"%s""%s",bottom_right, string_colour);
      *sixel_dividend = *sixel_dividend - SIXEL_BOTTOM_RIGHT;
      assert(*sixel_dividend == temp);
   }
   if(*sixel_dividend >= SIXEL_BOTTOM_LEFT)
   {
      teletext[j][i].sixel.block_5 = ON;
      assert(teletext[j][i].sixel.block_5 == ON);
      assert(rgb_value(teletext[j][i].graphics, string_colour));
      fprintf(file,"%s""%s", bottom_left, string_colour);
      temp = *sixel_dividend-SIXEL_BOTTOM_LEFT;
      *sixel_dividend = *sixel_dividend-SIXEL_BOTTOM_LEFT;
      assert( *sixel_dividend == temp);
   }
   return 1;
}
int middle_sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file, int *sixel_dividend)
{
   char string_colour[MEDIUM_CONSTANT];
   char middle_right[LONG_CONSTANT] = "<rect width=\"8\" height=\"6\" x=\"8\" y=\"6\" style=\"fill:";
   char middle_left[LONG_CONSTANT] = "<rect width=\"8\" height=\"6\" x=\"0\" y=\"6\" style=\"fill:";
   int temp;
   if(*sixel_dividend >= SIXEL_MIDDLE_RIGHT)
   {
      teletext[j][i].sixel.block_4 = ON;
      assert(teletext[j][i].sixel.block_4 == ON);
      assert(rgb_value(teletext[j][i].graphics, string_colour));
      fprintf(file,"%s""%s",middle_right, string_colour);
      temp = *sixel_dividend - SIXEL_MIDDLE_RIGHT;
      *sixel_dividend = *sixel_dividend - SIXEL_MIDDLE_RIGHT;
      assert(temp == *sixel_dividend);
   }
   if(*sixel_dividend >= SIXEL_MIDDLE_LEFT)
   {
      teletext[j][i].sixel.block_3 = ON;
      rgb_value(teletext[j][i].graphics, string_colour);
      fprintf(file,"%s""%s",middle_left, string_colour);
      temp = *sixel_dividend-SIXEL_MIDDLE_LEFT;
      *sixel_dividend = *sixel_dividend-SIXEL_MIDDLE_LEFT;
      assert(temp == *sixel_dividend);
   }
   return 1;
}

int top_sixel_switch(Code teletext[HEIGHT][LENGTH], int j, int i, FILE *file, int *sixel_dividend)
{
   char string_colour[MEDIUM_CONSTANT];
   char top_right[LONG_CONSTANT] = "<rect width=\"30\" height=\"6\" x=\"8\" y=\"0\" style=\"fill:";
   char top_left[LONG_CONSTANT] = "<rect width=\"8\" height=\"6\" x=\"0\" y=\"0\" style=\"fill:";
   int temp;
   if(*sixel_dividend >= SIXEL_TOP_RIGHT)
   {
      teletext[j][i].sixel.block_2 = ON;
      assert(teletext[j][i].sixel.block_2 == ON);
      assert(rgb_value(teletext[j][i].graphics, string_colour));
      fprintf(file,"%s""%s",top_right, string_colour);
      *sixel_dividend = *sixel_dividend-SIXEL_TOP_RIGHT;
      temp = *sixel_dividend;
      assert(temp == *sixel_dividend);
   }
   if(*sixel_dividend >= SIXEL_TOP_LEFT)
   {
      teletext[j][i].sixel.block_1 = ON;
      assert(teletext[j][i].sixel.block_1 == ON);
      rgb_value(teletext[j][i].graphics, string_colour);
      fprintf(file,"%s""%s",top_left, string_colour);
   }
   return 1;
}

/* Function : Held graphics facilitator */
int sixel_dividend_value(Code teletext[HEIGHT][LENGTH], int i, int j, int *sixel_dividend, int temp_sixel)
{
   int temp;
   if(teletext[i][j].held!=ON)
   {
      temp = temp_sixel - SIXEL_CONVERSION;
      *sixel_dividend = temp_sixel - SIXEL_CONVERSION;
      assert(temp == *sixel_dividend);
   }
   else
   {
      assert(*sixel_dividend == teletext[i][j].hex_code);
   }
   return 1;
}

/* Evaluates and assigns height */
int size(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i)
{
  char double_h[MEDIUM_CONSTANT]="Size =\"25\"";
  char single[MEDIUM_CONSTANT]="Size =\"5\"";
  char reduce[MEDIUM_CONSTANT]="Size =\"-2\"";

  if(teletext[j][i].double_height==LOWER_HALF)
  {
      fprintf(file, "%s", double_h);
  }
  if(teletext[j][i].double_height==UPPER_HALF)
  {
      fprintf(file, "%s", reduce);
  }
  else
  {
      fprintf(file, "%s", single);
  }
   return 1;
}

int reset_graph(Code teletext[HEIGHT][LENGTH], int j, int i)
{
   if(teletext[j][i].contiguous_graphics==ON)
   {
      if(teletext[j][i].hex_code>=BLAST_THROUGH_MIN && teletext[j][i].hex_code<=BLAST_THROUGH_MAX)
      {
         teletext[j][i].foreground_colour = teletext[j][i].graphics-GRAPHICS_TO_COLOUR;
         assert(teletext[j][i].foreground_colour == teletext[j][i].graphics-GRAPHICS_TO_COLOUR);
      }
   }
   return 1;
}

/*Function : Assigns meaning */
int meaning_code(Code teletext[HEIGHT][LENGTH], FILE *file, int j, int i)
{
   assert(empty_space(teletext, file, j, i));
   assert(characters(teletext, file, j, i));
   return 1;
}

/* Function: Edit values */
int empty_space(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i)
{
   char string[SHORT_CONSTANT] = "&nbsp";
   if(teletext[j][i].hex_code == NON_COMMAND_CODES || (teletext[j][i].hex_code >= Red && teletext[j][i].hex_code <= White))
   {
      fprintf(file ,"%s", string);
   }
   return 1;
}
int characters(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i)
{
   int character;
   if(teletext[j][i].double_height== UPPER_HALF)
   {
      teletext[j][i].hex_code = NON_COMMAND_CODES;
      assert(teletext[j][i].hex_code == NON_COMMAND_CODES);
      character = teletext[j][i].hex_code;
      assert(character == teletext[j][i].hex_code);
      fprintf(file, "%c", character);
   }
   if(teletext[j][i].hex_code>NON_COMMAND_CODES && teletext[j][i].hex_code< UNUSED)
   {
      if(teletext[j][i].contiguous_graphics == OFF)
      {
         character = teletext[j][i].hex_code-CONVERT_ASCII;
         assert(character == teletext[j][i].hex_code-CONVERT_ASCII);
         fprintf(file, "%c", character);
      }
   }
   if(teletext[j][i].hex_code<CONVERT_ASCII)
   {
       character = teletext[j][i].hex_code;
       assert(character == teletext[j][i].hex_code);
       fprintf(file, "%c", character);
   }
   if(teletext[j][i].contiguous_graphics==ON)
   {
      if(teletext[j][i].hex_code>=BLAST_THROUGH_MIN && teletext[j][i].hex_code<=BLAST_THROUGH_MAX)
      {
         character = teletext[j][i].hex_code-CONVERT_ASCII;
         assert(character == teletext[j][i].hex_code-CONVERT_ASCII);
         fprintf(file, "%c", character);
      }
   }
   return 1;
}

/* Function : Colours */
int colours(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i)
{
   char colour_blue[MEDIUM_CONSTANT] = "COLOR = \"Blue\">";
   char colour_green[MEDIUM_CONSTANT] = "COLOR = \"Green\">";
   char colour_magenta[MEDIUM_CONSTANT] = "COLOR = \"Magenta\">";
   char colour_yellow[MEDIUM_CONSTANT] = "COLOR = \"Yellow\">";
   char colour_cyan[MEDIUM_CONSTANT] = "COLOR = \"Cyan\">";
   char colour_red[MEDIUM_CONSTANT] = "COLOR = \"Red\">";
   char colour_default[MEDIUM_CONSTANT] = "COLOR = \"White\">";

   if(teletext[j][i].foreground_colour >= Red && teletext[j][i].foreground_colour <= White)
   {
     if(teletext[j][i].foreground_colour == Red)
     {
         fprintf(file, "%s", colour_red);
         return 1;
     }
     if(teletext[j][i].foreground_colour == Blue)
     {
         fprintf(file, "%s", colour_blue);
         return 1;
     }
     if(teletext[j][i].foreground_colour == Green)
     {
         fprintf(file, "%s", colour_green);
         return 1;
     }
     if(teletext[j][i].foreground_colour == Yellow)
     {
         fprintf(file, "%s", colour_yellow);
         return 1;
     }
     if(teletext[j][i].foreground_colour == Magenta)
     {
         fprintf(file, "%s", colour_magenta);
         return 1;
     }
     if(teletext[j][i].foreground_colour == Cyan)
     {
         fprintf(file, "%s", colour_cyan);
         return 1;
     }
     else
     {
         fprintf(file, "%s", colour_default);
     }
   }
   return 1;
}

int background_render(Code teletext[HEIGHT][LENGTH], FILE *file,  int j, int i)
{
   char column_data[SHORT_CONSTANT] = "<th> <td";
   char colour_blue[MEDIUM_CONSTANT] = " BGCOLOR = \"Blue\"";
   char colour_green[MEDIUM_CONSTANT] = " BGCOLOR =\"Green\"";
   char colour_magenta[MEDIUM_CONSTANT] = " BGCOLOR =\"Magenta\"";
   char colour_yellow[MEDIUM_CONSTANT] = " BGCOLOR =\"Yellow\"";
   char colour_cyan[MEDIUM_CONSTANT] = " BGCOLOR =\"Cyan\"";
   char colour_red[MEDIUM_CONSTANT] = " BGCOLOR =\"Red\"";
   char colour_default[MEDIUM_CONSTANT] = " BGCOLOR =\"Black\"";
   char colour_white[MEDIUM_CONSTANT] = " BGCOLOR =\"White\"";

   fprintf(file, "\t%s", column_data);

   if(teletext[j][i].held==ON)
   {
     teletext[j][i].back_colour = teletext[j][i].graphics - GRAPHICS_TO_COLOUR;
   }

   if(teletext[j][i].back_colour >= Red && teletext[j][i].back_colour <= White)
   {
     if(teletext[j][i].back_colour == Red)
     {
         fprintf(file, "%s", colour_red);
         return 1;
     }
     if(teletext[j][i].back_colour == Blue)
     {
         fprintf(file, "%s", colour_blue);
         return 1;
     }
     if(teletext[j][i].back_colour == Green)
     {
         fprintf(file, "%s", colour_green);
         return 1;
     }
     if(teletext[j][i].back_colour == Yellow)
     {
         fprintf(file, "%s", colour_yellow);
         return 1;
     }
     if(teletext[j][i].back_colour == Magenta)
     {
         fprintf(file, "%s", colour_magenta);
         return 1;
     }
     if(teletext[j][i].back_colour == Cyan)
     {
         fprintf(file, "%s", colour_cyan);
         return 1;
     }
     if(teletext[j][i].back_colour == White)
     {
       fprintf(file, "%s", colour_white);
       return 1;
     }
   }
   else
   {
      fprintf(file, "%s", colour_default);
   }
   return 1;
}
