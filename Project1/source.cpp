// source.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

bool AIM;
bool TRIG;
bool T1PAUSED = true;
bool T2PAUSED = true;
bool KILLALL = false;
bool T1STARTED;
bool T2STARTED;
bool T3STARTED;
bool T4STARTED;

ProcessFunctions ProcessToolbox;
MemoryFunctions MemoryToolbox;
EntityFunctions EntityToolbox;
PlayerFunctions PlayerToolbox;
MiscFunctions MiscToolbox;
EnityInfo_t EnityList;
PlayerInfo_t PlayerInfo;

HANDLE TargetProcess;
DWORD DwClient;
DWORD DwEngine;
string aimval = "ON";
string trigval = "ON";
float PitchMinPunch = 2.f;
float PitchMaxPunch = 2.f;
float YawMinPunch = 2.f;
float YawMaxPunch = 2.f;
int TargetBone = 10;
float smoothamount = 20.f;
int firstshotdelay = 1;
int followupdelay = 0;
int shotwait = 1;
DWORD TrigVkeyCode = 4;
DWORD AimVkeyCode = 1;
int MemSleep = 1;
CConsole con1;

void Init()
{
	ProcessToolbox.EnableDebugPriv();
	if (ProcessToolbox.ReadSettings())
	{
		TargetProcess = NULL;
		cout << "$ Waiting for game to start..." << endl;
		while (!TargetProcess)
		{
			cout << ".";
			TargetProcess = ProcessToolbox.GetProcessHandleByName(L"csgo.exe");
			Sleep(100);
		}
		cout << endl;
		DwClient = NULL;
		cout << "$ Getting client.dll handle..." << endl;
		while (!DwClient)
		{
			cout << ".";
			DwClient = ProcessToolbox.GetModuleHandleByName(L"client.dll", GetProcessId(TargetProcess));
			Sleep(100);
		}
		cout << endl;
		DwEngine = NULL;
		cout << "$ Getting Engine.dll handle..." << endl;
		while (!DwEngine)
		{
			cout << ".";
			DwEngine = ProcessToolbox.GetModuleHandleByName(L"Engine.dll", GetProcessId(TargetProcess));
			Sleep(100);
		}
		cout << endl;
	}
	else
	{
		ExitThread(0);
	}
}

DWORD WINAPI ReadEntityInfo(LPVOID PARAMS)
{
	T3STARTED = true;
	cout << "$ ReadEntityInfo thread started." << endl;
	while (!KILLALL)
	{
		for (int i = 0; i < 64; i++)
		{
			EnityList.Entity[i] = EntityToolbox.GetEntity(i);
		}
		Sleep(MemSleep);
	}
	ExitThread(0);
}

DWORD WINAPI ReadPlayerInfo(LPVOID PARAMS)
{
	T4STARTED = true;
	cout << "$ ReadPlayerInfo thread started." << endl;
	while (!KILLALL)
	{
		PlayerInfo.Player = PlayerToolbox.GetLocalPlayer();
		Sleep(MemSleep);
	}
	ExitThread(0);
}

DWORD WINAPI Aimbot(LPVOID PARAMS)
{
	T1STARTED = true;
	cout << "$ Aimbot thread started." << endl;
	while (!KILLALL)
	{
		if (AIM)
		{
			T1PAUSED = false;
			if (GetAsyncKeyState(AimVkeyCode) & 0x8000)
			{
				Player_t Player = PlayerInfo.Player;
				int EntityId = Player.CrosshairEnt - 1;
				if (EntityId >= 0 && EntityId < 64)
				{
					Entity_t Ent = EnityList.Entity[EntityId];
					while (Ent.Valid && !Ent.IsDead && Ent.Team != Player.Team && Player.ShotsFired >= shotwait && GetAsyncKeyState(AimVkeyCode) & 0x8000 && Player.Valid)
					{
						Player = PlayerInfo.Player;
						Ent = EnityList.Entity[EntityId];
						if (Ent.Valid && !Ent.IsDead && Ent.Team != Player.Team && Player.Valid)
						{
							Vec3_t CompEnemyPos = MiscToolbox.VelocityComp(Ent.HeadPos, Ent.VecVelocity, Player.VecVelocity, MiscToolbox.get3ddist(Player.VecOrigin, Ent.HeadPos));
							Vec3_t AimAngles = MiscToolbox.ClampAngle(MiscToolbox.CalcAngle(Player.VecOrigin, CompEnemyPos, Player.PunchAngs, Player.VecViewOrigin));
							AimAngles = MiscToolbox.ClampAngle(MiscToolbox.SmoothAngle(Player.ViewAngles, AimAngles, smoothamount));
							PlayerToolbox.SetViewAngles(Player.EnginePointerAddr, AimAngles);
						}
						Sleep(1);
					}
				}
				else
				{
					Sleep(1);
				}
			}
			else
			{
				Sleep(1);
			}
		}
		else
		{
			T1PAUSED = true;
			Sleep(10);
		}
	}
	ExitThread(0);
}

DWORD WINAPI Triggerbot(LPVOID PARAMS)
{
	T2STARTED = true;
	cout << "$ Triggerbot thread started." << endl;
	while (!KILLALL)
	{
		if (TRIG)
		{
			T2PAUSED = false;
			if (GetAsyncKeyState(TrigVkeyCode) & 0x8000)
			{
				Player_t Player = PlayerInfo.Player;
				int EntityId = Player.CrosshairEnt - 1;
				if (EntityId >= 0 && EntityId < 64)
				{
					Entity_t Ent = EnityList.Entity[EntityId];
					if (Ent.Valid && !Ent.IsDead && Ent.Team != Player.Team)
					{
						Sleep(firstshotdelay);
						MiscToolbox.Click();
						Sleep(followupdelay);
					}
					else
					{
						Sleep(1);
					}
				}
				else
				{
					Sleep(1);
				}
			}
			else
			{
				Sleep(1);
			}
		}
		else
		{
			T2PAUSED = true;
			Sleep(10);
		}
	}
	ExitThread(0);
}

DWORD WINAPI HackMain(LPVOID PARAMS)
{
	cout << "$ --- Shitty paste --- " << endl << "~~~~~~~~~~~~~~~~~ made by me & only for myself :( ~~~~~~~~~~~~~~~~~" << endl;
	system("pause");
	Init();
	AIM = false;
	TRIG = false;
	T1STARTED = false;
	T2STARTED = false;
	T3STARTED = false;
	T4STARTED = false;
	cout << "$ Waiting for uwu..." << endl;
	CreateThread(0, 0, &ReadPlayerInfo, 0, 0, 0);
	CreateThread(0, 0, &ReadEntityInfo, 0, 0, 0);
	CreateThread(0, 0, &Aimbot, 0, 0, 0);
	CreateThread(0, 0, &Triggerbot, 0, 0, 0);
	while (!T1STARTED || !T2STARTED || !T3STARTED || !T4STARTED)
	{
		Sleep(100);
	}
	cout << endl;
	TRIG = true;
	AIM = true;
	while (!KILLALL)
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_F8) < 0)
		{
			while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_F8) < 0)
			{
				Sleep(100);
			}
			KILLALL = true;
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_NUMPAD3) < 0)
		{
			while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_NUMPAD3) < 0)
			{
				Sleep(100);
			}
			cout << "$ Current settings: " << endl
				<< "$ Smoothing factor (float) = " << smoothamount << endl
				<< "$ Minimum RCS pitch compensation scale (float) = " << PitchMinPunch << endl
				<< "$ Maximum RCS pitch compensation scale (float) = " << PitchMaxPunch << endl
				<< "$ Minimum RCS yaw compensation scale (float) = " << YawMinPunch << endl
				<< "$ Maximum RCS yaw compensation scale (float) = " << YawMaxPunch << endl
				<< "$ Bone number to target (int) = " << TargetBone << endl
				<< "$ Amount of shots to ignore (int) = " << shotwait << endl
				<< "$ Virtual-Key code of button to use for aimbot (int) = " << AimVkeyCode << endl
				<< "$ Millisecond delay for first trigger shot (int) = " << firstshotdelay << endl
				<< "$ Millisecond delay for following trigger shots (int) = " << followupdelay << endl
				<< "$ Virtual-Key code of button to use for triggerbot (int) = " << TrigVkeyCode << endl
				<< "$ Done." << endl;
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_HOME) < 0)
		{
			while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_HOME) < 0)
			{
				Sleep(100);
			}
			cout << "$ Reloading hack..." << endl;
			bool storedAIM = AIM;
			bool storedTRIG = TRIG;
			AIM = false;
			TRIG = false;
			cout << "$ Waiting for threads to enter a paused state...";
			while (!T1PAUSED || !T2PAUSED)
			{
				cout << ".";
				Sleep(100);
			}
			cout << endl;
			Init();
			cout << "$ Threads should be restored." << endl;
			AIM = storedAIM;
			TRIG = storedTRIG;
		}

		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_PRIOR) < 0)
		{
			while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_PRIOR) < 0)
			{
				Sleep(100);
			}
			if (AIM)
			{
				AIM = false;
			}
			else
			{
				AIM = true;
			}
			if (AIM)
			{
				aimval = "ON";
			}
			else
			{
				aimval = "OFF";
			}
			cout << "$ AIM: " << aimval << " | TRIG: " << trigval << endl;
		}
		if (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_NEXT) < 0)
		{
			while (GetAsyncKeyState(VK_CONTROL) < 0 && GetAsyncKeyState(VK_NEXT) < 0)
			{
				Sleep(100);
			}
			if (TRIG)
			{
				TRIG = false;
			}
			else
			{
				TRIG = true;
			}
			if (TRIG)
			{
				trigval = "ON";
			}
			else
			{
				trigval = "OFF";
			}
			cout << "$ AIM: " << aimval << " | TRIG: " << trigval << endl;
		}
		Sleep(25);
	}
	cout << "$ Ending Main thread..." << endl;
	con1.~CConsole();
	ExitThread(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: CreateThread(0, 0, &HackMain, 0, 0, 0);
	}
	return TRUE;
}