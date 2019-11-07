/*
* Name : Hugo Duret
* Student ID : 20555806
* Assigment : Project 1 - Vacuum Cleaner - Fall 2019
*/

#include "agents.h"
#include  <cstdlib> // for rand()
#include  <QString>

agents::agents()
{

}

// Directions are given clockwise with integers (1 : North, 2 : East, 3 : South, 4 : West)

unsigned int strategy_one(unsigned int location, unsigned int last_move) {
    //The vacuum cleaner will repeat the following steps in that order until all
    //dirty squares are cleaned:
    //    1)      Keep moving to the east until it hits the east boundary.
    //    2)      Move one square to the south. If the south boundary is reached, go to (5).
    //    3)      Keep moving to the west until it hits the west boundary.
    //    4)      Move one square to the south and go to (1). If the south boundary is reached, go to (6).
    //    5)      Keep moving to the north until it hits the north boundary and then go to (3)
    //    6)      Keep moving to the north until it hits the north boundary and then go to (1)

    // move east if possible, and not currently moving west
    if (location != 3 and location != 6 and location != 7 and last_move != 4 and last_move != 1) {
        return 2;
    }
    // move south if possible, and not currently moving south or north
    if (location != 8 and location != 4 and location != 7 and last_move != 3 and last_move != 4 and last_move != 1) {
        return 3;
    }
    // move west if possible
    if (location != 1 and location != 5 and location != 8 and last_move != 1) {
        return 4;
    }
    // move south if possible, and not currently moving south or north
    if (location != 8 and location != 4 and location != 7 and last_move != 3 and last_move != 1) {
        return 3;
    }
    // move north if possible
    if (location != 2 and location != 5 and location != 6) {
        return 1;
    }
    // Only when north edge reached
    return 0;

}

unsigned int strategy_two() {

    // From the current square, the vacuum cleaner will move to one of the four
    // neighboring squares, to the east, west, north or south, with equal probability (1/4).

    int number_of_directions = 4;

    unsigned int randNum =  static_cast<unsigned int>(rand()%number_of_directions) + 1;

    return randNum;
}

unsigned int agents::get_direction(unsigned int strategy, unsigned int location, unsigned int last_move) {
    unsigned int direction = 0;

    switch (strategy) {
    case 1:
        direction = strategy_one(location, last_move);
        break;
    case 2:
        direction = strategy_two();
        break;
    default:
        throw QString("Error: strategy not valid in get_direction() : " + static_cast<QString>(strategy));
    }

    return direction;
}










