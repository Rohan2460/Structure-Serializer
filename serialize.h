#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    F_INT,
    F_FLOAT,
    F_ByteStream
} FieldType;

typedef struct {
    size_t offset;
    FieldType type;
} FieldFormat;

typedef struct {
    int size;
    uint8_t *ptr;
} ByteStream;

size_t serialize_struct(void *data, FieldFormat *struct_format, int field_count, uint8_t *buffer);
size_t deserialize_struct(uint8_t *data, FieldFormat *struct_format, int field_count, void *output);
void readByteStream(ByteStream *source, char *buff);
size_t serializeCalculateSize(void* struct_ptr, FieldFormat *struct_format, int field_count);

#endif