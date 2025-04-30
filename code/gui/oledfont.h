#ifndef __OLEDFONT_H
#define __OLEDFONT_H 	   

typedef struct
{
  unsigned char Index[2];
  char Msk[32];
} typFNT_GB16;;

typedef struct
{
  unsigned char Index[2];
  char Msk[72];
} typFNT_GB24;

typedef struct
{
  unsigned char Index[2];
  char Msk[128];
} typFNT_GB32;


extern const unsigned char F6x8[][8];
extern const unsigned char F8X16[] ;
extern const typFNT_GB16 cfont16[];
extern const typFNT_GB24 cfont24[];
extern const typFNT_GB32 cfont32[];

#endif


