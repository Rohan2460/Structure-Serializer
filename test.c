#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "serialize.h"

#define arraySize(array, type) (sizeof(array) / sizeof(type))

int main() {
    
    typedef struct Vec3 {
        int x;
        int y;
        int z;
    } Vec3_t;

    typedef struct Player {
        int id;
        int items[3];
        Vec3_t position;
        char text[10];
    } Player_t;

    enum StructEnum {
        S_Vec3,
        S_Player
    };

    FieldDesc vec3_desc[] = {
        FIELD(Vec3_t, x, F_INT32, 1),
        FIELD(Vec3_t, y, F_INT32, 1),
        FIELD(Vec3_t, z, F_INT32, 1),
    };
    
    struct_register(S_Vec3, vec3_desc, arraySize(vec3_desc, FieldDesc));
    
    FieldDesc player_desc[] = {
        FIELD(Player_t, id, F_INT32, 1),
        FIELD(Player_t, items, F_INT32, 3),
        FIELD_STRUCT(Player_t, position, 1, S_Vec3),
        FIELD(Player_t, text, F_BYTE, 10)
    };

    struct_register(S_Player, player_desc, 4);

    Vec3_t v1 = {10, 5, 11};
    Player_t p1 = {8, {3, 4, 5}, v1, "Hello"};

    uint8_t buf[sizeof(v1)];
    memset(buf, 0, sizeof(buf));
    serialize_struct(&v1, S_Vec3, buf);

    uint8_t buf2[sizeof(p1)];
    memset(buf2, 0, sizeof(buf2));
    serialize_struct(&p1, S_Player, buf2);

    Player_t out;
    deserialize_struct(buf2, S_Player, &out);


    return 0;
}
