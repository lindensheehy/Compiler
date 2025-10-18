#pragma once


bool floatToString(float value, char* string, int MAXLENGTH, int MAXDECIMALDIGITS);
bool stringToFloat(const char* string, float* result, int MAXLENGTH);

bool doubleToString(double value, char* string, int MAXLENGTH, int MAXDECIMALDIGITS);
bool stringToDouble(const char* string, double* result, int MAXLENGTH);

bool intToString(int value, char* string, int MAXLENGTH);
bool stringToInt(const char* string, int* result, int MAXLENGTH);

bool stringHexToInt(const char* string, int* result, int MAXLENGTH);
