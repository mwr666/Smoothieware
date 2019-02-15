#pragma once

#include <stdint.h>

/**
 * Icons
 *
 * Made with Marlin Bitmap Converter
 * http://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from the file 'firmware-logo.png'
 */

#define icon_width  8
#define icon_height 8

static const uint8_t hotend_icon[] = {
  0x7C, // .#####..
  0x10, // ...#....
  0x7C, // .#####..
  0x10, // ...#....
  0x38, // ..###...
  0x38, // ..###...
  0x10, // ...#....
  0x00  // ........
};

static const uint8_t bed_icon[] = {
  0x24, // ..#..#..
  0x48, // .#..#...
  0x24, // ..#..#..
  0x48, // .#..#...
  0x24, // ..#..#..
  0x00, // ........
  0xFE, // #######.
  0x00  // ........
};

static const uint8_t fan_icon[] = {
  0x00, // ........
  0x4C, // .#..##..
  0x68, // .##.#...
  0x10, // ...#....
  0x2C, // ..#.##..
  0x64, // .##..#..
  0x00, // ........
  0x00  // ........
};

static const uint8_t mcu_icon[] = {

  0x41, // .#.....#
  0x63, // .##...##
  0x55, // .#.#.#.#
  0x49, // .#..#..#
  0x00, // ........
  0x75, // .###.#.#
  0x45, // .#...#.#
  0x77  // .###.###
};

static const uint8_t mot_icon[] = {

  0x41, // .#.....#
  0x63, // .##...##
  0x55, // .#.#.#.#
  0x49, // .#..#..#
  0x00, // ........
  0x77, // .###.###
  0x52, // .#.#..#.
  0x72  // .###..#.
};


static const uint8_t speed_icon[] = {
  0x90, // #..#....
  0x48, // .#..#...
  0x24, // ..#..#..
  0x12, // ...#..#.
  0x24, // ..#..#..
  0x48, // .#..#...
  0x90, // #..#....
  0x00  // ........
};

static const uint8_t time_icon[] = {
  0x38, // ..###...
  0x44, // .#...#..
  0x92, // #..#..#.
  0x9E, // #..####.
  0x82, // #.....#.
  0x44, // .#...#..
  0x38, // ..###...
  0x00  // ........
};

static const uint8_t plus_icon[] = {
  0x00, // ........
  0x18, // ...##...
  0x18, // ...##...
  0x7E, // .######.
  0x7E, // .######.
  0x18, // ...##...
  0x18, // ...##...
  0x00  // ........
};


static const uint8_t minus_icon[] = {
  0x00, // ........
  0x00, // ........
  0x00, // ........
  0x7E, // .######.
  0x7E, // .######.
  0x00, // ........
  0x00, // ........
  0x00  // ........
};



static const uint8_t up_icon[] = {
  0x10, // ...#....
  0x38, // ..###...
  0x54, // .#.#.#..
  0x92, // #..#..#.
  0x10, // ...#....
  0x10, // ...#....
  0x10, // ...#....
  0x10  // ...#....
};

static const uint8_t down_icon[] = {
  0x10, // ...#....
  0x10, // ...#....
  0x10, // ...#....
  0x10, // ...#....
  0x92, // #..#..#.
  0x54, // .#.#.#..
  0x38, // ..###...
  0x10  // ...#....
};

static const uint8_t left_icon[] = {
  0x10, // ...#....
  0x20, // ..#.....
  0x40, // .#......
  0xFF, // ########
  0x40, // .#......
  0x20, // ..#.....
  0x10, // ...#....
  0x00  // ........
};

static const uint8_t right_icon[] = {
  0x08, // ....#...
  0x04, // .....#..
  0x02, // ......#.
  0xFF, // ########
  0x02, // ......#.
  0x04, // .....#..
  0x08, // ....#...
  0x00  // ........
};

static const uint8_t front_icon[] = {
  0x01, // .......#
  0x02, // ......#.
  0x04, // .....#..
  0x88, // #...#...
  0x90, // #..#....
  0xA0, // #.#.....
  0xC0, // ##......
  0xF8  // #####...
};

static const uint8_t back_icon[] = {
  0x1F, // ...#####
  0x03, // ......##
  0x05, // .....#.#
  0x09, // ....#..#
  0x10, // ...#....
  0x20, // ..#.....
  0x40, // .#......
  0x80  // #.......
};

static const uint8_t ok_icon[] = {
  0x00, // ........
  0x49, // .#..#..#
  0xAA, // #.#.#.#.
  0xAC, // #.#.##..
  0xAA, // #.#.#.#.
  0x49, // .#..#..#
  0x00, // ........
  0x00  // ........
};

// TODO: Remove once new watch screen is supported on all displays
static const uint8_t large_icons[] = { // 16x80 - he1, he2, he3, bed, fan
    0x3f, 0xfc, 0x3f, 0xfc, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0x7f, 0x7f, 0x7e, 0x3f, 0x7c, 0x1f,
    0x78, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80,
    0x01, 0x80, 0x3f, 0xfc, 0x3f, 0xfc, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0x7f, 0x7c, 0x7e, 0x3d,
    0xfc, 0x1c, 0x78, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80,
    0x01, 0x80, 0x01, 0x80, 0x3f, 0xfc, 0x3f, 0xfc, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0x7f, 0x7c,
    0x7e, 0x3f, 0x7c, 0x1c, 0x78, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00,
    0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x08, 0x88, 0x11, 0x10, 0x22, 0x20, 0x22,
    0x20, 0x11, 0x10, 0x08, 0x88, 0x04, 0x44, 0x04, 0x44, 0x08, 0x88, 0x11, 0x10, 0x22, 0x20,
    0x00, 0x00, 0x7f, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0x39, 0xec, 0x43, 0xe2, 0x9b, 0xc9, 0xa3,
    0x85, 0x03, 0x85, 0xc3, 0x00, 0xe0, 0x3e, 0xf9, 0xbf, 0xfd, 0x9f, 0x7c, 0x07, 0x00, 0xc3,
    0xa1, 0xc0, 0xa1, 0xc5, 0x93, 0xd9, 0x47, 0xc2, 0x37, 0x9c
};
