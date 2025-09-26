// #include <iostream>
// #include <string>

// using namespace std;

// // **Game state constants**
// const int GAME_NONE = 0;
// const int GAME_CHECKMATE = 1;
// const int GAME_STALEMATE = 2;

// // **Forward declarations**
// class ChessBoard;
// class Position;

// // **Helper function for case-insensitive promotion**
// string toTitleCase(const string& input) {
//     string result = input;
//     if (result.empty()) return result;
//     result[0] = toupper(result[0]);
//     for (size_t i = 1; i < result.length(); i++) {
//         result[i] = tolower(result[i]);
//     }
//     return result;
// }

// // **Displayable Interface**
// class Displayable {
// public:
//     virtual void display() const = 0;
//     virtual ~Displayable() {}
// };

// // **Position Class**
// class Position {
// private:
//     int row;
//     int col;
// public:
//     Position(int r = -1, int c = -1) : row(r), col(c) {}
//     int getRow() const { return row; }
//     int getCol() const { return col; }
// };

// // **Forward declaration of helper function**
// Position algebraicToIndices(const string& pos);

// // **Abstract ChessPiece Base Class**
// class ChessPiece : public Displayable {
// protected:
//     string color;
//     int x, y;
//     bool hasMoved;
// public:
//     ChessPiece(string col, int xPos, int yPos) : color(col), x(xPos), y(yPos), hasMoved(false) {}
//     virtual ~ChessPiece() {}
//     virtual bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion) = 0;
//     void display() const { cout << getSymbol(); }
//     virtual string getSymbol() const = 0;
//     virtual bool canAttack(int targetX, int targetY, const ChessBoard& board) const = 0;
//     string getColor() const { return color; }
//     int getX() const { return x; }
//     int getY() const { return y; }
//     void setPosition(int newX, int newY) { x = newX; y = newY; hasMoved = true; }
//     bool getHasMoved() const { return hasMoved; }
// };

// // **Concrete Piece Classes**

// // ***Pawn***
// class Pawn : public ChessPiece {
// public:
//     Pawn(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos) {}
//     bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
//     bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
//     string getSymbol() const { return color == "White" ? "♟" : "♙"; }
// };

// // ***Rook***
// class Rook : public ChessPiece {
// public:
//     Rook(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos) {}
//     bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
//     bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
//     string getSymbol() const { return color == "White" ? "♜" : "♖"; }
// };

// // ***Knight***
// class Knight : public ChessPiece {
// public:
//     Knight(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos) {}
//     bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
//     bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
//     string getSymbol() const { return color == "White" ? "♞" : "♘"; }
// };

// // ***Bishop***
// class Bishop : public ChessPiece {
// public:
//     Bishop(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos) {}
//     bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
//     bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
//     string getSymbol() const { return color == "White" ? "♝" : "♗"; }
// };

// // ***Queen***
// class Queen : public ChessPiece {
// public:
//     Queen(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos) {}
//     bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
//     bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
//     string getSymbol() const { return color == "White" ? "♛" : "♕"; }
// };

// // ***King***
// class King : public ChessPiece {
// public:
//     King(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos) {}
//     bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
//     bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
//     string getSymbol() const { return color == "White" ? "♚" : "♔"; }
// };

// // **Square Class**
// class Square {
// private:
//     ChessPiece* piece;
// public:
//     Square() : piece(nullptr) {}
//     ~Square() { delete piece; }
//     void setPiece(ChessPiece* p) { piece = p; }
//     ChessPiece* getPiece() const { return piece; }
// };

// // **ChessBoard Class**
// class ChessBoard : public Displayable {
// private:
//     Square squares[8][8];
//     int lastMoveFromX, lastMoveFromY, lastMoveToX, lastMoveToY;
//     bool lastMoveWasPawnTwoSquares;
//     int halfMoveClock;
//     string positionHistory[1000];
//     int positionCount;

//     string getPositionHash() const {
//         string hash;
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = squares[i][j].getPiece();
//                 if (piece) {
//                     hash += piece->getSymbol();
//                 } else {
//                     hash += '.';
//                 }
//             }
//         }
//         // Include castling availability
//         hash += canCastle("White", true) ? "WKS" : "";
//         hash += canCastle("White", false) ? "WQS" : "";
//         hash += canCastle("Black", true) ? "BKS" : "";
//         hash += canCastle("Black", false) ? "BQS" : "";
//         // Include en passant target square
//         if (lastMoveWasPawnTwoSquares) {
//             int enPassantX = (lastMoveFromX + lastMoveToX) / 2;
//             hash += algebraicFromIndices(enPassantX, lastMoveToY);
//         }
//         return hash;
//     }

// public:
//     ChessBoard() : lastMoveFromX(-1), lastMoveFromY(-1), lastMoveToX(-1), lastMoveToY(-1),
//                    lastMoveWasPawnTwoSquares(false), halfMoveClock(0), positionCount(0) {
//         for (int i = 0; i < 1000; i++) positionHistory[i] = "";
//     }
//     ~ChessBoard() {}
//     void initializeBoard() {
//         for (int i = 0; i < 8; i++) {
//             squares[1][i].setPiece(new Pawn("Black", 1, i));
//             squares[6][i].setPiece(new Pawn("White", 6, i));
//         }
//         squares[0][0].setPiece(new Rook("Black", 0, 0));
//         squares[0][7].setPiece(new Rook("Black", 0, 7));
//         squares[7][0].setPiece(new Rook("White", 7, 0));
//         squares[7][7].setPiece(new Rook("White", 7, 7));
//         squares[0][1].setPiece(new Knight("Black", 0, 1));
//         squares[0][6].setPiece(new Knight("Black", 0, 6));
//         squares[7][1].setPiece(new Knight("White", 7, 1));
//         squares[7][6].setPiece(new Knight("White", 7, 6));
//         squares[0][2].setPiece(new Bishop("Black", 0, 2));
//         squares[0][5].setPiece(new Bishop("Black", 0, 5));
//         squares[7][2].setPiece(new Bishop("White", 7, 2));
//         squares[7][5].setPiece(new Bishop("White", 7, 5));
//         squares[0][3].setPiece(new Queen("Black", 0, 3));
//         squares[7][3].setPiece(new Queen("White", 7, 3));
//         squares[0][4].setPiece(new King("Black", 0, 4));
//         squares[7][4].setPiece(new King("White", 7, 4));
//     }
//     void display() const {
//         cout << "  a b c d e f g h\n";
//         for (int i = 0; i < 8; i++) {
//             cout << (8 - i) << " ";
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = squares[i][j].getPiece();
//                 if (piece) {
//                     piece->display();
//                     cout << " ";
//                 } else {
//                     cout << ". ";
//                 }
//             }
//             cout << (8 - i) << "\n";
//         }
//         cout << "  a b c d e f g h\n";
//     }
//     ChessPiece* getPieceAt(int x, int y) const {
//         if (x >= 0 && x < 8 && y >= 0 && y < 8) {
//             return squares[x][y].getPiece();
//         }
//         return nullptr;
//     }
//     void setPieceAt(int x, int y, ChessPiece* piece) {
//         if (x >= 0 && x < 8 && y >= 0 && y < 8) {
//             squares[x][y].setPiece(piece);
//         }
//     }
//     bool isValidMove(ChessPiece* piece, int newX, int newY) const {
//         if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
//             return false;
//         }
//         ChessPiece* target = getPieceAt(newX, newY);
//         if (target && target->getColor() == piece->getColor()) {
//             return false;
//         }
//         return true;
//     }
//     bool isKingInCheck(const string& color) const {
//         int kingX = -1, kingY = -1;
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = getPieceAt(i, j);
//                 if (piece && piece->getColor() == color && dynamic_cast<King*>(piece)) {
//                     kingX = i;
//                     kingY = j;
//                     break;
//                 }
//             }
//             if (kingX != -1) break;
//         }
//         string opponentColor = (color == "White") ? "Black" : "White";
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = getPieceAt(i, j);
//                 if (piece && piece->getColor() == opponentColor) {
//                     if (piece->canAttack(kingX, kingY, *this)) {
//                         return true;
//                     }
//                 }
//             }
//         }
//         return false;
//     }
//     bool isSquareUnderAttack(int x, int y, const string& attackerColor) const {
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = getPieceAt(i, j);
//                 if (piece && piece->getColor() == attackerColor) {
//                     if (piece->canAttack(x, y, *this)) {
//                         return true;
//                     }
//                 }
//             }
//         }
//         return false;
//     }
//     bool canCastle(const string& color, bool kingside) const {
//         int kingX = (color == "White") ? 7 : 0;
//         int kingY = 4;
//         int rookY = kingside ? 7 : 0;
//         ChessPiece* king = getPieceAt(kingX, kingY);
//         ChessPiece* rook = getPieceAt(kingX, rookY);

//         if (!king || !dynamic_cast<King*>(king) || king->getHasMoved()) {
//             return false;
//         }
//         if (!rook || !dynamic_cast<Rook*>(rook) || rook->getHasMoved()) {
//             return false;
//         }
//         if (isKingInCheck(color)) {
//             return false;
//         }

//         int yStart = kingside ? 5 : 1;
//         int yEnd = kingside ? 7 : 0;
//         int yStep = kingside ? 1 : -1;
//         for (int y = yStart; y != yEnd; y += yStep) {
//             if (getPieceAt(kingX, y)) {
//                 return false;
//             }
//         }

//         string opponentColor = (color == "White") ? "Black" : "White";
//         int checkYStart = kingY;
//         int checkYEnd = kingside ? kingY + 2 : kingY - 2;
//         int checkYStep = kingside ? 1 : -1;
//         for (int y = checkYStart; y != checkYEnd + checkYStep; y += checkYStep) {
//             if (isSquareUnderAttack(kingX, y, opponentColor)) {
//                 return false;
//             }
//         }
//         return true;
//     }

//     int isGameOver(const string& color) {
//         bool inCheck = isKingInCheck(color);
//         string opponentColor = (color == "White") ? "Black" : "White";
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = getPieceAt(i, j);
//                 if (piece && piece->getColor() == color) {
//                     int origX = piece->getX();
//                     int origY = piece->getY();
//                     for (int newX = 0; newX < 8; newX++) {
//                         for (int newY = 0; newY < 8; newY++) {
//                             bool needsPromotion = false;
//                             if (piece->move(newX, newY, *this, needsPromotion)) {
//                                 ChessPiece* target = getPieceAt(newX, newY);
//                                 setPieceAt(newX, newY, piece);
//                                 setPieceAt(origX, origY, nullptr);
//                                 bool stillInCheck = isKingInCheck(color);
//                                 setPieceAt(origX, origY, piece);
//                                 setPieceAt(newX, newY, target);
//                                 piece->setPosition(origX, origY);
//                                 if (!stillInCheck) {
//                                     return GAME_NONE;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         if (inCheck) {
//             return GAME_CHECKMATE;
//         }
//         return GAME_STALEMATE;
//     }
//     ChessPiece* promotePawn(int x, int y, const string& pieceType, const string& color) {
//         ChessPiece* piece = getPieceAt(x, y);
//         if (!piece || !dynamic_cast<Pawn*>(piece)) {
//             return nullptr;
//         }
//         ChessPiece* newPiece = nullptr;
//         if (pieceType == "Queen") {
//             newPiece = new Queen(color, x, y);
//         } else if (pieceType == "Rook") {
//             newPiece = new Rook(color, x, y);
//         } else if (pieceType == "Bishop") {
//             newPiece = new Bishop(color, x, y);
//         } else if (pieceType == "Knight") {
//             newPiece = new Knight(color, x, y);
//         } else {
//             return nullptr;
//         }
//         setPieceAt(x, y, newPiece);
//         return newPiece;
//     }
//     bool isFiftyMoveDraw() const {
//         return halfMoveClock >= 100;
//     }
//     bool isThreefoldRepetition() const {
//         if (positionCount == 0) return false;
//         string current = positionHistory[positionCount - 1];
//         int count = 0;
//         for (int i = 0; i < positionCount; i++) {
//             if (positionHistory[i] == current) {
//                 count++;
//             }
//         }
//         return count >= 3;
//     }
//     void updateMoveHistory(int fromX, int fromY, int toX, int toY, ChessPiece* piece, bool isCapture) {
//         lastMoveFromX = fromX;
//         lastMoveFromY = fromY;
//         lastMoveToX = toX;
//         lastMoveToY = toY;
//         lastMoveWasPawnTwoSquares = false;
//         if (dynamic_cast<Pawn*>(piece)) {
//             if ((fromX - toX == 2 || toX - fromX == 2) && fromY == toY) {
//                 lastMoveWasPawnTwoSquares = true;
//             }
//         }
//         halfMoveClock++;
//         if (isCapture || dynamic_cast<Pawn*>(piece)) {
//             halfMoveClock = 0;
//         }
//         if (positionCount < 1000) {
//             positionHistory[positionCount++] = getPositionHash();
//         }
//     }
//     string algebraicFromIndices(int x, int y) const {
//         string pos;
//         pos += char('a' + y);
//         pos += char('8' - x);
//         return pos;
//     }
//     int getLastMoveToX() const { return lastMoveToX; }
//     int getLastMoveFromX() const { return lastMoveFromX; }
// };

// // **Move Class**
// class Move {
// private:
//     int fromX, fromY, toX, toY;
// public:
//     Move(int fx, int fy, int tx, int ty) : fromX(fx), fromY(fy), toX(tx), toY(ty) {}
//     int getFromX() const { return fromX; }
//     int getFromY() const { return fromY; }
//     int getToX() const { return toX; }
//     int getToY() const { return toY; }
// };

// // **Player Class**
// class Player {
// private:
//     string color;
//     ChessPiece* pieces[16];
//     int pieceCount;
// public:
//     Player(string col) : color(col), pieceCount(0) {
//         for (int i = 0; i < 16; i++) {
//             pieces[i] = nullptr;
//         }
//     }
//     ~Player() {}
//     void addPiece(ChessPiece* piece) {
//         if (pieceCount < 16) {
//             pieces[pieceCount++] = piece;
//         }
//     }
//     string getColor() const { return color; }
//     ChessPiece** getPieces(int& count) {
//         count = pieceCount;
//         return pieces;
//     }
//     void removePiece(ChessPiece* piece) {
//         for (int i = 0; i < pieceCount; i++) {
//             if (pieces[i] == piece) {
//                 for (int j = i; j < pieceCount - 1; j++) {
//                     pieces[j] = pieces[j + 1];
//                 }
//                 pieces[pieceCount - 1] = nullptr;
//                 pieceCount--;
//                 break;
//             }
//         }
//     }
// };

// // **Game Class**
// class Game {
// private:
//     ChessBoard board;
//     Player whitePlayer;
//     Player blackPlayer;
//     Player* currentPlayer;
// public:
//     Game() : whitePlayer("White"), blackPlayer("Black"), currentPlayer(&whitePlayer) {
//         board.initializeBoard();
//         initializePlayers();
//     }
//     void initializePlayers() {
//         for (int i = 0; i < 8; i++) {
//             for (int j = 0; j < 8; j++) {
//                 ChessPiece* piece = board.getPieceAt(i, j);
//                 if (piece) {
//                     if (piece->getColor() == "White") {
//                         whitePlayer.addPiece(piece);
//                     } else {
//                         blackPlayer.addPiece(piece);
//                     }
//                 }
//             }
//         }
//     }
//     void startGame() {
//         bool gameOver = false;
//         while (!gameOver) {
//             board.display();
//             cout << currentPlayer->getColor() << "'s turn. Enter move (e.g., e2 e4, O-O, O-O-O) or 'resign' to quit: ";
//             string input;
//             cin >> input;
//             if (input == "resign") {
//                 cout << currentPlayer->getColor() << " resigns. " << (currentPlayer == &whitePlayer ? "Black" : "White") << " wins!\n";
//                 gameOver = true;
//                 break;
//             }
//             string from, to;
//             if (input == "O-O") {
//                 from = (currentPlayer->getColor() == "White") ? "e1" : "e8";
//                 to = (currentPlayer->getColor() == "White") ? "g1" : "g8";
//             } else if (input == "O-O-O") {
//                 from = (currentPlayer->getColor() == "White") ? "e1" : "e8";
//                 to = (currentPlayer->getColor() == "White") ? "c1" : "c8";
//             } else {
//                 from = input;
//                 cin >> to;
//             }
//             Position fromPos = algebraicToIndices(from);
//             Position toPos = algebraicToIndices(to);
//             if (fromPos.getRow() == -1 || toPos.getRow() == -1) {
//                 cout << "Invalid position format\n";
//                 continue;
//             }
//             Move move(fromPos.getRow(), fromPos.getCol(), toPos.getRow(), toPos.getCol());
//             try {
//                 makeMove(move);
//                 string opponentColor = (currentPlayer == &whitePlayer) ? "Black" : "White";
//                 int state = board.isGameOver(opponentColor);
//                 if (state == GAME_CHECKMATE) {
//                     board.display();
//                     cout << "Checkmate! " << currentPlayer->getColor() << " wins!\n";
//                     gameOver = true;
//                     break;
//                 } else if (state == GAME_STALEMATE) {
//                     board.display();
//                     cout << "Stalemate! The game is a draw.\n";
//                     gameOver = true;
//                     break;
//                 }
//                 if (board.isFiftyMoveDraw()) {
//                     board.display();
//                     cout << "Draw by 50-move rule!\n";
//                     gameOver = true;
//                     break;
//                 }
//                 if (board.isThreefoldRepetition()) {
//                     board.display();
//                     cout << "Draw by threefold repetition!\n";
//                     gameOver = true;
//                     break;
//                 }
//                 switchPlayer();
//             } catch (const char* error) {
//                 cout << "Error: " << error << "\n";
//             }
//         }
//         if (!gameOver) {
//             board.display();
//             cout << "Game ended.\n";
//         }
//     }
//     void makeMove(const Move& move) {
//         int fromX = move.getFromX();
//         int fromY = move.getFromY();
//         int toX = move.getToX();
//         int toY = move.getToY();
//         ChessPiece* piece = board.getPieceAt(fromX, fromY);
//         if (!piece) {
//             throw "No piece at starting position";
//         }
//         if (piece->getColor() != currentPlayer->getColor()) {
//             throw "Not your piece";
//         }
//         bool needsPromotion = false;
//         bool isCapture = board.getPieceAt(toX, toY) != nullptr;
//         if (!board.isValidMove(piece, toX, toY) || !piece->move(toX, toY, board, needsPromotion)) {
//             throw "Invalid move";
//         }
//         ChessPiece* target = board.getPieceAt(toX, toY);
//         bool isEnPassant = false;
//         if (dynamic_cast<Pawn*>(piece) && toY != fromY && !target && board.getPieceAt(toX, fromY)) {
//             target = board.getPieceAt(toX, fromY);
//             isCapture = true;
//             isEnPassant = true;
//         }
//         bool isCastling = dynamic_cast<King*>(piece) && (toY - fromY == 2 || toY - fromY == -2);
//         ChessPiece* rook = nullptr;
//         int rookFromX = 0, rookFromY = 0, rookToX = 0, rookToY = 0;
//         if (isCastling) {
//             bool kingside = toY > fromY;
//             rookFromY = kingside ? 7 : 0;
//             rookToY = kingside ? 5 : 3;
//             rookFromX = rookToX = fromX;
//             rook = board.getPieceAt(rookFromX, rookFromY);
//             if (!board.canCastle(piece->getColor(), kingside)) {
//                 throw "Invalid castling move";
//             }
//         }
//         board.setPieceAt(toX, toY, piece);
//         board.setPieceAt(fromX, fromY, nullptr);
//         if (isCastling) {
//             board.setPieceAt(rookToX, rookToY, rook);
//             board.setPieceAt(rookFromX, rookFromY, nullptr);
//         }
//         if (isEnPassant) {
//             board.setPieceAt(toX, fromY, nullptr);
//         }
//         if (board.isKingInCheck(currentPlayer->getColor())) {
//             board.setPieceAt(fromX, fromY, piece);
//             board.setPieceAt(toX, toY, target);
//             if (isCastling) {
//                 board.setPieceAt(rookFromX, rookFromY, rook);
//                 board.setPieceAt(rookToX, rookToY, nullptr);
//             }
//             if (isEnPassant) {
//                 board.setPieceAt(toX, fromY, target);
//             }
//             piece->setPosition(fromX, fromY);
//             throw "Move puts your king in check";
//         }
//         if (needsPromotion) {
//             string pieceType;
//             cout << "Pawn promotion! Choose piece (Queen, Rook, Bishop, Knight): ";
//             cin >> pieceType;
//             pieceType = toTitleCase(pieceType);
//             ChessPiece* newPiece = board.promotePawn(toX, toY, pieceType, piece->getColor());
//             if (!newPiece) {
//                 board.setPieceAt(fromX, fromY, piece);
//                 board.setPieceAt(toX, toY, target);
//                 if (isEnPassant) {
//                     board.setPieceAt(toX, fromY, target);
//                 }
//                 piece->setPosition(fromX, fromY);
//                 throw "Invalid promotion piece";
//             }
//             currentPlayer->removePiece(piece);
//             delete piece;
//             piece = newPiece;
//             currentPlayer->addPiece(piece);
//         }
//         if (isCapture && !isEnPassant) {
//             Player* opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
//             opponent->removePiece(target);
//             delete target;
//         }
//         if (isEnPassant) {
//             Player* opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
//             opponent->removePiece(target);
//             delete target;
//         }
//         board.updateMoveHistory(fromX, fromY, toX, toY, piece, isCapture || isEnPassant);
//     }
//     void switchPlayer() {
//         currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
//     }
// };

// // **Piece Movement and Attack Implementations**

// bool Pawn::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
//     needsPromotion = false;
//     int direction = (color == "White") ? -1 : 1;
//     int startX = (color == "White") ? 6 : 1;
//     int promotionRank = (color == "White") ? 0 : 7;
//     if (newY == y) {
//         if (newX == x + direction && !board.getPieceAt(newX, newY)) {
//             if (newX == promotionRank) {
//                 needsPromotion = true;
//             }
//             setPosition(newX, newY);
//             return true;
//         }
//         if (x == startX && newX == x + 2 * direction &&
//             !board.getPieceAt(x + direction, y) && !board.getPieceAt(newX, newY)) {
//             setPosition(newX, newY);
//             return true;
//         }
//     } else if (newX - x == direction && (newY - y == 1 || y - newY == 1)) {
//         ChessPiece* target = board.getPieceAt(newX, newY);
//         if (target && target->getColor() != color) {
//             if (newX == promotionRank) {
//                 needsPromotion = true;
//             }
//             setPosition(newX, newY);
//             return true;
//         }
//         // En passant
//         if (!target && newX == board.getLastMoveToX() &&
//             abs(board.getLastMoveFromX() - board.getLastMoveToX()) == 2 &&
//             board.getPieceAt(newX, y) &&
//             board.getPieceAt(newX, y)->getColor() != color) {
//             setPosition(newX, newY);
//             return true;
//         }
//     }
//     return false;
// }

// bool Pawn::canAttack(int targetX, int targetY, const ChessBoard& board) const {
//     int direction = (color == "White") ? -1 : 1;
//     return (targetX - x == direction && (targetY - y == 1 || y - targetY == 1));
// }

// bool Rook::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
//     needsPromotion = false;
//     if (newX == x) {
//         int startY = y < newY ? y + 1 : newY + 1;
//         int endY = y < newY ? newY : y;
//         for (int cy = startY; cy < endY; cy++) {
//             if (board.getPieceAt(x, cy)) return false;
//         }
//     } else if (newY == y) {
//         int startX = x < newX ? x + 1 : newX + 1;
//         int endX = x < newX ? newX : x;
//         for (int cx = startX; cx < endX; cx++) {
//             if (board.getPieceAt(cx, y)) return false;
//         }
//     } else {
//         return false;
//     }
//     ChessPiece* target = board.getPieceAt(newX, newY);
//     if (!target || target->getColor() != color) {
//         setPosition(newX, newY);
//         return true;
//     }
//     return false;
// }

// bool Rook::canAttack(int targetX, int targetY, const ChessBoard& board) const {
//     if (targetX == x) {
//         int startY = y < targetY ? y + 1 : targetY + 1;
//         int endY = y < targetY ? targetY : y;
//         for (int cy = startY; cy < endY; cy++) {
//             if (board.getPieceAt(x, cy)) return false;
//         }
//         return true;
//     } else if (targetY == y) {
//         int startX = x < targetX ? x + 1 : targetX + 1;
//         int endX = x < targetX ? targetX : x;
//         for (int cx = startX; cx < endX; cx++) {
//             if (board.getPieceAt(cx, y)) return false;
//         }
//         return true;
//     }
//     return false;
// }

// bool Knight::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
//     needsPromotion = false;
//     int dx = newX > x ? newX - x : x - newX;
//     int dy = newY > y ? newY - y : y - newY;
//     if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
//         ChessPiece* target = board.getPieceAt(newX, newY);
//         if (!target || target->getColor() != color) {
//             setPosition(newX, newY);
//             return true;
//         }
//     }
//     return false;
// }

// bool Knight::canAttack(int targetX, int targetY, const ChessBoard& board) const {
//     int dx = targetX > x ? targetX - x : x - targetX;
//     int dy = targetY > y ? targetY - y : y - targetY;
//     return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
// }

// bool Bishop::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
//     needsPromotion = false;
//     if (newX - x == newY - y || newX - x == -(newY - y)) {
//         int dx = (newX > x) ? 1 : -1;
//         int dy = (newY > y) ? 1 : -1;
//         int cx = x + dx;
//         int cy = y + dy;
//         while (cx != newX && cy != newY) {
//             if (board.getPieceAt(cx, cy)) return false;
//             cx += dx;
//             cy += dy;
//         }
//         ChessPiece* target = board.getPieceAt(newX, newY);
//         if (!target || target->getColor() != color) {
//             setPosition(newX, newY);
//             return true;
//         }
//     }
//     return false;
// }

// bool Bishop::canAttack(int targetX, int targetY, const ChessBoard& board) const {
//     if (targetX - x == targetY - y || targetX - x == -(targetY - y)) {
//         int dx = (targetX > x) ? 1 : -1;
//         int dy = (targetY > y) ? 1 : -1;
//         int cx = x + dx;
//         int cy = y + dy;
//         while (cx != targetX && cy != targetY) {
//             if (board.getPieceAt(cx, cy)) return false;
//             cx += dx;
//             cy += dy;
//         }
//         return true;
//     }
//     return false;
// }

// bool Queen::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
//     needsPromotion = false;
//     if (newX == x || newY == y) {
//         return Rook(color, x, y).move(newX, newY, board, needsPromotion);
//     }
//     if (newX - x == newY - y || newX - x == -(newY - y)) {
//         return Bishop(color, x, y).move(newX, newY, board, needsPromotion);
//     }
//     return false;
// }

// bool Queen::canAttack(int targetX, int targetY, const ChessBoard& board) const {
//     if (targetX == x || targetY == y) {
//         return Rook(color, x, y).canAttack(targetX, targetY, board);
//     }
//     if (targetX - x == targetY - y || targetX - x == -(targetY - y)) {
//         return Bishop(color, x, y).canAttack(targetX, targetY, board);
//     }
//     return false;
// }

// bool King::move(int newX, int newY, ChessBoard& board, bool& needsPromotion) {
//     needsPromotion = false;
//     int dx = newX > x ? newX - x : x - newX;
//     int dy = newY > y ? newY - y : y - newY;
//     if (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0)) {
//         ChessPiece* target = board.getPieceAt(newX, newY);
//         if (!target || target->getColor() != color) {
//             setPosition(newX, newY);
//             return true;
//         }
//     }
//     // Castling
//     if (!hasMoved && newX == x && abs(newY - y) == 2) {
//         bool kingside = newY > y;
//         if (board.canCastle(color, kingside)) {
//             setPosition(newX, newY);
//             return true;
//         }
//     }
//     return false;
// }

// bool King::canAttack(int targetX, int targetY, const ChessBoard& board) const {
//     int dx = targetX > x ? targetX - x : x - targetX;
//     int dy = targetY > y ? targetY - y : y - targetY;
//     return dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0);
// }

// // **Helper Function for Algebraic Notation**
// Position algebraicToIndices(const string& pos) {
//     if (pos.length() != 2) return Position(-1, -1);
//     char col = pos[0];
//     char row = pos[1];
//     if (col < 'a' || col > 'h' || row < '1' || row > '8') return Position(-1, -1);
//     int colIndex = col - 'a';
//     int rowIndex = 7 - (row - '1');
//     return Position(rowIndex, colIndex);
// }

// // **Main Function**
// int main() 
// {
//     Game game1;
//     game1.startGame();
//     return 0;
// }








#include <iostream>
#include <string>

using namespace std;

// **Game state constants**
const int GAME_NONE = 0;
const int GAME_CHECKMATE = 1;
const int GAME_STALEMATE = 2;

// **Forward declarations**
class ChessBoard;
class Position;

// **Helper function for case-insensitive promotion**
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

// **Displayable Interface**
class Displayable {
public:
    virtual void display() const = 0;
    virtual ~Displayable() {}
};

// **Position Class**
class Position {
private:
    int row;
    int col;
public:
    Position(int r = -1, int c = -1) : row(r), col(c) {}
    int getRow() const { return row; }
    int getCol() const { return col; }
};

// **Forward declaration of helper function**
Position algebraicToIndices(const string& pos);

// **Abstract ChessPiece Base Class**
class ChessPiece : public Displayable {
public:
    static const int PAWN_TYPE = 0;
    static const int ROOK_TYPE = 1;
    static const int KNIGHT_TYPE = 2;
    static const int BISHOP_TYPE = 3;
    static const int QUEEN_TYPE = 4;
    static const int KING_TYPE = 5;

protected:
    string color;
    int x, y;
    bool hasMoved;
    int type;

public:
    ChessPiece(string col, int xPos, int yPos, int t)
        : color(col), x(xPos), y(yPos), hasMoved(false), type(t) {}
    virtual ~ChessPiece() {}
    virtual bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion) = 0;
    void display() const { cout << getSymbol(); }
    virtual string getSymbol() const = 0;
    virtual bool canAttack(int targetX, int targetY, const ChessBoard& board) const = 0;
    string getColor() const { return color; }
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY) { x = newX; y = newY; hasMoved = true; }
    bool getHasMoved() const { return hasMoved; }
    int getType() const { return type; }
};

// **Concrete Piece Classes**
class Pawn : public ChessPiece {
public:
    Pawn(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, PAWN_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♟" : "♙"; }
};

class Rook : public ChessPiece {
public:
    Rook(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, ROOK_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♜" : "♖"; }
};

class Knight : public ChessPiece {
public:
    Knight(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, KNIGHT_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♞" : "♘"; }
};

class Bishop : public ChessPiece {
public:
    Bishop(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, BISHOP_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♝" : "♗"; }
};

class Queen : public ChessPiece {
public:
    Queen(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, QUEEN_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♛" : "♕"; }
};

class King : public ChessPiece {
public:
    King(string col, int xPos, int yPos) : ChessPiece(col, xPos, yPos, KING_TYPE) {}
    bool move(int newX, int newY, ChessBoard& board, bool& needsPromotion);
    bool canAttack(int targetX, int targetY, const ChessBoard& board) const;
    string getSymbol() const { return color == "White" ? "♚" : "♔"; }
};

// **Square Class**
class Square {
private:
    ChessPiece* piece;
public:
    Square() : piece(nullptr) {}
    ~Square() { delete piece; }
    void setPiece(ChessPiece* p) { piece = p; }
    ChessPiece* getPiece() const { return piece; }
};

// **ChessBoard Class**
class ChessBoard : public Displayable {
private:
    Square squares[8][8];
    int lastMoveFromX, lastMoveFromY, lastMoveToX, lastMoveToY;
    bool lastMoveWasPawnTwoSquares;
    int halfMoveClock;
    string positionHistory[1000];
    int positionCount;

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
    ChessBoard() : lastMoveFromX(-1), lastMoveFromY(-1), lastMoveToX(-1), lastMoveToY(-1),
                   lastMoveWasPawnTwoSquares(false), halfMoveClock(0), positionCount(0) {
        for (int i = 0; i < 1000; i++) positionHistory[i] = "";
    }
    ~ChessBoard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                squares[i][j].setPiece(nullptr);
            }
        }
    }
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
    ChessPiece* getPieceAt(int x, int y) const {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            return squares[x][y].getPiece();
        }
        return nullptr;
    }
    void setPieceAt(int x, int y, ChessPiece* piece) {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            squares[x][y].setPiece(piece);
        }
    }
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
    bool isFiftyMoveDraw() const {
        return halfMoveClock >= 100;
    }
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
    string algebraicFromIndices(int x, int y) const {
        string pos;
        pos += char('a' + y);
        pos += char('8' - x);
        return pos;
    }
    int getLastMoveToX() const { return lastMoveToX; }
    int getLastMoveFromX() const { return lastMoveFromX; }
};

// **Move Class**
class Move {
private:
    int fromX, fromY, toX, toY;
public:
    Move(int fx, int fy, int tx, int ty) : fromX(fx), fromY(fy), toX(tx), toY(ty) {}
    int getFromX() const { return fromX; }
    int getFromY() const { return fromY; }
    int getToX() const { return toX; }
    int getToY() const { return toY; }
};

// **Player Class**
class Player {
private:
    string color;
    ChessPiece* pieces[16];
    int pieceCount;
public:
    Player(string col) : color(col), pieceCount(0) {
        for (int i = 0; i < 16; i++) {
            pieces[i] = nullptr;
        }
    }
    ~Player() {
        for (int i = 0; i < pieceCount; i++) {
            delete pieces[i];
            pieces[i] = nullptr;
        }
    }
    void addPiece(ChessPiece* piece) {
        if (pieceCount < 16) {
            pieces[pieceCount++] = piece;
        }
    }
    string getColor() const { return color; }
    ChessPiece** getPieces(int& count) {
        count = pieceCount;
        return pieces;
    }
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

// **Game Class**
class Game {
private:
    ChessBoard board;
    Player whitePlayer;
    Player blackPlayer;
    Player* currentPlayer;
public:
    Game() : whitePlayer("White"), blackPlayer("Black"), currentPlayer(&whitePlayer) {
        board.initializeBoard();
        initializePlayers();
    }
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
    void switchPlayer() {
        currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
    }
};

// **Piece Movement and Attack Implementations**
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

bool Pawn::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    int direction = (color == "White") ? -1 : 1;
    return (targetX - x == direction && (targetY - y == 1 || y - targetY == 1));
}

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

bool Knight::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    int dx = targetX > x ? targetX - x : x - targetX;
    int dy = targetY > y ? targetY - y : y - targetY;
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

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

bool King::canAttack(int targetX, int targetY, const ChessBoard& board) const {
    if (targetX < 0 || targetX > 7 || targetY < 0 || targetY > 7) return false;
    int dx = targetX > x ? targetX - x : x - targetX;
    int dy = targetY > y ? targetY - y : y - targetY;
    return dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0);
}

// **Helper Function for Algebraic Notation**
Position algebraicToIndices(const string& pos) {
    if (pos.length() != 2) return Position(-1, -1);
    char col = pos[0];
    char row = pos[1];
    if (col < 'a' || col > 'h' || row < '1' || row > '8') return Position(-1, -1);
    int colIndex = col - 'a';
    int rowIndex = 7 - (row - '1');
    return Position(rowIndex, colIndex);
}

// **Main Function**
int main() {
    Game game1;
    game1.startGame();
    return 0;
}