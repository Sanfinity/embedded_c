/*========================================================
 * mario_state.h
 *
 * Bitfield-based packet to store Mario's runtime state.
 * Intended for learning / embedded / save-game usage.
 *
 * NOTE:
 *  - Bitfield layout is compiler-dependent.
 *  - Safe only when encoding/decoding on same platform.
 *========================================================*/

#ifndef MARIO_STATE_H
#define MARIO_STATE_H

#include <stdint.h>

/* Mario power-up state */
typedef enum {
    MARIO_POWER_SMALL = 0,
    MARIO_POWER_SUPER,
    MARIO_POWER_FIRE,
    MARIO_POWER_STAR
} mario_power_t;

typedef enum {
    MARIO_DIR_LEFT = 0,
    MARIO_DIR_RIGHT
} mario_direction_t;

typedef enum {
    ENEMY_NONE = 0,
    ENEMY_GOOMBA,
    ENEMY_KOOPA,
    ENEMY_BOWSER
} mario_enemy_t;

/*--------------------------------------------------------
 * Mario packed state structure
 *
 * Total size target: 16 bits (2 bytes)
 *--------------------------------------------------------*/
typedef struct
{
    uint16_t power : 2;
    uint16_t direction : 1;
    uint16_t is_jumping : 1;
    uint16_t is_running : 1;
    uint16_t lives : 3; // Remaining lives (0–7)
    uint16_t level : 4; // Current level number (0–15)
    uint16_t enemy : 2;
    uint16_t reserved : 2; // Reserved for future expansion (padding)

} mario_state_t;

typedef struct
{
    uint32_t x_tile      : 6;   /* Horizontal tile index */
    uint32_t x_sub       : 4;   /* Sub-tile precision */
    uint32_t y_tile      : 5;   /* Vertical tile index */

    int32_t  velocity_y  : 5;   /* Signed vertical velocity */

    uint32_t power       : 2;   /* mario_power_t */
    uint32_t on_ground   : 1;   /* Physics flag */
    uint32_t enemy_near  : 1;   /* Proximity sensor */

    uint32_t checksum    : 4;   /* XOR of previous nibbles */

} mario_snapshot_bits_t;

typedef union {
    mario_snapshot_bits_t bits;
    uint8_t raw[3];
} mario_snapshot_t;

/*--------------------------------------------------------
 * Helper macros (optional)
 *--------------------------------------------------------*/

/* Size check (use static_assert in C11 if you want) */
#define MARIO_STATE_SIZE_BYTES (sizeof(mario_state_t))

#endif /* MARIO_STATE_H */
