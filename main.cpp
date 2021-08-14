#include <algorithm>
#include <iostream>
#include <limits>

void drawBoard(char pos[]) {
    std::cout << '\n';
    for (int i = 0; i < 9; i = i + 3) {
        std::cout << pos[i] << "|" << pos[i + 1] << "|" << pos[i + 2]
                  << std::endl;
        if (i < 6) {
            std::cout << "-+-+-" << std::endl;
        }
    }
    std::cout << '\n';
}

// 0 1 2
// 3 4 5
// 6 7 8
bool checkWin(char pos[], bool turn) {
    char symbol;
    turn ? symbol = 'X' : symbol = 'O';
    if ((pos[0] == symbol && pos[1] == symbol && pos[2] == symbol) ||
        (pos[3] == symbol && pos[4] == symbol && pos[5] == symbol) ||
        (pos[6] == symbol && pos[7] == symbol && pos[8] == symbol) ||
        (pos[0] == symbol && pos[3] == symbol && pos[6] == symbol) ||
        (pos[1] == symbol && pos[4] == symbol && pos[7] == symbol) ||
        (pos[0] == symbol && pos[4] == symbol && pos[8] == symbol) ||
        (pos[2] == symbol && pos[4] == symbol && pos[6] == symbol)) {
        return true;
    } else
        return false;
}

bool gameMode() {
    while (1) {
        std::cout << "1: Single-player Mode\n2: Two-player Mode\n\n";
        std::cout << "Enter Selection: ";
        int choice;
        std::cin >> choice;

        if (choice == 1)
            return true;
        else if (choice == 2)
            return false;
        else {
            std::cout << "INVALID SELECTION\n Try again.\n\n\n";
            continue;
        }
    }
}

// 0 1 2
// 3 4 5
// 6 7 8
int evaluate(char pos[]) {
    // columns
    for (int i = 0; i < 3; i++) {
        if (pos[i] == pos[3 + i] && pos[3 + i] == pos[6 + i]) {
            if (pos[i] == 'O')
                return +10;
            else if (pos[i] == 'X')
                return -10;
        }
    }

    // rows
    for (int i = 0; i <= 6; i = i + 3) {
        if (pos[i] == pos[1 + i] && pos[1 + i] == pos[2 + i]) {
            if (pos[i] == 'O')
                return +10;
            else if (pos[i] == 'X')
                return -10;
        }
    }

    // diagonal 0 4 8
    if (pos[0] == pos[4] && pos[4] == pos[8]) {
        if (pos[0] == 'O')
            return +10;
        else if (pos[0] == 'X')
            return -10;
    }

    // diagonal 2 4 6
    if (pos[2] == pos[4] && pos[4] == pos[6]) {
        if (pos[2] == 'O')
            return +10;
        else if (pos[2] == 'X')
            return -10;
    }

    return 0;
}

bool isMoveLeft(char pos[]) {
    for (int i = 0; i < 9; i++) {
        if (pos[i] == ' ') {
            return true;
        }
    }
    return false;
}

int minimax(char pos[], bool isMaxTurn) {
    int moveScore = evaluate(pos);

    if (moveScore == 10) {
        return moveScore;
    }

    if (moveScore == -10) {
        return moveScore;
    }

    if (isMoveLeft(pos) == false)
        return 0;

    if (isMaxTurn) {
        int bestScore = std::numeric_limits<int>::min();

        for (int i = 0; i < 9; i++) {
            if (pos[i] == ' ') {
                pos[i] = 'O';

                bestScore = std::max(bestScore, minimax(pos, !isMaxTurn));

                pos[i] = ' ';
            }
        }

        return bestScore;
    }

    else {

        int bestScore = std::numeric_limits<int>::max();

        for (int i = 0; i < 9; i++) {
            if (pos[i] == ' ') {
                pos[i] = 'X';

                bestScore = std::min(bestScore, minimax(pos, !isMaxTurn));

                pos[i] = ' ';
            }
        }

        return bestScore;
    }
}

int bestMove(char pos[]) {
    int bestScore = std::numeric_limits<int>::min();
    int bestPos = -1;

    for (int i = 0; i < 9; i++) {
        if (pos[i] == ' ') {
            pos[i] = 'O';

            int moveScore = minimax(pos, false);

            pos[i] = ' ';

            if (moveScore > bestScore) {
                bestPos = i;
                bestScore = moveScore;
            }
        }
    }

    return bestPos;
}

int main() {
    bool playerTurn = true;
    bool singlePlayer;
    char pos[9];
    int field;
    int turnCount;

    for (int i = 0; i < 9; i++) {
        pos[i] = ' ';
    }

    singlePlayer = gameMode();

    if (singlePlayer) {
        drawBoard(pos);

        while (1) {

            std::cout << std::endl;

            std::cout << "Player (X), Select a field (1-9): ";
            std::cin >> field;
            std::cout << "\n";

            if (field < 1 || field > 9 || pos[field - 1] != ' ') {
                drawBoard(pos);
                std::cout << "INVALID FIELD" << std::endl;
                continue;
            }
            pos[field - 1] = 'X';

            if (checkWin(pos, playerTurn)) {

                playerTurn ? std::cout << "Player 1 won!!"
                           : std::cout << "Computer won!!";
                break;
            }

            playerTurn ? playerTurn = false : playerTurn = true;

            turnCount++;
            if (turnCount == 9) {
                std::cout << "It's a draw!" << std::endl;
                break;
            }

            std::cout << "Computer's turn...\n";

            std::cout << bestMove(pos) << "\n";
            pos[bestMove(pos)] = 'O';

            drawBoard(pos);

            if (checkWin(pos, playerTurn)) {

                playerTurn ? std::cout << "Player 1 won!!"
                           : std::cout << "Computer won!!";
                break;
            }

            playerTurn ? playerTurn = false : playerTurn = true;

            turnCount++;
            if (turnCount == 9) {
                std::cout << "It's a draw!" << std::endl;
                break;
            }
        }
    }

    else {

        drawBoard(pos);

        while (1) {

            std::cout << std::endl;

            playerTurn ? std::cout << "Player 1 (X), "
                       : std::cout << "Player 2 (O), ";
            std::cout << "Select a field (1-9): ";
            std::cin >> field;
            std::cout << std::endl;

            if (field < 1 || field > 9 || pos[field - 1] != ' ') {
                drawBoard(pos);
                std::cout << "INVALID FIELD" << std::endl;
                continue;
            } else {

                playerTurn ? pos[field - 1] = 'X' : pos[field - 1] = 'O';

                drawBoard(pos);

                if (checkWin(pos, playerTurn)) {

                    playerTurn ? std::cout << "Player 1 won!!"
                               : std::cout << "Player 2 won!!";
                    break;
                }

                turnCount++;
                if (turnCount == 9) {
                    std::cout << "It's a draw!" << std::endl;
                    break;
                }

                playerTurn ? playerTurn = false : playerTurn = true;
            }
        }
    }

    std::cin.get();
    std::cin.get();
    return 0;
}