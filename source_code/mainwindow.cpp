/*
* Name : Hugo Duret
* Student ID : 20555806
* Assigment : Project 1 - Vacuum Cleaner - Fall 2019
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow() {
//    check all the arrays on construtor
    delete grid_data;
    delete grid_window;
    delete agent;

}

unsigned int MainWindow::benchmarking_specific(unsigned int size, unsigned int percentage_dirty, unsigned int strategy) {
    this->set_size(size);
    this->set_percentage_dirty(percentage_dirty);
    this->set_strategy_choice(strategy);

    unsigned int nb_moves = this->launch_simulation(true);

    return nb_moves;

}

void MainWindow::benchmarking_manager() {
    const unsigned int repeat_value = 5;
    const unsigned int world_side_size = this->get_size();
    const unsigned int nb_percentages = 3;
    const unsigned int nb_strategies = 2;
    const unsigned int percentages[nb_percentages] = {25, 50, 75};
    const unsigned int strategies[nb_strategies] = {1, 2};
    double * results[nb_strategies];

    // initialise the results array
    for (unsigned int s = 0; s < nb_strategies ; s++) {
        results[s] = new double[nb_percentages];
        for (unsigned int p = 0; p < nb_percentages; p++) {
            results[s][p] = 0.0;
        }
    }

    // compute the results
    for (unsigned int s = 0; s < nb_strategies ; s++) {
        for (unsigned int p = 0; p < nb_percentages; p++) {
            for (unsigned int i = 0; i < repeat_value; i++) {
                results[s][p] += static_cast<double>(benchmarking_specific(world_side_size, percentages[p], strategies[s]));
            }
            results[s][p] = results[s][p] / static_cast<double>(repeat_value);
        }
    }

    // Print the results on the GUI
    ui->centralwidget->findChild<QLabel*>("r_25_1")->setText(QString::number(results[0][0]));
    ui->centralwidget->findChild<QLabel*>("r_25_2")->setText(QString::number(results[1][0]));
    ui->centralwidget->findChild<QLabel*>("r_50_1")->setText(QString::number(results[0][1]));
    ui->centralwidget->findChild<QLabel*>("r_50_2")->setText(QString::number(results[1][1]));
    ui->centralwidget->findChild<QLabel*>("r_75_1")->setText(QString::number(results[0][2]));
    ui->centralwidget->findChild<QLabel*>("r_75_2")->setText(QString::number(results[1][2]));
}

unsigned int MainWindow::launch_simulation(bool is_benchmark = false) {
    // If it's a new simulation, we remove the last simulation
    if (this->grid_data != nullptr) {
        delete this->grid_data;
    }
    // and reset the GUI
    if (this->grid_window != nullptr) {
        this->grid_window->setColumnCount(0);
        this->grid_window->setRowCount(0);
        this->grid_window->reset();
    }

    unsigned int size = this->get_size();
    unsigned int percentage_dirty = this->get_percentage_dirty();
    unsigned int strategy_choice = this->get_strategy_choice();

    this->init_grid_window();
    this->set_nb_moves(0);
    this->set_nb_squares_cleaned(0);

    this->grid_data = new world_map(size, percentage_dirty);

    // ***** vacuum cleaner's position *****
    unsigned int i_position = static_cast<unsigned int>(rand()) % size;   // in the range 0 to size
    unsigned int y_position = static_cast<unsigned int>(rand()) % size;   // in the range 0 to size
    this->set_vacuum_cleaner_position_i(i_position);
    this->set_vacuum_cleaner_position_j(y_position);
    // if the square in which the vacuum cleaner starts is dirty, we clean it as a first move
    if(this->grid_data->get_square_state(i_position, y_position) == 1) {
        this->set_nb_moves(this->get_nb_moves()+1);
        this->set_nb_squares_cleaned(this->get_nb_squares_cleaned()+1);
    }
    this->grid_data->set_square_state(i_position, y_position, 3);

    unsigned int nb_dirty_squares_at_start = this->grid_data->get_nb_dirty_squares_start();
    while (this->get_nb_squares_cleaned() < nb_dirty_squares_at_start) {
        // In case of agent stuck, we need a limit
        if (this->get_nb_moves() > this->get_nb_moves_limit()) {
            return this->get_nb_moves_limit() ;
        }
        // delay is defined here to allow the user to change the speed during the simulation
        unsigned int delay = this->get_delay();

        // if the button "stop" is clicked, we quit
        if (this->get_simulation_is_running() == false and is_benchmark == false) {
            return 0;
        }

        // move_vacuum_cleaner() is responsible for counting the moves and the cleaning
        this->move_vacuum_cleaner(strategy_choice);

        if (is_benchmark == false) {
            this->update_grid_window(this->grid_data);

            // wait delay ms
            QEventLoop loop;
            QTimer::singleShot(static_cast<int>(delay), &loop, SLOT(quit()));
            loop.exec();
        }
    }
    if (is_benchmark == false) {
        ui->centralwidget->findChild<QLabel*>("completed")->setText(QString("COMPLETED"));
        this->set_simulation_is_running(false);
    }

    return this->get_nb_moves();
}

bool MainWindow::is_move_valid(unsigned int direction) {
    unsigned int size = grid_data->get_size();
    const unsigned int current_position_i = this->get_vacuum_cleaner_position_i();
    const unsigned int current_position_j = this->get_vacuum_cleaner_position_j();
    unsigned int new_position_i = current_position_i;
    unsigned int new_position_j = current_position_j;

    try {
        switch (direction) {
        case 1: //north
            new_position_i = current_position_i-1;
            break;
        case 2: //east
            new_position_j = current_position_j+1;
            break;
        case 3: //south
            new_position_i = current_position_i+1;
            break;
        case 4: //west
            new_position_j = current_position_j-1;
            break;
        default:
            return false;
        }
    } catch (...) {
        return false;
    }
    if (new_position_i >= size or new_position_j >= size) {
        return false;
    }

    return true;
}

void MainWindow::move_vacuum_cleaner(unsigned int strategy_choice) {
    const unsigned int old_position_i = this->get_vacuum_cleaner_position_i();
    const unsigned int old_position_j = this->get_vacuum_cleaner_position_j();
    unsigned int current_location_value = this->grid_data->get_square_location(old_position_i, old_position_j);
    unsigned int direction = agent->get_direction(strategy_choice, current_location_value, this->get_last_move());
    if (strategy_choice == 1) {
        this->set_last_move(direction);
    }

    // it the move is not valid, the vacuum cleaner is idle for this turn
    if (this->is_move_valid(direction) == false) {
        this->set_nb_moves(this->get_nb_moves() + 1);
        return;
    }

    switch (direction) {
    case 1: //north
        this->set_vacuum_cleaner_position_i(old_position_i-1);
        break;
    case 2: //east
        this->set_vacuum_cleaner_position_j(old_position_j+1);
        break;
    case 3: //south
        this->set_vacuum_cleaner_position_i(old_position_i+1);
        break;
    case 4: //west
        this->set_vacuum_cleaner_position_j(old_position_j-1);
        break;
    default:
        this->set_nb_moves(this->get_nb_moves() + 1);
        return;
    }

    const unsigned int new_position_i = this->get_vacuum_cleaner_position_i();
    const unsigned int new_position_j = this->get_vacuum_cleaner_position_j();
    // if the vacuum cleaner moved on a dirty square, it cleans it
    if (grid_data->get_square_state( new_position_i, new_position_j ) == 1 ) {
        this->set_nb_squares_cleaned(this->get_nb_squares_cleaned() + 1 );
    }

    grid_data->set_square_state(old_position_i, old_position_j, 2);
    grid_data->set_square_state(new_position_i, new_position_j, 3);
    this->set_nb_moves(this->get_nb_moves() + 1);
    return;
}

void MainWindow::init_grid_window() {
    unsigned int size = this->get_size();
    unsigned int square_size = this->get_square_size();

    this->grid_window = ui->centralwidget->findChild<QTableWidget*>("grid");
    for(int i = 0; i < static_cast<int>(size); ++i) {
        this->grid_window->model()->insertRow(i);
    }
    for(int j = 0; j < static_cast<int>(size); ++j) {
        this->grid_window->model()->insertColumn(j);
    }

    for(int i = 0; i < static_cast<int>(size); ++i) {
        for(int j = 0; j < static_cast<int>(size); ++j) {
            this->grid_window->setItem(i, j, new QTableWidgetItem("0"));
            this->grid_window->item(i, j)->setBackground(Qt::white);
            this->grid_window->item(i, j)->setForeground(Qt::white);
        }
    }

    // style : size
    this->setMinimumSize(static_cast<int>(size*square_size*2.5), static_cast<int>(size*square_size*2));
    this->grid_window->horizontalHeader()->setVisible(false);
    this->grid_window->verticalHeader()->setVisible(false);
    // style : scrollbars if needed
    this->grid_window->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->grid_window->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // style : no modification by the user
    this->grid_window->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // style : set size of squares
    this->grid_window->horizontalHeader()->setDefaultSectionSize(static_cast<int>(square_size));
    this->grid_window->verticalHeader()->setDefaultSectionSize(static_cast<int>(square_size));
}

void MainWindow::update_grid_window(world_map * grid_data) {
    unsigned int size = this->get_size();
    QTableWidget * grid = this->grid_window;

    for(int i = 0; i < static_cast<int>(size); i++) {
            for(int j = 0; j < static_cast<int>(size); j++) {
                unsigned int square_state = grid_data->get_square_state(static_cast<unsigned int>(i), static_cast<unsigned int>(j));
                if (square_state == 0) {
                    grid->item(i, j)->setText("0");
                    grid->item(i, j)->setBackground(Qt::white);
                    grid->item(i, j)->setForeground(Qt::white);
                } else if (square_state == 1) {
                    grid->item(i, j)->setText("1");
                    grid->item(i, j)->setBackground(Qt::black);
                    grid->item(i, j)->setForeground(Qt::black);
                } else if (square_state == 2) {
                    grid->item(i, j)->setText("2");
                    grid->item(i, j)->setBackground(Qt::green);
                    grid->item(i, j)->setForeground(Qt::green);
                } else if (square_state == 3) {
                    grid->item(i, j)->setText("3");
                    grid->item(i, j)->setBackground(Qt::blue);
                    grid->item(i, j)->setForeground(Qt::blue);
                }
            }
        }
    this->update_gui_values();

}

void MainWindow::update_gui_values() {
    const unsigned int nb_moves = this->get_nb_moves();
    const unsigned int nb_dirty_square_start = this->grid_data->get_nb_dirty_squares_start();
    const unsigned int nb_cleaned_square = this->get_nb_squares_cleaned();
    const unsigned int nb_dirty_square_start_remaining = nb_dirty_square_start - nb_squares_cleaned;
    ui->centralwidget->findChild<QLabel*>("nb_moves_value")->setText(QString::number(nb_moves));
    ui->centralwidget->findChild<QLabel*>("nb_squares_cleaned_value")->setText(QString::number(nb_cleaned_square));
    ui->centralwidget->findChild<QLabel*>("nb_squares_dirty_start_value")->setText(QString::number(nb_dirty_square_start));
    ui->centralwidget->findChild<QLabel*>("nb_squares_dirty_remain_value_3")->setText(QString::number(nb_dirty_square_start_remaining));
    ui->centralwidget->findChild<QLabel*>("completed")->setText(QString(" "));
}

void MainWindow::on_size_spinBox_valueChanged(int arg1)
{
    if (this->get_simulation_is_running() == false) {
        this->set_size(static_cast<unsigned int>(arg1));
    }
}

void MainWindow::on_percent_spinBox_valueChanged(int arg1)
{
    if (this->get_simulation_is_running() == false) {
        this->set_percentage_dirty(static_cast<unsigned int>(arg1));
    }
}

void MainWindow::on_strategy_spinBox_valueChanged(int arg1)
{
    if (this->get_simulation_is_running() == false) {
        this->set_strategy_choice(static_cast<unsigned int>(arg1));
    }
}

void MainWindow::on_start_button_clicked()
{
    if (this->get_simulation_is_running() == false) {
        this->set_simulation_is_running(true);
        this->launch_simulation();
    }
}

void MainWindow::on_stop_button_clicked()
{
    this->set_simulation_is_running(false);
}

void MainWindow::on_quit_clicked()
{
    QApplication::quit();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->set_delay(static_cast<unsigned int>(arg1));
}


void MainWindow::on_benchmark_button_clicked()
{
    this->benchmarking_manager();
}
