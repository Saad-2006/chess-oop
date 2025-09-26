#include <iostream>
#include <string>

using namespace std;

/** @brief Game state constants for determining game outcome. */
const int GAME_NONE = 0;       ///< Game is ongoing.
const int GAME_CHECKMATE = 1;  ///< Game ends in checkmate.
const int GAME_STALEMATE = 2;  ///< Game ends in stalemate.

// Forward declarations
class ChessBoard;
class Position;

/**
 * @brief Converts a string to title case (first letter uppercase, rest lowercase) using ASCII manipulation.
 * @param input The input string to convert.
 * @return The title-cased string (e.g., "queen" -> "Queen").
 */
string toTitleCase(const string& input) {
    string result = input;
    if (result.empty()) return result;
    // Convert first character to uppercase
    if (result[0] >= 'a' && result[0] <= 'z') {
        result[0] -= 32; // e.g., 'q' (97) to 'Q' (65)
    }
    // Convert remaining characters to lowercase
    for (size_t i = 1; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] += 32; // e.g., 'U' (85) to 'u' (117)
        }
    }
    return result;
}

/**
 * @brief Interface for objects that can be displayed on the console.
 */
class Displayable {
public:
    /** @brief Displays the object (e.g., a chess piece or board). */
    virtual void display() const = 0;
    /** @brief Virtual destructor for safe inheritance. */
    virtual ~Displayable() {}
};

/**
 * @brief Represents a position on the chessboard using row and column indices.
 */
class Position {
private:
    int row; ///< Row index (0-7, where 0 is 8th rank, 7 is 1st rank).
    int col; ///< Column index (0-7, where 0 is 'a', 7 is 'h').

public:
    /**
     * @brief Constructs a Position with optional row and column.
     * @param r Row index (default -1 for invalid).
     * @param c Column index (default -1 for invalid).
     */
    Position(int r = -1, int c = -1) : row(r), col(c) {}
    /** @brief Gets the row index. @return Row index. */
    int getRow() const { return row; }
    /** @brief Gets the column index. @return Column index. */
    int getCol() const { return col; }
};

/**
 * @brief Converts algebraic notation (e.g., "e4") to board indices.
 * @param pos The algebraic position (e.g., "e4").
 * @return Position object with row and column indices, or (-1, -1) if invalid.
 */
Position algebraicToIndices(const string& pos);

/**
 * @brief Abstract base class for chess pieces, implementing Displayable.
 */
class ChessPiece : public Displayable {
public:
    static const int PAWN_TYPE = 0;   ///< Pawn piece type.
    static const int ROOK_TYPE = 1;   ///< Rook piece type.
    static const int KNIGHT_TYPE = 2; ///< Knight piece type.
    static const int BISHOP_TYPE = 3; ///< Bishop piece type.
    static const int QUEEN_TYPE = 4;  ///< Queen piece type.
    static const int KING_TYPE = 5;   ///< King piece type.

protected:
    string color;    ///< Piece color ("White" or "Black").
    int x, y;        ///< Current position (row, column).
    bool hasMoved;   ///< Tracks if the piece has moved.
    int type;        ///< Piece type (e.g., PAWN_TYPE).

public:
    /**
     * @brief Constructs a ChessPiece.
     * @param col Color of the piece ("White" or "Black").
     * @param xPos Row position (0-7).
     * @param yPos Column position (0-7).
     * @param t Piece type (e.g., PAWN_TYPE).
     */
    ChessPiece(string col, int xPos, int yPos, int t)
        : color(col), x(xPos), y(yPos), hasMoved(false), type(t) {}
    /** @brief Virtual destructor for safe inheritance. */
    virtual ~ChessPiece() {}
    /**
     * @brief Attempts to move the piece to a new position.
     * @param newX Target row.
     * @param newY Target column.
     * @param board Reference to the chessboard.
     * @param needsPromotion Set to true if pawn promotion is required.
     * @return True if the move is valid and executed, false otherwise.
     */
    virtual bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion) = 0;
    /** @brief Displays the piece's symbol. */
    void display() const { cout << getSymbol(); }
    /** @brief Gets the piece's display symbol. @return Unicode symbol. */
    virtual string getSymbol() const = 0;
    /**
     * @brief Checks if the piece can attack a target square.
     * @param targetX Target row.
     * @param targetY Target column.
     * @param board Reference to the chessboard.
     * @return True if the piece can attack the square, false otherwise.
     */
    virtual bool canAttack(int targetX, int targetY, const ChessBoard& board) const = 0;
    /** @brief Gets the piece's color. @return "White" or "Black". */
    string getColor() const { return color; }
    /** @brief Gets the current row. @return Row index. */
    int getX() const { return x; }
    /** @brief Gets the current column. @return Column index. */
    int getY() const { return y; }
    /**
     * @brief Updates the piece's position and marks it as moved.
     * @param newX New row.
     * @param newY New column.
     */
    void setPosition(int newX, int newY) { x = newX; y = newY; hasMoved = true; }
    /** @brief Checks if the piece has moved. @return True if moved. */
    bool getHasMoved() const { return hasMoved; }
    /** @brief Gets the piece type. @return Type constant (e.g., PAWN_TYPE). */
    int getType() const { return type; }
};

/**
 * @brief Represents a Pawn piece.
 */
class Pawn : public ChessPiece {
public:
    /**
     * @brief Constructs a Pawn.
     * @param col Color ("White" or "Black").
     * @param xPos Row position.
     * @param yPos Column position.
     */
    Pawn(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, PAWN_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♟" : "♙"; }
};

/**
 * @brief Represents a Rook piece.
 */
class Rook : public ChessPiece {
public:
    /**
     * @brief Constructs a Rook.
     * @param col Color ("White" or "Black").
     * @param xPos Row position.
     * @param yPos Column position.
     */
    Rook(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, ROOK_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♜" : "♖"; }
};

/**
 * @brief Represents a Knight piece.
 */
class Knight : public ChessPiece {
public:
    /**
     * @brief Constructs a Knight.
     * @param col Color ("White" or "Black").
     * @param xPos Row position.
     * @param yPos Column position.
     */
    Knight(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, KNIGHT_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♞" : "♘"; }
};

/**
 * @brief Represents a Bishop piece.
 */
class Bishop : public ChessPiece {
public:
    /**
     * @brief Constructs a Bishop.
     * @param col Color ("White" or "Black").
     * @param xPos Row position.
     * @param yPos Column position.
     */
    Bishop(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, BISHOP_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♝" : "♗"; }
};

/**
 * @brief Represents a Queen piece.
 */
class Queen : public ChessPiece {
public:
    /**
     * @brief Constructs a Queen.
     * @param col Color ("White" or "Black").
     * @param xPos Row position.
     * @param yPos Column position.
     */
    Queen(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, QUEEN_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♛" : "♕"; }
};

/**
 * @brief Represents a King piece.
 */
class King : public ChessPiece {
public:
    /**
     * @brief Constructs a King.
     * @param col Color ("White" or "Black").
     * @param xPos Row position.
     * @param yPos Column position.
     */
    King(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, KING_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♚" : "♔"; }
};

/**
 * @brief Represents a single square on the chessboard.
 */
class Square {
private:
    ChessPiece* piece; ///< Pointer to the piece on this square, or nullptr.

public:
    /** @brief Constructs an empty square. */
    Square() : piece(nullptr) {}
    /** @brief Destructor, deletes the piece if present. */
    ~Square() { delete piece; }
    /**
     * @brief Sets a piece on the square.
     * @param p Pointer to the ChessPiece (can be nullptr).
     */
    void setPiece(ChessPiece* p) { piece = p; }
    /** @brief Gets the piece on the square. @return Pointer to the piece, or nullptr. */
    ChessPiece* getPiece() const { return piece; }
};

/**
 * @brief Represents the chessboard, managing pieces and game state.
 */
class ChessBoard : public Displayable {
private:
    Square squares[8][8];                       ///< 8x8 grid of squares.
    int lastMoveFromX, lastMoveFromY;           ///< Source of the last move.
    int lastMoveToX, lastMoveToY;               ///< Destination of the last move.
    bool lastMoveWasPawnTwoSquares;             ///< Tracks if the last move was a pawn's two-square advance.
    int halfMoveClock;                          ///< Counts half-moves for the 50-move rule.
    string positionHistory[1000];               ///< Stores board position hashes for threefold repetition.
    int positionCount;                          ///< Number of stored positions.

    /**
     * @brief Generates a hash of the current board position for repetition detection.
     * @return String representing the board state.
     */
    string getPositionHash() const {
        string hash;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = squares[i][j].getPiece();
                if (piece) {
                    hash += piece->getSymbol();
                } else {
                    hash += '.';
                }
            }
        }
        hash += canCastle("White", true) ? "WKS" : "";
        hash += canCastle("White", false) ? "WQS" : "";
        hash += canCastle("Black", true) ? "BKS" : "";
        hash += canCastle("Black", false) ? "BQS" : "";
        if (lastMoveWasPawnTwoSquares) {
            int enPassantX = (lastMoveFromX + lastMoveToX) / 2;
            hash += algebraicFromIndices(enPassantX, lastMoveToY);
        }
        return hash;
    }

public:
    /** @brief Constructs an empty chessboard with initialized state. */
    ChessBoard() : lastMoveFromX(-1), lastMoveFromY(-1), lastMoveToX(-1), lastMoveToY(-1),
                   lastMoveWasPawnTwoSquares(false), halfMoveClock(0), positionCount(0) {
        for (int i = 0; i < 1000; i++) positionHistory[i] = "";
    }
    /** @brief Destructor, clears all pieces. */
    ~ChessBoard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                squares[i][j].setPiece(nullptr);
            }
        }
    }
    /** @brief Initializes the board with standard chess setup. */
    void initializeBoard() {
        for (int i = 0; i < 8; i++) {
            squares[1][i].setPiece(new Pawn("Black", 1, i));
            squares[6][i].setPiece(new Pawn("White", 6, i));
        }
        squares[0][0].setPiece(new Rook("Black", 0, 0));
        squares[0][7].setPiece(new Rook("Black", 0, 7));
        squares[7][0].setPiece(new Rook("White", 7, 0));
        squares[7][7].setPiece(new Rook("White", 7, 7));
        squares[0][1].setPiece(new Knight("Black", 0, 1));
        squares[0][6].setPiece(new Knight("Black", 0, 6));
        squares[7][1].setPiece(new Knight("White", 7, 1));
        squares[7][6].setPiece(new Knight("White", 7, 6));
        squares[0][2].setPiece(new Bishop("Black", 0, 2));
        squares[0][5].setPiece(new Bishop("Black", 0, 5));
        squares[7][2].setPiece(new Bishop("White", 7, 2));
        squares[7][5].setPiece(new Bishop("White", 7, 5));
        squares[0][3].setPiece(new Queen("Black", 0, 3));
        squares[7][3].setPiece(new Queen("White", 7, 3));
        squares[0][4].setPiece(new King("Black", 0, 4));
        squares[7][4].setPiece(new King("White", 7, 4));
    }
    /** @brief Displays the current board state. */
    void display() const {
        cout << "  a b c d e f g h\n";
        for (int i = 0; i < 8; i++) {
            cout << (8 - i) << " ";
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = squares[i][j].getPiece();
                if (piece) {
                    piece->display();
                    cout << " ";
                } else {
                    cout << ". ";
                }
            }
            cout << (8 - i) << "\n";
        }
        cout << "  a b c d e f g h\n";
    }
    /**
     * @brief Gets the piece at a position.
     * @param x Row index.
     * @param y Column index.
     * @return Pointer to the piece, or nullptr if empty or invalid.
     */
    ChessPiece* getPieceAt(int x, int y) const {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            return squares[x][y].getPiece();
        }
        return nullptr;
    }
    /**
     * @brief Sets a piece at a position.
     * @param x Row index.
     * @param y Column index.
     * @param piece Pointer to the piece (can be nullptr).
     */
    void setPieceAt(int x, int y, ChessPiece* piece) {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            squares[x][y].setPiece(piece);
        }
    }
    /**
     * @brief Checks if a move is valid for a piece.
     * @param piece The piece to move.
     * @param newX Target row.
     * @param newY Target column.
     * @return True if the move is valid, false otherwise.
     */
    bool isValidMove(ChessPiece* piece, int newX, int newY) const {
        if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
            return false;
        }
        ChessPiece* target = getPieceAt(newX, newY);
        if (target && target->getColor() == piece->getColor()) {
            return false;
        }
        return true;
    }
    /**
     * @brief Checks if a player's king is in check.
     * @param color The player's color ("White" or "Black").
     * @return True if the king is in check, false otherwise.
     */
    bool isKingInCheck(const string& color) const {
        int kingX = -1, kingY = -1;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = getPieceAt(i, j);
                if (piece && piece->getColor() == color && piece->getType() == ChessPiece::KING_TYPE) {
                    kingX = i;
                    kingY = j;
                    break;
                }
            }
            if (kingX != -1) break;
        }
        if (kingX == -1 || kingY == -1) return false;
        string opponentColor = (color == "White") ? "Black" : "White";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = getPieceAt(i, j);
                if (piece && piece->getColor() == opponentColor) {
                    if (piece->canAttack(kingX, kingY, *this)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    /**
     * @brief Checks if a square is under attack by a player.
     * @param x Row index.
     * @param y Column index.
     * @param attackerColor The attacking player's color.
     * @return True if the square is under attack, false otherwise.
     */
    bool isSquareUnderAttack(int x, int y, const string& attackerColor) const {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = getPieceAt(i, j);
                if (piece && piece->getColor() == attackerColor) {
                    if (piece->canAttack(x, y, *this)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    /**
     * @brief Checks if castling is possible.
     * @param color The player's color.
     * @param kingside True for kingside castling, false for queenside.
     * @return True if castling is possible, false otherwise.
     */
    bool canCastle(const string& color, bool kingside) const {
        int kingX = (color == "White") ? 7 : 0;
        int kingY = 4;
        int rookY = kingside ? 7 : 0;
        ChessPiece* king = getPieceAt(kingX, kingY);
        ChessPiece* rook = getPieceAt(kingX, rookY);
        if (!king || king->getType() != ChessPiece::KING_TYPE || king->getHasMoved()) {
            return false;
        }
        if (!rook || rook->getType() != ChessPiece::ROOK_TYPE || rook->getHasMoved()) {
            return false;
        }
        if (isKingInCheck(color)) {
            return false;
        }
        int yStart = kingside ? 5 : 1;
        int yEnd = kingside ? 7 : 0;
        int yStep = kingside ? 1 : -1;
        for (int y = yStart; y != yEnd; y += yStep) {
            if (getPieceAt(kingX, y)) {
                return false;
            }
        }
        string opponentColor = (color == "White") ? "Black" : "White";
        int checkYStart = kingY;
        int checkYEnd = kingside ? kingY + 2 : kingY - 2;
        int checkYStep = kingside ? 1 : -1;
        for (int y = checkYStart; y != checkYEnd + checkYStep; y += checkYStep) {
            if (isSquareUnderAttack(kingX, y, opponentColor)) {
                return false;
            }
        }
        return true;
    }
    /**
     * @brief Determines if the game is over for a player.
     * @param color The player's color.
     * @return GAME_CHECKMATE, GAME_STALEMATE, or GAME_NONE.
     */
    int isGameOver(const string& color) {
        bool inCheck = isKingInCheck(color);
        string opponentColor = (color == "White") ? "Black" : "White";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = getPieceAt(i, j);
                if (piece && piece->getColor() == color) {
                    int origX = piece->getX();
                    int origY = piece->getY();
                    for (int newX = 0; newX < 8; newX++) {
                        for (int newY = 0; newY < 8; newY++) {
                            bool needsPromotion = false;
                            if (piece->move(newX, newY, *this, needsPromotion)) {
                                ChessPiece* target = getPieceAt(newX, newY);
                                setPieceAt(newX, newY, piece);
                                setPieceAt(origX, origY, nullptr);
                                bool stillInCheck = isKingInCheck(color);
                                setPieceAt(origX, origY, piece);
                                setPieceAt(newX, newY, target);
                                piece->setPosition(origX, origY);
                                if (!stillInCheck) {
                                    return GAME_NONE;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (inCheck) {
            return GAME_CHECKMATE;
        }
        return GAME_STALEMATE;
    }
    /**
     * @brief Promotes a pawn to a new piece.
     * @param x Row of the pawn.
     * @param y Column of the pawn.
     * @param pieceType Type to promote to ("Queen", "Rook", "Bishop", "Knight").
     * @param color The pawn's color.
     * @return Pointer to the new piece, or nullptr if invalid.
     */
    ChessPiece* promotePawn(int x, int y, const string& pieceType, const string& color) {
        ChessPiece* piece = getPieceAt(x, y);
        if (!piece || piece->getType() != ChessPiece::PAWN_TYPE) {
            return nullptr;
        }
        ChessPiece* newPiece = nullptr;
        if (pieceType == "Queen") {
            newPiece = new Queen(color, x, y);
        } else if (pieceType == "Rook") {
            newPiece = new Rook(color, x, y);
        } else if (pieceType == "Bishop") {
            newPiece = new Bishop(color, x, y);
        } else if (pieceType == "Knight") {
            newPiece = new Knight(color, x, y);
        } else {
            return nullptr;
        }
        setPieceAt(x, y, newPiece);
        return newPiece;
    }
    /**
     * @brief Checks for a draw by the 50-move rule.
     * @return True if 50 moves (100 half-moves) have passed without capture or pawn move.
     */
    bool isFiftyMoveDraw() const {
        return halfMoveClock >= 100;
    }
    /**
     * @brief Checks for a draw by threefold repetition.
     * @return True if the same position has occurred three times.
     */
    bool isThreefoldRepetition() const {
        if (positionCount == 0) return false;
        string current = positionHistory[positionCount - 1];
        int count = 0;
        for (int i = 0; i < positionCount; i++) {
            if (positionHistory[i] == current) {
                count++;
            }
        }
        return count >= 3;
    }
    /**
     * @brief Updates move history for draw detection.
     * @param fromX Source row.
     * @param fromY Source column.
     * @param toX Destination row.
     * @param toY Destination column.
     * @param piece The moved piece.
     * @param isCapture True if the move was a capture.
     */
    void updateMoveHistory(int fromX, int fromY, int toX, int toY, ChessPiece* piece, bool isCapture) {
        lastMoveFromX = fromX;
        lastMoveFromY = fromY;
        lastMoveToX = toX;
        lastMoveToY = toY;
        lastMoveWasPawnTwoSquares = false;
        if (piece->getType() == ChessPiece::PAWN_TYPE) {
            if ((fromX - toX == 2 || toX - fromX == 2) && fromY == toY) {
                lastMoveWasPawnTwoSquares = true;
            }
        }
        halfMoveClock++;
        if (isCapture || piece->getType() == ChessPiece::PAWN_TYPE) {
            halfMoveClock = 0;
        }
        if (positionCount < 1000) {
            positionHistory[positionCount++] = getPositionHash();
        }
    }
    /**
     * @brief Converts indices to algebraic notation.
     * @param x Row index.
     * @param y Column index.
     * @return Algebraic notation (e.g., "e4").
     */
    string algebraicFromIndices(int x, int y) const {
        string pos;
        pos += char('a' + y);
        pos += char('8' - x);
        return pos;
    }
    /** @brief Gets the last move's destination row. @return Row index. */
    int getLastMoveToX() const { return lastMoveToX; }
    /** @brief Gets the last move's source row. @return Row index. */
    int getLastMoveFromX() const { return lastMoveFromX; }
};

/**
 * @brief Represents a chess move from one position to another.
 */
class Move {
private:
    int fromX, fromY; ///< Source position.
    int toX, toY;     ///< Destination position.

public:
    /**
     * @brief Constructs a Move.
     * @param fx Source row.
     * @param fy Source column.
     * @param tx Destination row.
     * @param ty Destination column.
     */
    Move(int fx, int fy, int tx, int ty) : fromX(fx), fromY(fy), toX(tx), toY(ty) {}
    /** @brief Gets source row. @return Row index. */
    int getFromX() const { return fromX; }
    /** @brief Gets source column. @return Column index. */
    int getFromY() const { return fromY; }
    /** @brief Gets destination row. @return Row index. */
    int getToX() const { return toX; }
    /** @brief Gets destination column. @return Column index. */
    int getToY() const { return toY; }
};

/**
 * @brief Represents a player and their pieces.
 */
class Player {
private:
    string color;            ///< Player's color ("White" or "Black").
    ChessPiece* pieces[16];  ///< Array of player's pieces.
    int pieceCount;          ///< Number of active pieces.

public:
    /**
     * @brief Constructs a Player.
     * @param col Player's color.
     */
    Player(string col) : color(col), pieceCount(0) {
        for (int i = 0; i < 16; i++) {
            pieces[i] = nullptr;
        }
    }
    /** @brief Destructor, deletes all pieces. */
    ~Player() {
        for (int i = 0; i < pieceCount; i++) {
            delete pieces[i];
            pieces[i] = nullptr;
        }
    }
    /**
     * @brief Adds a piece to the player's collection.
     * @param piece Pointer to the ChessPiece.
     */
    void addPiece(ChessPiece* piece) {
        if (pieceCount < 16) {
            pieces[pieceCount++] = piece;
        }
    }
    /** @brief Gets the player's color. @return "White" or "Black". */
    string getColor() const { return color; }
    /**
     * @brief Gets the player's pieces.
     * @param count Output parameter for the number of pieces.
     * @return Array of piece pointers.
     */
    ChessPiece** getPieces(int& count) {
        count = pieceCount;
        return pieces;
    }
    /**
     * @brief Removes a piece from the player's collection.
     * @param piece Pointer to the ChessPiece to remove.
     */
    void removePiece(ChessPiece* piece) {
        for (int i = 0; i < pieceCount; i++) {
            if (pieces[i] == piece) {
                for (int j = i; j < pieceCount - 1; j++) {
                    pieces[j] = pieces[j + 1];
                }
                pieces[pieceCount - 1] = nullptr;
                pieceCount--;
                break;
            }
        }
    }
};

/**
 * @brief Manages the chess game, including players and the board.
 */
class Game {
private:
    ChessBoard board;           ///< The chessboard.
    Player whitePlayer;         ///< White player.
    Player blackPlayer;         ///< Black player.
    Player* currentPlayer;      ///< Pointer to the current player.

public:
    /** @brief Constructs a new game with initialized board and players. */
    Game() : whitePlayer("White"), blackPlayer("Black"), currentPlayer(&whitePlayer) {
        board.initializeBoard();
        initializePlayers();
    }
    /** @brief Initializes players with their starting pieces. */
    void initializePlayers() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ChessPiece* piece = board.getPieceAt(i, j);
                if (piece) {
                    if (piece->getColor() == "White") {
                        whitePlayer.addPiece(piece);
                    } else {
                        blackPlayer.addPiece(piece);
                    }
                }
            }
        }
    }
    /** @brief Starts and runs the game loop. */
    void startGame() {
        bool gameOver = false;
        while (!gameOver) {
            board.display();
            cout << currentPlayer->getColor() << "'s turn. Enter move (e.g., e2 e4, O-O, O-O-O) or 'resign' to quit: ";
            string input;
            cin >> input;
            if (input == "resign") {
                cout << currentPlayer->getColor() << " resigns. " << (currentPlayer == &whitePlayer ? "Black" : "White") << " wins!\n";
                gameOver = true;
                break;
            }
            string from, to;
            if (input == "O-O") {
                from = (currentPlayer->getColor() == "White") ? "e1" : "e8";
                to = (currentPlayer->getColor() == "White") ? "g1" : "g8";
            } else if (input == "O-O-O") {
                from = (currentPlayer->getColor() == "White") ? "e1" : "e8";
                to = (currentPlayer->getColor() == "White") ? "c1" : "c8";
            } else {
                from = input;
                cin >> to;
            }
            Position fromPos = algebraicToIndices(from);
            Position toPos = algebraicToIndices(to);
            if (fromPos.getRow() == -1 || toPos.getRow() == -1) {
                cout << "Invalid position format\n";
                continue;
            }
            Move move(fromPos.getRow(), fromPos.getCol(), toPos.getRow(), toPos.getCol());
            try {
                makeMove(move);
                string opponentColor = (currentPlayer == &whitePlayer) ? "Black" : "White";
                int state = board.isGameOver(opponentColor);
                if (state == GAME_CHECKMATE) {
                    board.display();
                    cout << "Checkmate! " << currentPlayer->getColor() << " wins!\n";
                    gameOver = true;
                    break;
                } else if (state == GAME_STALEMATE) {
                    board.display();
                    cout << "Stalemate! The game is a draw.\n";
                    gameOver = true;
                    break;
                }
                if (board.isFiftyMoveDraw()) {
                    board.display();
                    cout << "Draw by 50-move rule!\n";
                    gameOver = true;
                    break;
                }
                if (board.isThreefoldRepetition()) {
                    board.display();
                    cout << "Draw by threefold repetition!\n";
                    gameOver = true;
                    break;
                }
                switchPlayer();
            } catch (const char* error) {
                cout << "Error: " << error << "\n";
            }
        }
        if (!gameOver) {
            board.display();
            cout << "Game ended.\n";
        }
    }
    /**
     * @brief Executes a move, handling captures, castling, en passant, and promotion.
     * @param move The move to execute.
     * @throws const char* Error message if the move is invalid.
     */
    void makeMove(const Move& move) {
        int fromX = move.getFromX();
        int fromY = move.getFromY();
        int toX = move.getToX();
        int toY = move.getToY();
        ChessPiece* piece = board.getPieceAt(fromX, fromY);
        if (!piece) {
            throw "No piece at starting position";
        }
        if (piece->getColor() != currentPlayer->getColor()) {
            throw "Not your piece";
        }
        bool needsPromotion = false;
        bool isCapture = board.getPieceAt(toX, toY) != nullptr;
        if (!board.isValidMove(piece, toX, toY) || !piece->move(toX, toY, board, needsPromotion)) {
            throw "Invalid move";
        }
        ChessPiece* target = board.getPieceAt(toX, toY);
        bool isEnPassant = false;
        if (piece->getType() == ChessPiece::PAWN_TYPE && toY != fromY && !target && board.getPieceAt(toX, fromY)) {
            target = board.getPieceAt(toX, fromY);
            isCapture = true;
            isEnPassant = true;
        }
        bool isCastling = piece->getType() == ChessPiece::KING_TYPE && (toY - fromY == 2 || toY - fromY == -2);
        ChessPiece* rook = nullptr;
        int rookFromX = 0, rookFromY = 0, rookToX = 0, rookToY = 0;
        if (isCastling) {
            bool kingside = toY > fromY;
            rookFromY = kingside ? 7 : 0;
            rookToY = kingside ? 5 : 3;
            rookFromX = rookToX = fromX;
            rook = board.getPieceAt(rookFromX, rookFromY);
            if (!board.canCastle(piece->getColor(), kingside)) {
                throw "Invalid castling move";
            }
        }
        ChessPiece* tempPiece = piece;
        board.setPieceAt(toX, toY, piece);
        board.setPieceAt(fromX, fromY, nullptr);
        if (isCastling) {
            board.setPieceAt(rookToX, rookToY, rook);
            board.setPieceAt(rookFromX, rookFromY, nullptr);
        }
        if (isEnPassant) {
            board.setPieceAt(toX, fromY, nullptr);
        }
        if (board.isKingInCheck(currentPlayer->getColor())) {
            board.setPieceAt(fromX, fromY, tempPiece);
            board.setPieceAt(toX, toY, target);
            if (isCastling) {
                board.setPieceAt(rookFromX, rookFromY, rook);
                board.setPieceAt(rookToX, rookToY, nullptr);
            }
            if (isEnPassant) {
                board.setPieceAt(toX, fromY, target);
            }
            tempPiece->setPosition(fromX, fromY);
            throw "Move puts your king in check";
        }
        if (needsPromotion) {
            string pieceType;
            cout << "Pawn promotion! Choose piece (Queen, Rook, Bishop, Knight): ";
            cin >> pieceType;
            pieceType = toTitleCase(pieceType);
            ChessPiece* newPiece = board.promotePawn(toX, toY, pieceType, piece->getColor());
            if (!newPiece) {
                board.setPieceAt(fromX, fromY, tempPiece);
                board.setPieceAt(toX, toY, target);
                if (isEnPassant) {
                    board.setPieceAt(toX, fromY, target);
                }
                tempPiece->setPosition(fromX, fromY);
                throw "Invalid promotion piece";
            }
            currentPlayer->removePiece(tempPiece);
            delete tempPiece;
            currentPlayer->addPiece(newPiece);
        }
        if (isCapture && !isEnPassant) {
            Player* opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
            opponent->removePiece(target);
            delete target;
        }
        if (isEnPassant) {
            Player* opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
            opponent->removePiece(target);
            delete target;
        }
        board.updateMoveHistory(fromX, fromY, toX, toY, board.getPieceAt(toX, toY), isCapture || isEnPassant);
    }
    /** @brief Switches the current player. */
    void switchPlayer() {
        currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
    }
};

// **Piece Movement and Attack Implementations**

/**
 * @brief Moves a pawn to a new position.
 * @param newX Target row.
 * @param newY Target column.
 * @param board Reference to the chessboard.
 * @param needsPromotion Set to true if promotion is required.
 * @return True if the move is valid, false otherwise.
 */
bool Pawn::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
    needsPromotion = false;
    int direction = (color == "White") ? -1 : 1;
    int startX = (color == "White") ? 6 : 1;
    int promotionRank = (color == "White") ? 0 : 7;
    if (newY == y) {
        if (newX == x + direction && !board.getPieceAt(newX, newY)) {
            if (newX == promotionRank) {
                needsPromotion = true;
            }
            setPosition(newX, newY);
            return true;
        }
        if (x == startX && newX == x + 2 * direction &&
            !board.getPieceAt(x + direction, y) && !board.getPieceAt(newX, newY)) {
            setPosition(newX, newY);
            return true;
        }
    } else if (newX - x == direction && (newY - y == 1 || y - newY == 1)) {
        ChessPiece* target = board.getPieceAt(newX, newY);
        if (target && target->getColor() != color) {
            if (newX == promotionRank) {
                needsPromotion = true;
            }
            setPosition(newX, newY);
            return true;
        }
        if (!target && newX == board.getLastMoveToX() &&
            abs(board.getLastMoveFromX() - board.getLastMoveToX()) == 2 &&
            board.getPieceAt(newX, y) &&
            board.getPieceAt(newX, y)->getColor() != color) {
            setPosition(newX, newY);
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a pawn can attack a target square.
 * @param targetX Target row.
 * @param targetY Target column.
 * @param board Reference to the chessboard.
 * @return True if the pawn can attack the square, false otherwise.
 */
bool Pawn::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    int direction = (color == "White") ? -1 : 1;
    return (targetX - x == direction && (targetY - y == 1 || y - targetY == 1));
}

/**
 * @brief Moves a rook to a new position.
 * @param newX Target row.
 * @param newY Target column.
 * @param board Reference to the chessboard.
 * @param needsPromotion Always false for rooks.
 * @return True if the move is valid, false otherwise.
 */
bool Rook::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
    needsPromotion = false;
    if (newX == x) {
        int startY = y < newY ? y + 1 : newY + 1;
        int endY = y < newY ? newY : y;
        for (int cy = startY; cy < endY; cy++) {
            if (board.getPieceAt(x, cy)) return false;
        }
    } else if (newY == y) {
        int startX = x < newX ? x + 1 : newX + 1;
        int endX = x < newX ? newX : x;
        for (int cx = startX; cx < endX; cx++) {
            if (board.getPieceAt(cx, y)) return false;
        }
    } else {
        return false;
    }
    ChessPiece* target = board.getPieceAt(newX, newY);
    if (!target || target->getColor() != color) {
        setPosition(newX, newY);
        return true;
    }
    return false;
}

/**
 * @brief Checks if a rook can attack a target square.
 * @param targetX Target row.
 * @param targetY Target column.
 * @param board Reference to the chessboard.
 * @return True if the rook can attack the square, false otherwise.
 */
bool Rook::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    if (targetX == x) {
        int startY = y < targetY ? y + 1 : targetY + 1;
        int endY = y < targetY ? targetY : y;
        for (int cy = startY; cy < endY; cy++) {
            if (board.getPieceAt(x, cy)) return false;
        }
        return true;
    } else if (targetY == y) {
        int startX = x < targetX ? x + 1 : targetX + 1;
        int endX = x < targetX ? targetX : x;
        for (int cx = startX; cx < endX; cx++) {
            if (board.getPieceAt(cx, y)) return false;
        }
        return true;
    }
    return false;
}

/**
 * @brief Moves a knight to a new position.
 * @param newX Target row.
 * @param newY Target column.
 * @param board Reference to the chessboard.
 * @param needsPromotion Always false for knights.
 * @return True if the move is valid, false otherwise.
 */
bool Knight::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
    needsPromotion = false;
    int dx = newX > x ? newX - x : x - newX;
    int dy = newY > y ? newY - y : y - newY;
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        ChessPiece* target = board.getPieceAt(newX, newY);
        if (!target || target->getColor() != color) {
            setPosition(newX, newY);
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a knight can attack a target square.
 * @param targetX Target row.
 * @param targetY Target column.
 * @param board Reference to the chessboard.
 * @return True if the knight can attack the square, false otherwise.
 */
bool Knight::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    int dx = targetX > x ? targetX - x : x - targetX;
    int dy = targetY > y ? targetY - y : y - targetY;
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

/**
 * @brief Moves a bishop to a new position.
 * @param newX Target row.
 * @param newY Target column.
 * @param board Reference to the chessboard.
 * @param needsPromotion Always false for bishops.
 * @return True if the move is valid, false otherwise.
 */
bool Bishop::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
    needsPromotion = false;
    if (newX - x == newY - y || newX - x == -(newY - y)) {
        int dx = (newX > x) ? 1 : -1;
        int dy = (newY > y) ? 1 : -1;
        int cx = x + dx;
        int cy = y + dy;
        while (cx != newX && cy != newY) {
            if (board.getPieceAt(cx, cy)) return false;
            cx += dx;
            cy += dy;
        }
        ChessPiece* target = board.getPieceAt(newX, newY);
        if (!target || target->getColor() != color) {
            setPosition(newX, newY);
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a bishop can attack a target square.
 * @param targetX Target row.
 * @param targetY Target column.
 * @param board Reference to the chessboard.
 * @return True if the bishop can attack the square, false otherwise.
 */
bool Bishop::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    if (targetX - x == targetY - y || targetX - x == -(targetY - y)) {
        int dx = (targetX > x) ? 1 : -1;
        int dy = (targetY > y) ? 1 : -1;
        int cx = x + dx;
        int cy = y + dy;
        while (cx != targetX && cy != targetY) {
            if (board.getPieceAt(cx, cy)) return false;
            cx += dx;
            cy += dy;
        }
        return true;
    }
    return false;
}

/**
 * @brief Moves a queen to a new position.
 * @param newX Target row.
 * @param newY Target column.
 * @param board Reference to the chessboard.
 * @param needsPromotion Always false for queens.
 * @return True if the move is valid, false otherwise.
 */
bool Queen::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
    needsPromotion = false;
    if (newX == x || newY == y) {
        return Rook(color, x, y).move(newX, newY, board, needsPromotion);
    }
    if (newX - x == newY - y || newX - x == -(newY - y)) {
        return Bishop(color, x, y).move(newX, newY, board, needsPromotion);
    }
    return false;
}

/**
 * @brief Checks if a queen can attack a target square.
 * @param targetX Target row.
 * @param targetY Target column.
 * @param board Reference to the chessboard.
 * @return True if the queen can attack the square, false otherwise.
 */
bool Queen::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    if (targetX == x || targetY == y) {
        return Rook(color, x, y).canAttack(targetX, targetY, board);
    }
    if (targetX - x == targetY - y || targetX - x == -(targetY - y)) {
        return Bishop(color, x, y).canAttack(targetX, targetY, board);
    }
    return false;
}

/**
 * @brief Moves a king to a new position, including castling.
 * @param newX Target row.
 * @param newY Target column.
 * @param board Reference to the chessboard.
 * @param needsPromotion Always false for kings.
 * @return True if the move is valid, false otherwise.
 */
bool King::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
    needsPromotion = false;
    int dx = newX > x ? newX - x : x - newX;
    int dy = newY > y ? newY - y : y - newY;
    if (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0)) {
        ChessPiece* target = board.getPieceAt(newX, newY);
        if (!target || target->getColor() != color) {
            setPosition(newX, newY);
            return true;
        }
    }
    if (!hasMoved && newX == x && abs(newY - y) == 2) {
        bool kingside = newY > y;
        if (board.canCastle(color, kingside)) {
            setPosition(newX, newY);
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a king can attack a target square.
 * @param targetX Target row.
 * @param targetY Target column.
 * @param board Reference to the chessboard.
 * @return True if the king can attack the square, false otherwise.
 */
bool King::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    int dx = targetX > x ? targetX - x : x - targetX;
    int dy = targetY > y ? targetY - y : y - targetY;
    return dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0);
}

/**
 * @brief Converts algebraic notation to board indices.
 * @param pos The algebraic position (e.g., "e4").
 * @return Position object with row and column indices, or (-1, -1) if invalid.
 */
Position algebraicToIndices(const string& pos) {
    if (pos.length() != 2) return Position(-1, -1);
    char col = pos[0];
    char row = pos[1];
    if (col < 'a' || col > 'h' || row < '1' || row > '8') return Position(-1, -1);
    int colIndex = col - 'a';
    int rowIndex = 7 - (row - '1');
    return Position(rowIndex, colIndex);
}

/**
 * @brief Main function to start the chess game.
 * @return 0 on successful execution.
 */
int main() {
    Game game1;
    game1.startGame();
    return 0;
}