#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "serialize.h"

static StructDesc registeredStructs[MAX_STRUCT];

// Reads a struct, format of that struct, number of fields in the struct and buffer to write to 
// Returns size parsed from data buffer
size_t serialize_struct(void *struct_data, int struct_id, uint8_t *buffer) {
    uint8_t *buff_start = buffer;
    
    StructDesc *fmt = &registeredStructs[struct_id];
    int field_count = fmt->field_count;
    
    for (int i = 0; i < field_count; i++) {

        void *data = (uint8_t *) struct_data + fmt->fields[i].offset;
        int array_count = fmt->fields[i].count;
        size_t field_size = getFieldSize(fmt->fields[i].type, fmt->fields[i].struct_id);

        if (fmt->fields[i].type == F_STRUCT) {

            for (int j = 0; j < array_count; j++) {
                // Check for incorrect fields  
                size_t parsed_size = serialize_struct(data, fmt->fields[j].struct_id, buffer);
                if (parsed_size == 0 && field_size != parsed_size) 
                    return 0;

                buffer += parsed_size;
            }
        }
        else {

            field_size *= array_count;
            if (field_size == 0) 
                return 0;

            memcpy(buffer, data, field_size);
            data += field_size;
            buffer += field_size;
        }
    }

    size_t buff_size = (size_t) (buffer - buff_start);
    return buff_size;
}

// Reads the serialized data, struct format, field count of the struct, and output struct
// Returns the size parsed from data buffer
size_t deserialize_struct(uint8_t *struct_data, int struct_id, void *output_struct) {
    void *data = (uint8_t *) struct_data;
    void *buff_start = data;

    StructDesc *fmt = &registeredStructs[struct_id];
    int field_count = fmt->field_count;

    for (int i = 0; i < field_count; i++) {
        uint8_t *output = (uint8_t *) output_struct + fmt->fields[i].offset;
        int array_count = fmt->fields[i].count;
        size_t field_size = getFieldSize(fmt->fields[i].type, fmt->fields[i].struct_id);


        if (fmt->fields[i].type == F_STRUCT) {
            for(int j = 0; j < array_count; j++) {
                size_t parsed_size = deserialize_struct(data, fmt->fields[j].struct_id, output);
                if (parsed_size == 0 && field_size != parsed_size) 
                    return 0;

                data += parsed_size;
            }
        }
        else {
            field_size *= array_count;
            if (field_size == 0)
                return 0;

            memcpy(output, data, field_size);
            output += field_size;
            data += field_size;
        }
    }

    size_t buff_size = (size_t) (data - buff_start);
    return buff_size;
}



// Returns size of struct without padding 
size_t register_struct(int struct_id, FieldDesc *fields, int field_count) {
    static int id = 0;
    if (struct_id < id && struct_id >= MAX_STRUCT)
        return 0;
    
    size_t total_size = 0;
    for(int i = 0; i < field_count; i++) {
        FieldDesc field = fields[i];

        size_t size = getFieldSize(field.type, field.struct_id) * field.count;
        if (size == 0)
            return 0;

        total_size += size;
    }

    registeredStructs[id++] = (StructDesc) { id, fields, field_count, total_size };
    return total_size;   
}

size_t getFieldSize(FieldType type, int struct_id) {
    switch (type) {
        case F_INT32: return sizeof(uint32_t);
        case F_FLOAT32: return sizeof(uint32_t);
        case F_BYTE: return sizeof(uint8_t);
        case F_STRUCT: return registeredStructs[struct_id].size;
        default: return 0;
    }
}

