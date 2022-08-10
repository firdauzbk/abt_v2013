#include "stdafx.h"
#pragma once
class CConsole {
	FILE oldstdin, oldstdout;
	bool ownconsole;
public:
	CConsole();
	~CConsole();
};