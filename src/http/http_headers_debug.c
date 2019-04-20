#include "http_headers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <str_stack.h>

#define MAX_PRINT_BUFFER    50

char* header_pointers_to_string(http_header_t* header);
char* header_meaningful_buffer_part_dump(http_header_t* header);

char* http_header_to_string(http_header_t* header)
{
    //semaphore, as we use a global buffer
/*    stack_head_t* stack = stack_init();
    str_stack_push(stack, header_pointers_to_string(header));
    for(int i = 0; i < NUM_HTTP_HEADERS; i++) {
        if(header->headers[i]->num_elems > 0){
            char* keyValues = header_key_to_string(header->headers[i]);
            str_stack_push(stack, keyValues);
        }
    }
    str_stack_push(stack, header_meaningful_buffer_part_dump(header));
    return str_stack_2_print_lines(stack);*/
return NULL;
}

#define MAX_HEADER_POINTER_LINE  500

char tmp_buffer[MAX_HEADER_POINTER_LINE];

char* header_pointers_to_string(http_header_t* header)
{

    sprintf(tmp_buffer, "fd:[%d], start of line:[%d], last semicolon:[%d], cur loc:[%d], max len:[%d]",
            header->fd, header->start_of_line, header->last_semicolon, header->cur_loc, header->max_len);
 /*   char* result = (char*) malloc(sizeof(char) * min(strlen(tmp_buffer), MAX_HEADER_POINTER_LINE));
    tmp_buffer[MAX_HEADER_POINTER_LINE -1] = '\0';
    strcpy(result, tmp_buffer);
    return result;*/
 return NULL;
}

#define MAX_BUFFER_DUMP_LINE    2048
char buffer_dump[MAX_BUFFER_DUMP_LINE];

char* header_meaningful_buffer_part_dump(http_header_t* header)
{
    //Extract the data between min(start_of_line, last_semicolon, cur_loc and the min(max_len, CONSTANT)
//    int start_char = min(min(header->start_of_line, header->last_semicolon), header->cur_loc);
    int j = 0;
/*    for(int i=start_char, i < limit; i++){
        char* car_dump = caracter_to_string(header->buff[i]);
        str_append(buffer_dump, car_dump);
        j++;
    }*/
  //  char* dest_buffer = (char*)malloc(sizeof(char) * min(MAX_PRINT_BUFFER, header->max_len - start_char) + 1);
    //strncpy(dest_buffer, header->buff + start_char, (header->max_len < MAX_PRINT_BUFFER)?(header->max_len):(MAX_PRINT_BUFFER -3));

  //  return dest_buffer;
  return NULL;
}