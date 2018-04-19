#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  // ensure proper usage
  if (argc != 2)
  {
    fprintf(stderr, "Usage: ./recover image\n");
    return 1;
  }

  // open input file and if it couldn't open throw an error.
  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
  {
      fprintf(stderr, "Could not open %s.\n", argv[2]);
      return 2;
  }
  unsigned char buffer[512];
  

  // close files
  fclose(file);

  return 0;
}
