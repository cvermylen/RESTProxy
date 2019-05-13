#include "http_headers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <str_stack.h>

#define MIN(a,b) (a < b)?(a):(b)

#define MAX_PRINT_BUFFER    50

char* header_pointers_to_string(http_header_t* header);
char* header_meaningful_buffer_part_dump(http_header_t* header);
char* header_key_to_string(int i, stack_head_t* head);

char* http_header_to_string(http_header_t* header)
{
    //semaphore, as we use a global buffer
    stack_head_t* stack = stack_init();
    str_stack_push(stack, header_pointers_to_string(header));
    for(int i = 0; i < NUM_HTTP_HEADERS; i++) {
        if(header->headers[i]->num_elems > 0){
            char* keyValues = header_key_to_string(i, header->headers[i]);
            str_stack_push(stack, keyValues);
        }
    }
    str_stack_push(stack, header_meaningful_buffer_part_dump(header));
    return str_stack_2_print_lines(stack);
}

char* header_key_to_string(int i, stack_head_t* head) {
    int size = strlen(HTTP_HEADER_STRINGS[i]);
    int* elem_sizes = (int*) malloc((sizeof (int)) * (head->num_elems+ 1));
    elem_sizes[0] = size;
    stack_node_t* e = head->root;
    for(int i = 0; i < head->num_elems; i++) {
        elem_sizes[i+1] = strlen(e->elem);
        size += elem_sizes[i+1];
        e = e->next;
    }
    char* result = (char*) malloc(((sizeof(char)) * size) + 2);
    strcpy(result, HTTP_HEADER_STRINGS[i]);
    e = head->root;
    for(int i = 0; i < head->num_elems; i++) {
        strcpy(&result[elem_sizes + i + 1], e->elem);
        e = e->next;
    }
    result[size] = 0x00;
    return result;
}

#define MAX_HEADER_POINTER_LINE  500

char tmp_buffer[MAX_HEADER_POINTER_LINE];

char* header_pointers_to_string(http_header_t* header)
{

    sprintf(tmp_buffer, "fd:[%d], start of line:[%d], last semicolon:[%d], cur loc:[%d], max len:[%d]",
            header->fd, header->start_of_line, header->last_semicolon, header->cur_loc, header->max_len);
    char* result = (char*) malloc(sizeof(char) * MIN(strlen(tmp_buffer), MAX_HEADER_POINTER_LINE));
    tmp_buffer[MAX_HEADER_POINTER_LINE -1] = '\0';
    strcpy(result, tmp_buffer);
    return result;
}

#define MAX_BUFFER_DUMP_LINE    2048
char buffer_dump[MAX_BUFFER_DUMP_LINE];
int dump_pos = 0;
char* character_to_string(char c);
int str_append(char[] dest, int start_pos, char* src);

char* header_meaningful_buffer_part_dump(http_header_t* header)
{
    //Extract the data between min(start_of_line, last_semicolon, cur_loc and the min(max_len, CONSTANT)
    int start_char = MIN(MIN(header->start_of_line, header->last_semicolon), header->cur_loc);
    int j = 0;
    for(int i=start_char; i < limit; i++){
        char* car_dump = character_to_string(header->buff[i]);
        dump_pos = str_append(buffer_dump, dump_pos, car_dump);
        j++;
    }
    char* dest_buffer = (char*)malloc(sizeof(char) * MIN(MAX_PRINT_BUFFER, header->max_len - start_char) + 1);
    strncpy(dest_buffer, header->buff + start_char, (header->max_len < MAX_PRINT_BUFFER)?(header->max_len):(MAX_PRINT_BUFFER -3));

    return dest_buffer;
}

int str_append(char[] dest, int start_pos, char* src) {
    int i = start_pos;
    for(; i < MAX_BUFFER_DUMP_LINE && src[i] != 0x00; i++) {
        dest[i] = src[i - start_pos];
    }
    return i;
}

int isCharacterPrintable(char c);

char* character_to_string(char c) {
    char* result;
    if (isCharacterPrintable(c)) {
        result = (char*)malloc(sizeof(char) * 2);
        result[0] = c;
        result[1] = 0x00;
    }else{
        result = (char*)malloc(sizeof(char) * 8);
        result[0] = '\'';
        result[1] = '\\';
        sprintf(&result[2], "%x", c);
        result[7] = '\'';
;       result[8] = 0x00;
    }

    return result;
}

int isCharacterPrintable(char c) {
    if ((c >= 0 && c <= 9) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return 1;
    }

    return 0;
}