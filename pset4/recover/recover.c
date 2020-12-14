#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Added it myself

// Recovering deleted images from an SD card.

int main(int argc, char *argv[])
{
    // Basic checks if user is giving the right info & if the file can be opened.
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    if (file == NULL)
    {
        printf("can't open that file\n");
        return 1;
    }

    // Looping through the image.
    int image_counter = 0; // For naming of the image
    int byte_counter = 0; // For counting the blocks
    FILE *image; // The new image file.
    char img_name[16]; // img name

    unsigned char byte[512];
    fread(byte, 512, 1, file);

    // unsigned char *storage = malloc(byte_counter);
    
    while (fread(byte, 512, 1, file) == 1)
    {
        // Start blocks management.
        if (byte[0] == 0xff && byte[1] == 0xd8 && byte[2] == 0xff && (byte[3] & 0xf0) == 0xe0)
        {
            if (image_counter > 0)
            {
                fclose(image);
                sprintf(img_name, "%03i.jpg", image_counter);
                image = fopen(img_name, "w");
                fwrite(byte, 512, 1, image);
                image_counter++;
            }
            
            if (image_counter == 0)
            {
                sprintf(img_name, "%03i.jpg", image_counter);
                image = fopen(img_name, "w");
                fwrite(byte, 512, 1, image);
                image_counter++;    
            }
        }
        
        // Writes inbetween start blocks
        else if (image_counter > 0) 
        {
            fwrite(byte, 512, 1, image);
        }
        byte_counter += 512;
    }

    //printf("%d bytes have been read, %d images found.\n", byte_counter, image_counter); // bytes check.

    // free(storage);
    fclose(image);
    fclose(file);
    return 0;

}
