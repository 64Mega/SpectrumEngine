// mio.h
// A.K.A Mega's IO functions, 'cause I felt like it.
// The most important functions here are getline and fgetline
// --
// Also includes some hefty though handy string handlers.
// --

#ifndef MIO_H
#define MIO_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <string.h>
#include <fstream>

namespace MIO
{

std::string getline();

bool same(std::string str1, std::string str2);

std::string fgetline(std::ifstream& in);

void printvs(std::vector<std::string> dat);

/// split() function for std::string.
/// Useful. VERY useful.
/// Returns a vector that contains each of the components separated by delim.

std::vector<std::string> split(std::string str, char delim);

/// strip() function for std::string.
/// Removes unwanted characters from the string and returns the stripped copy.

std::string strip(std::string str, char delim);

/// These two functions are really useful, and necessary for a lot of
/// basic string manipulation (For ease of use anyway).

std::string upper(std::string str);

std::string lower(std::string str);

bool haschar(std::string str, char d);

}
#endif

