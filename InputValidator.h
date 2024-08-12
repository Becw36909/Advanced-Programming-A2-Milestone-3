#ifndef ASSIGN2_INPUTVALIDATOR_H
#define ASSIGN2_INPUTVALIDATOR_H

#include <string>

class InputValidator {
public:
    // Validate player name
    static bool isValidName(const std::string& name);

    // Validate file name
    static bool isFileNameValid(const std::string& filename);

    // Validate file format is acceptable
    static bool isFileFormatValid(const std::string& data);

    // Validate if place tile is correct format
    static bool isValidPlaceCommand(const std::string& input);
};

#endif // ASSIGN2_INPUTVALIDATOR_H
