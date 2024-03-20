#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include<Windows.h>

#define SIZE 200
#define PLANSZA_DEF_X 60
#define PLANSZA_DEF_Y 1
#define FILE_NAME_SIZE 100
#define LEGENDA_DEF_X 1
#define LEGENDA_DEF_Y 1

class game {
public:
	int round = 0;
	int roundW = 0;
	int roundL = 1;
	int handicap = 0;
};

class user {
public:
	float score = 0;
	int handicap = 0;
};

class board {
public:
	int boardS = 19;
	int boardW = 18;
	int board_W = 0;
	int windowSizeX = 0;
	int windowSizeY = 0;
	int breakpointsX = 0;
	int breakpointsY = 0;
};

void printF(char plansza1[(SIZE + 2) * (SIZE + 2)], board board1) { //tablica ktora sie miesci w oknie

	for (int i = 0; i < board1.boardS + 2; i++) {
		gotoxy(PLANSZA_DEF_X, PLANSZA_DEF_Y + i);
		for (int j = 0; j < board1.boardS + 2; j++) {
			textbackground(DARKGRAY);
			textcolor(LIGHTMAGENTA);
			putch(plansza1[i * (SIZE + 2) + j]);
		}
	}
}

void printFS(char plansza1[(SIZE + 2) * (SIZE + 2)], board board1) {
	int p = 0, l = 0;

	if (board1.boardS + 2 < board1.windowSizeX) p = board1.boardS + 2;
	else p = board1.windowSizeX;

	if (board1.boardS + 2 < board1.windowSizeY) l = board1.boardS + 2;
	else p = board1.windowSizeY;

	for (int i = 0; i < l; i++) {
		gotoxy(PLANSZA_DEF_X, PLANSZA_DEF_Y + i);
		for (int j = 0; j < p; j++) {
			textbackground(DARKGRAY);
			textcolor(LIGHTMAGENTA);
			putch(plansza1[i * (SIZE + 2) + j]);
		}
	}
}

void print(char plansza1[(SIZE + 2) * (SIZE + 2)], game game, int plansza3[(SIZE + 2) * (SIZE + 2)], board board1) {
	for (int i = 0; i < board1.boardS + 2; i++) {
		gotoxy(PLANSZA_DEF_X, PLANSZA_DEF_Y + i);
		for (int j = 0; j < board1.boardS + 2; j++) {
			textbackground(DARKGRAY);
			if (plansza3[i * (SIZE + 2) + j] == 1)
			{
				textcolor(WHITE);
				plansza1[i * (SIZE + 2) + j] = 'O';
			}
			else if (plansza3[i * (SIZE + 2) + j] == 2) {
				textcolor(BLACK);
				plansza1[i * (SIZE + 2) + j] = 'O';
			}
			else textcolor(LIGHTMAGENTA);

			putch(plansza1[i * (SIZE + 2) + j]);
		}
	}
}

auto createBoard(char plansza2[(SIZE + 2) * (SIZE + 2)], board board1) -> void
{
	for (int i = 0; i < board1.boardS + 2; i++) {
		for (int j = 0; j < board1.boardS + 2; j++) {
			if (i == board1.boardS + 1) plansza2[i * (SIZE + 2) + j] = 193;
			else if (i == 0) plansza2[i * (SIZE + 2) + j] = 194; 
			else if (j == board1.boardS + 1) plansza2[i * (SIZE + 2) + j] = 180; 
			else if (j == 0) plansza2[i * (SIZE + 2) + j] = 195; 
			else plansza2[i * (SIZE + 2) + j] = 197; 
			if (i == 0 && j == 0) plansza2[i * (SIZE + 2) + j] = 218;
			if (i == board1.boardS + 1 && j == 0) plansza2[i * (SIZE + 2) + j] = 192; 
			if (i == 0 && j == board1.boardS + 1) plansza2[i * (SIZE + 2) + j] = 191; 
			if (i == board1.boardS + 1 && j == board1.boardS + 1) plansza2[i * (SIZE + 2) + j] = 217; 
		}
	}
}

auto createColorBoard(int plansza3[(SIZE + 2) * (SIZE + 2)], board board1) -> void
{ 
	for (int i = 0; i < board1.boardS + 2; i++) {
		for (int j = 0; j < board1.boardS + 2; j++) {
			plansza3[i * (SIZE + 2) + j] = 0;
		}
	}
}

auto kill(char plansza2[(SIZE + 2) * (SIZE + 2)], game game, int plansza[(SIZE + 2) * (SIZE + 2)], board board1) -> bool
{ 
	for (int x = 1; x < board1.boardS + 2; x++) {
		for (int y = 1; y < board1.boardS + 2; y++) {
			if (plansza2[x * (SIZE + 2) + y] == 'O') {
				if ((x == 1 && y == 1) || 
					(x == board1.boardS && y == 1) || 
					(x == board1.boardS && y == board1.boardS) || 
					(x == 1 && y == board1.boardS) ||
					(y == board1.boardS && plansza[(x - 1) * (SIZE + 2) + y] == game.roundL && plansza[(x + 1) * (SIZE + 2) + y] == game.roundL && plansza[x * (SIZE + 2) + (y - 1)] == game.roundL) ||
					(y == 1 && plansza[(x - 1) * (SIZE + 2) + y] == game.roundL && plansza[(x + 1) * (SIZE + 2) + y] == game.roundL && plansza[x * (SIZE + 2) + (y + 1)] == game.roundL) ||
					(x == board1.boardS && plansza[x * (SIZE + 2) + (y + 1)] == game.roundL && plansza[x * (SIZE + 2) + (y - 1)] == game.roundL && plansza[(x - 1) * (SIZE + 2) + y] == game.roundL) ||
					(x == 1 && plansza[x * (SIZE + 2) + (y + 1)] == game.roundL && plansza[x * (SIZE + 2) + (y - 1)] == game.roundL && plansza[(x + 1) * (SIZE + 2) + y] == game.roundL) ||
					(plansza[(x - 1) * (SIZE + 2) + y] == game.roundL && plansza[(x + 1) * (SIZE + 2) + y] == game.roundL && plansza[x * (SIZE + 2) + (y - 1)] == game.roundL && plansza[x * (SIZE + 2) + (y + 1)] == game.roundL)) {

					plansza2[x * (SIZE + 2) + y] = 197;
					plansza[x * (SIZE + 2) + y] = 0;

					return true;
				}
			}
		}
	}
	return false;
}


auto StoneAllowed(char plansza2[(SIZE + 2) * (SIZE + 2)], game game, int plansza[(SIZE + 2) * (SIZE + 2)], int x, int y,
                  board board1) -> bool
{
	if (plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X)] == 2 || plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X)] == 1) return 0; //czy jest tam kamien
	else if ((x - 1 == PLANSZA_DEF_X) && (y - 1 == PLANSZA_DEF_Y) && (plansza[(y * (SIZE + 2)) + (x - 60)] == game.roundL) && (plansza[((y - 1) * (SIZE + 2)) + (x - 59)]) == game.roundL) return 0; //lewy gorny rog
	else if ((y == PLANSZA_DEF_Y + board1.boardS) && (x - 1 == PLANSZA_DEF_X) && (plansza[((y - 1) * (SIZE + 2)) + (x - 59)] == game.roundL) && (plansza[((y - 2) * (SIZE + 2)) + (x - 60)] == game.roundL))  return 0; //lewy dolny rog
	else if ((y == PLANSZA_DEF_Y + board1.boardS) && (x + 1 == PLANSZA_DEF_X + board1.boardS + 1) && (plansza[((y - 1) * (SIZE + 2)) + (x - 61)] == game.roundL) && (plansza[((y - 2) * (SIZE + 2)) + (x - 60)] == game.roundL))  return 0; //prawy dolny rog
	else if ((y - 1 == PLANSZA_DEF_Y) && (x + 1 == PLANSZA_DEF_X + SIZE + 1) && (plansza[((y - 1) * (SIZE + 2)) + (x - 61)] == game.roundL) && (plansza[(y * (SIZE + 2)) + (x - 60)] == game.roundL))  return 0; //prawy gorny rog
	else if (x + 1 == PLANSZA_DEF_X + board1.boardS + 1 && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X - 1)] == game.roundL && plansza[(y - PLANSZA_DEF_Y - 1) * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL && plansza[(y * (SIZE + 2)) + (x - PLANSZA_DEF_X)] == game.roundL) return 0;
	else if (x - 1 == PLANSZA_DEF_X && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X + 1)] == game.roundL && plansza[(y - PLANSZA_DEF_Y - 1) * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL && plansza[y * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL) return 0;
	else if (y - 1 == PLANSZA_DEF_Y && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X - 1)] == game.roundL && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - 59)] == game.roundL && plansza[y * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL) return 0;
	else if (y + 1 == PLANSZA_DEF_Y + board1.boardS + 1 && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X + 1)] == game.roundL && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X - 1)] == game.roundL && plansza[(y - PLANSZA_DEF_Y - 1) * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL) return 0;
	else if (plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X - 1)] == game.roundL && plansza[(y - PLANSZA_DEF_Y) * (SIZE + 2) + (x - PLANSZA_DEF_X + 1)] == game.roundL && plansza[y * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL && plansza[(y - PLANSZA_DEF_Y - 1) * (SIZE + 2) + (x - PLANSZA_DEF_X)] == game.roundL) return 0; //srodek
	else { return 1; }
}

auto save(game game, int plansza[(SIZE + 2) * (SIZE + 2)], user userB, user userW, board board1) -> void
{ 

	char customFileName[100] = {}; 
	int c;
	char zn;
	int i = 0;

	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 16);
	textbackground(YELLOW);
	textcolor(BLACK);
	cputs(" Podaj nazwe pliku (.txt): ");
	do {
		c = getche();
		if (c == '\r') {
			break;
		}
		customFileName[i] = c;
		i++;
	} while (c != 0x0d);

	customFileName[i] = '\0';

	FILE* saveFile;
	saveFile = fopen(customFileName, "w"); 

	if (saveFile == NULL) {
		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 16);
		cputs("File not found");
	}
	else {
		fprintf(saveFile, "%d %d %.1f %.1f", board1.boardS, game.round, userB.score, userW.score);
		fprintf(saveFile, "\n");
		for (int j = 0; j < board1.boardS + 2; j++) {
			for (int n = 0; n < board1.boardS + 2; n++) {
				c = plansza[j * (SIZE + 2) + n];
				fprintf(saveFile, "%d ", c); 
			}
		}
		fclose(saveFile);
	}
}

auto open(game* game1, int plansza[(SIZE + 2) * (SIZE + 2)], user* userB, user* userW, board* board1) -> void
{ 
	char customFileName[100] = {};
	int c;
	char zn;
	int i = 0;
	int j = 0, n = 0, g = 0, h = 0;

	gotoxy(1, 17);
	textbackground(YELLOW);
	textcolor(BLACK);
	cputs(" Podaj nazwe pliku (.txt) do odczytu: ");

	do {
		c = getche();
		if (c == '\r') {
			break;
		}
		customFileName[i] = c;
		i++;
	} while (c != 0x0d);

	customFileName[i] = '\0';

	FILE* openFile;

	openFile = fopen(customFileName, "r");

	if (openFile == NULL) {
		cputs("Error Reading File");
	}
	else {
		fscanf(openFile, "%d %d %f %f", &board1->boardS, &game1->round, &userB->score, &userW->score); //zmieniamy wartosc klas na pobrane z pliku
		for (int j = 0; j < board1->boardS + 2; j++) {
			for (int n = 0; n < board1->boardS + 2; n++) {
				fscanf(openFile, "%d ", &plansza[j * (SIZE + 2) + n]);
			}
		}
		fclose(openFile);
	}
}

auto printLegend() -> void
{
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y);
	cputs(" strzalki: poruszanie kursorem na planszy ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 1);
	cputs(" q: zamkniecie programu ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 2);
	cputs(" n: rozpoczecie nowej gry ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 3);
	cputs(" enter: potwierdzenie wyboru i zakonczenie tury gracza ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 4);
	cputs(" esc: anulowanie obecnej akcji ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 5);
	cputs(" i: polozenie kamienia na planszy ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 6);
	cputs(" s: zapis stanu gry ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 7);
	cputs(" l: wczytanie stanu gry ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 8);
	cputs(" o: zmiana rozmiaru planszy ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 9);
	cputs(" u: gra z handicapem ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 10);
	cputs(" f: zakonczenie gry ");
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 23);
	cputs(" Zuzanna Nowak, s193165 ");
}

auto chooseSize(board* board1) -> void
{
	int licznik = -1; //podany przez uzytkownika
	int zn = 0;
	int custom = 0;
	int i = 0, sizeR = 0;
	char c;
	char rozmiar[6];

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	board1->windowSizeX = columns - PLANSZA_DEF_X;
	board1->windowSizeY = rows - PLANSZA_DEF_Y;

	textcolor(WHITE);
	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 19);
	cputs(" Plansza moze miec rozmiar: 9x9, 13x13, 19x19, dowolny ");

	gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 20);
	cputs(" Wybrany: [kliknij p]     \n");

	do {
		zn = getch();
		if (zn == 0x0d) break;
		if (licznik + 1 > 3) licznik = 0;
		else licznik++;
		if (licznik == 0) {
			textbackground(BLUE);
			gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 21);
			cputs("       9x9      ");
			sizeR = 9;
			custom = 0;
		}
		else if (licznik == 1) {
			textbackground(BLUE);
			gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 21);
			cputs("      13x13      ");
			sizeR = 13;
			custom = 0;
		}
		else if (licznik == 2) {
			textbackground(BLUE);
			gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 21);
			cputs("      19x19      ");
			sizeR = 19;
			custom = 0;
		}
		else if (licznik == 3) {
			gotoxy(LEGENDA_DEF_Y, LEGENDA_DEF_Y + 21);
			cputs(" dowolny rozmiar ");
			custom = 1;
		}
	} while (zn == 0x70);

	if (custom == 1) {
		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 22);

		cputs(" Podaj prawidlowy rozmiar: ");
		do {
			c = getche();
			if (c == '\r') {
				break;
			}
			rozmiar[i] = c;
			i++;
		} while (c != 0x0d);
		sscanf(rozmiar, "%d", &sizeR);

		if (sizeR % 2 == 0) ++sizeR;

		for (int j = 0; j < 6; j++) {
			rozmiar[j] = ' ';
		}
	}

	if (sizeR > board1->windowSizeX || sizeR > board1->windowSizeY) {
		board1->board_W = 1;
		board1->boardW = sizeR;
	}

	board1->boardS = sizeR;
}

auto printS(char plansza1[(SIZE + 2) * (SIZE + 2)], game game, int plansza3[(SIZE + 2) * (SIZE + 2)],
            board board1) -> void
{

	int p = 0, l = 0, z = 0;

	for (int i = board1.breakpointsY; i < board1.windowSizeY + board1.breakpointsY; i++) {
		gotoxy(PLANSZA_DEF_X, PLANSZA_DEF_Y + z);
		for (int j = board1.breakpointsX; j < board1.windowSizeX + board1.breakpointsX; j++) {
			textbackground(DARKGRAY);
			if (plansza3[i * (SIZE + 2) + j] == 1)
			{
				textcolor(WHITE);
				plansza1[i * (SIZE + 2) + j] = 'O';
			}
			else if (plansza3[i * (SIZE + 2) + j] == 2) {
				textcolor(BLACK);
				plansza1[i * (SIZE + 2) + j] = 'O';
			}
			else textcolor(LIGHTMAGENTA);

			putch(plansza1[i * (SIZE + 2) + j]);
		}
		z++;
	}
}

auto main() -> int
{
	int zn = 0, x = PLANSZA_DEF_X + 1, y = PLANSZA_DEF_Y + 1, attr = 7, back = 0, zero = 0, i = 0, defx = PLANSZA_DEF_X + 1, defy = LEGENDA_DEF_Y + 1, f = 0, handicap = 0, ok = 0, p = 0, l = 0, kol = 1, rzad = 1;

	char txt[32], pozycja[32], WRound[30], confirm[102] = { " Potwierdzasz swoj ruch ? Wcisnij ENTER. Jezeli chcesz anulowac ruch wcisnij ESCAPE. " };
	char scoreT[100], handi[30], breakX[30], breakY[50];

	int* plansza; //plansza na kolor 1 top biale 0 to 2 to czarne
	plansza = new int[(SIZE + 2) * (SIZE + 2)];

	user userB, userW;
	game gameC;
	board board1;

	game* a = &gameC;
	user* b = &userW;
	user* c = &userB;
	board* board2 = &board1;

	char* plansza2;
	plansza2 = new char[(SIZE + 2) * (SIZE + 2)];

	createBoard(plansza2, board1);
	createColorBoard(plansza, board1);

	printF(plansza2, board1);

#ifndef __cplusplus 
	Conio2_Init();
#endif

	settitle("Zuzanna Nowak, s193165");

	_setcursortype(_NOCURSOR);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	board1.windowSizeX = columns - PLANSZA_DEF_X;
	board1.windowSizeY = rows - PLANSZA_DEF_Y;

	do {
		if (board1.boardS + 2 < board1.windowSizeX) board1.windowSizeX = board1.boardS + 2;

		if (board1.boardS + 2 < board1.windowSizeY) board1.windowSizeY = board1.boardS + 2;


		if (gameC.round % 2 == 0) {
			gameC.roundW = 2;
			gameC.roundL = 1;
		}
		else {
			gameC.roundW = 1;
			gameC.roundL = 2;
		}

		textbackground(BLACK);
		clrscr();
		textcolor(WHITE);
		textbackground(LIGHTBLUE);

		printLegend();

		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 25);
		textbackground(RED);
		sprintf(breakX, " Kolumna: %d Rzad: %d", kol, rzad);
		cputs(breakX);


		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 26);
		textbackground(RED);
		sprintf(breakY, " BreakpointsX: %d BreakpointsY: %d", board1.breakpointsX, board1.breakpointsY);
		cputs(breakY);

		textbackground(LIGHTBLUE);
		if (zero) sprintf(txt, " key code: 0x00 0x%02x ", zn);
		else sprintf(txt, " key code: 0x%02x ", zn);
		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 10);
		cputs(txt);

		if (gameC.round != 0) { //zabicie
			if (kill(plansza2, gameC, plansza, board1) == 1) {
				gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 24);
				textbackground(LIGHTRED);
				cputs(" ZABOJSTWO! ");
				if (gameC.round % 2 == 0) { //odejmowanie punktow
					if (userB.score == 0) {
						userB.score = 0;
					}
					else {
						if (userB.score - 1 < 0) userB.score = 0;
						else --userB.score;
					}
					++userW.score;
				}
				else {
					if (userW.score == 0) {
						userW.score = 0;
					}
					else {
						if (userW.score - 1 < 0) userW.score = 0;
						else --userW.score;
					}
					++userB.score;
				}
			}
		}

		textcolor(WHITE); 
		textbackground(LIGHTBLUE);
		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 11);
		sprintf(pozycja, " pozycja kursora: %d, %d ", x, y);
		cputs(pozycja);


		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 12); 
		sprintf(WRound, " Runda: %d ", gameC.round);
		cputs(WRound);

		if (board1.board_W == 1) { 
			printS(plansza2, gameC, plansza, board1);
		}
		else print(plansza2, gameC, plansza, board1);

		gotoxy(x, y);
		if (gameC.round % 2 == 0) textcolor(BLACK);
		else textcolor(WHITE);
		textbackground(DARKGRAY);
		putch('O');

		sprintf(scoreT, " Czarne -> %.1f || Biale -> %.1f ", userB.score, userW.score); //wyniki
		textbackground(LIGHTBLUE);
		textcolor(WHITE);
		gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 13);
		cputs(scoreT);

		gotoxy(1, 30); 

		if (board1.board_W == 1) { 
			if (StoneAllowed(plansza2, gameC, plansza, kol + PLANSZA_DEF_X, rzad + PLANSZA_DEF_Y, board1) == true) {
				textbackground(GREEN);
				cputs("W tym miejscu mozna postawic kamien.");
			}
			else {
				textbackground(RED);
				cputs("W tym miejscu NIE mozna postawic kamienia. Jest to oczywiste samobojstwo, lub w tym miejscu stoi juz kamien.");
			}
		}
		else {
			if (StoneAllowed(plansza2, gameC, plansza, x, y, board1) == true) {
				textbackground(GREEN);
				cputs("W tym miejscu mozna postawic kamien.");
			}
			else {
				textbackground(RED);
				cputs("W tym miejscu NIE mozna postawic kamienia. Jest to oczywiste samobojstwo, lub w tym miejscu stoi juz kamien.");
			}
		}

		zero = 0;
		zn = getch();
		if (zn == 0) {
			zero = 1;
			zn = getch();
			if (board1.board_W == 0) { 
				if (zn == 0x48) {
					if (y - 1 < defy) y = defy;
					else {
						y--;
						rzad = y - PLANSZA_DEF_Y;
					}
				}
				else if (zn == 0x50) {
					if (y + 1 > defy + board1.boardS - 1) y = defy + board1.boardS - 1;
					else {
						y++;
						rzad = y - PLANSZA_DEF_Y;
					}
				}
				else if (zn == 0x4b) {
					if (x - 1 < defx) x = defx;
					else {
						x--;
						kol = x - PLANSZA_DEF_X;
					}
				}
				else if (zn == 0x4d) {
					if (x + 1 > defx + board1.boardS - 1) x = defx + board1.boardS - 1;
					else {
						x++;
						kol = x - PLANSZA_DEF_X;
					}
				}
			}
			else {
				if (zn == 0x48) {
					if (board1.breakpointsY > 0) board1.breakpointsY--;
					else if (board1.breakpointsY == 0) {
						if (y - 1 <= defy) y = defy;
						else y--;
					}
					if (y == defy) rzad = PLANSZA_DEF_Y + board1.breakpointsY;
					else rzad = y - PLANSZA_DEF_Y + board1.breakpointsY;
				}
				else if (zn == 0x50) {
					if (board1.boardS + 2 > board1.windowSizeY && board1.breakpointsY < (board1.boardS - board1.windowSizeY + 2)) {
						board1.breakpointsY++;
					}
					if (board1.breakpointsY + board1.windowSizeY >= board1.boardS + 2) {
						if (y + 1 > PLANSZA_DEF_Y + board1.windowSizeY - 2) {
							y = defy + board1.windowSizeY - 3;
						}
						else y++;
					}
					if (y == defy) {
						rzad = board1.breakpointsY + 1;
					}
					else rzad = y - PLANSZA_DEF_Y + board1.breakpointsY;
				}
				else if (zn == 0x4b) {
					if (board1.breakpointsX > 0) {
						board1.breakpointsX--;
					}
					else if (board1.breakpointsX == 0) {
						if (x - 1 <= defx) x = defx;
						else x--;
					}
					if (kol = x - PLANSZA_DEF_X - board1.breakpointsX == 0) kol = 1;
					if (x == defx) {
						kol = board1.breakpointsX + 1;
					}
					else kol = x - PLANSZA_DEF_X + board1.breakpointsX;
				}
				else if (zn == 0x4d) {
					if (board1.boardS + 2 > board1.windowSizeX && board1.breakpointsX < (board1.boardS - board1.windowSizeX + 2)) board1.breakpointsX++;
					if (board1.breakpointsX + board1.windowSizeX == board1.boardS + 2 || board1.breakpointsX + board1.windowSizeX > board1.boardS + 2) {
						if (x + 1 > PLANSZA_DEF_X + board1.windowSizeX - 2) x = defx + board1.windowSizeX - 3; 
						else x++;
					}
					kol = x - PLANSZA_DEF_X + board1.breakpointsX;
				}
			}
		}
		else if (zn == 0x73) { 
			save(gameC, plansza, userB, userW, board1);
		}
		else if (zn == 0x6f) { 
			gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 16);
			cputs("Gra jest w toku, czy chcesz ja zresetowac \ni rozpoczac nowa z innym rozmiarem planszy? \n-> wcisnij ENTER");
			ok = getch();
			if (ok == 0x0d) {
				x = defx;
				y = defy;

				createColorBoard(plansza, board1); 
				createBoard(plansza2, board1);

				userB.score = 0; 
				userW.score = 0;

				if (board1.board_W == 1) {
					printFS(plansza2, board1);
				}
				else printF(plansza2, board1); 

				board1.breakpointsX = 0;
				board1.breakpointsY = 0;
				kol = 1;
				rzad = 1;
				gameC.round = 0; 
				handicap = 1;
				ok = 0;

				chooseSize(board2);
				clrscr();
				createColorBoard(plansza, board1); 
				createBoard(plansza2, board1);

				if (board1.board_W == 1) {
					printFS(plansza2, board1);
				}
				else printF(plansza2, board1);
			}
			if (ok == 0x0b) {
				gotoxy(1, 20);
				cputs("Akcja anulowana");
			}
		}
		else if (zn == 0x75) { 
			gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 18);
			cputs("Gra jest w toku, czy chcesz ja zresetowac \ni rozpoczac nowa z handicapem? -> wcisnij ENTER");
			ok = getch();
			if (ok == 0x0d) {
				x = defx;
				y = defy;
				userB.handicap = 0;
				createColorBoard(plansza, board1); 
				createBoard(plansza2, board1);

				userB.score = 0; 
				userW.score = 0;

				if (board1.board_W == 1) {
					printFS(plansza2, board1);
				}
				else printF(plansza2, board1); 

				board1.breakpointsX = 0;
				board1.breakpointsY = 0;
				kol = 1;
				rzad = 1;
				gameC.round = 0;
				handicap = 1;
				ok = 0;
				do {
					gotoxy(x, y); 
					textcolor(BLACK);
					textbackground(DARKGRAY);
					putch('O');

					textcolor(WHITE);
					textbackground(LIGHTRED);
					gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 16);
					cputs(" Wybrales opcje gry z handicapem, czarny kladzie \n swoje pionki! [wcisniej i w miejscu w \nktorym chcesz postawic kamien nastepnie wcisnij ENTER]\n.Aby anulowac wydarzenie nalezy wcisnac ESCAPE ");
					ok = getch();

					if (board1.board_W == 1) {
						if (ok == 0x48) {
							if (board1.breakpointsY > 0) board1.breakpointsY--;
							else if (board1.breakpointsY == 0) {
								if (y - 1 <= defy) y = defy;
								else y--;
							}
							if (y == defy) rzad = PLANSZA_DEF_Y + board1.breakpointsY;
							else rzad = y - PLANSZA_DEF_Y + board1.breakpointsY;
						}
						else if (ok == 0x50) {
							if (board1.boardS + 2 > board1.windowSizeY && board1.breakpointsY < (board1.boardS - board1.windowSizeY + 2)) {
								board1.breakpointsY++;
							}
							if (board1.breakpointsY + board1.windowSizeY >= board1.boardS + 2) {
								if (y + 1 > PLANSZA_DEF_Y + board1.windowSizeY - 2) {
									y = defy + board1.windowSizeY - 3; 
								}
								else y++;
							}
							if (y == defy) {
								rzad = board1.breakpointsY + 1;
							}
							else rzad = y - PLANSZA_DEF_Y + board1.breakpointsY;
						}
						else if (ok == 0x4b) {
							if (board1.breakpointsX > 0) {
								board1.breakpointsX--;
							}
							else if (board1.breakpointsX == 0) {
								if (x - 1 <= defx) x = defx;
								else x--;
							}
							if (kol = x - PLANSZA_DEF_X - board1.breakpointsX == 0) kol = 1;
							if (x == defx) {
								kol = board1.breakpointsX + 1;
							}
							else kol = x - PLANSZA_DEF_X + board1.breakpointsX;
						}
						else if (ok == 0x4d) {
							if (board1.boardS + 2 > board1.windowSizeX && board1.breakpointsX < (board1.boardS - board1.windowSizeX + 2)) board1.breakpointsX++;
							if (board1.breakpointsX + board1.windowSizeX == board1.boardS + 2 || board1.breakpointsX + board1.windowSizeX > board1.boardS + 2) {
								if (x + 1 > PLANSZA_DEF_X + board1.windowSizeX - 2) x = defx + board1.windowSizeX - 3; 
								else x++;
							}
							kol = x - PLANSZA_DEF_X + board1.breakpointsX;
						}
						else if (ok == 0x69) {
							if (StoneAllowed(plansza2, gameC, plansza, kol, rzad, board1) == true) {
								plansza[((rzad) * (SIZE + 2)) + (kol)] = 2;
								plansza2[((rzad) * (SIZE + 2)) + (kol)] = 'O';
								++userB.handicap;
								gameC.handicap = 1;
							}
						}
						else if (ok == 0x1b) { 
							x = defx;
							y = defy;

							createColorBoard(plansza, board1);
							createBoard(plansza2, board1);

							userB.handicap = 0;
							gameC.handicap = 0;
							handicap = 0;
							gameC.round = 0;
							printFS(plansza2, board1);

							break;
						}

					}
					else {
						if (ok == 0x48) {
							if (y - 1 < defy) y = defy;
							else y--;
						}
						else if (ok == 0x50) {
							if (y + 1 > defy + board1.boardS - 2) y = defy + board1.boardS - 1;
							else y++;
						}
						else if (ok == 0x4b) {
							if (x - 1 < defx) x = defx;
							else x--;
						}
						else if (ok == 0x4d) {
							if (x + 1 > defx + board1.boardS - 2) x = defx + board1.boardS - 1;
							else x++;
						}
						else if (ok == 0x69) {
							if (StoneAllowed(plansza2, gameC, plansza, x, y, board1) == true) {
								plansza[((y - PLANSZA_DEF_Y) * (SIZE + 2)) + (x - PLANSZA_DEF_X)] = 2;
								plansza2[((y - PLANSZA_DEF_Y) * (SIZE + 2)) + (x - PLANSZA_DEF_X)] = 'O';
								++userB.handicap;
								gameC.handicap = 1;
							}
						}
						else if (ok == 0x1b) { 
							x = defx;
							y = defy;

							createColorBoard(plansza, board1); 
							createBoard(plansza2, board1);

							userB.handicap = 0; 
							gameC.handicap = 0;
							handicap = 0;
							gameC.round = 0;
							printF(plansza2, board1); 
							break;
						}
					}

					textcolor(WHITE);
					textbackground(LIGHTBLUE);
					printLegend();

					if (board1.board_W == 1) {
						printS(plansza2, gameC, plansza, board1);
					}
					else print(plansza2, gameC, plansza, board1);
					gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 10);

					textcolor(WHITE); //pozycja kursora
					textbackground(LIGHTBLUE);
					sprintf(pozycja, " pozycja kursora: %d, %d ", x, y);
					cputs(pozycja);

					gotoxy(x, y);
					textcolor(BLACK);
					textbackground(DARKGRAY);
					putch('O');

					gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 27);
					sprintf(handi, " Kamieni postawionych %d ", userB.handicap);
					cputs(handi);
				} while (ok != 0x0d);
				userW.score = userW.score + (userB.handicap * 0.5); //0.5 za kazdy kamien postawiony
				if (handicap != 0) ++gameC.round;
			}
			if (ok == 0x0b) {
				gotoxy(LEGENDA_DEF_X, LEGENDA_DEF_Y + 19);
				cputs("Akcja anulowana");
			}
		}
		else if (zn == 0x69) { // wcisnie i -> chce polozyc kamien
			if (board1.board_W == 1) {
				if (StoneAllowed(plansza2, gameC, plansza, kol + PLANSZA_DEF_X, rzad + PLANSZA_DEF_Y, board1) == true) {
					gotoxy(1, 29);
					textcolor(WHITE);
					textbackground(GREEN);
					cputs(confirm);
					zn = getch();
					if (zn == 0x0d) { //podmieniamy

						if (gameC.round % 2 == 0) plansza[rzad * (SIZE + 2) + kol] = 2; //podstawienie - kolory
						else plansza[rzad * (SIZE + 2) + kol] = 1;

						plansza2[rzad * (SIZE + 2) + kol] = 'O'; //podstawienie znaku 

						++gameC.round;
					}
					else if (zn != 0x1b) { //jak nie kliknie escape to sie nie moze ruszac, dziala takze dla wiekszej
						do {
							zn = getch();
							if (zn == 0) {
								zero = 1;
								zn = getch();
								if (zn == 0x48) {
									if (y - 1 < defy) y = defy;
									else y--;
								}
								else if (zn == 0x50) {
									if (y + 1 > defy + SIZE - 2) y = defy + SIZE - 1;
									else y++;
								}
								else if (zn == 0x4b) {
									if (x - 1 < defx) x = defx;
									else x--;
								}
								else if (zn == 0x4d) {
									if (x + 1 > defx + SIZE - 2) x = defx + SIZE - 1;
									else x++;
								}
							}
						} while (zn != 0x1b);
					}
				}
			}
			else {
				if (StoneAllowed(plansza2, gameC, plansza, x, y, board1) == true) {
					gotoxy(1, 29);
					textcolor(WHITE);
					textbackground(GREEN);
					cputs(confirm);
					zn = getch();
					if (zn == 0x0d) { //podmieniamy

						if (gameC.round % 2 == 0) plansza[((y - PLANSZA_DEF_Y) * (SIZE + 2)) + (x - PLANSZA_DEF_X)] = 2; //podstawienie - kolory
						else plansza[((y - PLANSZA_DEF_Y) * (SIZE + 2)) + (x - PLANSZA_DEF_X)] = 1;

						plansza2[((y - PLANSZA_DEF_Y) * (SIZE + 2)) + (x - PLANSZA_DEF_X)] = 'O'; //podstawienie znaku 

						++gameC.round;
					}
					else if (zn != 0x1b) { //jak nie kliknie escape to sie nie moze ruszac, dziala takze dla wiekszej
						do {
							zn = getch();
							if (zn == 0) {
								zero = 1;
								zn = getch();
								if (zn == 0x48) {
									if (y - 1 < defy) y = defy;
									else y--;
								}
								else if (zn == 0x50) {
									if (y + 1 > defy + SIZE - 2) y = defy + SIZE - 1;
									else y++;
								}
								else if (zn == 0x4b) {
									if (x - 1 < defx) x = defx;
									else x--;
								}
								else if (zn == 0x4d) {
									if (x + 1 > defx + SIZE - 2) x = defx + SIZE - 1;
									else x++;
								}
							}
						} while (zn != 0x1b);
					}
				}
			}
		}
		else if (zn == 0x6e) { //nowa gra
			x = defx;
			y = defy;

			createColorBoard(plansza, board1); //zerujemy plansze z polami i ich kolorami
			createBoard(plansza2, board1);

			userB.score = 0; //zerujemy wyniki
			userW.score = 0;

			if (board1.board_W == 1) {
				printFS(plansza2, board1);
			}
			else printF(plansza2, board1); //czysta plansza

			board1.breakpointsX = 0;
			board1.breakpointsY = 0;
			kol = 1;
			rzad = 1;
			gameC.round = 0; //zerowa runda
		}
		else if (zn == 0x6c) { //ODCZYTYWANIE Z PLIKU
			gameC.round = 0;
			userB.score = 0;
			userW.score = 0;
			userB.handicap = 0;

			open(a, plansza, c, b, board2);

			createBoard(plansza2, board1);

			if (board1.boardS == 1) {
				printFS(plansza2, board1);
			}
			else printF(plansza2, board1); //czysta plansza

			clrscr();
		}
	} while (zn != 0x71);

	gotoxy(1, LEGENDA_DEF_X + 20);
	textbackground(LIGHTGRAY);
	textcolor(BLACK);
	_setcursortype(_NORMALCURSOR);

	delete[] plansza;
	delete[] plansza2;
	return 0;
}