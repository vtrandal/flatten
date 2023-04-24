#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#define NUM_IMAGES 10000  // number of images to display

int read_idx_header(const char *filename);
int read_mnist_images(const char *filename, uint8_t *images, int num_images);
int flatten_images(const char *filename, uint8_t *images, int num_images);
int display_images(const char *filename, int num_images);

int read_mnist_images(const char *filename, uint8_t *images, int num_images) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "read_mnist_images Error: could not open file '%s'\n", filename);
        return -1;
    }

    // Read the magic number
    uint32_t magic_number;
    fread(&magic_number, sizeof(uint32_t), 1, fp);
    magic_number = ntohl(magic_number);
    printf("read_mnist_images Magic number: 0x%08X\n", magic_number);

    if (magic_number != 0x00000803) {
        fprintf(stderr, "read_mnist_images Error: invalid magic number for '%s'\n", filename);
        fclose(fp);
        return -1;
    }

    // Read the number of images
    uint32_t num_images_file;
    fread(&num_images_file, sizeof(uint32_t), 1, fp);
    num_images_file = ntohl(num_images_file);
    printf("read_mnist_images Number of images: %d\n", num_images_file);

    if ((int)num_images_file < num_images) {
        fprintf(stderr, "read_mnist_images Warning: '%s' only contains %d images (requested %d)\n",
                filename, num_images_file, num_images);
        num_images = num_images_file;
    }

    // Seek to byte 16 to skip the header
    fseek(fp, 16, SEEK_SET);

    // Read the image data
    uint32_t rows = 28, cols = 28;
    uint8_t buf[rows * cols];
    for (int i = 0; i < num_images; i++) {
        fread(buf, sizeof(uint8_t), rows * cols, fp);
        for (int j = 0; j < (int)(rows * cols); j++) {
            images[i * rows * cols + j] = buf[j];
        }
    }

    fclose(fp);
    return num_images;
}
