#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_SIZE 1000

int default_setting(unsigned char array[1000], int i, FILE *fp); 
int test_colours(unsigned char array[1000], int i, FILE *fp);
int test_writing(unsigned char array[1000], int i, FILE *fp);
int single_height(unsigned char array[1000], int i, FILE *fp); 
int double_height(unsigned char array[1000], int i, FILE *fp); 
int hold_generator(unsigned char array[1000], int i, FILE *fp);

int main()
{  

   int i=1, j = 40;
   unsigned char array[MAX_SIZE];
   FILE *fp;
   
   fp = fopen("tester.m7", "w");
   if (fp == NULL)
   {
      printf("Error opening file!\n");
      exit(1);
   }
   
   for(i=0; i<1000; i++)
   {
      assert(test_colours(array, i, fp));
      assert(test_writing(array, i, fp));
      assert(double_height(array, i, fp)); 
      assert(single_height(array, i, fp));
      assert(symbol_numbers(array, i, fp));
      assert(hold_generator(array, i, fp));
      assert(default_setting(array, i, fp));
   }
   
   fclose(fp);
   return 0;   
}  
int default_setting(unsigned char array[1000], int i, FILE *fp) 
{
      array[i] = 0xA0;
      fprintf(fp, "%c", array[i]);
      return 1;
}
      
int test_colours(unsigned char array[1000], int i, FILE *fp)
{     
   if(i==40)
      {
         array[i] = 0x81;
         fprintf(fp, "%c", array[i]);
      } 
      if(i==41)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);
      }
      
      if(i==78)
      {
         array[i] = 0x82;
         fprintf(fp, "%c", array[i]);
      }
      if(i==79)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);     
      }
      
      if(i==116)
      {
         array[i] = 0x83;
         fprintf(fp, "%c", array[i]);
      }
      if(i==117)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);     
      }
      
      if(i==154)
      {
         array[i] = 0x84;
         fprintf(fp, "%c", array[i]);
      } 
      if(i==155)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);
      }
      if(i==192)
      {
         array[i] = 0x85;
         fprintf(fp, "%c", array[i]);
      }
      if(i==193)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);     
      }
      if(i==230)
      {
         array[i] = 0x86;
         fprintf(fp, "%c", array[i]);
      }
      if(i==231)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);     
      }
      if(i==268)
      {
         array[i] = 0x87;
         fprintf(fp, "%c", array[i]);
      }
      if(i==269)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);     
      }
   return 1;     
}      

int test_writing(unsigned char array[1000], int i, FILE *fp)
{
      if(i == 320) /* Plain white text */
      {
         array[i] = 0xCE;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 321)
      {
         array[i] = 0xC5;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 322)
      {
         array[i] = 0xC9;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 323)
      {
         array[i] = 0xCC;
         fprintf(fp, "%c", array[i]); 
      }

      if(i==390) /*Blue text */
      {
         array[i] = 0x84;
         fprintf(fp, "%c", array[i]);   
      }
      if(i==391)
      {
         array[i] = 0xD4;
         fprintf(fp, "%c", array[i]);     
      }
     
      if(i == 392)
      {
         array[i] = 0xC8;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 393)
      {
         array[i] = 0xC5;
         fprintf(fp, "%c", array[i]);
          
      }
      if(i == 394)
      {
         array[i] = 0x82;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 395)
      {
         array[i] = 0x9D;
         fprintf(fp, "%c", array[i]);
          
      }
      if(i == 396) /* Red text */
      {
         array[i] = 0x81;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 397)
      {
         array[i] = 0xC8;
         fprintf(fp, "%c", array[i]); 
      } 
      if(i== 398)
      {
         array[i] = 0xCF;
         fprintf(fp, "%c", array[i]);     
      }
     
      if(i == 399)
      {
         array[i] = 0xD2;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 400)
      {
         array[i] = 0xD2;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 401)
      {
         array[i] = 0xCF;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 402)
      {
         array[i] = 0xD2;
         fprintf(fp, "%c", array[i]); 
      }
   return 1;       
}

int double_height(unsigned char array[1000], int i, FILE *fp)
{

      if(i==490)  /* Double Height */
      {
         array[i] = 0x8d;
         fprintf(fp, "%c", array[i]);   
      }
      if(i==516)
      {
         array[i] = 0x8d;
         fprintf(fp, "%c", array[i]);   
      }
      
      if(i==491)
      {
         array[i] = 0xc4;
         fprintf(fp, "%c", array[i]);     
      }
      if(i==518)
      {
         array[i] = 0xc4;
         fprintf(fp, "%c", array[i]);     
      } 
          
      if(i == 492)
      {
         array[i] = 0xef;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 519)
      {
         array[i] = 0xef;
         fprintf(fp, "%c", array[i]); 
      }
      
      if(i == 493)
      {
         array[i] = 0xf5;
         fprintf(fp, "%c", array[i]);          
      }            
      if(i == 520)
      {
         array[i] = 0xf5;
         fprintf(fp, "%c", array[i]);          
      }
      
      if(i == 494)
      {
         array[i] = 0xe2;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 521)
      {
         array[i] = 0xe2;
         fprintf(fp, "%c", array[i]); 
      }
      
      if(i == 495)
      {
         array[i] = 0xec;
         fprintf(fp, "%c", array[i]);
          
      }
      if(i == 522)
      {
         array[i] = 0xec;
         fprintf(fp, "%c", array[i]);
          
      }
      
      if(i == 496)
      {
         array[i] = 0xe5;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 523)
      {
         array[i] = 0xe5;
         fprintf(fp, "%c", array[i]); 
      }
      
      if(i == 497)
      {
         array[i] = 0xa0;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 524)
      {
         array[i] = 0xa0;
         fprintf(fp, "%c", array[i]); 
      } 
       
      if(i== 498)
      {
         array[i] = 0xe5;
         fprintf(fp, "%c", array[i]);     
      }
      if(i == 525)
      {
         array[i] = 0xe5;
         fprintf(fp, "%c", array[i]); 
      }      
      
      if(i == 499)
      {
         array[i] = 0xe9;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 526)
      {
         array[i] = 0xe9;
         fprintf(fp, "%c", array[i]); 
      }
      
      if(i == 500)
      {
         array[i] = 0xe7;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 527)
      {
         array[i] = 0xe7;
         fprintf(fp, "%c", array[i]); 
      }
      
      if(i == 501)
      {
         array[i] = 0xe8;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 528)
      {
         array[i] = 0xe8;
         fprintf(fp, "%c", array[i]); 
      }
      
      if(i == 502)
      {
         array[i] = 0xf4;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 529)
      {
         array[i] = 0xf4;
         fprintf(fp, "%c", array[i]); 
      }
      
   return 1;
}
int  single_height(unsigned char array[1000], int i, FILE *fp)
{
      if(i==530)  /* Single height */
      {
         array[i] = 0X8c;
         fprintf(fp, "%c", array[i]); 
      }      
   return 1;   
}

int symbol_numbers(unsigned char array[1000], int i, FILE *fp)
{
      if(i == 535)   /* Insert a symbol */
      {
         array[i] = 0XBD;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 536) /* Number code */
      {
         array[i] = 0XB1;
         fprintf(fp, "%c", array[i]); 
      }
      if(i == 537)
      {
         array[i] = 0XB6;
         fprintf(fp, "%c", array[i]); 
      }
   return 1;     
}      
int hold_generator(unsigned char array[1000], int i, FILE *fp)
{
   int x;

   if(i == 600)
   {
      array[i] = 0x9E;
      fprintf(fp, "%c", array[i]); 
   }
   if(i>601 && i < 620)
   {
      array[i] = rand() % (0x97 + 1 - 0x90) + 0x90;
      x = rand() % (0x97 + 1 - 0x90) + 0x90;
      printf("%d\n", x);
      fprintf(fp, "%c", array[i]); 
   }
   return 1;
}

 
