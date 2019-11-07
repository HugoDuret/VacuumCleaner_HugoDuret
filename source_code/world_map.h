/*
* Name : Hugo Duret
* Student ID : 20555806
* Assigment : Project 1 - Vacuum Cleaner - Fall 2019
*/

#ifndef WORLD_MAP_H
#define WORLD_MAP_H

class world_map
{
private:
    unsigned int *** grid= nullptr;
    unsigned int size;
    unsigned int nb_dirty_squares_start=0;
public:
    world_map(unsigned int size, unsigned int percentage_dirty);
    ~world_map();
    unsigned int get_size() const {return size;}
    unsigned int get_nb_dirty_squares_start() const {return nb_dirty_squares_start;}
    unsigned int get_square_state(unsigned int i, unsigned int j)const {return grid[i][j][0] ;}
    unsigned int get_square_location(unsigned int i, unsigned int j)const {return grid[i][j][1] ;}
    void set_square_state(unsigned int i, unsigned int j, unsigned int s)const {this->grid[i][j][0] = s ;}
    void set_nb_dirty_squares_start(unsigned int n) {this->nb_dirty_squares_start = n ;}
};

#endif // WORLD_MAP_H
