#include <iostream>
#include <limits>
#include <random>

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
// Returns scores for the minimax algo
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

// Text output jobs
class Utils {
  private:
    int field;
    bool playerTurn;
    bool isSinglePlayer;
    int difficulty;

  public:
    Utils(int field, bool playerTurn, bool isSinglePlayer) {
        this->field = field;
        this->playerTurn = playerTurn;
        this->isSinglePlayer = isSinglePlayer;
    }

    int promptForInput() {

        if (!isSinglePlayer) {
            playerTurn ? std::cout << "Player 1 (X), "
                       : std::cout << "Player 2 (O), ";
            std::cout << "Select a field (1-9): ";
        } else {

            std::cout << "Player (X), Select a field (1-9): ";
        }

        std::cin >> field;
        std::cout << "\n";

        return field;
    }

    bool difficultySelection() {
        std::cout << "1: Easy Difficulty\n";
        std::cout << "2: Hard Difficulty\n";
        std::cout << "\n";
        std::cout << "Choose Difficulty: ";

        std::cin >> difficulty;
        std::cout << "\n";
        switch (difficulty) {
        case 1:
            return false;
        case 2:
            return true;
        default:
            std::cout << "Invalid Selection! Choosing Easy Difficulty...\n";
            return false;
        }
    }

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

    void winningMessage(bool playerTurn) {
        if (isSinglePlayer) {
            playerTurn ? std::cout << "Player 1 won!!"
                       : std::cout << "Computer won!!";
        } else {
            playerTurn ? std::cout << "Player 1 won!!"
                       : std::cout << "Player 2 won!!";
        }
    }

    void drawMessage() { std::cout << "It's a Draw!!\n"; }
};

// Simple logic related jobs
class Logic {
  private:
    bool playerTurn;

  public:
    Logic(bool playerTurn) { this->playerTurn = playerTurn; }

    bool validateField(char pos[], int field) {
        if (field < 1 || field > 9 || pos[field - 1] != ' ') {
            std::cout << "INVALID FIELD" << std::endl;
            return false;
        }
        return true;
    }

    bool switchTurn() {
        playerTurn ? playerTurn = false : playerTurn = true;
        return playerTurn;
    }

    bool isMoveLeft(char pos[]) {
        for (int i = 0; i < 9; i++) {
            if (pos[i] == ' ') {
                return true;
            }
        }
        return false;
    }

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
};

int minimax(char pos[], bool isMaxTurn, Logic &logic) {
    int moveScore = evaluate(pos);

    if (moveScore == 10) {
        return moveScore;
    }

    if (moveScore == -10) {
        return moveScore;
    }

    if (logic.isMoveLeft(pos) == false)
        return 0;

    if (isMaxTurn) {
        int bestScore = std::numeric_limits<int>::min();

        for (int i = 0; i < 9; i++) {
            if (pos[i] == ' ') {
                pos[i] = 'O';

                bestScore =
                    std::max(bestScore, minimax(pos, !isMaxTurn, logic));

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

                bestScore =
                    std::min(bestScore, minimax(pos, !isMaxTurn, logic));

                pos[i] = ' ';
            }
        }

        return bestScore;
    }
}

int bestMove(char pos[], Logic &logic, bool isHard) {
    int bestScore = std::numeric_limits<int>::min();
    int bestPos = -1;
    int random;

    if (!isHard) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9);
        random = dist(rng);
    }

    for (int i = 0; i < 9; i++) {
        if (pos[i] == ' ') {
            pos[i] = 'O';

            if (!isHard && random >= 5) {
                return i;
            }

            int moveScore = minimax(pos, false, logic);

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
    char pos[9];
    int field;

    for (int i = 0; i < 9; i++) {
        pos[i] = ' ';
    }

#ifdef __linux__
    system("figlet TicTacToe");
#endif

    // Singleplayer
    if (gameMode()) {

        Utils utils(field, playerTurn, true);
        Logic logic(playerTurn);

        std::cout << std::endl;
        bool isHard = utils.difficultySelection();

        utils.drawBoard(pos);

        while (1) {

            std::cout << std::endl;

            field = utils.promptForInput();

            if (!logic.validateField(pos, field)) {
                utils.drawBoard(pos);
                continue;
            }

            pos[field - 1] = 'X';

            if (logic.checkWin(pos, playerTurn)) {
                utils.winningMessage(playerTurn);
                break;
            }

            playerTurn = logic.switchTurn();

            if (!logic.isMoveLeft(pos)) {
                utils.drawBoard(pos);
                utils.drawMessage();
                break;
            }

            std::cout << "Computer's turn...\n";

            pos[bestMove(pos, logic, isHard)] = 'O';

            utils.drawBoard(pos);

            if (logic.checkWin(pos, playerTurn)) {
                utils.winningMessage(playerTurn);
                break;
            }

            if (!logic.isMoveLeft(pos)) {
                utils.drawBoard(pos);
                utils.drawMessage();
                break;
            }

            playerTurn = logic.switchTurn();
        }
    }

    else {
        Utils utils(field, playerTurn, false);
        Logic logic(playerTurn);

        utils.drawBoard(pos);

        while (1) {

            std::cout << std::endl;

            field = utils.promptForInput();

            if (!logic.validateField(pos, field)) {
                utils.drawBoard(pos);
                continue;
            } else {

                playerTurn ? pos[field - 1] = 'X' : pos[field - 1] = 'O';

                utils.drawBoard(pos);

                if (logic.checkWin(pos, playerTurn)) {
                    utils.winningMessage(playerTurn);
                    break;
                }

                if (!logic.isMoveLeft(pos)) {
                    utils.drawBoard(pos);
                    utils.drawMessage();
                    break;
                }

                playerTurn = logic.switchTurn();
            }
        }
    }

    std::cin.get();
    std::cin.get();
    return 0;
}
