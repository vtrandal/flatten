#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#define NUM_IMAGES 10000  // number of images to display

int read_idx_header(const char *filename);
int read_mnist_images(const char *filename, uint8_t *images, int num_images);
int flatten_images(const char *filename, uint8_t *images, int num_images);
int display_images(const char *filename, int num_images);

int display_images(const char *filename, int num_images) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "display_images Error: could not open file '%s' for reading\n", filename);
        return -1;
    }

    // Read the file header
    uint32_t magic_number, num_images_file, rows, cols;
    fread(&magic_number, sizeof(uint32_t), 1, fp);
    magic_number = ntohl(magic_number);  // convert to host byte order
    fread(&num_images_file, sizeof(uint32_t), 1, fp);
    num_images_file = ntohl(num_images_file);  // convert to host byte order
    fread(&rows, sizeof(uint32_t), 1, fp);
    rows = ntohl(rows);  // convert to host byte order
    fread(&cols, sizeof(uint32_t), 1, fp);
    cols = ntohl(cols);  // convert to host byte order

    //VTR: Print the file header. Is it really flattened?
    printf("display_images VTR: magic_number = 0x%08X  num_images_file = %d   rows = %d   cols = %d\n",
        magic_number, num_images_file, rows, cols);

    // Check that the number of images in the file matches the expected number
    if ((int)num_images_file < num_images) {
        fprintf(stderr, "display_images Error: file '%s' contains only %d images (expected at least %d)\n",
                filename, num_images_file, num_images);
        return -1;
    }

    // Display the images as ASCII art
    for (int i = 0; i < num_images; i++) {
        printf("display_images Image %d:\n", i);
        for (int r = 0; r < 28; r++) {
            for (int c = 0; c < 28; c++) {
                uint8_t pixel;
                fread(&pixel, sizeof(uint8_t), 1, fp);
                if (pixel > 128) {
                    printf("#");
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    fclose(fp);
    return 0;
}
