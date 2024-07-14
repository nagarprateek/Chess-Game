#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>

using namespace std;

enum class Color { WHITE, BLACK };

class Piece {
public:
    Color color;
    string symbol;

    Piece(Color c, string s) : color(c), symbol(s) {}

    virtual bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) = 0;
};

class Pawn : public Piece {
public:
    bool isFirstMove;

    Pawn(Color c) : Piece(c, (c == Color::WHITE) ? "P" : "p"), isFirstMove(true) {}

    bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) override {
        int direction = (color == Color::WHITE) ? 1 : -1;
        
        // Normal move
        if (startY == endY && board[endX][endY] == nullptr) {
            if (startX + direction == endX) {
                isFirstMove = false;
                return true;
            } else if (isFirstMove && startX + 2 * direction == endX && board[startX + direction][endY] == nullptr) {
                isFirstMove = false;
                return true;
            }
        }
        
        // Capture move
        if (abs(startY - endY) == 1 && startX + direction == endX && board[endX][endY] != nullptr && board[endX][endY]->color != color) {
            isFirstMove = false;
            return true;
        }
        
        return false;
    }
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, (c == Color::WHITE) ? "R" : "r") {}

    bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) override {
        if (startX == endX || startY == endY) {
            int xDirection = (endX - startX == 0) ? 0 : (endX - startX) / abs(endX - startX);
            int yDirection = (endY - startY == 0) ? 0 : (endY - startY) / abs(endY - startY);
            for (int i = 1; i < max(abs(endX - startX), abs(endY - startY)); ++i) {
                if (board[startX + i * xDirection][startY + i * yDirection] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, (c == Color::WHITE) ? "N" : "n") {}

    bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) override {
        int dx = abs(endX - startX);
        int dy = abs(endY - startY);
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
            return true;
        }
        return false;
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, (c == Color::WHITE) ? "B" : "b") {}

    bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) override {
        if (abs(endX - startX) == abs(endY - startY)) {
            int xDirection = (endX - startX == 0) ? 0 : (endX - startX) / abs(endX - startX);
            int yDirection = (endY - startY == 0) ? 0 : (endY - startY) / abs(endY - startY);
            for (int i = 1; i < abs(endX - startX); ++i) {
                if (board[startX + i * xDirection][startY + i * yDirection] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, (c == Color::WHITE) ? "Q" : "q") {}

    bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) override {
        if (startX == endX || startY == endY) {
            int xDirection = (endX - startX == 0) ? 0 : (endX - startX) / abs(endX - startX);
            int yDirection = (endY - startY == 0) ? 0 : (endY - startY) / abs(endY - startY);
            for (int i = 1; i < max(abs(endX - startX), abs(endY - startY)); ++i) {
                if (board[startX + i * xDirection][startY + i * yDirection] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        if (abs(endX - startX) == abs(endY - startY)) {
            int xDirection = (endX - startX == 0) ? 0 : (endX - startX) / abs(endX - startX);
            int yDirection = (endY - startY == 0) ? 0 : (endY - startY) / abs(endY - startY);
            for (int i = 1; i < abs(endX - startX); ++i) {
                if (board[startX + i * xDirection][startY + i * yDirection] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class King : public Piece {
public:
    King(Color c) : Piece(c, (c == Color::WHITE) ? "K" : "k") {}

    bool isValidMove(int startX, int startY, int endX, int endY, vector<vector<Piece*>>& board) override {
        int dx = abs(endX - startX);
        int dy = abs(endY - startY);
        if ((dx <= 1 && dy <= 1)) {
            return true;
        }
        return false;
    }
};

class Board {
public:
    vector<vector<Piece*>> board;
    map<char, int> columnMap;
    pair<int, int> whiteKingPos;
    pair<int, int> blackKingPos;

    Board() {
        board.resize(8, vector<Piece*>(8, nullptr));
        columnMap = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}};

        // Initialize pieces for a new game
        for (int i = 0; i < 8; ++i) {
            board[1][i] = new Pawn(Color::WHITE);
            board[6][i] = new Pawn(Color::BLACK);
        }
        board[0][0] = new Rook(Color::WHITE); board[0][7] = new Rook(Color::WHITE);
        board[7][0] = new Rook(Color::BLACK); board[7][7] = new Rook(Color::BLACK);
        board[0][1] = new Knight(Color::WHITE); board[0][6] = new Knight(Color::WHITE);
        board[7][1] = new Knight(Color::BLACK); board[7][6] = new Knight(Color::BLACK);
        board[0][2] = new Bishop(Color::WHITE); board[0][5] = new Bishop(Color::WHITE);
        board[7][2] = new Bishop(Color::BLACK); board[7][5] = new Bishop(Color::BLACK);
        board[0][3] = new Queen(Color::WHITE); board[0][4] = new King(Color::WHITE);
        board[7][3] = new Queen(Color::BLACK); board[7][4] = new King(Color::BLACK);

        // Initialize king positions
        whiteKingPos = {0, 4};
        blackKingPos = {7, 4};
    }

    ~Board() {
        for (auto& row : board) {
            for (auto& piece : row) {
                delete piece;
            }
        }
    }

    void printBoard() {
        for (int i = 7; i >= 0; --i) {
            cout << i + 1 << " ";
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == nullptr) {
                    cout << " . ";
                } else {
                    cout << " " << board[i][j]->symbol << " ";
                }
            }
            cout << endl;
        }
        cout << "   a  b  c  d  e  f  g  h" << endl;
    }

   bool movePiece(const string& move, Color color) {
    int startX = move[1] - '1';
    int startY = columnMap[move[0]];
    int endX = move[3] - '1';
    int endY = columnMap[move[2]];

    Piece* piece = board[startX][startY];
    if (piece == nullptr || piece->color != color) {
        cout << "Invalid move: No " << (color == Color::WHITE ? "White" : "Black") << " piece at the start position." << endl;
        return false;
    }

    if (piece->isValidMove(startX, startY, endX, endY, board)) {
        if (board[endX][endY] != nullptr && board[endX][endY]->color != color) {
            delete board[endX][endY];
        }
        board[endX][endY] = piece;
        board[startX][startY] = nullptr;

        // Update king's position if moved
        if (piece->symbol == "K") {
            whiteKingPos = {endX, endY};
        } else if (piece->symbol == "k") {
            blackKingPos = {endX, endY};
        }

        // Check for check and checkmate
        if (isInCheck(color)) {
            // Revert the move
            board[startX][startY] = piece;
            board[endX][endY] = nullptr;

            cout << "Invalid move: " << (color == Color::WHITE ? "White" : "Black") << " is in check." << endl;
            return false;
        }

        return true;
    } else {
        cout << "Invalid move: Piece cannot move to the destination." << endl;
        return false;
    }
}



    bool isInCheck(Color color) {
        pair<int, int> kingPos = (color == Color::WHITE) ? whiteKingPos : blackKingPos;
        Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

        // Check if any opponent's piece can attack the king
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Piece* piece = board[i][j];
                if (piece != nullptr && piece->color == opponentColor) {
                    if (piece->isValidMove(i, j, kingPos.first, kingPos.second, board)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool isCheckmate(Color color) {
        // Check if the king is in check
        if (!isInCheck(color)) {
            return false;
        }

        // Check if any move can get the king out of check
        for (int startX = 0; startX < 8; ++startX) {
            for (int startY = 0; startY < 8; ++startY) {
                Piece* piece = board[startX][startY];
                if (piece != nullptr && piece->color == color) {
                    for (int endX = 0; endX < 8; ++endX) {
                        for (int endY = 0; endY < 8; ++endY) {
                            if (piece->isValidMove(startX, startY, endX, endY, board)) {
                                // Simulate the move
                                Piece* capturedPiece = board[endX][endY];
                                board[endX][endY] = piece;
                                board[startX][startY] = nullptr;

                                // Check if the king is still in check after the move
                                bool kingStillInCheck = isInCheck(color);

                                // Undo the move
                                board[startX][startY] = piece;
                                board[endX][endY] = capturedPiece;

                                if (!kingStillInCheck) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }

        return true;
    }
};

int main() {
    Board board;
    string move;
    Color currentPlayer = Color::WHITE;

    while (true) {
        board.printBoard();
        cout << endl;
        cout << (currentPlayer == Color::WHITE ? "White" : "Black") << "'s move: ";
        getline(cin, move);

        if (move == "quit") {
            break;
        }

        if (move.length() == 4 && isalpha(move[0]) && isdigit(move[1]) && isalpha(move[2]) && isdigit(move[3])) {
            if (board.movePiece(move, currentPlayer)) {
                currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
            }
        } else {
            cout << "Invalid input. Please enter a move in the format 'e2e4' or type 'quit' to exit." << endl;
        }
    }

    return 0;
}
