#include<iostream>
#include<fstream>
#include<set>
#include<vector>
using namespace std;

class SudokuGrid {
public:
    int grid[9][9]; // Holds the grid values

    SudokuGrid() {
        // Initialize grid with zeros (empty cells)
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                grid[i][j] = 0;
            }
        }
    }

    void displayGrid() {
        cout << "++=====================================++";
        for (int row = 0; row < 9; row++) {
            cout << "\n||";
            for (int col = 0; col < 9; col++) {
                if (grid[row][col] != 0)
                    cout << grid[row][col] << " ";
                else
                    cout << "  ";
                if ((col + 1) % 3 == 0) cout << "||";
            }
            cout << endl;
            if ((row + 1) % 3 == 0)
                cout << "++=====================================++";
        }
        cout << endl;
    }

    bool isValid(int row, int col, int value) {
        // Check row
        for (int c = 0; c < 9; c++) {
            if (grid[row][c] == value) return false;
        }

        // Check column
        for (int r = 0; r < 9; r++) {
            if (grid[r][col] == value) return false;
        }

        // Check 3x3 subgrid
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;
        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if (grid[r][c] == value) return false;
            }
        }

        return true;
    }

    // Check if there are any invalid numbers in the grid before solving
    bool validateGrid() {
        // Check all cells for valid values (1–9 or 0 for empty)
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                int value = grid[i][j];
                if (value < 0 || value > 9) {
                    cout << "Error: Invalid value " << value << " at position (" << i + 1 << "," << j + 1 << ").\n";
                    return false;
                }
    
                // If the cell is not empty, temporarily clear it, then check for duplicates
                if (value != 0) {
                    grid[i][j] = 0;  // Temporarily clear the cell
                    if (!isValid(i, j, value)) {
                        cout << "Error: Invalid placement of value " << value << " at position (" << i + 1 << "," << j + 1 << ").\n";
                        return false;
                    }
                    grid[i][j] = value;  // Restore the original value
                }
            }
        }
        return true;
    }
    
    bool isSolved() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (grid[i][j] == 0) return false;
            }
        }
        return true;
    }

    vector<pair<int, int>> getEmptyCells() {
        vector<pair<int, int>> emptyCells;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (grid[i][j] == 0) {
                    emptyCells.push_back({i, j});
                }
            }
        }
        return emptyCells;
    }

    set<int> getPossibleValues(int row, int col) {
        set<int> possibleValues;
        for (int value = 1; value <= 9; value++) {
            if (isValid(row, col, value)) {
                possibleValues.insert(value);
            }
        }
        return possibleValues;
    }

    void readFromConsole() {
        cout << "\nEnter the specified value when prompted.\n";
        cout << "Enter 0 if cell is empty.\n\n";

        for (int rowIter = 0; rowIter < 9; rowIter++) { // Iterating over cells to read values.
            for (int colIter = 0; colIter < 9; colIter++) {
                int enteredValue;
                cout << "Enter value for cell[" << rowIter + 1 << "][" << colIter + 1 << "] --> ";
                cin >> enteredValue;

                if (!(enteredValue >= 0 && enteredValue <= 9)) { // Checking for bounds in input.
                    while (true) { // Loop until valid input is read.
                        cout << "Oops! You seem to have entered a wrong value! Try again.\n";
                        cout << "Enter value for cell [" << rowIter + 1 << "][" << colIter + 1 << "] --> ";
                        cin >> enteredValue;

                        if (enteredValue >= 0 && enteredValue <= 9) break;
                    }
                }

                grid[rowIter][colIter] = enteredValue;
            }
            cout << "-------\n"; // Display a break after every row for convenience.
        }
        cout << endl;
    }

    void readFromFile(const string& filename) {
        ifstream input_file;
        input_file.open(filename, ios::in);

        int rowIter, colIter;
        for (rowIter = 0; rowIter < 9; rowIter++) { // Iterating over file values.
            for (colIter = 0; colIter < 9; colIter++) {
                int readValue;
                input_file >> readValue;

                if (!(readValue >= 0 && readValue <= 9)) { // Checking bounds for input.
                    cout << "\nValue " << ((rowIter * 9) + colIter + 1) << " in " << filename;
                    cout << " seems to be wrong! Correct the value and try again!\n";
                    exit(EXIT_FAILURE);
                }

                grid[rowIter][colIter] = readValue;

                if (input_file.eof()) break; // Breaking if EOF is reached.
            }
        }

        input_file.close();
        cout << endl;
    }
};

class SudokuSolver {
public:
    SudokuGrid grid;

    SudokuSolver() {}

    bool solve() {
        return forwardCheckSolve();
    }

    bool forwardCheckSolve() {
        if (grid.isSolved()) {
            return true;
        }

        vector<pair<int, int>> emptyCells = grid.getEmptyCells();
        if (emptyCells.empty()) return false;

        // Use MRV (minimum remaining values) heuristic to choose the next cell
        pair<int, int> cell = chooseCellWithFewestOptions(emptyCells);
        int row = cell.first;
        int col = cell.second;

        set<int> possibleValues = grid.getPossibleValues(row, col);
        for (int value : possibleValues) {
            grid.grid[row][col] = value;

            // Forward checking: eliminate this value from the possible values of neighboring cells
            if (forwardCheckSolve()) {
                return true;
            }

            grid.grid[row][col] = 0; // Backtrack if this value didn't work
        }
        return false;
    }

    pair<int, int> chooseCellWithFewestOptions(const vector<pair<int, int>>& emptyCells) {
        pair<int, int> selectedCell = emptyCells[0];
        size_t minOptions = 10; // changed to size_type
        for (const auto& cell : emptyCells) {
            int row = cell.first;
            int col = cell.second;
            set<int> possibleValues = grid.getPossibleValues(row, col);
            if (possibleValues.size() < minOptions) {
                selectedCell = cell;
                minOptions = possibleValues.size();
            }
        }
        return selectedCell;
    }
};

int main() {
    SudokuGrid sudokuGrid;
    int option;

    cout << "\n======================\n";
    cout << "    Sudoku Solver\n";
    cout << "======================\n\n";
    cout << "Choose input method:\n";
    cout << "1. Input puzzle manually\n";
    cout << "2. Load puzzle from file\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> option;

    if (option == 1) {
        sudokuGrid.readFromConsole();
    } else if (option == 2) {
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        sudokuGrid.readFromFile(filename);
		//sudokuGrid.displayGrid();
    } else {
        cout << "Invalid option. Exiting.\n";
        return 1;
    }

    // Validate the grid
    if (!sudokuGrid.validateGrid()) {
        cout << "Exiting due to invalid grid.\n";
        return 1;
    }

	// Display initial grid
	cout << "\nInitial Sudoku Puzzle:\n";
	sudokuGrid.displayGrid();

	// Solve the puzzle
	SudokuSolver solver;
	solver.grid = sudokuGrid;

	if (solver.solve()) {
		cout << "\nSolved Sudoku Puzzle:\n";
		solver.grid.displayGrid();
	} else {
		cout << "\nNo solution exists for the given puzzle.\n";
	}

	return 0;
}
