#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "serialize.h"


int main() {
    
    struct temp {
        int i;
        ByteStream s;
        float j;
    };

    FieldFormat temp_format[]  = {
        { offsetof(struct temp, i), F_INT },
        { offsetof(struct temp, s), F_ByteStream },
        { offsetof(struct temp, j), F_FLOAT },
    };

    char str[] = "abcd";
    struct temp t1 = { 543, { sizeof(str), (uint8_t *) str}, 66.12345};

    int field_count = sizeof(temp_format) / sizeof(FieldFormat);
    uint8_t buf[serializeCalculateSize(&t1, temp_format, field_count)];
    
    memset(buf, 0, sizeof(buf));
    size_t data_size = serialize_struct(&t1, temp_format, field_count, buf);

    printf("Size of temp: %ld\n", sizeof(t1));
    printf("Size of data: %ld\n", data_size);
    // printf("Data: %x\n", *buf);

    struct temp out;
    deserialize_struct(buf, temp_format, field_count, &out);
    
    char text[out.s.size];
    readByteStream(&out.s, text);

    printf("Text: %s\n", text);

    return 0;
}
