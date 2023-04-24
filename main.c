#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#define NUM_IMAGES 10000  // number of images to display

int read_idx_header(const char *filename);
int read_mnist_images(const char *filename, uint8_t *images, int num_images);
int flatten_images(const char *filename, uint8_t *images, int num_images);
int display_images(const char *filename, int num_images);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "main Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Print the header information for the input IDX file
    printf("main Input file: %s\n", argv[1]);
    read_idx_header(argv[1]);

    uint8_t images[NUM_IMAGES * 28 * 28];
    int num_images = read_mnist_images(argv[1], images, NUM_IMAGES);
    if (num_images < 0) {
        fprintf(stderr, "main Error: failed to read images from '%s'\n", argv[1]);
        return 1;
    }

#if 0
    display_images(argv[1], NUM_IMAGES);
#else
    // Write the flattened images to a new IDX file
    printf("\nmain Flattening %d images to output.idx\n", num_images);

    flatten_images("output.idx", images, NUM_IMAGES);
    display_images("output.idx", NUM_IMAGES);

    // Print the header information for the output IDX file
    printf("main Output file: output.idx\n");
    read_idx_header("output.idx");
#endif

    return 0;
}
