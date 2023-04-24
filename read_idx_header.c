#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

int read_idx_header(const char *filename);
int read_mnist_images(const char *filename, uint8_t *images, int num_images);
int flatten_images(const char *filename, uint8_t *images, int num_images);
int display_images(const char *filename, int num_images);

int read_idx_header(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "read_idx_header Error: could not open file '%s'\n", filename);
        return -1;
    }

    // Printing the header information for the IDX file
    printf("read_idx_header Printing the header information for the IDX file %s\n", filename);

    // Read the magic number
    uint32_t magic_number;
    fread(&magic_number, sizeof(uint32_t), 1, fp);
    magic_number = ntohl(magic_number);

    // Read the number of images or labels
    uint32_t num_items;
    fread(&num_items, sizeof(uint32_t), 1, fp);
    num_items = ntohl(num_items);

    // Read the number of rows and columns for images, or skip 8 bytes for labels
    uint32_t rows = 0, cols = 0;
    if (magic_number == 0x00000803) {
        fread(&rows, sizeof(uint32_t), 1, fp);
        rows = ntohl(rows);
        fread(&cols, sizeof(uint32_t), 1, fp);
        cols = ntohl(cols);
    } else if (magic_number == 0x00000801) {
        fseek(fp, 8, SEEK_CUR);
    } else {
        fprintf(stderr, "read_idx_header Error: invalid magic number(0x%08X) for '%s'\n", magic_number, filename);
        fclose(fp);
        return -1;
    }

    printf("read_idx_header Magic number: 0x%08X\n", magic_number);
    printf("read_idx_Number of items: %d\n", num_items);
    if (rows != 0 && cols != 0) {
        printf("read_idx_header Image dimensions: %d x %d\n", rows, cols);
    }

    fclose(fp);
    return 0;
}
