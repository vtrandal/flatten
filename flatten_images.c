#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#define NUM_IMAGES 10000  // number of images to display

int read_idx_header(const char *filename);
int read_mnist_images(const char *filename, uint8_t *images, int num_images);
int flatten_images(const char *filename, uint8_t *images, int num_images);
int display_images(const char *filename, int num_images);

int flatten_images(const char *filename, uint8_t *images, int num_images) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "flatten_images Error: could not open file '%s' for writing\n", filename);
        return -1;
    }

    // Write the file header
    uint32_t magic_number = 0x00000803;
    magic_number = htonl(magic_number);  // convert to big-endian byte order
    fwrite(&magic_number, sizeof(uint32_t), 1, fp);

    uint32_t num_images_file = num_images;
    num_images_file = htonl(num_images_file);  // convert to big-endian byte order
    fwrite(&num_images_file, sizeof(uint32_t), 1, fp);

    uint32_t rows_inet = 1, cols_inet = 28 * 28;
    rows_inet = htonl(rows_inet);  // convert to big-endian byte order
    cols_inet = htonl(cols_inet);  // convert to big-endian byte order
    fwrite(&rows_inet, sizeof(uint32_t), 1, fp);
    fwrite(&cols_inet, sizeof(uint32_t), 1, fp);

    uint32_t rows = 1, cols = 28 * 28;
    // Flatten the images to 1D and write to file
    for (int i = 0; i < num_images; i++) {
        for (int j = 0; j < (int)(rows * cols); j++) {
            uint8_t pixel = images[i * rows * cols + j];
            fwrite(&pixel, sizeof(uint8_t), 1, fp);
        }
    }

    fclose(fp);
    return 0;
}

