#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage : ./recover IMAGE\n");
        return 1;
    }
    // Open memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        return 1;
    }
    // Initialize variables
    BYTE *buffer = malloc(BLOCK_SIZE);
    int count = 0;
    FILE *img;
    char *filename = malloc(sizeof(char) * 10);
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        //Look for a beginning of JPEG
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (count != 0)
            {
                fclose(img);
            }
            // Write 512 bytes until a new JPEG file.
            sprintf(filename , "%03i.jpg", count);
            img = fopen(filename, "w");
            count++;
        }
        // If already found a JPEG file.
        if (count > 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }

    }
    free(filename);
    free(buffer);
    fclose(img);
    fclose(card);
}
