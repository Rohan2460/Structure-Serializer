#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "serialize.h"

#define arrayLength(array) (sizeof(array) / sizeof(array[0]))


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
        char text[6];
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
    

    size_t vec3_size = register_struct(S_Vec3, vec3_desc, arrayLength(vec3_desc));
    if(vec3_size == 0) {
        printf("Failed to register struct S_Vec3\n");
        return -1;
    }
    
    FieldDesc player_desc[] = {
        FIELD(Player_t, id, F_INT32, 1),
        FIELD(Player_t, items, F_INT32, 3),
        FIELD_STRUCT(Player_t, position, 1, S_Vec3),
        FIELD(Player_t, text, F_BYTE, 6)
    };

    size_t player_size = register_struct(S_Player, player_desc, arrayLength(player_desc));
    if (player_size == 0) {
        printf("Failed to register struct S_Player\n");
        return -1;
    }

    Vec3_t v1 = {10, 5, 11};
    Player_t p1 = {8, {3, 4, 5}, v1, "Hello"};

    uint8_t buf[vec3_size];
    memset(buf, 0, sizeof(buf));

    if (serialize_struct(&v1, S_Vec3, buf) != vec3_size) {
        printf("Serialize error S_Vec3\n");
        return -1;
    }

    uint8_t buf2[player_size];
    memset(buf2, 0, sizeof(buf2));

    if (serialize_struct(&p1, S_Player, buf2) != player_size) {
        printf("Serialize error S_Player\n");
        return -1;
    }

    Player_t out;
    size_t return_size = deserialize_struct(buf2, S_Player, &out);

    if(return_size == 0 && return_size != player_size) {
        printf("De-serialize error S_Player\n");
        return -1;
    }


    return 0;
}
