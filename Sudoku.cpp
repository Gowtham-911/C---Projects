#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

class Sudoku {
private:
    vector<vector<int>> board;
    const int SIZE = 9;
    const int TIME_LIMIT = 30; // Time limit in seconds

public:
    Sudoku() {
        board = generatePuzzle();
    }

    vector<vector<int>> generatePuzzle() {
        vector<vector<int>> newBoard(SIZE, vector<int>(SIZE, 0));
        srand(time(0));
        int count = 0;
        while (count < 20) {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            int num = (rand() % 9) + 1;
            if (newBoard[row][col] == 0 && isValid(newBoard, row, col, num)) {
                newBoard[row][col] = num;
                count++;
            }
        }
        return newBoard;
    }

    bool isValid(vector<vector<int>>& grid, int row, int col, int num) {
        for (int x = 0; x < SIZE; ++x) {
            if (grid[row][x] == num || grid[x][col] == num) {
                return false;
            }
        }
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grid[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }
        return true;
    }

    void display() {
        cout << "🧩 Sudoku Board 🧩" << endl;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cout << (board[i][j] ? to_string(board[i][j]) : ".") << " ";
                if (j == 2 || j == 5) cout << "| ";
            }
            cout << endl;
            if (i == 2 || i == 5) cout << "------+-------+------" << endl;
        }
    }

    bool solve(vector<vector<int>>& grid) {
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                if (grid[row][col] == 0) {
                    for (int num = 1; num <= SIZE; ++num) {
                        if (isValid(grid, row, col, num)) {
                            grid[row][col] = num;
                            if (solve(grid)) {
                                return true;
                            }
                            grid[row][col] = 0;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    void play() {
        int row, col, num;
        auto start = chrono::steady_clock::now();
        while (true) {
            // Time check
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
            if (elapsed > TIME_LIMIT) {
                cout << "⏳ Time's up! Game over!" << endl;
                break;
            }

            display();
            cout << "Time remaining: " << (TIME_LIMIT - elapsed) << " seconds." << endl;
            cout << "Enter row (0-8), column (0-8), and number (1-9) to fill (or -1 to quit): ";
            cin >> row >> col >> num;

            if (row == -1 || col == -1 || num == -1) {
                cout << "Exiting the game!" << endl;
                break;
            }

            if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && num >= 1 && num <= 9) {
                if (board[row][col] == 0 && isValid(board, row, col, num)) {
                    board[row][col] = num;
                    if (isComplete()) {
                        cout << "🎉 Congratulations! You've solved the Sudoku! 🎉" << endl;
                        break;
                    }
                } else {
                    cout << "❌ Invalid move! Try again." << endl;
                }
            } else {
                cout << "❌ Invalid input! Please enter valid row, column, and number." << endl;
            }
        }
    }

    bool isComplete() {
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                if (board[i][j] == 0)
                    return false;
        return true;
    }
};

int main() {
    Sudoku sudoku;
    sudoku.play();
    return 0;
}
