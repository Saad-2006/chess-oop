/**
 * @file latest_chess.cpp
 * @brief console-based chess game
 * @author Saad Mehmood Athar [24L-3050]
 * @date May 2025
 * 
 * This program implements a fully functional chess game with support for standard rules,
 * including check, checkmate, stalemate, castling, en passant, pawn promotion, and draw conditions.
 * The game is played on a console interface where players input moves in algebraic notation.
 */

 #include <iostream>
 #include <string>
 
 using namespace std;
 
 // Game state constants
 const int GAME_NONE = 0;      ///< Game is ongoing
 const int GAME_CHECKMATE = 1; ///< Game ended in checkmate
 const int GAME_STALEMATE = 2; ///< Game ended in stalemate
 
 /**
  * @brief Converts a string to title case (e.g., "queen" to "Queen").
  * @param input The input string to convert.
  * @return The string in title case.
  */
 string toTitleCase(const string& input) {
     string result = input;
     if (result.empty()) return result;
     if (result[0] >= 'a' && result[0] <= 'z') result[0] -= 32;
     for (int i = 1; i < result.length(); i++) {
         if (result[i] >= 'A' && result[i] <= 'Z') result[i] += 32;
     }
     return result;
 }
 
 /**
  * @brief Interface for objects that can be displayed.
  */
 class Displayable {
 public:
     virtual void display() const = 0;
     virtual ~Displayable() {}
 };
 
 /**
  * @brief Represents a position on the chessboard using row and column indices.
  */
 class Position {
 private:
     int row; ///< Row index (0-7, where 0 is rank 8 and 7 is rank 1).
     int col; ///< Column index (0-7, where 0 is file 'a' and 7 is file 'h').
 
 public:
     /**
      * @brief Constructs a Position object.
      * @param r Row index (default -1).
      * @param c Column index (default -1).
      */
     Position(int r = -1, int c = -1) : row(r), col(c) {}
 
     /**
      * @brief Gets the row index.
      * @return The row index.
      */
     int getRow() const { return row; }
 
     /**
      * @brief Gets the column index.
      * @return The column index.
      */
     int getCol() const { return col; }
 };
 
 /**
  * @brief Converts algebraic notation (e.g., "e4") to board indices.
  * @param pos The position in algebraic notation (e.g., "e4").
  * @return A Position object with the corresponding row and column indices, or (-1, -1) if invalid.
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
  * @brief Abstract base class for all chess pieces.
  */
 class ChessPiece : public Displayable {
 public:
     static const int PAWN_TYPE = 0;   ///< Type identifier for Pawn.
     static const int ROOK_TYPE = 1;   ///< Type identifier for Rook.
     static const int KNIGHT_TYPE = 2; ///< Type identifier for Knight.
     static const int BISHOP_TYPE = 3; ///< Type identifier for Bishop.
     static const int QUEEN_TYPE = 4;  ///< Type identifier for Queen.
     static const int KING_TYPE = 5;   ///< Type identifier for King.
 
 protected:
     string color;    ///< Color of the piece ("White" or "Black").
     int x, y;        ///< Position of the piece on the board (row, col).
     bool hasMoved;   ///< Tracks if the piece has moved (for castling and pawn double moves).
     int type;        ///< Type of the piece (PAWN_TYPE, ROOK_TYPE, etc.).
 
 public:
     /**
      * @brief Constructs a ChessPiece.
      * @param col Color of the piece ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      * @param t Type of the piece.
      */
     ChessPiece(string col, int xPos, int yPos, int t)
         : color(col), x(xPos), y(yPos), hasMoved(false), type(t) {}
 
     virtual ~ChessPiece() {}
 
     /**
      * @brief Attempts to move the piece to a new position.
      * @param newX The target row.
      * @param newY The target column.
      * @param board The chessboard to move on.
      * @param needsPromotion Output parameter indicating if a pawn needs promotion.
      * @return True if the move is valid and executed, false otherwise.
      */
     virtual bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion) = 0;
 
     /**
      * @brief Displays the piece's symbol to the console.
      */
     void display() const { cout << getSymbol(); }
 
     /**
      * @brief Gets the Unicode symbol for the piece.
      * @return The piece's symbol as a string.
      */
     virtual string getSymbol() const = 0;
 
     /**
      * @brief Checks if the piece can attack a target position.
      * @param targetX The target row.
      * @param targetY The target column.
      * @param board The chessboard to check on.
      * @return True if the piece can attack the target, false otherwise.
      */
     virtual bool canAttack(int targetX, int targetY, const class ChessBoard& board) const = 0;
 
     /**
      * @brief Gets the color of the piece.
      * @return The color ("White" or "Black").
      */
     string getColor() const { return color; }
 
     /**
      * @brief Gets the row position of the piece.
      * @return The row index.
      */
     int getX() const { return x; }
 
     /**
      * @brief Gets the column position of the piece.
      * @return The column index.
      */
     int getY() const { return y; }
 
     /**
      * @brief Sets the position of the piece and marks it as moved.
      * @param newX The new row.
      * @param newY The new column.
      */
     void setPosition(int newX, int newY) { x = newX; y = newY; hasMoved = true; }
 
     /**
      * @brief Temporarily sets the position for attack checks (used during simulations).
      * @param newX The temporary row.
      * @param newY The temporary column.
      */
     void setPositionForAttack(int newX, int newY) const {
         const_cast<ChessPiece*>(this)->x = newX;
         const_cast<ChessPiece*>(this)->y = newY;
     }
 
     /**
      * @brief Checks if the piece has moved.
      * @return True if the piece has moved, false otherwise.
      */
     bool getHasMoved() const { return hasMoved; }
 
     /**
      * @brief Gets the type of the piece.
      * @return The piece type (e.g., PAWN_TYPE, KING_TYPE).
      */
     int getType() const { return type; }
 };
 
 /**
  * @brief Represents a pawn piece in chess.
  */
 class Pawn : public ChessPiece {
 public:
     /**
      * @brief Constructs a Pawn.
      * @param col Color of the pawn ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      */
     Pawn(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, PAWN_TYPE) {}
 
     bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion);
     bool canAttack(int targetX, int targetY, const class ChessBoard& board) const;
     string getSymbol() const { return color == "White" ? "♟" : "♙"; }
 };
 
 /**
  * @brief Represents a rook piece in chess.
  */
 class Rook : public ChessPiece {
 public:
     /**
      * @brief Constructs a Rook.
      * @param col Color of the rook ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      */
     Rook(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, ROOK_TYPE) {}
 
     bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion);
     bool canAttack(int targetX, int targetY, const class ChessBoard& board) const;
     string getSymbol() const { return color == "White" ? "♜" : "♖"; }
 };
 
 /**
  * @brief Represents a knight piece in chess.
  */
 class Knight : public ChessPiece {
 public:
     /**
      * @brief Constructs a Knight.
      * @param col Color of the knight ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      */
     Knight(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, KNIGHT_TYPE) {}
 
     bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion);
     bool canAttack(int targetX, int targetY, const class ChessBoard& board) const;
     string getSymbol() const { return color == "White" ? "♞" : "♘"; }
 };
 
 /**
  * @brief Represents a bishop piece in chess.
  */
 class Bishop : public ChessPiece {
 public:
     /**
      * @brief Constructs a Bishop.
      * @param col Color of the bishop ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      */
     Bishop(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, BISHOP_TYPE) {}
 
     bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion);
     bool canAttack(int targetX, int targetY, const class ChessBoard& board) const;
     string getSymbol() const { return color == "White" ? "♝" : "♗"; }
 };
 
 /**
  * @brief Represents a queen piece in chess.
  */
 class Queen : public ChessPiece {
 public:
     /**
      * @brief Constructs a Queen.
      * @param col Color of the queen ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      */
     Queen(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, QUEEN_TYPE) {}
 
     bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion);
     bool canAttack(int targetX, int targetY, const class ChessBoard& board) const;
     string getSymbol() const { return color == "White" ? "♛" : "♕"; }
 };
 
 /**
  * @brief Represents a king piece in chess.
  */
 class King : public ChessPiece {
 public:
     /**
      * @brief Constructs a King.
      * @param col Color of the king ("White" or "Black").
      * @param xPos Row position on the board.
      * @param yPos Column position on the board.
      */
     King(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, KING_TYPE) {}
 
     bool move(int newX, int newY, class ChessBoard& board, bool& needsPromotion);
     bool canAttack(int targetX, int targetY, const class ChessBoard& board) const;
     string getSymbol() const { return color == "White" ? "♚" : "♔"; }
 };
 
 /**
  * @brief Represents a single square on the chessboard.
  */
 class Square {
 private:
     ChessPiece* piece; ///< The piece occupying this square (nullptr if empty).
 
 public:
     /**
      * @brief Constructs an empty Square.
      */
     Square() : piece(nullptr) {}
 
     /**
      * @brief Destroys the Square and deletes the piece it contains.
      */
     ~Square() { delete piece; }
 
     /**
      * @brief Sets the piece on this square.
      * @param p The piece to place on the square (can be nullptr).
      */
     void setPiece(ChessPiece* p) { piece = p; }
 
     /**
      * @brief Gets the piece on this square.
      * @return Pointer to the piece, or nullptr if the square is empty.
      */
     ChessPiece* getPiece() const { return piece; }
 };
 
 /**
  * @brief Represents the chessboard and manages game state.
  */
 class ChessBoard : public Displayable {
 private:
     Square squares[8][8];                ///< 8x8 grid of squares.
     int lastMoveFromX, lastMoveFromY;    ///< Coordinates of the last move's starting position.
     int lastMoveToX, lastMoveToY;        ///< Coordinates of the last move's ending position.
     bool lastMoveWasPawnTwoSquares;      ///< Tracks if the last move was a pawn double move (for en passant).
     int halfMoveClock;                   ///< Counter for the 50-move draw rule.
     string positionHistory[1000];        ///< History of board positions for threefold repetition.
     int positionCount;                   ///< Number of positions in the history.
 
     /**
      * @brief Generates a hash of the current board position for threefold repetition.
      * @return A string representing the board state, including castling and en passant info.
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
     /**
      * @brief Constructs an empty ChessBoard.
      */
     ChessBoard() : lastMoveFromX(-1), lastMoveFromY(-1), lastMoveToX(-1), lastMoveToY(-1),
                    lastMoveWasPawnTwoSquares(false), halfMoveClock(0), positionCount(0) {
         for (int i = 0; i < 1000; i++) positionHistory[i] = "";
     }
 
     /**
      * @brief Destroys the ChessBoard and its squares.
      */
     ~ChessBoard() {}
 
     /**
      * @brief Initializes the board with the standard chess starting position.
      */
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
 
     /**
      * @brief Displays the current state of the board to the console.
      */
     void display() const {
         cout << "  a b c d e f g h\n";
         for (int i = 0; i < 8; i++) {
             cout << (8 - i) << " ";
             for (int j = 0; j < 8; j++) {
                 ChessPiece* piece = squares[i][j].getPiece();
                 if (piece) piece->display();
                 else cout << ".";
                 cout << " ";
             }
             cout << (8 - i) << "\n";
         }
         cout << "  a b c d e f g h\n";
     }
 
     /**
      * @brief Gets the piece at a specific position.
      * @param x The row index.
      * @param y The column index.
      * @return Pointer to the piece at (x, y), or nullptr if empty or out of bounds.
      */
     ChessPiece* getPieceAt(int x, int y) const {
         if (x < 0 || x > 7 || y < 0 || y > 7) return nullptr;
         return squares[x][y].getPiece();
     }
 
     /**
      * @brief Sets a piece at a specific position on the board.
      * @param x The row index.
      * @param y The column index.
      * @param piece The piece to place (can be nullptr).
      */
     void setPieceAt(int x, int y, ChessPiece* piece) {
         if (x >= 0 && x < 8 && y >= 0 && y < 8) squares[x][y].setPiece(piece);
     }
 
     /**
      * @brief Checks if a move is valid for a piece (basic validation, e.g., not moving onto same-color piece).
      * @param piece The piece to move.
      * @param newX The target row.
      * @param newY The target column.
      * @return True if the move is valid, false otherwise.
      */
     bool isValidMove(ChessPiece* piece, int newX, int newY) const {
         if (!piece || newX < 0 || newX >= 8 || newY < 0 || newY >= 8) return false;
         ChessPiece* target = getPieceAt(newX, newY);
         if (target && target->getColor() == piece->getColor()) return false;
         return true;
     }
 
     /**
      * @brief Checks if the king of a given color is in check.
      * @param color The color of the king to check ("White" or "Black").
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
         if (kingX == -1) return false;
         string opponentColor = (color == "White") ? "Black" : "White";
         for (int i = 0; i < 8; i++) {
             for (int j = 0; j < 8; j++) {
                 ChessPiece* piece = getPieceAt(i, j);
                 if (piece && piece->getColor() == opponentColor) {
                     piece->setPositionForAttack(i, j);
                     if (piece->canAttack(kingX, kingY, *this)) {
                         return true;
                     }
                 }
             }
         }
         return false;
     }
 
     /**
      * @brief Checks if a square is under attack by a given color.
      * @param x The row of the square.
      * @param y The column of the square.
      * @param attackerColor The color of the attacking pieces ("White" or "Black").
      * @return True if the square is under attack, false otherwise.
      */
     bool isSquareUnderAttack(int x, int y, const string& attackerColor) const {
         for (int i = 0; i < 8; i++) {
             for (int j = 0; j < 8; j++) {
                 ChessPiece* piece = getPieceAt(i, j);
                 if (piece && piece->getColor() == attackerColor) {
                     piece->setPositionForAttack(i, j);
                     if (piece->canAttack(x, y, *this)) return true;
                 }
             }
         }
         return false;
     }
 
     /**
      * @brief Checks if a player can castle.
      * @param color The player's color ("White" or "Black").
      * @param kingside True for kingside castling, false for queenside.
      * @return True if castling is possible, false otherwise.
      */
     bool canCastle(const string& color, bool kingside) const {
         int kingX = (color == "White") ? 7 : 0;
         int kingY = 4;
         int rookY = kingside ? 7 : 0;
         ChessPiece* king = getPieceAt(kingX, kingY);
         ChessPiece* rook = getPieceAt(kingX, rookY);
         if (!king || king->getType() != ChessPiece::KING_TYPE || king->getHasMoved()) return false;
         if (!rook || rook->getType() != ChessPiece::ROOK_TYPE || rook->getHasMoved()) return false;
         if (isKingInCheck(color)) return false;
         int yStart = kingside ? 5 : 1;
         int yEnd = kingside ? 7 : 0;
         int yStep = kingside ? 1 : -1;
         for (int y = yStart; y != yEnd; y += yStep) {
             if (getPieceAt(kingX, y)) return false;
         }
         string opponentColor = (color == "White") ? "Black" : "White";
         int checkYStart = kingY;
         int checkYEnd = kingside ? kingY + 2 : kingY - 2;
         int checkYStep = kingside ? 1 : -1;
         for (int y = checkYStart; y != checkYEnd + checkYStep; y += checkYStep) {
             if (isSquareUnderAttack(kingX, y, opponentColor)) return false;
         }
         return true;
     }
 
     /**
      * @brief Determines if the game is over for a player.
      * @param color The player's color ("White" or "Black").
      * @return GAME_NONE if the game continues, GAME_CHECKMATE if checkmate, or GAME_STALEMATE if stalemate.
      */
     int isGameOver(const string& color) {
         bool inCheck = isKingInCheck(color);
         bool hasLegalMove = false;
 
         for (int i = 0; i < 8; i++) {
             for (int j = 0; j < 8; j++) {
                 ChessPiece* piece = getPieceAt(i, j);
                 if (piece && piece->getColor() == color) {
                     int origX = piece->getX();
                     int origY = piece->getY();
                     for (int newX = 0; newX < 8; newX++) {
                         for (int newY = 0; newY < 8; newY++) {
                             bool needsPromotion = false;
                             if (isValidMove(piece, newX, newY)) {
                                 ChessPiece* target = getPieceAt(newX, newY);
                                 bool canMove = piece->move(newX, newY, *this, needsPromotion);
                                 if (canMove) {
                                     setPieceAt(newX, newY, piece);
                                     setPieceAt(origX, origY, nullptr);
                                     ChessPiece* promotedPiece = nullptr;
                                     if (needsPromotion) {
                                         promotedPiece = new Queen(piece->getColor(), newX, newY);
                                         setPieceAt(newX, newY, promotedPiece);
                                     }
                                     bool stillInCheck = isKingInCheck(color);
                                     setPieceAt(origX, origY, piece);
                                     setPieceAt(newX, newY, target);
                                     if (promotedPiece) {
                                         setPieceAt(newX, newY, target);
                                         delete promotedPiece;
                                     }
                                     piece->setPosition(origX, origY);
                                     if (!stillInCheck) {
                                         hasLegalMove = true;
                                         break;
                                     }
                                 }
                             }
                         }
                         if (hasLegalMove) break;
                     }
                     if (hasLegalMove) break;
                 }
             }
             if (hasLegalMove) break;
         }
 
         if (hasLegalMove) return GAME_NONE;
         return inCheck ? GAME_CHECKMATE : GAME_STALEMATE;
     }
 
     /**
      * @brief Promotes a pawn to a new piece type.
      * @param x The row of the pawn.
      * @param y The column of the pawn.
      * @param pieceType The type to promote to ("Queen", "Rook", "Bishop", "Knight").
      * @param color The color of the pawn.
      * @return Pointer to the new piece, or nullptr if promotion fails.
      */
     ChessPiece* promotePawn(int x, int y, const string& pieceType, const string& color) {
         ChessPiece* piece = getPieceAt(x, y);
         if (!piece || piece->getType() != ChessPiece::PAWN_TYPE) return nullptr;
         ChessPiece* newPiece = nullptr;
         if (pieceType == "Queen") newPiece = new Queen(color, x, y);
         else if (pieceType == "Rook") newPiece = new Rook(color, x, y);
         else if (pieceType == "Bishop") newPiece = new Bishop(color, x, y);
         else if (pieceType == "Knight") newPiece = new Knight(color, x, y);
         if (newPiece) setPieceAt(x, y, newPiece);
         return newPiece;
     }
 
     /**
      * @brief Checks if the game is a draw by the 50-move rule.
      * @return True if 50 moves have passed without a capture or pawn move, false otherwise.
      */
     bool isFiftyMoveDraw() const { return halfMoveClock >= 100; }
 
     /**
      * @brief Checks if the game is a draw by threefold repetition.
      * @return True if the same position has occurred three times, false otherwise.
      */
     bool isThreefoldRepetition() const {
         if (positionCount == 0) return false;
         string current = positionHistory[positionCount - 1];
         int count = 0;
         for (int i = 0; i < positionCount; i++) {
             if (positionHistory[i] == current) count++;
         }
         return count >= 3;
     }
 
     /**
      * @brief Updates the move history for draw rules and en passant.
      * @param fromX The starting row.
      * @param fromY The starting column.
      * @param toX The ending row.
      * @param toY The ending column.
      * @param piece The piece that moved.
      * @param isCapture True if the move was a capture, false otherwise.
      */
     void updateMoveHistory(int fromX, int fromY, int toX, int toY, ChessPiece* piece, bool isCapture) {
         lastMoveFromX = fromX;
         lastMoveFromY = fromY;
         lastMoveToX = toX;
         lastMoveToY = toY;
         lastMoveWasPawnTwoSquares = false;
         if (piece) {
             if (piece->getType() == ChessPiece::PAWN_TYPE) {
                 int diff = toX - fromX;
                 if ((diff == 2 || diff == -2) && fromY == toY) lastMoveWasPawnTwoSquares = true;
             }
             halfMoveClock++;
             if (isCapture || piece->getType() == ChessPiece::PAWN_TYPE) halfMoveClock = 0;
         } else {
             halfMoveClock++;
             if (isCapture) halfMoveClock = 0;
         }
         if (positionCount < 1000) positionHistory[positionCount++] = getPositionHash();
     }
 
     /**
      * @brief Converts board indices to algebraic notation.
      * @param x The row index.
      * @param y The column index.
      * @return The position in algebraic notation (e.g., "e4").
      */
     string algebraicFromIndices(int x, int y) const {
         string pos;
         pos += char('a' + y);
         pos += char('8' - x);
         return pos;
     }
 
     /**
      * @brief Gets the row of the last move's ending position.
      * @return The row index.
      */
     int getLastMoveToX() const { return lastMoveToX; }
 
     /**
      * @brief Gets the row of the last move's starting position.
      * @return The row index.
      */
     int getLastMoveFromX() const { return lastMoveFromX; }
 };
 
 /**
  * @brief Represents a chess move from one position to another.
  */
 class Move {
 private:
     int fromX, fromY; ///< Starting position (row, col).
     int toX, toY;     ///< Ending position (row, col).
 
 public:
     /**
      * @brief Constructs a Move.
      * @param fx Starting row.
      * @param fy Starting column.
      * @param tx Ending row.
      * @param ty Ending column.
      */
     Move(int fx, int fy, int tx, int ty) : fromX(fx), fromY(fy), toX(tx), toY(ty) {}
 
     /**
      * @brief Gets the starting row.
      * @return The starting row index.
      */
     int getFromX() const { return fromX; }
 
     /**
      * @brief Gets the starting column.
      * @return The starting column index.
      */
     int getFromY() const { return fromY; }
 
     /**
      * @brief Gets the ending row.
      * @return The ending row index.
      */
     int getToX() const { return toX; }
 
     /**
      * @brief Gets the ending column.
      * @return The ending column index.
      */
     int getToY() const { return toY; }
 };
 
 /**
  * @brief Represents a player in the chess game.
  */
 class Player {
 private:
     string color;          ///< The player's color ("White" or "Black").
     ChessPiece* pieces[16];///< Array of the player's pieces.
     int pieceCount;        ///< Number of pieces the player has.
 
 public:
     /**
      * @brief Constructs a Player.
      * @param col The player's color ("White" or "Black").
      */
     Player(string col) : color(col), pieceCount(0) {
         for (int i = 0; i < 16; i++) pieces[i] = nullptr;
     }
 
     /**
      * @brief Destroys the Player.
      */
     ~Player() {}
 
     /**
      * @brief Adds a piece to the player's collection.
      * @param piece The piece to add.
      */
     void addPiece(ChessPiece* piece) {
         if (piece && pieceCount < 16) pieces[pieceCount++] = piece;
     }
 
     /**
      * @brief Removes a piece from the player's collection.
      * @param piece The piece to remove.
      */
     void removePiece(ChessPiece* piece) {
         for (int i = 0; i < pieceCount; i++) {
             if (pieces[i] == piece) {
                 for (int j = i; j < pieceCount - 1; j++) pieces[j] = pieces[j + 1];
                 pieces[pieceCount - 1] = nullptr;
                 pieceCount--;
                 break;
             }
         }
     }
 
     /**
      * @brief Gets the player's color.
      * @return The color ("White" or "Black").
      */
     string getColor() const { return color; }
 };
 
 /**
  * @brief Manages the chess game, including the board and players.
  */
 class Game {
 private:
     ChessBoard board;        ///< The chessboard.
     Player whitePlayer;      ///< The white player.
     Player blackPlayer;      ///< The black player.
     Player* currentPlayer;   ///< Pointer to the current player.
 
 public:
     /**
      * @brief Constructs a new Game, initializing the board and players.
      */
     Game() : whitePlayer("White"), blackPlayer("Black"), currentPlayer(&whitePlayer) {
         board.initializeBoard();
         initializePlayers();
     }
 
     /**
      * @brief Initializes the players by assigning pieces from the board.
      */
     void initializePlayers() {
         for (int i = 0; i < 8; i++) {
             for (int j = 0; j < 8; j++) {
                 ChessPiece* piece = board.getPieceAt(i, j);
                 if (piece) {
                     if (piece->getColor() == "White") whitePlayer.addPiece(piece);
                     else blackPlayer.addPiece(piece);
                 }
             }
         }
     }
 
     /**
      * @brief Starts and runs the chess game loop.
      */
     void startGame() {
         bool gameOver = false;
         while (!gameOver) {
             board.display();
             cout << currentPlayer->getColor() << "'s turn. Enter move (e.g., e2 e4, O-O, O-O-O) or 'resign': ";
             string input;
             cin >> input;
             if (input == "resign") {
                 cout << currentPlayer->getColor() << " resigns. "
                      << (currentPlayer == &whitePlayer ? "Black" : "White") << " wins!\n";
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
                 if (board.isKingInCheck(opponentColor)) {
                     cout << opponentColor << " is in check!\n";
                 }
                 int state = board.isGameOver(opponentColor);
                 if (state == GAME_CHECKMATE) {
                     board.display();
                     cout << "Checkmate! " << currentPlayer->getColor() << " wins!\n";
                     gameOver = true;
                 } else if (state == GAME_STALEMATE) {
                     board.display();
                     cout << "Stalemate! The game is a draw.\n";
                     gameOver = true;
                 } else if (board.isFiftyMoveDraw()) {
                     board.display();
                     cout << "Draw by 50-move rule!\n";
                     gameOver = true;
                 } else if (board.isThreefoldRepetition()) {
                     board.display();
                     cout << "Draw by threefold repetition!\n";
                     gameOver = true;
                 } else {
                     switchPlayer();
                 }
             } catch (const char* error) {
                 cout << "Error: " << error << "\n";
             }
         }
     }
 
     /**
      * @brief Executes a move for the current player.
      * @param move The move to execute.
      * @throws const char* Error message if the move is invalid.
      */
     void makeMove(const Move& move) {
         int fromX = move.getFromX();
         int fromY = move.getFromY();
         int toX = move.getToX();
         int toY = move.getToY();
 
         ChessPiece* piece = board.getPieceAt(fromX, fromY);
         if (!piece) throw "No piece at starting position";
         if (piece->getColor() != currentPlayer->getColor()) throw "Not your piece";
 
         bool needsPromotion = false;
         if (!board.isValidMove(piece, toX, toY) || !piece->move(toX, toY, board, needsPromotion)) {
             throw "Invalid move";
         }
 
         ChessPiece* target = board.getPieceAt(toX, toY);
         bool isCapture = target != nullptr;
         bool isEnPassant = false;
         if (piece->getType() == ChessPiece::PAWN_TYPE && toY != fromY && !target &&
             board.getPieceAt(toX, fromY)) {
             target = board.getPieceAt(toX, fromY);
             isCapture = true;
             isEnPassant = true;
         }
 
         bool isCastling = piece->getType() == ChessPiece::KING_TYPE &&
                          (toY - fromY == 2 || toY - fromY == -2);
         ChessPiece* rook = nullptr;
         int rookFromX = 0, rookFromY = 0, rookToX = 0, rookToY = 0;
         if (isCastling) {
             bool kingside = toY > fromY;
             rookFromY = kingside ? 7 : 0;
             rookToY = kingside ? 5 : 3;
             rookFromX = rookToX = fromX;
             rook = board.getPieceAt(rookFromX, rookFromY);
             if (!board.canCastle(piece->getColor(), kingside)) throw "Invalid castling move";
         }
 
         board.setPieceAt(toX, toY, piece);
         board.setPieceAt(fromX, fromY, nullptr);
 
         if (isCastling) {
             board.setPieceAt(rookToX, rookToY, rook);
             board.setPieceAt(rookFromX, rookFromY, nullptr);
         }
 
         if (isEnPassant) {
             Player* opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
             opponent->removePiece(target);
             board.setPieceAt(toX, fromY, nullptr);
             delete target;
             target = nullptr;
         }
 
         if (board.isKingInCheck(currentPlayer->getColor())) {
             board.setPieceAt(fromX, fromY, piece);
             board.setPieceAt(toX, toY, nullptr);
             if (isCastling) {
                 board.setPieceAt(rookFromX, rookFromY, rook);
                 board.setPieceAt(rookToX, rookToY, nullptr);
             }
             if (isEnPassant) {
                 board.setPieceAt(toX, fromY, target);
             }
             piece->setPosition(fromX, fromY);
             throw "Move puts your king in check";
         }
 
         if (needsPromotion) {
             string pieceType;
             cout << "Pawn promotion! Choose piece (Queen, Rook, Bishop, Knight): ";
             cin >> pieceType;
             pieceType = toTitleCase(pieceType);
             currentPlayer->removePiece(piece);
             ChessPiece* newPiece = board.promotePawn(toX, toY, pieceType, piece->getColor());
             if (!newPiece) {
                 board.setPieceAt(fromX, fromY, piece);
                 board.setPieceAt(toX, toY, nullptr);
                 currentPlayer->addPiece(piece);
                 throw "Invalid promotion piece";
             }
             currentPlayer->addPiece(newPiece);
             delete piece;
             piece = newPiece;
         }
 
         if (isCapture && !isEnPassant) {
             Player* opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
             opponent->removePiece(target);
             delete target;
             target = nullptr;
         }
 
         board.updateMoveHistory(fromX, fromY, toX, toY, board.getPieceAt(toX, toY), isCapture || isEnPassant);
     }
 
     /**
      * @brief Switches the current player to the opponent.
      */
     void switchPlayer() {
         currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
     }
 };
 
 /**
  * @brief Implements the move logic for a pawn, including forward moves, captures, en passant, and promotion.
  * @param newX The target row.
  * @param newY The target column.
  * @param board The chessboard to move on.
  * @param needsPromotion Output parameter indicating if promotion is needed.
  * @return True if the move is valid and executed, false otherwise.
  */
 bool Pawn::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
     needsPromotion = false;
     int direction = (color == "White") ? -1 : 1;
     int startX = (color == "White") ? 6 : 1;
     int promotionRank = (color == "White") ? 0 : 7;
     if (newY == y) {
         if (newX == x + direction && !board.getPieceAt(newX, newY)) {
             if (newX == promotionRank) needsPromotion = true;
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
             if (newX == promotionRank) needsPromotion = true;
             setPosition(newX, newY);
             return true;
         }
         if (!target && newX == board.getLastMoveToX() &&
             (board.getLastMoveFromX() - board.getLastMoveToX() == 2 ||
              board.getLastMoveToX() - board.getLastMoveFromX() == 2) &&
             board.getPieceAt(newX, y) && board.getPieceAt(newX, y)->getColor() != color) {
             setPosition(newX, newY);
             return true;
         }
     }
     return false;
 }
 
 /**
  * @brief Checks if the pawn can attack a target position.
  * @param targetX The target row.
  * @param targetY The target column.
  * @param board The chessboard to check on.
  * @return True if the pawn can attack the target, false otherwise.
  */
 bool Pawn::canAttack(int targetX, int targetY, const ChessBoard& board) const {
     int direction = (color == "White") ? -1 : 1;
     return (targetX - x == direction && (targetY - y == 1 || y - targetY == 1));
 }
 
 /**
  * @brief Implements the move logic for a rook, allowing horizontal and vertical moves.
  * @param newX The target row.
  * @param newY The target column.
  * @param board The chessboard to move on.
  * @param needsPromotion Output parameter (always false for rooks).
  * @return True if the move is valid and executed, false otherwise.
  */
 bool Rook::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
     needsPromotion = false;
     if (newX == x) {
         int startY = (y < newY) ? y + 1 : newY + 1;
         int endY = (y < newY) ? newY : y;
         for (int cy = startY; cy < endY; cy++) {
             if (board.getPieceAt(x, cy)) return false;
         }
     } else if (newY == y) {
         int startX = (x < newX) ? x + 1 : newX + 1;
         int endX = (x < newX) ? newX : x;
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
  * @brief Checks if the rook can attack a target position.
  * @param targetX The target row.
  * @param targetY The target column.
  * @param board The chessboard to check on.
  * @return True if the rook can attack the target, false otherwise.
  */
 bool Rook::canAttack(int targetX, int targetY, const ChessBoard& board) const {
     if (targetX == x) {
         int startY = (y < targetY) ? y + 1 : targetY + 1;
         int endY = (y < targetY) ? targetY : y;
         for (int cy = startY; cy < endY; cy++) {
             if (board.getPieceAt(x, cy)) return false;
         }
         return true;
     } else if (targetY == y) {
         int startX = (x < targetX) ? x + 1 : targetX + 1;
         int endX = (x < targetX) ? targetX : x;
         for (int cx = startX; cx < endX; cx++) {
             if (board.getPieceAt(cx, y)) return false;
         }
         return true;
     }
     return false;
 }
 
 /**
  * @brief Implements the move logic for a knight, allowing L-shaped moves.
  * @param newX The target row.
  * @param newY The target column.
  * @param board The chessboard to move on.
  * @param needsPromotion Output parameter (always false for knights).
  * @return True if the move is valid and executed, false otherwise.
  */
 bool Knight::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
     needsPromotion = false;
     int dx = newX - x;
     int dy = newY - y;
     if ((dx * dx + dy * dy == 5) && !(dx == 0 || dy == 0)) {
         ChessPiece* target = board.getPieceAt(newX, newY);
         if (!target || target->getColor() != color) {
             setPosition(newX, newY);
             return true;
         }
     }
     return false;
 }
 
 /**
  * @brief Checks if the knight can attack a target position.
  * @param targetX The target row.
  * @param targetY The target column.
  * @param board The chessboard to check on.
  * @return True if the knight can attack the target, false otherwise.
  */
 bool Knight::canAttack(int targetX, int targetY, const ChessBoard& board) const {
     int dx = targetX - x;
     int dy = targetY - y;
     return (dx * dx + dy * dy == 5) && !(dx == 0 || dy == 0);
 }
 
 /**
  * @brief Implements the move logic for a bishop, allowing diagonal moves.
  * @param newX The target row.
  * @param newY The target column.
  * @param board The chessboard to move on.
  * @param needsPromotion Output parameter (always false for bishops).
  * @return True if the move is valid and executed, false otherwise.
  */
 bool Bishop::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
     needsPromotion = false;
     int dx = newX - x;
     int dy = newY - y;
     int absDx = (dx > 0) ? dx : -dx;
     int absDy = (dy > 0) ? dy : -dy;
     if (absDx == absDy && dx != 0) {
         int stepX = (dx > 0) ? 1 : -1;
         int stepY = (dy > 0) ? 1 : -1;
         int cx = x + stepX;
         int cy = y + stepY;
         while (cx != newX) {
             if (board.getPieceAt(cx, cy)) return false;
             cx += stepX;
             cy += stepY;
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
  * @brief Checks if the bishop can attack a target position.
  * @param targetX The target row.
  * @param targetY The target column.
  * @param board The chessboard to check on.
  * @return True if the bishop can attack the target, false otherwise.
  */
 bool Bishop::canAttack(int targetX, int targetY, const ChessBoard& board) const {
     int dx = targetX - x;
     int dy = targetY - y;
     int absDx = (dx > 0) ? dx : -dx;
     int absDy = (dy > 0) ? dy : -dy;
     if (absDx == absDy && dx != 0) {
         int stepX = (dx > 0) ? 1 : -1;
         int stepY = (dy > 0) ? 1 : -1;
         int cx = x + stepX;
         int cy = y + stepY;
         while (cx != targetX) {
             if (board.getPieceAt(cx, cy)) return false;
             cx += stepX;
             cy += stepY;
         }
         return true;
     }
     return false;
 }
 
 /**
  * @brief Implements the move logic for a queen, allowing horizontal, vertical, and diagonal moves.
  * @param newX The target row.
  * @param newY The target column.
  * @param board The chessboard to move on.
  * @param needsPromotion Output parameter (always false for queens).
  * @return True if the move is valid and executed, false otherwise.
  */
 bool Queen::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
     needsPromotion = false;
     if (newX == x || newY == y) {
         Rook rook(color, x, y);
         return rook.move(newX, newY, board, needsPromotion);
     }
     int dx = newX - x;
     int dy = newY - y;
     int absDx = (dx > 0) ? dx : -dx;
     int absDy = (dy > 0) ? dy : -dy;
     if (absDx == absDy && dx != 0) {
         Bishop bishop(color, x, y);
         return bishop.move(newX, newY, board, needsPromotion);
     }
     return false;
 }
 
 /**
  * @brief Checks if the queen can attack a target position.
  * @param targetX The target row.
  * @param targetY The target column.
  * @param board The chessboard to check on.
  * @return True if the queen can attack the target, false otherwise.
  */
 bool Queen::canAttack(int targetX, int targetY, const ChessBoard& board) const {
     if (targetX == x || targetY == y) {
         Rook rook(color, x, y);
         return rook.canAttack(targetX, targetY, board);
     }
     int dx = targetX - x;
     int dy = targetY - y;
     int absDx = (dx > 0) ? dx : -dx;
     int absDy = (dy > 0) ? dy : -dy;
     if (absDx == absDy && dx != 0) {
         Bishop bishop(color, x, y);
         return bishop.canAttack(targetX, targetY, board);
     }
     return false;
 }
 
 /**
  * @brief Implements the move logic for a king, allowing one-square moves and castling.
  * @param newX The target row.
  * @param newY The target column.
  * @param board The chessboard to move on.
  * @param needsPromotion Output parameter (always false for kings).
  * @return True if the move is valid and executed, false otherwise.
  */
 bool King::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
     needsPromotion = false;
     int dx = newX - x;
     int dy = newY - y;
     int absDx = (dx > 0) ? dx : -dx;
     int absDy = (dy > 0) ? dy : -dy;
     if ((absDx <= 1 && absDy <= 1) && !(dx == 0 && dy == 0)) {
         ChessPiece* target = board.getPieceAt(newX, newY);
         if (!target || target->getColor() != color) {
             setPosition(newX, newY);
             return true;
         }
     } else if (!hasMoved && newX == x && (dy == 2 || dy == -2)) {
         bool kingside = dy > 0;
         if (board.canCastle(color, kingside)) {
             setPosition(newX, newY);
             return true;
         }
     }
     return false;
 }
 
 /**
  * @brief Checks if the king can attack a target position.
  * @param targetX The target row.
  * @param targetY The target column.
  * @param board The chessboard to check on.
  * @return True if the king can attack the target, false otherwise.
  */
 bool King::canAttack(int targetX, int targetY, const ChessBoard& board) const {
     int dx = targetX - x;
     int dy = targetY - y;
     int absDx = (dx > 0) ? dx : -dx;
     int absDy = (dy > 0) ? dy : -dy;
     return (absDx <= 1 && absDy <= 1) && !(dx == 0 && dy == 0);
 }
 
 /**
  * @brief Main function to start the chess game.
  * @return 0 on successful execution.
  */
 int main() 
 {
     Game game;
     game.startGame();
     return 0;
 }