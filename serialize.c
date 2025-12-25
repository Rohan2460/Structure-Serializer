#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "serialize.h"


// Reads a struct, format of that struct, number of fields in the struct and buffer to write to 
// Returns size parsed from data buffer
size_t serialize_struct(void *data, int struct_id, uint8_t *buffer) {
    uint8_t *buff_start = buffer;
    
    StructType *fmt = &registeredStructs[struct_id];
    int field_count = fmt->field_count;
    

    for (int i = 0; i < field_count; i++) {
        void *field = (uint8_t *) data + fmt->fields[i].offset;
        int array_count = fmt->fields[i].count;
        
        switch(fmt->fields[i].type) {

            case F_INT32: {
                for(int i = 0; i < array_count; i++) {     
                    memcpy(buffer, field, sizeof(uint32_t));
                    field += sizeof(uint32_t);
                    buffer += sizeof(uint32_t);
                }
                break;
            }
            case F_FLOAT32: {
                for(int i = 0; i < array_count; i++) { 
                    memcpy(buffer, field, sizeof(uint32_t));
                    field += sizeof(uint32_t);
                    buffer += sizeof(uint32_t);
                }
                break;
            }
            case F_BYTE: {
                // offset of fields is unknown so assign it to struct 
                for(int i = 0; i < array_count; i++) {     
                    memcpy(buffer, field, sizeof(uint8_t));
                    buffer += sizeof(uint8_t);
                    field += sizeof(uint8_t);
                }
                break;
            }
            case F_STRUCT: {
                size_t parsed_size = serialize_struct(field, fmt->fields[i].struct_id, buffer);
                buffer += parsed_size;
                break;
            }
        }
    }

    size_t buff_size = (size_t) (buffer - buff_start);
    return buff_size;
}

// Reads the serialized data, struct format, field count of the struct, and output struct
// Returns the size parsed from data buffer
size_t deserialize_struct(uint8_t *data, int struct_id, void *output_struct) {
    void *field = (uint8_t *) data;
    void *buff_start = field;

    StructType *fmt = &registeredStructs[struct_id];
    int field_count = fmt->field_count;

    for (int j = 0; j < field_count; j++) {
        uint8_t *output = (uint8_t *) output_struct + fmt->fields[j].offset;
        int array_count = fmt->fields[j].count;

        switch(fmt->fields[j].type) {

            case F_INT32: {
                for(int i = 0; i < array_count; i++) {
                    memcpy(output, field, sizeof(uint32_t));
                    output += sizeof(uint32_t);
                    field += sizeof(uint32_t);
                }
                break;
            }
            case F_FLOAT32: {
                for(int i = 0; i < array_count; i++) {    
                    memcpy(output, field, sizeof(uint32_t));
                    output += sizeof(uint32_t);
                    field += sizeof(uint32_t);
                }
                break;
            }
            case F_BYTE: {
                for(int i = 0; i < array_count; i++) {    
                    memcpy(output, field, sizeof(uint8_t));
                    output += sizeof(uint8_t);
                    field += sizeof(uint8_t);
                }
                break;
            }
            case F_STRUCT: {
                size_t parsed_size = deserialize_struct(field, fmt->fields[j].struct_id, output);
                field += parsed_size;
                break;
            }
        }
    }

    size_t buff_size = (size_t) (field - buff_start);
    return buff_size;
}

// void readByteStream(ByteStream *source, char *buff) {
//     memcpy(buff, source->ptr, source->size);
// }

// size_t serializeCalculateSize(void *struct_ptr, FieldFormat *struct_format, int field_count) {
//     size_t size = 0;

//     for (int i = 0; i < field_count; i++) {
//         switch(struct_format[i].type) {
//             case F_INT:
//                 size += sizeof(int);
//                 break;
            
//             case F_FLOAT:
//                 size += sizeof(float);
//                 break;
//             case F_ByteStream:
//                 size += sizeof(int);
//                 int data_size = *(int *) (struct_ptr + struct_format[i].offset);
//                 size += data_size;
//                 break;
//         }
//     }

//     return size;
// }


int struct_register(int struct_id, FieldDesc *struct_desc, int field_count) {
    static int id = 0;
    if (struct_id < id)
        return -1;
    
    registeredStructs[id++] = (StructType) { id, struct_desc, field_count };
    return 0;
}