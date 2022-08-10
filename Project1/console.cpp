#include "stdafx.h"

static BOOL WINAPI MyConsoleCtrlHandler(DWORD dwCtrlEvent) { return dwCtrlEvent == CTRL_C_EVENT; }

const char* version = "Ver.2.4.2";

CConsole::CConsole() : ownconsole(false) {
	if (!AllocConsole()) return;
	SetConsoleCtrlHandler(MyConsoleCtrlHandler, TRUE);
	SetConsoleTitleA(version);
	HANDLE ConInput = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE ConOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	const int in = _open_osfhandle(INT_PTR(ConInput), _O_TEXT);
	const int out = _open_osfhandle(INT_PTR(ConOutput), _O_TEXT);
	oldstdin = *stdin;
	oldstdout = *stdout;
	*stdin = *_fdopen(in, "r");
	*stdout = *_fdopen(out, "w");
	ownconsole = true;
	if (!RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND))
	{
		cout << "ERROR: Failed to remove menu from console. " << GetLastError() << endl;
	}
	if (!SetConsoleTextAttribute(ConOutput, FOREGROUND_GREEN | FOREGROUND_INTENSITY))
	{
		cout << "ERROR: Failed to set console text attributes. " << GetLastError() << endl;
	}
	if (!SetConsoleMode(ConInput, ENABLE_MOUSE_INPUT | ENABLE_WRAP_AT_EOL_OUTPUT))
	{
		cout << "ERROR: Failed to set console mode. " << GetLastError() << endl;
	}
}

CConsole::~CConsole() {
	if (ownconsole) {
		fclose(stdout);
		fclose(stdin);
		*stdout = oldstdout;
		*stdin = oldstdin;
		SetConsoleCtrlHandler(MyConsoleCtrlHandler, FALSE);
		FreeConsole();
	}
}