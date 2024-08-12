#include "InputValidator.h"
#include <cctype>
#include <regex>

bool InputValidator::isValidName(const std::string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!std::isupper(c)) return false;
    }
    return true;
}

bool InputValidator::isFileNameValid(const std::string& filename) {
    // Check if filename is not empty and contains only valid characters ending with .txt
    std::regex validFileNamePattern("^[a-zA-Z0-9./_-]+\\.txt$");
    bool isValid = !filename.empty() && std::regex_match(filename, validFileNamePattern);
    return isValid;
}

// Check if the file format is valid according to the specified game format
bool InputValidator::isFileFormatValid(const std::string& data) {
std::regex validFileFormatPattern(
    "^[a-zA-Z0-9 ]+\n"               // Player 1 name
    "\\d+\n"                         // Player 1 score
    "([A-Z][1-6],)*[A-Z][1-6]?\n"    // Player 1 hand
    "[a-zA-Z0-9 ]+\n"                // Player 2 name
    "\\d+\n"                         // Player 2 score
    "([A-Z][1-6],)*[A-Z][1-6]?\n"    // Player 2 hand
    "\\d+,\\d+\n"                    // Board size
    "([A-Z][1-6]@\\w\\d,)*([A-Z][1-6]@\\w\\d)\n" // Board tiles
    "([A-Z][1-6],)*[A-Z][1-6]?\n"    // Tile bag contents
    "[a-zA-Z0-9 ]+$"                 // Current player name
);

    return std::regex_match(data, validFileFormatPattern);
}
