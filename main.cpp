// #include <stdio.h>
// #include <libavformat/avformat.h>

// int main(int argc, char* argv[]) {
//     if (argc < 2) {
//         printf("Usage: %s <input_file>\n", argv[0]);
//         return 1;
//     }

//     const char* input_filename = argv[1];
//     AVFormatContext* format_ctx = NULL;

//     if (avformat_open_input(&format_ctx, input_filename, NULL, NULL) < 0) {
//         fprintf(stderr, "Could not open input file %s\n", input_filename);
//         return 1;
//     }

//     if (avformat_find_stream_info(format_ctx, NULL) < 0) {
//         fprintf(stderr, "Failed to retrieve input stream information\n");
//         return 1;
//     }

//     printf("Input file: %s\n", input_filename);
//     printf("Duration: %ld seconds\n", format_ctx->duration / AV_TIME_BASE);

//     avformat_close_input(&format_ctx);
//     return 0;
// }

#include <iostream>

int main(int argc, char * argv[]){
    std::cout<<"Hello, World!"<<std::endl;
    return 0;
}