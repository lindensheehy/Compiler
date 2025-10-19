#include "core/casting.h"

bool floatToString(float value, char* string, int MAXLENGTH, int MAXDECIMALDIGITS) {
    return doubleToString(static_cast<double>(value), string, MAXLENGTH, MAXDECIMALDIGITS);
}

bool stringToFloat(const char* string, float* result, int MAXLENGTH) {

    if (result == nullptr) return false;

    double doubleResult;
    const bool passed = stringToDouble(string, &doubleResult, MAXLENGTH);
    *result = static_cast<float>(doubleResult);

    return passed; 

}

bool doubleToString(double value, char* string, int MAXLENGTH, int MAXDECIMALDIGITS) {

    if (string == nullptr) return false;

    // Not enough space in the string for a number and '\0'
    if (MAXLENGTH < 2) return false;

    // Using intToString to handle the integer portion of the double
    const int intPart = static_cast<int>((value >= 0) ? value : -value);
    bool intPartSucceded;

    // Add a negative sign if needed
    if (value < 0) {
        string[0] = '-';
        intPartSucceded = intToString(intPart, &(string[1]), MAXLENGTH - 1);
    }

    else {
        intPartSucceded = intToString(intPart, string, MAXLENGTH);
    }

    if (!intPartSucceded) return false;
    
    // Find index where intToString left off
    int index = 0;
    while (string[index] != '\0') index++;

    // Handle decimal part if necessary
    if (MAXDECIMALDIGITS > 0) {

        // Check length before starting
        if (index >= MAXLENGTH - 1) return false;

        string[index] = '.';
        index++;

        double absValue = (value < 0) ? -value : value;
        double absIntPart = (intPart < 0) ? -intPart : intPart;
        
        double decimalPart = absValue - absIntPart;
        int decimalDigit;

        // Add all the requested decimal digits
        for (int i = 0; i < MAXDECIMALDIGITS; ++i) {

            decimalPart *= 10;
            decimalDigit = static_cast<int>(decimalPart);
            decimalPart -= decimalDigit;

            if (index >= MAXLENGTH - 1) return false;

            string[index++] = '0' + decimalDigit;

        }

    }

    // Null-terminate the string
    if (index >= MAXLENGTH) return false;
    string[index] = '\0';

    return true;

}

bool stringToDouble(const char* string, double* result, int MAXLENGTH) {

    if (string == nullptr) return false;
    if (result == nullptr) return false;

    double returnValue = 0.0;
    int i = 0;
    char c;
    int holder; // Holds the int equivalent of the current char being parsed. ex. '1' -> 1
    bool negative = false;
    double decimalPlace = 0.1;

    // Check for negative
    if (string[i] == '-') {
        negative = true;
        i++;
    }

    while (i < MAXLENGTH) {

        c = string[i];

        // Break cases

        // Done
        if (c == '\0') goto end;

        // Leave this loop to do the loop for the decimal part
        if (c == '.') break;

        // Invalid char
        if (c < '0' || c > '9') return false;

        holder = c - '0';
        returnValue = (returnValue * 10) + holder;

        i++;

    }

    i++; // Skip the decimal where the index currently is

    while (i < MAXLENGTH) {

        c = string[i];

        // Break Cases

        // Done
        if (c == '\0') goto end;

        // Invalid char
        if (c < '0' || c > '9') return false;

        holder = c - '0';
        returnValue += decimalPlace * holder;

        decimalPlace /= 10;
        i++;

    }

    end:
    *result = (negative) ? -returnValue : returnValue;
    return true;

}

bool intToString(long long value, char* string, int MAXLENGTH) {

    // Not enough space in the string
    if (MAXLENGTH < 2) return false;

    if (value == 0) {
        string[0] = '0';
        string[1] = '\0';
        return true;
    }

    // Determine the sign and get the absolute value
    const bool isNegative = value < 0;
    int absValue = isNegative ? -value : value;

    // Calculate the number of digits
    int numDigits = 0;
    int tempValue = absValue;
    while (tempValue > 0) {
        tempValue /= 10;
        numDigits++;
    }

    // If the number is negative, we need one extra space for the '-' sign
    const int strLength = numDigits + (isNegative ? 1 : 0);
    
    // Return false if theres not enough room in the string for the value (+1 for '\0')
    if (strLength + 1 > MAXLENGTH) return false;

    // Fill the string with characters

    string[strLength] = '\0'; // Null-terminate the string

    for (int i = strLength - 1; i >= 0; i--) {
        string[i] = (absValue % 10) + '0'; // Convert digit to character
        absValue /= 10;
    }

    // If the number is negative, add the '-' sign
    if (isNegative) {
        string[0] = '-';
    }

    return true;

}

bool stringToInt(const char* string, int* result, int MAXLENGTH) {

    if (string == nullptr) return false;
    if (result == nullptr) return false;

    int returnValue = 0;
    int i = 0;
    char c;
    int holder; // Holds the int equivalent of the current char being parsed. ex. '1' -> 1
    bool negative = false;

    // Check for negative
    if (string[i] == '-') {
        negative = true;
        i++;
    }

    while (i < MAXLENGTH) {

        c = string[i];

        // Break cases

        // Done
        if (c == '\0') break;

        // Invalid char
        if (c < '0' || c > '9') return false;

        holder = c - '0';
        returnValue = (returnValue * 10) + holder;

        i++;

    }

    *result = (negative) ? -returnValue : returnValue;
        
    return true;

}

bool stringHexToInt(const char* string, int* result, int MAXLENGTH) {

    if (string == nullptr) return false;
    if (result == nullptr) return false;

    int returnValue = 0;
    int i = 0;
    char c;
    int holder;

    // Check for optional "0x" or "0X" prefix
    if (string[i] == '0' && (string[i + 1] == 'x' || string[i + 1] == 'X')) {
        i += 2;
    }

    while (i < MAXLENGTH) {

        c = string[i];

        // Done
        if (c == '\0') break;

        // Convert hex character to integer
        if (c >= '0' && c <= '9') {
            holder = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            holder = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            holder = c - 'a' + 10;
        } else {
            return false; // Invalid character for hex
        }

        returnValue = (returnValue * 16) + holder;
        i++;

    }

    *result = returnValue;

    return true;
    
}
