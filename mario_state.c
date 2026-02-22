/*========================================================
 * main.c
 *
 * Test harness for Mario state bit packing.
 *========================================================*/

#include <stdint.h>
#include <stdio.h>
#include "mario_state.h"

#define X_TILE_BITS 6U
#define X_TILE_MAX ((1U << X_TILE_BITS) - 1U)
#define X_TILE_MASK (X_TILE_MAX)
#define X_TILE_SHIFT 0U

#define X_SUB_BITS 4U
#define X_SUB_MAX ((1U << X_SUB_BITS) - 1U)
#define X_SUB_SHIFT 6U
#define X_SUB_MASK (X_SUB_MAX << X_SUB_SHIFT)

#define Y_TILE_BITS 5U
#define Y_TILE_MAX ((1U << Y_TILE_BITS) - 1U)
#define Y_TILE_SHIFT 2U
#define Y_TILE_MASK (Y_TILE_MAX << Y_TILE_SHIFT)

#define VELOCITY_Y_BITS 5U
#define VELOCITY_Y_MIN (-(1 << (VELOCITY_Y_BITS - 1U)))
#define VELOCITY_Y_MAX ((1 << (VELOCITY_Y_BITS - 1U)) - 1)
#define VELOCITY_Y_SHIFT_LOW 7U
#define VELOCITY_Y_SHIFT_HIGH 0U
#define VELOCITY_Y_MASK_LOW (1U << VELOCITY_Y_SHIFT_LOW)
#define VELOCITY_Y_MASK_HIGH 0x0FU

/*========================================================
 * API — YOU IMPLEMENT EVERYTHING
 *========================================================*/

void mario_snapshot_clear(mario_snapshot_t *snap);
void mario_snapshot_set_position(mario_snapshot_t *snap, uint8_t x_tile, uint8_t x_sub, uint8_t y_tile);
void mario_snapshot_set_velocity(mario_snapshot_t *snap, int8_t velocity_y);
uint8_t mario_snapshot_compute_checksum(const mario_snapshot_t *snap);
uint8_t mario_snapshot_is_valid(const mario_snapshot_t *snap);
void mario_snapshot_serialize(const mario_snapshot_t *snap, uint8_t out[3]);
void mario_snapshot_deserialize(mario_snapshot_t *snap, const uint8_t in[3]);

int main(void)
{
    mario_state_t mario;

    mario.power = MARIO_POWER_STAR;
    mario.direction = MARIO_DIR_LEFT;
    mario.enemy = ENEMY_GOOMBA;
    mario.is_jumping = 0;
    mario.is_running = 1;
    mario.level = 3;
    mario.lives = 3;

    printf("Mario state size: %zu bytes\n", MARIO_STATE_SIZE_BYTES);
    printf("Lives: %u, Level: %u\n", mario.lives, mario.level);

    /* Test snapshot functions */
    printf("\n=== Testing Snapshot Functions ===\n");
    
    mario_snapshot_t snap1, snap2;
    uint8_t buffer[3];
    
    /* Clear and set position/velocity */
    mario_snapshot_clear(&snap1);
    mario_snapshot_set_position(&snap1, 10, 5, 8);
    mario_snapshot_set_velocity(&snap1, -5);
    
    printf("Snapshot 1 raw bytes: [0x%02X, 0x%02X, 0x%02X]\n", 
           snap1.raw[0], snap1.raw[1], snap1.raw[2]);
    
    /* Compute and display checksum */
    uint8_t checksum = mario_snapshot_compute_checksum(&snap1);
    printf("Computed checksum: 0x%X\n", checksum);
    
    /* Serialize with checksum */
    mario_snapshot_serialize(&snap1, buffer);
    printf("Serialized buffer: [0x%02X, 0x%02X, 0x%02X]\n", 
           buffer[0], buffer[1], buffer[2]);
    
    /* Deserialize into new snapshot */
    mario_snapshot_clear(&snap2);
    mario_snapshot_deserialize(&snap2, buffer);
    printf("Deserialized raw:  [0x%02X, 0x%02X, 0x%02X]\n", 
           snap2.raw[0], snap2.raw[1], snap2.raw[2]);
    
    /* Validate checksums */
    uint8_t valid1 = mario_snapshot_is_valid(&snap1);
    uint8_t valid2 = mario_snapshot_is_valid(&snap2);
    printf("Snapshot 1 valid: %u, Snapshot 2 valid: %u\n", valid1, valid2);

    return 0;
}

/* Clear snapshot safely */
void mario_snapshot_clear(mario_snapshot_t *snap) {
    if (snap == NULL) return;
    // memset(&snap->raw,0,sizeof(snap->raw)); -> Needs <string.h> and also not embedded graded
    for (size_t i = 0U; i < sizeof(snap->raw); i++) {
        snap->raw[i] = 0U;
    }
}

static uint8_t clamp_x_tile(uint8_t x_tile) {
    return (x_tile > X_TILE_MAX) ? X_TILE_MAX : x_tile;
}

static uint8_t clamp_x_sub(uint8_t x_sub) {
    return (x_sub > X_SUB_MAX) ? X_SUB_MAX : x_sub;
}

static uint8_t clamp_y_tile(uint8_t y_tile) {
    return (y_tile > Y_TILE_MAX) ? Y_TILE_MAX : y_tile;
}

/* Set position with bounds enforcement */
void mario_snapshot_set_position(mario_snapshot_t *snap, uint8_t x_tile, uint8_t x_sub, uint8_t y_tile)
{
    if (snap == NULL) return;
    // snap->bits.x_tile = x_tile;
    // snap->bits.x_sub = x_sub;
    // snap->bits.y_tile = y_tile;

    x_tile = clamp_x_tile(x_tile);
    snap->raw[0] &= (uint8_t)~X_TILE_MAX;
    snap->raw[0] |= (uint8_t)(x_tile << X_TILE_SHIFT);

    x_sub = clamp_x_sub(x_sub);
    uint8_t x_sub_low = (uint8_t)(x_sub & 0x03U);
    uint8_t x_sub_high = (uint8_t)((x_sub>>2) & 0x03U);
    snap->raw[0] &= (uint8_t)~(0x03U << X_SUB_SHIFT);
    snap->raw[0] |= (uint8_t)(x_sub_low << X_SUB_SHIFT);
    snap->raw[1] &= (uint8_t)~(0x03U);
    snap->raw[1] |= x_sub_high;

    y_tile = clamp_y_tile(y_tile);
    snap->raw[1] &= (uint8_t)~(Y_TILE_MASK);
    snap->raw[1] |= (uint8_t)(y_tile << Y_TILE_SHIFT);
}

static void clamp_velocity_y(int8_t *velocity_y) {
    if (velocity_y == NULL) return;
    if (*velocity_y > VELOCITY_Y_MAX) {
        *velocity_y = VELOCITY_Y_MAX;
    } else if (*velocity_y < VELOCITY_Y_MIN) {
        *velocity_y = VELOCITY_Y_MIN;
    }
}

/* Set vertical velocity (signed, clamped) */
void mario_snapshot_set_velocity(mario_snapshot_t *snap, int8_t velocity_y)
{
    if (snap == NULL) return;
    clamp_velocity_y(&velocity_y);
    uint8_t encoded = velocity_y & 0x1F;

    uint8_t velocity_y_low = encoded & 1U;
    uint8_t velocity_y_high = (encoded >> 1U) & 0x0FU;

    snap->raw[1] &= (uint8_t)~(VELOCITY_Y_MASK_LOW);
    snap->raw[1] |= (uint8_t)(velocity_y_low << VELOCITY_Y_SHIFT_LOW);
    snap->raw[2] &= (uint8_t)~(VELOCITY_Y_MASK_HIGH);
    snap->raw[2] |= velocity_y_high;
}

/* Compute 4-bit XOR checksum over first 20 bits */
uint8_t mario_snapshot_compute_checksum(const mario_snapshot_t *snap)
{
    if (snap == NULL) return 0;
    
    /* XOR five 4-bit groups from the 20-bit data field */
    uint8_t group1 = snap->raw[0] & 0x0FU;
    uint8_t group2 = (snap->raw[0] >> 4U) & 0x0FU;
    uint8_t group3 = snap->raw[1] & 0x0FU;
    uint8_t group4 = (snap->raw[1] >> 4U) & 0x0FU;
    uint8_t group5 = snap->raw[2] & 0x0FU;  /* Only data bits, not checksum */
    
    return (group1 ^ group2 ^ group3 ^ group4 ^ group5) & 0x0FU;
}

/* Validate checksum */
uint8_t mario_snapshot_is_valid(const mario_snapshot_t *snap)
{
    if (snap == NULL) return 0;
    
    /* Extract stored checksum from bits 4-7 of raw[2] */
    uint8_t stored_checksum = (snap->raw[2] >> 4U) & 0x0FU;
    uint8_t computed_checksum = mario_snapshot_compute_checksum(snap);
    
    return (stored_checksum == computed_checksum) ? 1U : 0U;
}

/* Serialize to buffer (explicit endian control) */
void mario_snapshot_serialize(const mario_snapshot_t *snap, uint8_t out[3])
{
    if (snap == NULL || out == NULL) return;
    
    /* Copy data bytes */
    out[0] = snap->raw[0];
    out[1] = snap->raw[1];
    
    /* Compute checksum and store in bits 4-7 of byte 3 */
    uint8_t checksum = mario_snapshot_compute_checksum(snap);
    out[2] = (snap->raw[2] & 0x0FU) | ((checksum & 0x0FU) << 4U);
}

/* Deserialize from buffer */
void mario_snapshot_deserialize(mario_snapshot_t *snap, const uint8_t in[3])
{
    if (snap == NULL || in == NULL) return;
    
    /* Copy from input buffer to snapshot */
    snap->raw[0] = in[0];
    snap->raw[1] = in[1];
    snap->raw[2] = in[2];
}