/*
* Name : Hugo Duret
* Student ID : 20555806
* Assigment : Project 1 - Vacuum Cleaner - Fall 2019
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QTimer>
#include "world_map.h"
#include "agents.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const unsigned int square_size = 10 ; // size in pixels
    unsigned int size = 10;
    const unsigned int nb_moves_limit = 100 * size * size ; // limits to nb_moves for one simulation
    bool simulation_is_running = false;
    unsigned int nb_squares_cleaned = 0;
    unsigned int percentage_dirty = 25;
    unsigned int strategy_choice =1;
    unsigned int nb_moves = 0;
    unsigned int vacuum_cleaner_position_i;
    unsigned int vacuum_cleaner_position_j;
    unsigned int delay = 1000;

    unsigned int last_move = 0; // hack explained in the report

    // GUI
    QTableWidget * grid_window = nullptr;
    void init_grid_window();
    void update_grid_window(world_map * grid_data);

    // Data structure
    world_map * grid_data = nullptr;
    agents * agent = nullptr;

    void move_vacuum_cleaner(unsigned int strategy_choice);
    bool is_move_valid(unsigned int direction);
    unsigned int launch_simulation(bool is_benchmark);
    unsigned int benchmarking_specific(unsigned int size, unsigned int percentage_dirty, unsigned int strategy);
    void benchmarking_manager();
    void update_gui_values();

    void set_delay(unsigned int d) {this->delay = d;}
    void set_size(unsigned int n) {this->size = n;}
    void set_simulation_is_running(bool b) {this->simulation_is_running = b;}
    void set_nb_squares_cleaned(unsigned int n) {this->nb_squares_cleaned = n;}
    void set_percentage_dirty(unsigned int p) {this->percentage_dirty = p;}
    void set_strategy_choice(unsigned int s) {this->strategy_choice = s;}
    void set_nb_moves(unsigned int n) {this->nb_moves = n;}
    void set_vacuum_cleaner_position_i(unsigned int i) {this->vacuum_cleaner_position_i = i;}
    void set_vacuum_cleaner_position_j(unsigned int j) {this->vacuum_cleaner_position_j = j;}
    void set_last_move(unsigned int n) {this->last_move = n;}

    unsigned int get_nb_moves_limit() const {return this->nb_moves_limit;}
    unsigned int get_delay() const {return this->delay;}
    unsigned int get_square_size() const {return this->square_size;}
    unsigned int get_size() const {return this->size;}
    bool get_simulation_is_running() const {return this->simulation_is_running;}
    unsigned int get_nb_squares_cleaned() const {return this->nb_squares_cleaned;}
    unsigned int get_percentage_dirty() const {return this->percentage_dirty;}
    unsigned int get_strategy_choice() const {return this->strategy_choice;}
    unsigned int get_nb_moves() const {return this->nb_moves;}
    unsigned int get_vacuum_cleaner_position_i() const {return this->vacuum_cleaner_position_i;}
    unsigned int get_vacuum_cleaner_position_j() const {return this->vacuum_cleaner_position_j;}
    unsigned int get_last_move() {return this->last_move;}

public slots:

private slots:
    void on_size_spinBox_valueChanged(int arg1);
    void on_strategy_spinBox_valueChanged(int arg1);
    void on_start_button_clicked();
    void on_stop_button_clicked();
    void on_percent_spinBox_valueChanged(int arg1);
    void on_quit_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_benchmark_button_clicked();
};
#endif // MAINWINDOW_H


