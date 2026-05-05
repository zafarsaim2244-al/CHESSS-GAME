#include<iostream>
#include<cstring>
#include<string>
#include<iomanip>
#include<cctype>// for character clarification and transformation
#include<limits>//finding range boundaries
using namespace std;
enum Color {
	white,black
};
class Piece {
	Color color;
	char symbol;
	int rows, columns;

public:
	Piece(Color c, char s, int r, int col) :color(c), symbol(s), rows(r), columns(col){}
virtual	~Piece() {

	}
	// we gonna make pure virtual function and making the class abstract for polymorphism
virtual bool isvalidmove(int rows,int columns,Piece*board[8][8])const = 0;// now every class mold this class according to its own rule

// setters and getters
void setcolor(Color c) {
	color = c;
}
Color getcolor() const {
	return color;
}
void setsymbol(char s) {
	symbol = s;
}
char getsymbol() {
	return symbol;
}
void setrowscoulmns(int r,int c) {
	rows = r;
	columns = c;
}
int getrows()const {
	return rows;
}
int getcolumns()const {
	return columns;
}

};
// no starting from the pawn

class Pawn:public Piece {
public:
	Pawn(Color c, int r, int columns) :Piece(c,(c == white ? 'P':'p'), r, columns) {}
	// actually we are giving symbol to pawn if it is white and if it is not
	string getname()const {
		return "Pawn";
	}
	// pawn moves one square and can move only forward and for the first it can move 2 swuare
	bool isvalidmove(int ro, int col, Piece* board[8][8])const override {
		// current position
		int currentR = getrows();
		int currentC = getcolumns();

		int direction = (getcolor() == white ? -1 : 1);// beacause we are designing the chess game where the white moves down and black moves up
		int startRow = (getcolor() == black ? 1 : 6);//white moving down and black moving up

		//row and column difference
		int rowdif = ro - currentR;
		int columndif = col - currentC;

		// for finding is the place empty
		if (columndif == 0) {
			// for single step mover
			if ((rowdif == direction) && (board[ro][col] = nullptr)) {
				return true;
			}
			// for first moving the two square
			if (rowdif == 2 * direction && currentR == startRow) {
				return true;
			}
			// for diagonally movement  and checking if it is on next column
			else if (abs(columndif) == 1 && rowdif == direction) {
				if (board[ro][col] != nullptr && board[ro][col]->getcolor() != getcolor()) {
					return true;
				}
			}
		}
		return false;
		
	}

};
class Rook :public Piece {
public:
	Rook(Color c, int r, int columns) :Piece(c, (c == white ? 'R' : 'r'), r, columns) {}

	string getname()const {
		return "Rook";
	}
	// Rook moves in the same row and in the same column
	bool isvalidmove(int ro, int col, Piece* board[8][8])const override {
		// check for the straight row or column
		if ((ro != getrows()) && (col != getcolumns())) {
			return  false;
		}
		int steprow = (ro == getrows()) ? 0 : (ro > getrows() ? 1 : -1);//1  is moving down and -1 is moving up
		int stepcolumns = (col == getcolumns()) ? 0 : (col > getcolumns()) ? 1 : -1;

		int checkrow = getrows() + steprow;
		int checkcolumn = getcolumns() + stepcolumns;
		 // a loop gonna check whether there is blockage or not
		
		while (checkrow != ro || checkcolumn != col) {


			if (board[checkrow][checkcolumn] != nullptr) {
				return false;// path is blocked
			}
			checkrow = checkrow + steprow;
			checkcolumn += stepcolumns;

		}

		if (board[ro][col] != nullptr && board[ro][col]->getcolor() == getcolor()) {
			return false;
		}
		return true;
	}


};
class knight :public Piece {
public:
	knight(Color c, int r, int columns) :Piece(c, (c == white ? 'K' : 'k'), r, columns) {}

	string getname()const {
		return "KNIGHT";
	}
	bool isvalidmove(int ro, int col, Piece* board[8][8])const override {
		int rowdiff = abs(ro - getrows());
		int columnsdif = abs(col - getcolumns());// for not having in the
		bool Lshape = (rowdiff == 2 && columnsdif == 1) || (rowdiff == 1 && columnsdif == 2);
		if (!Lshape) {
			return false;
		}


		if (board[ro][col] != nullptr && board[ro][col]->getcolor() == getcolor()) {
			return false;
		}
		return true;


	}


};
class Bishop :public Piece {

	Bishop(Color c, int r, int columns) :Piece(c, (c == white ? 'B' : 'b'), r, columns) {}
	string getname()const {
		return "Bishop";

	}

	bool isvalidmove(int ro, int col, Piece* board[8][8])const override {

		int rowdiff = abs(ro - getrows());
		int columnsdif = abs(col - getcolumns());

		if (rowdiff != columnsdif || rowdiff == 0) {
			return false;
		}
		// for diagonally movement
		int steprow =  (ro > getrows() ? 1 : -1);//1  is moving down and -1 is moving up
		int stepcolumns =  (col > getcolumns() ? 1 : -1);
		// we are actually searching for obstacles
		int checkR = getrows() +steprow;
		int checkC = getcolumns() + stepcolumns;

		while (checkR != ro && checkC != col) {
			if (board[checkR][checkC] != nullptr) {
				return false;
				//there is some obstruction
			}
			 checkR = checkR + steprow;
			 checkC += stepcolumns;
		}


		if (board[ro][col] != nullptr && board[ro][col]->getcolor() == getcolor()) {
			return false;
		}
		return true;


	}

};
class Queen :public Piece {
public:
	Queen(Color c, int r, int columns) :Piece(c, (c == white ? 'Q' : 'q'), r, columns) {}
	string getname()const {
		return "Queen";

	}
	// Queen is the only piece which can move to everywhere even in the enemy lines to kill them
	bool isvalidmove(int ro, int col, Piece* board[8][8])const override {

		int rowdiff = abs(ro - getrows());
		int columnsdif = abs(col - getcolumns());

		// first we have to check the rook and the  BIshop move

		bool Rookmove;
		if (getrows() == ro || getcolumns() == col) {
			Rookmove = true;
		}

		bool bishopmove;
		if (rowdiff == columnsdif) {
			bishopmove = true;
		}
		if (!Rookmove || !bishopmove) {
			return false;// because the move is invalid and they are moving neither staraight nor diagoanl 
		}
		// obsrtuctionn checking going on
		int steprow = (ro == getrows()) ? 0 : (ro > getrows() ? 1 : -1);//1  is moving down and -1 is moving up
		int stepcolumns = (col == getcolumns()) ? 0 : (col > getcolumns()) ? 1 : -1;

		int checkrow = getrows() + steprow;
		int checkcolumn = getcolumns() + stepcolumns;

		while (checkrow != ro && checkcolumn != col) {
			if (board[checkrow][checkcolumn] != nullptr) {
				
					return false;
					//there is some obstruction
				}
			checkrow += steprow;
			checkcolumn += stepcolumns;
			}
		// knowing that whether it reaches the destination or not

		if (board[ro][col] != nullptr && board[ro][col]->getcolor() == getcolor()) {
			return false;
		}
		return true;

		}

	
};
class king :public Piece {

public:
	king(Color c, int r, int columns) :Piece(c, (c == white ? 'K' : 'k'), r, columns) {}
	string getname()const {
		return "KING";

	}
	// king only can move only a  square if it is empty and if not it is unable to move

	bool isvalidmove(int ro, int col, Piece* board[8][8])const override {
		int rowdiff = abs(ro - getrows());
		int columnsdif = abs(col - getcolumns());

		if ((rowdiff <= 1 && columnsdif <= 1) && !(rowdiff == 0 && columnsdif == 0)) {
			// can move to 
			if (board[ro][col] == nullptr || board[ro][col]->getcolor() != getcolor()) {
				return true;
			}
		}

		return false;
	}

};
class board {
private:
	Piece* Board[8][8];
public:
	board() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				Board[i][j] = nullptr;

			}
		}
	}
		// destructor
		~board() {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					delete Board[i][j];

				}
			}
		
		}
		void setupBoard() {
			// Spawn Black Pieces
			[0][0] = new Rook(black, 0, 0);
			grid[0][7] = new Rook(black, 0, 7);
			grid[0][1] = new Knight(black, 0, 1);
			grid[0][6] = new Knight(black, 0, 6);
			grid[0][2] = new Bishop(black, 0, 2);
			grid[0][5] = new Bishop(black, 0, 5);
			grid[0][3] = new Queen(black, 0, 3);
			grid[0][4] = new King(black, 0, 4);
			for (int i = 0; i < 8; i++) grid[1][i] = new Pawn(black, 1, i);

			// Spawn White Pieces
			grid[7][0] = new Rook(white, 7, 0);
			grid[7][7] = new Rook(white, 7, 7);
			grid[7][1] = new Knight(white, 7, 1);
			grid[7][6] = new Knight(white, 7, 6);
			grid[7][2] = new Bishop(white, 7, 2);
			grid[7][5] = new Bishop(white, 7, 5);
			grid[7][3] = new Queen(white, 7, 3);
			grid[7][4] = new King(white, 7, 4);
			for (int i = 0; i < 8; i++) grid[6][i] = new Pawn(white, 6, i);
		}

		void display() const {
			cout << "\n    a b c d e f g h" << endl;
			cout << "  +-----------------+" << endl;
			for (int i = 0; i < 8; i++) {
				cout << 8 - i << " | "; // Rank labels
				for (int j = 0; j < 8; j++) {
					if (grid[i][j] == nullptr)
						cout << ". ";
					else
						cout << grid[i][j]->getsymbol() << " ";
				}
				cout << "| " << 8 - i << endl;
			}
			cout << "  +-----------------+" << endl;
			cout << "    a b c d e f g h\n" << endl;
		}

		// A helper to get a piece at a specific coordinate
		Piece* getPiece(int r, int c) {
			if (r >= 0 && r < 8 && c >= 0 && c < 8)
				return grid[r][c];
			return nullptr;
		}

		// The logic to actually update the array when a move is made
		bool movePiece(int startR, int startC, int endR, int endC) {
			Piece* p = grid[startR][startC];

			// Basic check: is there a piece to move?
			if (p == nullptr) return false;

			// Check internal piece rules
			if (p->isvalidmove(endR, endC, grid)) {
				// If capturing, delete the opponent's piece
				if (grid[endR][endC] != nullptr) {
					delete grid[endR][endC];
				}

				// Move the pointer
				grid[endR][endC] = p;
				grid[startR][startC] = nullptr;

				// Update the piece's internal coordinates
				p->setrowscoulmns(endR, endC);
				return true;
			}
			return false;
		}
};
	}




};
