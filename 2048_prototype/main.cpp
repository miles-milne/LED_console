#include <iostream>
#include <iomanip>
#include <ctime>
#include <stack>
#include <conio.h>

using std::cout, std::cin;

class Grid {

private:
    unsigned int _row;
    unsigned int _col;
    unsigned int _score;

    int* _grid;

public:

    Grid(unsigned int rows, unsigned int cols) {
        _row = rows;
        _col = cols;
        _score = 0;

       _grid = new int[_row * _col];

    }


    ~Grid() {
        delete[] _grid;
        _grid = nullptr;
    }


    // checks if there are any elements in the grid that are equal to zero
    bool vacancy() {
        for (int k = 0; k < _row*_col; ++k) {
            if (_grid[k] == 0) return true;
        }

        return false;
    }


    // updates the board in accordance to the user input
    bool move(char input) {

        int* temp_line;
        int* temp_grid = new int[_row*_col];

        // creates copy of _grid
        for (int i = 0; i < _row*_col; ++i)
            temp_grid[i] = _grid[i];

        if (input == 'w') { // UP
            temp_line = new int[_row];

            for (int c = 0; c < _col; ++c) {
                for (int r = 0; r < _row; ++r)
                    temp_line[r] = _grid[c + _col*r];

                _score += shift_line(temp_line, _row);

                for (int r = 0; r < _row; ++r)
                    _grid[c + _col*r] = temp_line[r];
            }

        }
        else if (input == 'd') { // RIGHT
            temp_line = new int[_col];

            for (int r = 0; r < _row; ++r) {

                for (int c = _col-1; c >= 0; --c)
                    temp_line[_col-1 -c] = _grid[_col*r + c];

                _score += shift_line(temp_line, _col);

                for (int c = _col-1; c >= 0; --c)
                    _grid[_col*r + c] = temp_line[_col-1 -c];
            }

        }
        else if (input == 's') { // DOWN
            temp_line = new int[_row];

            for (int c = 0; c < _col; ++c) {
                for (int r = _row-1; r >= 0; --r)
                    temp_line[_row-1 - r] = _grid[c + _col*r];

                _score += shift_line(temp_line, _row);

                for (int r = _row-1; r >= 0; --r)
                    _grid[c + _col*r] = temp_line[_row-1 - r];
            }

        }
        else if (input == 'a') { // LEFT
            temp_line = new int[_col];

            for (int r = 0; r < _row; ++r) {

                for (int c = 0; c < _col; ++c)
                    temp_line[c] = _grid[_col*r + c];

                _score += shift_line(temp_line, _col);

                for (int c = 0; c < _col; ++c)
                    _grid[_col*r + c] = temp_line[c];
            }

        }
        else if (input == 'r') {
            delete[] temp_grid;
            clear_grid();
            _score = 0;
            return true;
        }
        else {
            cout << "Invalid input! ";
            delete[] temp_grid;
            return false;
        }

        bool not_identical = false;

        // checks if grid is identical before and after.
        for (int k = 0; k < _row*_col; ++k) {
            if (_grid[k] != temp_grid[k]) {
                not_identical = true;
                break;
            }
        }

        delete[] temp_line;
        delete[] temp_grid;
        return not_identical;
    }

    // helper function that deals with shifting and merging cells in a line (i.e. row/col) after the user makes a move
    // This function will be called regardless if the given line has no valid shifts or is completely empty
    static int shift_line(int* line, int size) {
        std:: stack<int> s;
        int num_empty = 0, score = 0;

        for (int k = 0; k < size; ++k) {

            if (line[k] != 0) {

                if (!s.empty() && s.top() == line[k]) {  //TODO will this if statement terminate before it checks for an undefined equality?
                    num_empty++;
                    s.top() *= 2;
                    score += s.top();

                    // This while loop ensures the next cell is properly pushed to the stack,
                    // as to avoid a "double merge"; for instance
                    // [0, 2, 2, 4] ---> [4, 4, 0, 0], NOT [0, 2, 2, 4] ---> [4, 4, 0, 0] --> [8, 0, 0, 0]
                    while (k < size-1) {
                        k++;

                        if (line[k] != 0) {
                            s.push(line[k]);
                            break;
                        }

                        num_empty++;
                    }
                }
                else { // merge does not occur
                    s.push(line[k]);
                }

            }
            else {
                num_empty++;
            }
        }

        if (num_empty == size)
            return 0;


        // pops the stack back into the array in reverse order
        for (int j = size-1; j >= 0; --j) {

            if (num_empty > 0) {
                line[j] = 0;
                --num_empty;
            }
            else {
                line[j] = s.top();
                s.pop();
            }
        }

        return score;
    }

    // FIXME this doesnt work
    bool has_moves() {

        for (int r = 0; r < _row; ++r) {
            for (int c = 0; c < _col; ++c) {

                // checks for equality to element on its right
               if (c != _col-1 && _grid[_col*r + c] == _grid[_col*r + c + 1])
                   return true;

               // checks for equality to element below
               if (r != _row-1 && _grid[_col*r + c] == _grid[_col*r + c + _col])
                   return true;
            }
        }

        return vacancy();
    }


    // Adds a random "2" or "4" to the grid if space is available
    void populate(unsigned int seed) {

        int* empty = new int[_row*_col];
        int num_empty = 0;

        for (int k = 0; k < _row*_col; ++k) {
            if(_grid[k] == 0) {
                empty[num_empty] = k;
                num_empty++;
            }
        }

        srand(seed);
        int element = (rand() % static_cast<int>(num_empty));
        int val = (rand() % static_cast<int>(4));


        if (val == 2) {
            _grid[empty[element]] = 4;
        }
        else {
            _grid[empty[element]] = 2;
        }

        delete[] empty;
    }


    // displays the grid on the console
    void print() {

        std:: ostringstream buffer;

        buffer << "\t\tScore:" << std:: setw(6) << _score <<"\n";

        for (int r = 0; r < _row; ++r) {
            buffer << "\t";

            for (int c = 0; c < _col; ++c) {
                buffer << "\x1B[90m[\033[0m";

                if (_grid[_col*r + c] != 0)
                    buffer << text_colour(_grid[_col*r + c]) <<
                    std::setw(4) << _grid[_col*r + c] << "\033[0m\x1B[90m ]\033[0m";
                else
                    buffer << "\x1B[90m     ]\033[0m";

            }
            buffer << "\n";

        }



        std:: system("cls");
        cout << buffer.str();
    }


    // ANSI colour escape codes windows only
    // https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
    static std::string text_colour(int val) {

        switch(val) {
            case(2):
                return "\x1B[37m"; // white
            case(4):
                return "\x1B[97m"; // bright white
            case(8):
                return "\x1B[91m"; // bright red
            case(16):
                return "\x1B[31m"; // red
            case(32):
                return "\x1B[35m"; // magenta
            case(64):
                return "\x1B[95m"; // bright magenta
            case(128):
                return "\x1B[96m"; // bright cyan
            case(256):
                return "\x1B[36m"; // cyan
            case(512):
                return "\x1B[32m"; // green
            case(1024):
                return "\x1B[92m"; // bright green
            case(2048):
                return "\x1B[93m"; // bright yellow
            case(4096):
                return "\x1B[33m"; // yellow
            default:
                return "\x1B[90m"; // bright black
        }
    }


    char input() {
        char input = _getch(); // allows for stdin without pressing 'enter'
        return tolower(input);
    }


    void clear_grid() {

        for (int k = 0; k < _row*_col; ++k) {
            _grid[k] = 0;
        }
    }


    void test_value_set() {

        unsigned int r, c;
        int val;


        cout << "Please enter the row:";
        cin >> r;

        cout << "Please enter the col:";
        cin >> c;



        if (r >= _row || c >= _col) {
            cout << "(" << r << ", " << c << ") is an invalid entry.\n";
            return;
        }

        cout << "Now, enter a value:";
        cin >> val;


        _grid[_col*r + c] = val;

    }


    int* get_grid() const {
        return _grid;
    }


    // "Game loop" of the 2048 clone
    void play() {
        std:: string name;
        cout << "Please enter your name: ";
        cin >> name;
        clear_grid();

        bool valid_action = true;


        while(name != "quit") {

            std:: system("cls");

            while(has_moves()) {

                if (vacancy())
                    populate((int) std::clock());

                print();
                cout << "\nEnter direction (W,A,S,D), or R to restart: ";
                while(!move(input())) {}
            }


            print();
            cout << name << ", it's game over man!\n";
            // TODO: save highscore

            cout << "Please enter your name, or type \"quit\" to exit: ";
            cin >> name;
            clear_grid();
        }
    }

};


int main() {

    //std:: system("Color 03");

    Grid game = Grid(4,4);
    game.play();

    // TODO: allow for creating games of custom size
    // Todo: Add scoreboard.txt using fstream


    /* Test line stack
    int score = 0;
    const int SIZE = 4;


    // <------ elements are pushed this way
    //int test[SIZE] = {0, 2, 0, 2,  8, 0, 8, 0, 8, 0, 8, 0, 4, 0, 0, 4, 0};
    int test[SIZE] = {0, 2048, 0, 2};

    score = Grid::line_stack(test, SIZE);

    cout << "score: " << score << "\n";
    for (int k = 0; k < SIZE; k++) {
        cout << test[k] << " ";
    }



    cin >> score;
    */

    return EXIT_SUCCESS;
}


