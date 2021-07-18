#ifndef DEFINES_H
#define DEFINES_H

#include <QColor>

namespace qnono {

    // Puzzle sizes
    #define MIN_PUZZLE_SIZE 3
    #define MAX_PUZZLE_SIZE 30

    // default button sizes
    #define DEFAULT_PUZZLE_SIZE 20
    #define DEFAULT_BUTTON_SIZE 20

    // default colors
    #define DEFAULT_COLOR_SOLID "#323296"
    #define DEFAULT_COLOR_BLANK "#D7D7D7"
    #define DEFAULT_COLOR_UNDECIDED "#FFFFFF"
    #define DEFAULT_COLOR_HINT_SOLID "#9696FF"
    #define DEFAULT_COLOR_HINT_BLANK "#FF0000"

    // Color struct
    struct paintValues {
        QColor solid;
        QColor blank;
        QColor undecided;
        QColor hint_solid;
        QColor hint_blank;
    };

    // status
    #define STATUS_UNDECIDED 2
    #define STATUS_BLANK 1
    #define STATUS_SOLID 0
    #define STATUS_HINT_BLANK 11
    #define STATUS_HINT_SOLID 10

}

#endif // DEFINES_H
