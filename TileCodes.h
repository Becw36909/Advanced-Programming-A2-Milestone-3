
#ifndef ASSIGN2_TILECODES_H
#define ASSIGN2_TILECODES_H

#include <unordered_map>

// Colours
#define RED 'R'
#define ORANGE 'O'
#define YELLOW 'Y'
#define GREEN 'G'
#define BLUE 'B'
#define PURPLE 'P'

// Shapes
#define CIRCLE 1
#define STAR_4 2
#define DIAMOND 3
#define SQUARE 4
#define STAR_6 5
#define CLOVER 6

// ANSI Color Codes
#define ANSI_COLOR_RESET "\x1b[0m"
const std::unordered_map<char, std::string> colorCodes = {
    {RED, "\x1b[31m"},    {ORANGE, "\x1b[38;5;214m"},  // No standard ANSI code,
                                                       // using extended color
    {YELLOW, "\x1b[33m"}, {GREEN, "\x1b[32m"},        {BLUE, "\x1b[34m"},
    {PURPLE, "\x1b[35m"}

};

#endif  // ASSIGN1_TILECODES_H
