/*
* Name : Hugo Duret
* Student ID : 20555806
* Assigment : Project 1 - Vacuum Cleaner - Fall 2019
*/

#include "world_map.h"
#include  <cstdlib> // for rand()

/*
 * Initializes the 2d grid containing the information
 * on the status and the location of every square
 * */
world_map::world_map(unsigned int size, unsigned int percentage_dirty) : size(size)
{
    // grid is a matrix where every element is an array of two values (status, location)
    this->grid = new unsigned int ** [size];
    for(unsigned int i = 0 ; i < size ; i++) {
        grid[i] = new unsigned int * [size];
        for(unsigned int j = 0 ; j < size ; j++) {
            grid[i][j] = new unsigned int [2];
        }
    }

    // ***** location's values *****
    // by default the values are 0
    for(unsigned int i = 0 ; i < size ; i++) {
        for(unsigned int j = 0 ; j < size ; j++) {
            grid[i][j][1] = 0;
        }
    }

    // west and east edges
    for(unsigned int i = 0 ; i < size ; i++) {
        grid[i][0][1] = 1;
        grid[i][size-1][1] = 3;
    }

    // north and south edges
    for(unsigned int j = 0 ; j < size ; j++) {
        grid[0][j][1] = 2;
        grid[size-1][j][1] = 4;
    }
    // corners
    grid[0][0][1] = 5;
    grid[0][size-1][1] = 6;
    grid[size-1][0][1] = 8;
    grid[size-1][size-1][1] = 7;

    // ***** status' values *****
    unsigned int nb_dirty_squares_start = 0;
    for(unsigned int i = 0 ; i < size ; i++) {
        for(unsigned int j = 0 ; j < size ; j++) {
            unsigned int rand_value =  static_cast<unsigned int>(rand()) % 100 + 1;   // in the range 1 to 100
            if (rand_value <= percentage_dirty) {
                this->set_square_state(i, j, 1);
                nb_dirty_squares_start+= 1;
            }
        }
    }

    this->set_nb_dirty_squares_start(nb_dirty_squares_start);
}

world_map::~world_map() {
    for(unsigned int i = 0 ; i < get_size() ; i++) {
        for(unsigned int j = 0 ; j < get_size() ; j++) {
            delete[] grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;

}












