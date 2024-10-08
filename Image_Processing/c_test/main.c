#include "image_process.h"

FILE            *open_file(char *filename, char *flag);
unsigned char   *get_jpeg_buffer(FILE *org_file);
void            exit_err(char *msg);

int main(int argc, char **argv)
{
    unsigned char   *org_buffer;
    unsigned char   *res_buffer;
    char            *org_file_name;
    char            *res_file_name;
    int             org_height;
    int             org_width;
    int             res_height;
    FILE            *org_file;
    FILE            *res_file;

    if (argc != 3)
        exit_err("usage: ./image_process <original file> <result file>");
    org_file_name = strdup(argv[1]);
    res_file_name = strdup(argv[2]);
	printf("Original Filename: %s / Result Filename: %s\n", org_file_name, res_file_name);
    org_file = open_file(org_file_name, "rb");
    if (org_file == NULL)
        exit_err("Failed to open original file.");
    org_buffer = get_jpeg_buffer(org_file);
    res_buffer = resize_image(org_buffer, &org_width, &org_height, RESULT_WIDTH, &res_height);
    res_file = open_file(res_file_name, "wb");
    if (res_file == NULL)
        exit_err("Failed to create result file.");
    write_1bit_bmp(res_file, res_buffer, RESULT_WIDTH, res_height);
    free(org_buffer);
    free(org_file_name);
    free(res_file_name);
	return (0);
}

FILE    *open_file(char *filename, char *flag)
{
    FILE *tmp_file;

    tmp_file = fopen(filename, flag);
    return tmp_file;
}

unsigned char   *get_jpeg_buffer(FILE *org_file)
{
    unsigned char   *image_buffer;
    unsigned char   *image_row_buffer;
    int             image_height;
    int             image_width;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, org_file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    image_height = cinfo.output_height;
    image_width = cinfo.output_width;
    image_buffer = (unsigned char *)malloc(image_height * image_width * sizeof(unsigned char));
    image_row_buffer = (unsigned char *)malloc(image_width * cinfo.output_components);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, &image_row_buffer, 1);
        for (int x = 0; x < image_width; x++)
        {
            image_buffer[(cinfo.output_scanline - 1) * image_width + x] =
                (image_row_buffer[x * 3] + image_row_buffer[x * 3 + 1] + image_row_buffer[x * 3 + 2]) / 3;
        }
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(image_row_buffer);
    return (image_buffer);
}

void    exit_err(char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}
