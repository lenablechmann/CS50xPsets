#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through every pixel and change the value a bit
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating averages.
            int average = round((((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue)) / 3);

            // Going through every pixel and setting to average.
            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    /* The Formula:
    sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
    sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
    sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue
    */
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating the sepia tones:
            int fresh_red = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            int fresh_green = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            int fresh_blue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);

            // Calculating new red + 255 cap
            if (fresh_red <= 255)
            {
                image[i][j].rgbtRed = fresh_red;
            }
            else if (fresh_red > 255)
            {
                image[i][j].rgbtRed = 255;
            }

            // Calculating new green + 255 cap
            if (fresh_green <= 255)
            {
                image[i][j].rgbtGreen = fresh_green;
            }

            else if (fresh_green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }

            // Calculating new blue + 255 cap
            if (fresh_blue <= 255)
            {
                image[i][j].rgbtBlue = fresh_blue;
            }

            else if (fresh_blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i <= height; i++)
    {
        for (int j = 0; j < (width / 2); j++) // with/2 so that it only flips half
        {
            // Swap image[i][j] with image[i][width - j]
            RGBTRIPLE tmp = image[i][j]; // RGBTRIPLE is a type, so typedef
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // ximage is going to be the untainted copy used for calculations
    // Because otherwise the averages will be changed
    RGBTRIPLE ximage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ximage[i][j] = image[i][j]; // the image copying
        }
    }

    for (int i = 1; i < (height - 1); i++)
    {
        for (int j = 1; j < (width - 1); j++)
        {
            int avgred = round(
                             (ximage[i - 1][j - 1].rgbtRed + ximage[i - 1][j].rgbtRed + ximage[i - 1][j + 1].rgbtRed
                              + ximage[i][j - 1].rgbtRed + ximage[i][j].rgbtRed + ximage[i][j + 1].rgbtRed
                              + ximage[i + 1][j - 1].rgbtRed + ximage[i + 1][j].rgbtRed + ximage[i + 1][j + 1].rgbtRed
                             ) / 9.0);

            int avggreen = round(
                               (ximage[i - 1][j - 1].rgbtGreen + ximage[i - 1][j].rgbtGreen + ximage[i - 1][j + 1].rgbtGreen
                                + ximage[i][j - 1].rgbtGreen + ximage[i][j].rgbtGreen + ximage[i][j + 1].rgbtGreen
                                + ximage[i + 1][j - 1].rgbtGreen + ximage[i + 1][j].rgbtGreen + ximage[i + 1][j + 1].rgbtGreen
                               ) / 9.0);

            int avgblue = round(
                              (ximage[i - 1][j - 1].rgbtBlue + ximage[i - 1][j].rgbtBlue + ximage[i - 1][j + 1].rgbtBlue
                               + ximage[i][j - 1].rgbtBlue + ximage[i][j].rgbtBlue + ximage[i][j + 1].rgbtBlue
                               + ximage[i + 1][j - 1].rgbtBlue + ximage[i + 1][j].rgbtBlue + ximage[i + 1][j + 1].rgbtBlue
                              ) / 9.0);

            // Going through every pixel and setting to average in the original image
            image[i][j].rgbtRed = avgred;
            image[i][j].rgbtGreen = avggreen;
            image[i][j].rgbtBlue = avgblue;
        }
    }

    // Edge cases, rows n columns
    // Upper row
    for (int j = 1; j < (width - 1); j++)
    {
        int avgred = round(
                         (ximage[0][j - 1].rgbtRed + ximage[0][j].rgbtRed + ximage[0][j + 1].rgbtRed
                          + ximage[0 + 1][j - 1].rgbtRed + ximage[0 + 1][j].rgbtRed + ximage[0 + 1][j + 1].rgbtRed
                         ) / 6.0);

        int avggreen = round(
                           (ximage[0][j - 1].rgbtGreen + ximage[0][j].rgbtGreen + ximage[0][j + 1].rgbtGreen
                            + ximage[0 + 1][j - 1].rgbtGreen + ximage[0 + 1][j].rgbtGreen + ximage[0 + 1][j + 1].rgbtGreen
                           ) / 6.0);

        int avgblue = round(
                          (ximage[0][j - 1].rgbtBlue + ximage[0][j].rgbtBlue + ximage[0][j + 1].rgbtBlue
                           + ximage[0 + 1][j - 1].rgbtBlue + ximage[0 + 1][j].rgbtBlue + ximage[0 + 1][j + 1].rgbtBlue
                          ) / 6.0);

        // Going through every pixel and setting to average.
        image[0][j].rgbtRed = avgred;
        image[0][j].rgbtGreen = avggreen;
        image[0][j].rgbtBlue = avgblue;
    }

    // Lower row !WRONG!
    for (int j = 1;  j < width - 1; j++)
    {
        int avgred = round(
                         (ximage[height - 2][j - 1].rgbtRed + ximage[height - 2][j].rgbtRed + ximage[height - 2][j + 1].rgbtRed
                          + ximage[height - 1][j - 1].rgbtRed + ximage[height - 1][j].rgbtRed + ximage[height - 1][j + 1].rgbtRed
                         ) / 6.0);

        int avggreen = round(
                           (ximage[height - 2][j - 1].rgbtGreen + ximage[height - 2][j].rgbtGreen + ximage[height - 2][j + 1].rgbtGreen
                            + ximage[height - 1][j - 1].rgbtGreen + ximage[height - 1][j].rgbtGreen + ximage[height - 1][j + 1].rgbtGreen
                           ) / 6.0);

        int avgblue = round(
                          (ximage[height - 2][j - 1].rgbtBlue + ximage[height - 2][j].rgbtBlue + ximage[height - 2][j + 1].rgbtBlue
                           + ximage[height - 1][j - 1].rgbtBlue + ximage[height - 1][j].rgbtBlue + ximage[height - 1][j + 1].rgbtBlue
                          ) / 6.0);

        // Going through every pixel and setting to average.
        image[height - 1][j].rgbtRed = avgred;
        image[height - 1][j].rgbtGreen = avggreen;
        image[height - 1][j].rgbtBlue = avgblue;
    }

    // Left column
    for (int i = 1;  i < height - 1; i++)
    {
        int avgred = round(
                         (ximage[i - 1][0].rgbtRed + ximage[i - 1][0 + 1].rgbtRed
                          + ximage[i][0].rgbtRed + ximage[i][0 + 1].rgbtRed
                          + ximage[i + 1][0].rgbtRed + ximage[i + 1][0 + 1].rgbtRed
                         ) / 6.0);

        int avggreen = round(
                           (ximage[i - 1][0].rgbtGreen + ximage[i - 1][0 + 1].rgbtGreen
                            + ximage[i][0].rgbtGreen + ximage[i][0 + 1].rgbtGreen
                            + ximage[i + 1][0].rgbtGreen + ximage[i + 1][0 + 1].rgbtGreen
                           ) / 6.0);

        int avgblue = round(
                          (ximage[i - 1][0].rgbtBlue + ximage[i - 1][0 + 1].rgbtBlue
                           + ximage[i][0].rgbtBlue + ximage[i][0 + 1].rgbtBlue
                           + ximage[i + 1][0].rgbtBlue + ximage[i + 1][0 + 1].rgbtBlue
                          ) / 6.0);

        // Going through every pixel and setting to average.
        image[i][0].rgbtRed = avgred;
        image[i][0].rgbtGreen = avggreen;
        image[i][0].rgbtBlue = avgblue;

    }
    
    //right column
    for (int i = 1;  i < height - 1; i++)
    {
        int avgred = round(
                         (ximage[i - 1][width - 2].rgbtRed + ximage[i - 1][width - 1].rgbtRed
                          + ximage[i][width - 2].rgbtRed + ximage[i][width - 1].rgbtRed
                          + ximage[i + 1][width - 2].rgbtRed + ximage[i + 1][width - 1].rgbtRed
                         ) / 6.0);

        int avggreen = round(
                           (ximage[i - 1][width - 2].rgbtGreen + ximage[i - 1][width - 1].rgbtGreen
                            + ximage[i][width - 2].rgbtGreen + ximage[i][width - 1].rgbtGreen
                            + ximage[i + 1][width - 2].rgbtGreen + ximage[i + 1][width - 1].rgbtGreen
                           ) / 6.0);

        int avgblue = round(
                          (ximage[i - 1][width - 2].rgbtBlue + ximage[i - 1][width - 1].rgbtBlue
                           + ximage[i][width - 2].rgbtBlue + ximage[i][width - 1].rgbtBlue
                           + ximage[i + 1][width - 2].rgbtBlue + ximage[i + 1][width - 1].rgbtBlue
                          ) / 6.0);

        // Going through every pixel and setting to average.
        image[i][width - 1].rgbtRed = avgred;
        image[i][width - 1].rgbtGreen = avggreen;
        image[i][width - 1].rgbtBlue = avgblue;
    }

    // Corner 1 upper left
    int avgred1 = round(
                      (ximage[0][0].rgbtRed + ximage[0][0 + 1].rgbtRed
                       + ximage[0 + 1][0].rgbtRed + ximage[0 + 1][0 + 1].rgbtRed
                      ) / 4.0);

    int avggreen1 = round(
                        (ximage[0][0].rgbtGreen + ximage[0][0 + 1].rgbtGreen
                         + ximage[0 + 1][0].rgbtGreen + ximage[0 + 1][0 + 1].rgbtGreen
                        ) / 4.0);

    int avgblue1 = round(
                       (ximage[0][0].rgbtBlue + ximage[0][0 + 1].rgbtBlue
                        + ximage[0 + 1][0].rgbtBlue + ximage[0 + 1][0 + 1].rgbtBlue
                       ) / 4.0);

    image[0][0].rgbtRed = avgred1;
    image[0][0].rgbtGreen = avggreen1;
    image[0][0].rgbtBlue = avgblue1;

    // Corner 2 upper right
    int avgred2 = round(
                      (ximage[0][width - 2].rgbtRed + ximage[0][width - 1].rgbtRed
                       + ximage[0 + 1][width - 2].rgbtRed + ximage[0 + 1][width - 1].rgbtRed
                      ) / 4.0);

    int avggreen2 = round(
                        (ximage[0][width - 2].rgbtGreen + ximage[0][width - 1].rgbtGreen
                         + ximage[0 + 1][width - 2].rgbtGreen + ximage[0 + 1][width - 1].rgbtGreen
                        ) / 4.0);

    int avgblue2 = round(
                       (ximage[0][width - 2].rgbtBlue + ximage[0][width - 1].rgbtBlue
                        + ximage[0 + 1][width - 2].rgbtBlue + ximage[0 + 1][width - 1].rgbtBlue
                       ) / 4.0);

    image[0][width - 1].rgbtRed = avgred2;
    image[0][width - 1].rgbtGreen = avggreen2;
    image[0][width - 1].rgbtBlue = avgblue2;

    //Corner 3 lower left !WRONG!
    int avgred3 = round(
                      (ximage[height - 1][0].rgbtRed + ximage[height - 1][0 + 1].rgbtRed
                       + ximage[height - 2][0].rgbtRed + ximage[height - 2][0 + 1].rgbtRed
                      ) / 4.0);

    int avggreen3 = round(
                        (ximage[height - 1][0].rgbtGreen + ximage[height - 1][0 + 1].rgbtGreen
                         + ximage[height - 2][0].rgbtGreen + ximage[height - 2][1].rgbtGreen
                        ) / 4.0);

    int avgblue3 = round(
                       (ximage[height - 1][0].rgbtBlue + ximage[height - 1][0 + 1].rgbtBlue
                        + ximage[height - 2][0].rgbtBlue + ximage[height - 2][1].rgbtBlue
                       ) / 4.0);

    image[height - 1][0].rgbtRed = avgred3;
    image[height - 1][0].rgbtGreen = avggreen3;
    image[height - 1][0].rgbtBlue = avgblue3;


    // lower right !WRONG!
    int avgred4 = round(
                      (ximage[height - 1][width - 1].rgbtRed + ximage[height - 1][width - 2].rgbtRed
                       + ximage[height - 2][width - 2].rgbtRed + ximage[height - 2][width - 1].rgbtRed
                      ) / 4.0);

    int avggreen4 = round(
                        (ximage[height - 1][width - 1].rgbtGreen + ximage[height - 1][width - 2].rgbtGreen
                         + ximage[height - 2][width - 2].rgbtGreen + ximage[height - 2][width - 1].rgbtGreen
                        ) / 4.0);

    int avgblue4 = round(
                       (ximage[height - 1][width - 1].rgbtBlue + ximage[height - 1][width - 2].rgbtBlue
                        + ximage[height - 2][width - 2].rgbtBlue + ximage[height - 2][width - 1].rgbtBlue
                       ) / 4.0);

    image[height - 1][width - 1].rgbtRed = avgred4;
    image[height - 1][width - 1].rgbtGreen = avggreen4;
    image[height - 1][width - 1].rgbtBlue = avgblue4;

    return;
}
