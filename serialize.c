#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "serialize.h"


// Reads a struct, format of that struct, number of fields in the struct and buffer to write to 
// Returns size parsed from data buffer
size_t serialize_struct(void *data, FieldFormat *struct_format, int field_count, uint8_t *buffer) {
    uint8_t *buff_start = buffer;

    for (int i = 0; i < field_count; i++) {
        void *field = (uint8_t *) data + struct_format[i].offset;

        switch(struct_format[i].type) {

            case F_INT: {
                memcpy(buffer, field, sizeof(int));
                buffer += sizeof(int);
                break;
            }
            case F_FLOAT: {
                memcpy(buffer, field, sizeof(float));
                buffer += sizeof(float);
                break;
            }
            case F_ByteStream: {
                // offset of fields is unknown so assign it to struct 
                ByteStream str;      
                memcpy(&str, field, sizeof(str));

                memcpy(buffer, &str.size, sizeof(int));
                buffer += sizeof(int);
                
                memcpy(buffer, str.ptr, str.size);
                buffer += str.size;
                break;
            }
        }
    }

    size_t buff_size = (size_t) (buffer - buff_start);
    return buff_size;
}

// Reads the serialized data, struct format, field count of the struct, and output struct
// Returns the size parsed from data buffer
size_t deserialize_struct(uint8_t *data, FieldFormat *struct_format, int field_count, void *output) {
    void *field = (uint8_t *) data;
    void *buff_start = field;

    for (int i = 0; i < field_count; i++) {
        output += struct_format[i].offset;

        switch(struct_format[i].type) {

            case F_INT: {
                memcpy(output, field, sizeof(int));
                field += sizeof(int);
                break;
            }
            case F_FLOAT: {
                memcpy(output, field, sizeof(float));
                field += sizeof(float);
                break;
            }
            case F_ByteStream: {
                ByteStream str;

                int str_len;
                memcpy(&str_len, field, sizeof(int));
                field += sizeof(int);

                str.size = str_len;
                str.ptr = (uint8_t *) field;   // points to buffer 

                // copy struct to output
                memcpy(output, &str, sizeof(str));

                field += str_len;
                break;
            }
        }
    }

    size_t buff_size = (size_t) (field - buff_start);
    return buff_size;
}

void readByteStream(ByteStream *source, char *buff) {
    memcpy(buff, source->ptr, source->size);
}

size_t serializeCalculateSize(void *struct_ptr, FieldFormat *struct_format, int field_count) {
    size_t size = 0;

    for (int i = 0; i < field_count; i++) {
        switch(struct_format[i].type) {
            case F_INT:
                size += sizeof(int);
                break;
            
            case F_FLOAT:
                size += sizeof(float);
                break;
            case F_ByteStream:
                size += sizeof(int);
                int data_size = *(int *) (struct_ptr + struct_format[i].offset);
                size += data_size;
                break;
        }
    }

    return size;
}
