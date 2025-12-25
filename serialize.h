#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stddef.h>
#include <stdint.h>

#define FIELD(parent_struct, member, type, count)  { type, offsetof(parent_struct, member), count }
#define FIELD_STRUCT(parent_struct, member, count, member_id) { F_STRUCT, offsetof(parent_struct, member), count, member_id }


typedef enum {
    F_INT32,
    F_FLOAT32,
    F_STRUCT,
    F_BYTE,
} FieldType;

typedef struct {
    FieldType type;   
    uint32_t offset;
    uint32_t count;
    uint16_t struct_id; 
} FieldDesc;

typedef struct {
    int id;
    const FieldDesc *fields;
    int field_count;
} StructType;

static StructType registeredStructs[100];

int struct_register(int struct_id, FieldDesc *struct_desc, int field_count);


size_t serialize_struct(void *data, int struct_id, uint8_t *buffer);
size_t deserialize_struct(uint8_t *data, int struct_id, void *output);
// void readByteStream(ByteStream *source, char *buff);
size_t serializeCalculateSize(void* struct_ptr, FieldDesc *struct_format, int field_count);

#endif