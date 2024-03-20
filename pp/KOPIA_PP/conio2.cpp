/*
 * template for the course Podstawy Programowania
 * only to be used for the project in this course
 * author: K.M. Ocetkiewicz
 */
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<windows.h>
#include"conio2.h"
#ifdef __GNUC__
#include<stdlib.h>
#endif

#ifdef __cplusplus
#define EXTERNC	extern "C"
#else
#define EXTERNC
#endif

int _wscroll;


typedef struct _Conio2ThreadData {
	int attrib;
	int charCount;
	int charValue;
	int charFlag;
	int ungetCount;
	int ungetBuf[16];
	int _wscroll;
	int width;
	int height;
	int origwidth;
	int origheight;
	int origdepth;
	int lastmode;
	HANDLE output;
	HANDLE input;
	DWORD prevOutputMode;
	DWORD prevInputMode;
} Conio2ThreadData;


static Conio2ThreadData thData;


static auto UpdateWScroll() -> void
{
	if (_wscroll != thData._wscroll) {
		if (_wscroll) SetConsoleMode(thData.output, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
		else SetConsoleMode(thData.output, ENABLE_PROCESSED_OUTPUT);
		thData._wscroll = _wscroll;
	}
};


static auto ToWinAttribs(const int attrib) -> WORD
{
	WORD rv = 0;
	if (attrib & 1) rv |= FOREGROUND_BLUE;
	if (attrib & 2) rv |= FOREGROUND_GREEN;
	if (attrib & 4) rv |= FOREGROUND_RED;
	if (attrib & 8) rv |= FOREGROUND_INTENSITY;
	if (attrib & 16) rv |= BACKGROUND_BLUE;
	if (attrib & 32) rv |= BACKGROUND_GREEN;
	if (attrib & 64) rv |= BACKGROUND_RED;
	if (attrib & 128) rv |= BACKGROUND_INTENSITY;
	return rv;
};


static auto FromWinAttribs(WORD attrib) -> int
{
	auto rv = 0;
	rv |= (attrib & FOREGROUND_BLUE) ? 1 : 0;
	rv |= (attrib & FOREGROUND_GREEN) ? 2 : 0;
	rv |= (attrib & FOREGROUND_RED) ? 4 : 0;
	rv |= (attrib & FOREGROUND_INTENSITY) ? 8 : 0;
	rv |= (attrib & BACKGROUND_BLUE) ? 16 : 0;
	rv |= (attrib & BACKGROUND_GREEN) ? 32 : 0;
	rv |= (attrib & BACKGROUND_RED) ? 64 : 0;
	rv |= (attrib & BACKGROUND_INTENSITY) ? 128 : 0;
	return rv;
};


static auto GetCP(int* x, int* y) -> void
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(thData.output, &info);
	if (x) *x = info.dwCursorPosition.X + 1;
	if (y) *y = info.dwCursorPosition.Y + 1;
};


static auto HandleKeyEvent(INPUT_RECORD* buf) -> int
{
	int ch = static_cast<int>(buf->Event.KeyEvent.uChar.AsciiChar) & 255;
	if (ch == 0) ch = 0x8000 + buf->Event.KeyEvent.wVirtualKeyCode;
	if (ch == 0x8010 || ch == 0x8011 || ch == 0x8012 || ch == 0x8014
		|| ch == 0x8090 || ch == 0x8091) return 0;
	thData.charCount = buf->Event.KeyEvent.wRepeatCount;
	thData.charFlag = ch & 0x8000 ? 1 : 0;
	if (thData.charFlag) thData.charCount *= 2;

	const int alt_pressed = (buf->Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0;
	const int ctrl_pressed = (buf->Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;

	switch (ch) {
	case 0x8000 + 33:	ch = 0x8000 + 73; break;
	case 0x8000 + 34:	ch = 0x8000 + 81; break;
	case 0x8000 + 35:	ch = 0x8000 + 79; break;
	case 0x8000 + 36:	ch = 0x8000 + 71; break;
	case 0x8000 + 37:	
		if (ctrl_pressed) ch = 0x8073;
		else if (alt_pressed) ch = 0x809b;
		else ch = 0x8000 + 75; break;
	case 0x8000 + 38:	
		if (ctrl_pressed) ch = 0x808d;
		else if (alt_pressed) ch = 0x8098;
		else ch = 0x8000 + 72; break;
	case 0x8000 + 39:	
		if (ctrl_pressed) ch = 0x8074;
		else if (alt_pressed) ch = 0x809d;
		else ch = 0x8000 + 77; break;
	case 0x8000 + 40:	
		if (ctrl_pressed) ch = 0x8091;
		else if (alt_pressed) ch = 0x80a0;
		else ch = 0x8000 + 80; break;
	case 0x8000 + 46:	ch = 0x8000 + 83; break;
	case 0x8000 + 112:	ch = 0x8000 + 59; break;
	case 0x8000 + 113:	ch = 0x8000 + 60; break;
	case 0x8000 + 114:	ch = 0x8000 + 61; break;
	case 0x8000 + 115:	ch = 0x8000 + 62; break;
	case 0x8000 + 116:	ch = 0x8000 + 63; break;
	case 0x8000 + 117:	ch = 0x8000 + 64; break;
	case 0x8000 + 118:	ch = 0x8000 + 65; break;
	case 0x8000 + 119:	ch = 0x8000 + 66; break;
	case 0x8000 + 120:	ch = 0x8000 + 67; break;
	case 0x8000 + 121:	ch = 0x8000 + 68; break;
	case 0x8000 + 122:	ch = 0x8000 + 133; break;
	case 0x8000 + 123:	ch = 0x8000 + 134; break;
	default: ;
	}

	thData.charValue = ch & 0x7fff;

	return 1;
};


static auto ResizeConsole(const HANDLE con, const int w, const int h, int d) -> void
{
	COORD s;
	SMALL_RECT r;
	CONSOLE_SCREEN_BUFFER_INFO info;
	BOOL res = GetConsoleScreenBufferInfo(con, &info);

	const int cw = info.srWindow.Right - info.srWindow.Left + 1;
	const int ch = info.srWindow.Bottom - info.srWindow.Top + 1;

	if (w < cw || h < ch) {
		r.Top = 0;
		r.Left = 0;
		r.Right = static_cast<SHORT>((min(w, cw) - 1));
		r.Bottom = static_cast<SHORT>((min(h, ch) - 1));
		SetConsoleWindowInfo(con, TRUE, &r);
	}

	if (d < h) 
		d = h;

	s.X = static_cast<SHORT>(w);
	s.Y = static_cast<SHORT>(d);
	SetConsoleScreenBufferSize(con, s);

	r.Top = 0;
	r.Left = 0;
	r.Right = static_cast<SHORT>(w - 1);
	r.Bottom = static_cast<SHORT>(h - 1);
	SetConsoleWindowInfo(con, TRUE, &r);
};


static auto ReloadConsoleSize() -> void
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (GetConsoleScreenBufferInfo(thData.output, &info)) {
		thData.width = info.srWindow.Right - info.srWindow.Left + 1;
		thData.height = info.srWindow.Bottom - info.srWindow.Top + 1;
	}
};


static auto InitConio2(Conio2ThreadData* data) -> void
{
	data->output = GetStdHandle(STD_OUTPUT_HANDLE);
	data->input = GetStdHandle(STD_INPUT_HANDLE);
	data->ungetCount = 0;
	data->charCount = 0;

	CONSOLE_SCREEN_BUFFER_INFO info;
	const BOOL rc = GetConsoleScreenBufferInfo(data->output, &info);
	if (rc) {
		data->origwidth = info.srWindow.Right - info.srWindow.Left + 1;
		data->origheight = info.srWindow.Bottom - info.srWindow.Top + 1;
		data->origdepth = info.dwSize.Y;
	}
	else {
		data->origwidth = 80;
		data->origheight = 25;
		data->origdepth = 25;
	}

	data->width = data->origwidth;
	data->height = data->origheight;

	data->attrib = 0x07;
	data->lastmode = C80;
	data->_wscroll = -1;
	SetConsoleTextAttribute(thData.output, ToWinAttribs(thData.attrib));
	GetConsoleMode(data->input, &(data->prevInputMode));
	GetConsoleMode(data->output, &(data->prevOutputMode));
	SetConsoleMode(data->input, ENABLE_PROCESSED_INPUT);
	UpdateWScroll();
};


static auto ExitConio2(Conio2ThreadData* data) -> void
{
	ResizeConsole(data->output, data->origwidth, data->origheight, data->origdepth);
	SetConsoleMode(data->input, data->prevInputMode);
	SetConsoleMode(data->output, data->prevOutputMode);
};


EXTERNC
auto gotoxy(const int x, const int y) -> void
{
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(thData.output, pos);
};


EXTERNC
auto wherex() -> int
{
	int x;
	GetCP(&x, nullptr);
	return x;
};


EXTERNC
auto wherey() -> int
{
	int y;
	GetCP(nullptr, &y);
	return y;
};


EXTERNC
auto _setcursortype(const int cur_t) -> void
{
	CONSOLE_CURSOR_INFO inf;
	GetConsoleCursorInfo(thData.output, &inf);
	if (cur_t == _NOCURSOR) {
		inf.bVisible = FALSE;
	}
	else if (cur_t == _NORMALCURSOR) {
		inf.bVisible = TRUE;
		inf.dwSize = 13;
	}
	else if (cur_t == _SOLIDCURSOR) {
		inf.bVisible = TRUE;
		inf.dwSize = 100;
	}
	else return;
	SetConsoleCursorInfo(thData.output, &inf);
};


EXTERNC
auto textattr(const int newattr) -> void
{
	thData.attrib = newattr;
	SetConsoleTextAttribute(thData.output, ToWinAttribs(thData.attrib));
};


EXTERNC
auto textbackground(const int newcolor) -> void
{
	thData.attrib = (thData.attrib & 0x0f) | ((newcolor & 15) << 4);
	SetConsoleTextAttribute(thData.output, ToWinAttribs(thData.attrib));
};


EXTERNC
auto textcolor(const int newcolor) -> void
{
	thData.attrib = (thData.attrib & 0xf0) | (newcolor & 15);
	SetConsoleTextAttribute(thData.output, ToWinAttribs(thData.attrib));
};


EXTERNC
auto clreol() -> void
{
	int cx, cy;
	COORD pos;
	DWORD nwr, len;
	GetCP(&cx, &cy);
	len = thData.width - cx + 1;

	if (len > 0) {
		pos.X = cx - 1;
		pos.Y = cy - 1;
		FillConsoleOutputCharacter(thData.output, ' ', len, pos, &nwr);
		FillConsoleOutputAttribute(thData.output, thData.attrib, len, pos, &nwr);
		gotoxy(cx, cy);
	}
};


EXTERNC
auto clrscr() -> void
{
	ReloadConsoleSize();
	DWORD nwr, len = thData.width * thData.height;
	COORD pos;
	pos.X = pos.Y = 0;
	FillConsoleOutputCharacter(thData.output, ' ', len, pos, &nwr);
	FillConsoleOutputAttribute(thData.output, thData.attrib, len, pos, &nwr);
};


EXTERNC
auto delline() -> void
{
	int y;
	y = wherey();
	movetext(1, y + 1, thData.width, thData.height, 1, y);
};


EXTERNC
auto insline() -> void
{
	int y;
	y = wherey();
	movetext(1, y, thData.width, thData.height - 1, 1, y + 1);
};


EXTERNC
auto getch() -> int
{
	BOOL rv;
	DWORD n;
	INPUT_RECORD buf;

	if (thData.ungetCount > 0) {
		thData.ungetCount--;
		return thData.ungetBuf[thData.ungetCount];
	}

	if (thData.charCount > 0) {
		thData.charCount--;
		if ((thData.charCount & 1) && thData.charFlag) return 0;
		else return thData.charValue;
	}

	while (true) {
		rv = ReadConsoleInput(thData.input, &buf, 1, &n);
		if (!rv) continue;
		if (buf.EventType != KEY_EVENT) continue;
		if (!buf.Event.KeyEvent.bKeyDown) continue;
		if (HandleKeyEvent(&buf)) break;
	}

	thData.charCount--;
	if ((thData.charCount & 1) && thData.charFlag) return 0;
	else return thData.charValue;
};


EXTERNC
auto getche() -> int
{
	int ch;
	ch = getch();
	putch(ch);
	return ch;
};


EXTERNC
auto kbhit() -> int
{
	BOOL rv;
	DWORD n;
	INPUT_RECORD buf;

	if (thData.ungetCount > 0) return 1;
	if (thData.charCount > 0) return 1;

	rv = PeekConsoleInput(thData.input, &buf, 1, &n);
	if (!rv) return 0;
	if (n == 0) return 0;
	rv = ReadConsoleInput(thData.input, &buf, 1, &n);
	if (!rv) return 0;
	if (buf.EventType != KEY_EVENT) return 0;
	if (!buf.Event.KeyEvent.bKeyDown) return 0;
	return HandleKeyEvent(&buf) ? 1 : 0;
};


EXTERNC
auto ungetch(const int ch) -> int
{
	if (thData.ungetCount >= 15) return EOF;
	thData.ungetBuf[thData.ungetCount++] = ch;
	return ch;
};


EXTERNC
auto cputs(const char* str) -> int
{
	DWORD count;
	if (str == NULL) return EOF;
	UpdateWScroll();
	if (WriteConsoleA(thData.output, str, strlen(str), &count, NULL)) return count;
	else return EOF;
};


EXTERNC
auto putch(const int c) -> int
{
	DWORD count;
	UpdateWScroll();
	if (WriteConsoleA(thData.output, &c, 1, &count, NULL)) return c;
	else return EOF;
};


EXTERNC
auto gettext(const int left, const int top, const int right, const int bottom, void* dest) -> int
{
	int i;
	SMALL_RECT trg;
	COORD pos, size;
	CHAR_INFO* buf;
	buf = (CHAR_INFO*)alloca((right - left + 1) * (bottom - top + 1) * sizeof(CHAR_INFO));

	pos.X = 0; pos.Y = 0;
	size.X = right - left + 1; size.Y = bottom - top + 1;
	trg.Left = left - 1; trg.Top = top - 1;
	trg.Right = right - 1; trg.Bottom = bottom - 1;
	ReadConsoleOutputA(thData.output, buf, size, pos, &trg);
	for (i = 0; i < (right - left + 1) * (bottom - top + 1); i++) {
		((char*)dest)[i * 2] = buf[i].Char.AsciiChar;
		((char*)dest)[i * 2 + 1] = FromWinAttribs(buf[i].Attributes);
	};
	return 1;
};


EXTERNC
auto movetext(const int left, const int top, const int right, const int bottom, const int destleft,
              const int desttop) -> int
{
	char* buf;
	buf = (char*)alloca((right - left + 1) * (bottom - top + 1) * 2);
	gettext(left, top, right, bottom, buf);
	puttext(destleft, desttop, destleft + right - left, desttop + bottom - top, buf);
	return 1;
};


EXTERNC
auto puttext(const int left, const int top, const int right, const int bottom, void* source) -> int
{
	int i;
	SMALL_RECT trg;
	COORD pos, size;
	CHAR_INFO* buf;
	buf = (CHAR_INFO*)alloca((right - left + 1) * (bottom - top + 1) * sizeof(CHAR_INFO));
	for (i = 0; i < (right - left + 1) * (bottom - top + 1); i++) {
		buf[i].Char.AsciiChar = ((char*)source)[i * 2];
		buf[i].Attributes = ToWinAttribs(((char*)source)[i * 2 + 1]);
	};
	pos.X = 0; pos.Y = 0;
	size.X = right - left + 1; size.Y = bottom - top + 1;
	trg.Left = left - 1; trg.Top = top - 1;
	trg.Right = right - 1; trg.Bottom = bottom - 1;
	WriteConsoleOutputA(thData.output, buf, size, pos, &trg);
	return 1;
};


EXTERNC
auto settitle(const char* title) -> void
{
	SetConsoleTitleA(title);
};


auto gettextinfo(struct text_info* info) -> void
{
	info->curx = wherex();
	info->cury = wherey();
	info->attribute = thData.attrib;
	info->normattr = 0x07;
	info->screenwidth = thData.width;
	info->screenheight = thData.height;
};


auto textmode(const int mode) -> void
{
	if (mode == C80) {
		thData.width = 80;
		thData.height = 25;
		ResizeConsole(thData.output, 80, 25, 25);
	}
	else if (mode == C4350) {
		thData.width = 80;
		thData.height = 50;
		ResizeConsole(thData.output, 80, 50, 50);
	}
	else if (mode == LASTMODE) {
		textmode(thData.lastmode);
	};
};


static auto Conio2_Exit(void) -> void
{
	ExitConio2(&thData);
};

auto Conio2_Init(void) -> int
{
	InitConio2(&thData);
	atexit(Conio2_Exit);
	return 0;
};

#ifdef __cplusplus
static int conio2_init = Conio2_Init();
#endif
