#include "LedTable.h"

struct CharDataStreamTuple {
    char ascii;
    const char* dataStream;
};

static const CharDataStreamTuple gCharDataStreamTable[] = {
    {' ',
    ""},
    {'A',
    "01110"
    "10001"
    "10001"
    "10001"
    "11111"
    "10001"
    "10001"},
    {'B',
    "11110"
    "10001"
    "10001"
    "11110"
    "10001"
    "10001"
    "11110"},
    {'C',
    "01110"
    "10001"
    "10000"
    "10000"
    "10000"
    "10001"
    "01110"},
    {'D',
    "11110"
    "10001"
    "10001"
    "10001"
    "10001"
    "10001"
    "11110"},
    {'E',
    "11111"
    "10000"
    "10000"
    "11110"
    "10000"
    "10000"
    "11111"},
    {'F',
    "11111"
    "10000"
    "10000"
    "11110"
    "10000"
    "10000"
    "10000"}
};

const char* lookupChar(char ascii) {
    static const unsigned int tableSize = sizeof(gCharDataStreamTable)/sizeof(gCharDataStreamTable[0]);
    for (unsigned int index = 0; index < tableSize; ++index) {
        const CharDataStreamTuple& tuple = gCharDataStreamTable[index];
        if (tuple.ascii == ascii) {
            return tuple.dataStream;
        }
    }

    return 0;
}
