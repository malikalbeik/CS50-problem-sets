// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4 || (atoi(argv[1]) > 100 || atoi(argv[1]) < 1))
    {
        fprintf(stderr, "Usage: resize newSize infile outfile\n");
        return 1;
    }

    // remember filenames and the size to resize by.
    char *infile = argv[2];
    char *outfile = argv[3];
    int new_size = atoi(argv[1]);

    // open input file and if it couldn't open throw an error.
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file and if it couldn't open throw an error.
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // update width and height
    bi.biWidth *= new_size;
    bi.biHeight *= new_size;

    // determine padding for scanlines
    int output_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int input_padding = (4 - (bi.biWidth / new_size * sizeof(RGBTRIPLE)) % 4) % 4;

    // set the bi an bf's sizes.
    bi.biSizeImage = ((3 * bi.biWidth) + output_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight) / new_size; i < biHeight; i++)
    {
        // creating an array of struct RGBTRIPLE that will hold the current scanline in it.
        RGBTRIPLE current_scanline[bi.biWidth];
        int position = 0;

        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth / new_size; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // iterate as many times as new_size, draw the pixle and add it to the current_scanline array.
            for (size_t k = 0; k < new_size; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                current_scanline[position] = triple;
                position++;
            }
        }

        // Add padding for the original scanlines.
        for (int k = 0; k < output_padding; k++)
        {
            fputc(0x00, outptr);
        }

        if (new_size != 1)
        {
            // add extra scanlines times new_size - 1.
            for (int a = 0; a < new_size - 1; a++)
            {
                // iterate over pixels in scanline
                for (int b = 0; b <  bi.biWidth; b++)
                {
                    fwrite(&current_scanline[b], sizeof(RGBTRIPLE), 1, outptr);
                }
                // Add padding for the new scanlines.
                for (int k = 0; k < output_padding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
        }

        // skip over padding, if any
        fseek(inptr, input_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
