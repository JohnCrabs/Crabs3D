## Synopsis

A c3d format file viewer created under Qt environment.

## Code Example

/*****************************************/
/*               Templates               */
/*****************************************/

namespace sys_endian {

    const unsigned one = 1U;

    inline bool little_endian() {
        return reinterpret_cast<const char*>(&one) + sizeof(unsigned) - 1;
    }

    inline bool big_endian() {
        return !little_endian();
    }

} //sys_endian

//SwapEndian
template <typename T>
T swapEndian(const T numIn) {
    T numOut;
    int size = sizeof(T);

    char* numToConvert = (char*) & numIn;
    char* numToReturn = (char*) & numOut;

    for(int i = 0, j = size-1; j >= 0; i++, j--)
        numToReturn[i] = numToConvert[j];

    return numOut;
}

//SwapEndian
template <typename T>
T returnByte(const T numIn, const int bit) {
    T numOut;

    int size = sizeof(T);

    char* numToConvert = (char*) & numIn;
    char* numToReturn = (char*) & numOut;

    for(int i = 0; i < size; i++)
         numToReturn[i] = 0;

    numToReturn[0] = numToConvert[bit-1];

    return numOut;
}

//ReadWord
template <typename T>
void readWord(T* word, const int bytes, FILE* file) {
    char* wordToRead = (char*) word;

    if((int)sizeof(T) > bytes)
        for(int i = 0; i < bytes; i++)
            wordToRead[i+1] = 0;

    for(int i = 0; i < bytes; i++) {
        wordToRead[i] = 0;
        fread(&wordToRead[i], 1, 1, file);
    }
}

//ReadWordWithEndian
template <typename T>
void readWordWithEndian(T* word, const int bytes, FILE* file, const int endian_flag) {
    char* wordToRead = (char*) word;

    if((int)sizeof(T) >= bytes)
        for(int i = 0; i < bytes; i++)
            wordToRead[i+1] = 0;

    if(endian_flag == SAME_ENDIAN)
        for(int i = 0; i < bytes; i++) {
            fread(&wordToRead[i], 1, 1, file);
        }
    else if(endian_flag == DIFF_ENDIAN) {
        if(bytes > SIZE_8_BIT)
            for(int i = bytes-1; i >= 0; i--) {
                wordToRead[i] = 0;
                fread(&wordToRead[i], 1, 1, file);
            }
        else
            for(int i = 0; i < bytes; i++) {
                wordToRead[i] = 0;
                fread(&wordToRead[i], 1, 1, file);
            }
    }
}

## Motivation

This project created as my Diploma Project for graduating from Rural Surveying of National Technical University of Athens. http://www.survey.ntua.gr/en/

## Installation - Compilation

1)Open the project with QtCreator
2)Build
3)Run

## Tests

This program can read almost every c3d file.

## Contributors

Currently only me.

## License
Copyright (C) Feb 2018 John Crabs <kavouras.ioannis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
