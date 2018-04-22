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

    // open input file and if it couldn't be opended throw an error.
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[2]);
        return 2;
    }

    // initialize the buffer: an array of 512 bytes.
    unsigned char buffer[512];

    FILE *current_photo = NULL;

    int current_file_number = 0;

    int photo_was_found = 0;

    while (fread(buffer, 512, 1, file))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            if (photo_was_found == 1 && current_photo)
            {
                // The start of a new photo was found, close the old photo.
                fclose(current_photo);
            }
            else
            {
                // photo was found.
                photo_was_found = 1;
            }

            // init a string of 8 chars => 3 for the number and 4 for .jpg and 1 for \0.
            char filename[8];
            sprintf(filename, "%03d.jpg", current_file_number);

            // create a photo file with the name of filename.
            current_photo = fopen(filename, "a");

            // increment the current_file_number.
            current_file_number++;
        }
        if (photo_was_found == 1)
        {
            // write 512 bytes to file once we start finding jpgs
            fwrite(&buffer, 512, 1, current_photo);
        }
    }


    // close files
    fclose(file);
    fclose(current_photo);
    return 0;
}
