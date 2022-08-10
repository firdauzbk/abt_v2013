#include "stdafx.h"

DWORD m_dwEntityList;
DWORD m_dwClientState;
DWORD m_dwLocalPlayer;

#pragma endregion 

#pragma region group2//these normally don't

DWORD m_iCrossHairID;
DWORD m_dwViewAngles;
DWORD m_vecViewOffset;
DWORD m_vecOrigin;
DWORD m_vecPunch;
DWORD m_iTeamNum;
DWORD m_iShotsFired;
DWORD m_dwBoneMatrix;
const DWORD DwEntitySize = 0x10;
DWORD m_iHealth;
DWORD m_lifeState;
DWORD m_vecVelocity;
DWORD m_fFlags;
#pragma endregion

#pragma region group3//other shit
string Settingspath = "C:\\config\\settings.txt";
CConsole Con1;
void ProcessFunctions::EnableDebugPriv()
{
	HANDLE HTOKEN;
	LUID LUID;
	TOKEN_PRIVILEGES TOKEN_PRIV;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &HTOKEN);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &LUID);
	TOKEN_PRIV.PrivilegeCount = 1;
	TOKEN_PRIV.Privileges[0].Luid = LUID;
	TOKEN_PRIV.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(HTOKEN, false, &TOKEN_PRIV, sizeof(TOKEN_PRIV), NULL, NULL);
	CloseHandle(HTOKEN);
}

DWORD ProcessFunctions::GetModuleHandleByName(wchar_t* ModuleName, DWORD ProcID)
{
	MODULEENTRY32 ENTRY;
	ENTRY.dwSize = sizeof(MODULEENTRY32);
	HANDLE HSNAP = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcID);
	if (Module32First(HSNAP, &ENTRY) == TRUE)
	{
		if (!_wcsicmp(ENTRY.szModule, ModuleName))
		{
			DWORD HMODULE = (DWORD)ENTRY.modBaseAddr;
			return HMODULE;
		}
		else
		{
			while (Module32Next(HSNAP, &ENTRY) == TRUE)
			{
				if (!_wcsicmp(ENTRY.szModule, ModuleName))
				{
					DWORD HMODULE = (DWORD)ENTRY.modBaseAddr;
					return HMODULE;
				}
			}
			return 0;
		}
	}
	else
	{
		CloseHandle(HSNAP);
		return{ 0 };
	}
}

HANDLE ProcessFunctions::GetProcessHandleByName(wchar_t* ProcessName)
{
	PROCESSENTRY32 ENTRY;
	ENTRY.dwSize = sizeof(PROCESSENTRY32);
	HANDLE HSNAP = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(HSNAP, &ENTRY) == TRUE)
	{
		if (!_wcsicmp(ENTRY.szExeFile, ProcessName))
		{
			HANDLE HPROC = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ENTRY.th32ProcessID);
			return HPROC;
		}
		else
		{
			while (Process32Next(HSNAP, &ENTRY) == TRUE)
			{
				if (!_wcsicmp(ENTRY.szExeFile, ProcessName))
				{
					HANDLE HPROC = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ENTRY.th32ProcessID);
					return HPROC;
				}
			}
			return 0;
		}
	}
	else
	{
		CloseHandle(HSNAP);
		return{ 0 };
	}
}

bool ProcessFunctions::ReadSettingsFile(std::string* data)
{
	ifstream read;
	if (ifstream(Settingspath))
	{
		read.open(Settingspath);
		string linedata;
		int currentline = 1;
		while (!read.eof())
		{
			getline(read, linedata);
			data[currentline] = linedata;
			currentline++;
		}
		read.close();
		return true;
	}
	else
	{
		return false;
	}
}

void ProcessFunctions::ParseSettings(std::string* data)
{
	data[1] = data[1].substr(data[1].find("=") + 1); //entlist
	data[2] = data[2].substr(data[2].find("=") + 1); //player
	data[3] = data[3].substr(data[3].find("=") + 1); //engineptr
	data[4] = data[4].substr(data[4].find("=") + 1); //crosshairid
	data[5] = data[5].substr(data[5].find("=") + 1); //vangle
	data[6] = data[6].substr(data[6].find("=") + 1); //vecview
	data[7] = data[7].substr(data[7].find("=") + 1); //vecorigin
	data[8] = data[8].substr(data[8].find("=") + 1); //vecpunch
	data[9] = data[9].substr(data[9].find("=") + 1); //teamnum
	data[10] = data[10].substr(data[10].find("=") + 1); //shotsfired
	data[11] = data[11].substr(data[11].find("=") + 1); //bonematrix
	data[12] = data[12].substr(data[12].find("=") + 1); //health
	data[13] = data[13].substr(data[13].find("=") + 1); //lifestate
	data[14] = data[14].substr(data[14].find("=") + 1); //vecvelocity
	data[15] = data[15].substr(data[15].find("=") + 1); //smooth
	data[16] = data[16].substr(data[16].find("=") + 1); //minpitch
	data[17] = data[17].substr(data[17].find("=") + 1); //maxpitch
	data[18] = data[18].substr(data[18].find("=") + 1); //minyaw
	data[19] = data[19].substr(data[19].find("=") + 1); //maxyaw
	data[20] = data[20].substr(data[20].find("=") + 1); //bone
	data[21] = data[21].substr(data[21].find("=") + 1); //shotignore
	data[22] = data[22].substr(data[22].find("=") + 1); //aimkey
	data[23] = data[23].substr(data[23].find("=") + 1); //1stshotdelay
	data[24] = data[24].substr(data[24].find("=") + 1); //followupdelay
	data[25] = data[25].substr(data[25].find("=") + 1); //trigkey

	cout << "$ Settings:" << endl
		<< endl << "$ -> Netvar/Offsets ->" << endl
		<< "$ m_dwEntityList (offset) = " << data[1].c_str() << endl
		<< "$ m_dwLocalPlayer (offset) = " << data[2].c_str() << endl
		<< "$ m_dwClientState (offset) = " << data[3].c_str() << endl
		<< "$ m_iCrossHairID (offset) = " << data[4].c_str() << endl
		<< "$ m_dwViewAngles (offset) = " << data[5].c_str() << endl
		<< "$ m_vecViewOffset (offset) = " << data[6].c_str() << endl
		<< "$ m_vecOrigin (offset) = " << data[7].c_str() << endl
		<< "$ m_vecPunch (offset) = " << data[8].c_str() << endl
		<< "$ m_iTeamNum (offset) = " << data[9].c_str() << endl
		<< "$ m_iShotsFired (offset) = " << data[10].c_str() << endl
		<< "$ m_dwBoneMatrix (offset) = " << data[11].c_str() << endl
		<< "$ m_iHealth (offset) = " << data[12].c_str() << endl
		<< "$ m_lifeState (offset) = " << data[13].c_str() << endl
		<< "$ m_vecVelocity (offset) = " << data[14].c_str() << endl

		<< endl << "$ -> Hack Customizations ->" << endl
		<< "$ Smoothing factor (float) = " << data[15] << endl
		<< "$ Minimum RCS pitch compensation scale (float) = " << data[16] << endl
		<< "$ Maximum RCS pitch compensation scale (float) = " << data[17] << endl
		<< "$ Minimum RCS yaw compensation scale (float) = " << data[18] << endl
		<< "$ Maximum RCS yaw compensation scale (float) = " << data[19] << endl
		<< "$ Bone number to target (int) = " << data[20] << endl
		<< "$ Amount of shots to ignore (int) = " << data[21] << endl
		<< "$ Virtual-Key code of button to use for aimbot (int) = " << data[22] << endl
		<< "$ Millisecond delay for first trigger shot (int) = " << data[23] << endl
		<< "$ Millisecond delay for following trigger shots (int) = " << data[24] << endl
		<< "$ Virtual-Key code of button to use for triggerbot (int) = " << data[25] << endl
		<< "$ Would you like to use these settings? [Y/N] : ";
	while (true)
	{
		string input;
		cin >> input;
		if (input == "Y" || input == "y")
		{
			m_dwEntityList = strtol(data[1].c_str(), 0, 0);
			m_dwLocalPlayer = strtol(data[2].c_str(), 0, 0);
			m_dwClientState = strtol(data[3].c_str(), 0, 0);
			m_iCrossHairID = strtol(data[4].c_str(), 0, 0);
			m_dwViewAngles = strtol(data[5].c_str(), 0, 0);
			m_vecViewOffset = strtol(data[6].c_str(), 0, 0);
			m_vecOrigin = strtol(data[7].c_str(), 0, 0);
			m_vecPunch = strtol(data[8].c_str(), 0, 0);
			m_iTeamNum = strtol(data[9].c_str(), 0, 0);
			m_iShotsFired = strtol(data[10].c_str(), 0, 0);
			m_dwBoneMatrix = strtol(data[11].c_str(), 0, 0);
			m_iHealth = strtol(data[12].c_str(), 0, 0);
			m_lifeState = strtol(data[13].c_str(), 0, 0);
			m_vecVelocity = strtol(data[14].c_str(), 0, 0);
			smoothamount = stof(data[15]);
			PitchMinPunch = stof(data[16]);
			PitchMaxPunch = stof(data[17]);
			YawMinPunch = stof(data[18]);
			YawMaxPunch = stof(data[19]);
			TargetBone = stoi(data[20]);
			shotwait = stoi(data[21]);
			AimVkeyCode = stoi(data[22]);
			firstshotdelay = stoi(data[23]);
			followupdelay = stoi(data[24]);
			TrigVkeyCode = stoi(data[25]);
			break;
		}
		else if (input == "N" || input == "n")
		{
			break;
		}
	}
}

bool ProcessFunctions::ReadSettings()
{
	string settings[26];
	if (ReadSettingsFile(settings))
	{
		ParseSettings(settings);
		return true;
	}
	else
	{
		MessageBox(0, L"No settings file found!", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}
}

template <typename ReadType> ReadType MemoryFunctions::Read(DWORD Address)
{
	ReadType Data;
	if (ReadProcessMemory(TargetProcess, (LPVOID)(Address), &Data, sizeof(ReadType), 0))
	{
		return Data;
	}
	else
	{
		return{ 0 };
	}
}

template <typename WriteType> WriteType MemoryFunctions::Write(DWORD Address, WriteType Data)
{
	if (WriteProcessMemory(TargetProcess, (LPVOID)(Address), &Data, sizeof(WriteType), 0))
	{
		return{ 1 };
	}
	else
	{
		return{ 0 };
	}
}

float MiscFunctions::get3ddist(Vec3_t myCoords, Vec3_t enemyCoords)
{
	return sqrt(
		powf(enemyCoords.x - myCoords.x, 2.0) +
		powf(enemyCoords.y - myCoords.y, 2.0) +
		powf(enemyCoords.z - myCoords.z, 2.0));
}

Vec3_t MiscFunctions::ClampAngle(Vec3_t SrcAngles)
{

	if (SrcAngles.x > 89.0f && SrcAngles.x <= 180.0f)
	{
		SrcAngles.x = 89.0f;
	}
	if (SrcAngles.x > 180.f)
	{
		SrcAngles.x -= 360.f;
	}
	if (SrcAngles.x < -89.0f)
	{
		SrcAngles.x = -89.0f;
	}
	if (SrcAngles.y > 180.f)
	{
		SrcAngles.y -= 360.f;
	}
	if (SrcAngles.y < -180.f)
	{
		SrcAngles.y += 360.f;
	}
	if (SrcAngles.z != 0.0f)
	{
		SrcAngles.z = 0.0f;
	}
	return SrcAngles;
}

float MiscFunctions::GetRandomFloat(float min, float max)
{
	random_device Random;
	mt19937 RandomGen(Random());
	uniform_real<float> Randomdistribute(min, max);
	return Randomdistribute(RandomGen);
}

Vec3_t MiscFunctions::CalcAngle(Vec3_t PlayerPos, Vec3_t EnemyPos, Vec3_t PunchAngs, Vec3_t VecViewOrigin)
{
	Vec3_t AimAngles;
	Vec3_t delta = { (PlayerPos.x - EnemyPos.x), (PlayerPos.y - EnemyPos.y), ((PlayerPos.z + VecViewOrigin.z) - EnemyPos.z) };
	float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	AimAngles.x = atanf(delta.z / hyp) * 57.295779513082f - PunchAngs.x * MiscToolbox.GetRandomFloat(PitchMinPunch, PitchMaxPunch);
	AimAngles.y = atanf(delta.y / delta.x) * 57.295779513082f - PunchAngs.y * MiscToolbox.GetRandomFloat(YawMinPunch, YawMaxPunch);
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
	{
		AimAngles.y += 180.0f;
	}
	return AimAngles;
}

Vec3_t MiscFunctions::SmoothAngle(Vec3_t SrcAngles, Vec3_t DestAngles, float SmoothAmount)
{
	if (SmoothAmount < 0.f)
	{
		SmoothAmount = 0.f;
	}
	Vec3_t SmoothedAngles;
	SmoothedAngles.x = DestAngles.x - SrcAngles.x;
	SmoothedAngles.y = DestAngles.y - SrcAngles.y;
	SmoothedAngles.z = 0.0f;
	SmoothedAngles = MiscToolbox.ClampAngle(SmoothedAngles);
	SmoothedAngles.x = SrcAngles.x + SmoothedAngles.x / 100.0f * smoothamount;
	SmoothedAngles.y = SrcAngles.y + SmoothedAngles.y / 100.0f * smoothamount;
	SmoothedAngles.z = 0.0f;
	SmoothedAngles = MiscToolbox.ClampAngle(SmoothedAngles);
	return SmoothedAngles;
}

Vec3_t MiscFunctions::VelocityComp(Vec3_t EnemyPos, Vec3_t EnemyVecVelocity, Vec3_t PlayerVecVelocity, float dist)
{
	EnemyPos.x += (EnemyVecVelocity.x) / dist;
	EnemyPos.y += (EnemyVecVelocity.y) / dist;
	EnemyPos.z += (EnemyVecVelocity.z) / dist;
	EnemyPos.x -= (PlayerVecVelocity.x) / dist;
	EnemyPos.y -= (PlayerVecVelocity.y) / dist;
	EnemyPos.z -= (PlayerVecVelocity.z) / dist;
	return EnemyPos;
}

void MiscFunctions::Click()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
}

DWORD EntityFunctions::GetBaseAddr(int PlayerNumber)
{
	if (DwClient)
	{
		return MemoryToolbox.Read<DWORD>(DwClient + m_dwEntityList + (DwEntitySize * PlayerNumber));
	}
	else
	{
		return 0;
	}
}

bool EntityFunctions::IsDead(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<bool>(BaseAddr + m_lifeState);
	}
	else
	{
		return 0;
	}
}

int EntityFunctions::GetTeam(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<int>(BaseAddr + m_iTeamNum);
	}
	else
	{
		return 0;
	}
}

Vec3_t EntityFunctions::GetVecVelocity(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<Vec3_t>(BaseAddr + m_vecVelocity);
	}
	else
	{
		return{ 0 };
	}
}

DWORD EntityFunctions::GetBoneMatrixAddr(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<DWORD>(BaseAddr + m_dwBoneMatrix);
	}
	else
	{
		return 0;
	}
}

Vec3_t EntityFunctions::GetBonePos(DWORD BoneMatrixAddr, int TargetBone)
{
	if (BoneMatrixAddr)
	{
		Vec3_t temp;
		temp.x = MemoryToolbox.Read<float>(BoneMatrixAddr + 0x30 * TargetBone + 0x0C);
		temp.y = MemoryToolbox.Read<float>(BoneMatrixAddr + 0x30 * TargetBone + 0x1C);
		temp.z = MemoryToolbox.Read<float>(BoneMatrixAddr + 0x30 * TargetBone + 0x2C);
		return temp;
	}
	else
	{
		return{ 0 };
	}
}

Entity_t EntityFunctions::GetEntity(int PlayerNumber)
{
	Entity_t Entity;
	Entity.Valid = true;
	Entity.BaseAddr = EntityToolbox.GetBaseAddr(PlayerNumber);
	if (Entity.BaseAddr)
	{
		Entity.IsDead = EntityToolbox.IsDead(Entity.BaseAddr);
		if (!Entity.IsDead)
		{
			Entity.Team = EntityToolbox.GetTeam(Entity.BaseAddr);
			Entity.BoneMatrixAddr = EntityToolbox.GetBoneMatrixAddr(Entity.BaseAddr);
			Entity.HeadPos = EntityToolbox.GetBonePos(Entity.BoneMatrixAddr, TargetBone);
			Entity.VecVelocity = EntityToolbox.GetVecVelocity(Entity.BaseAddr);
		}
	}
	else
	{
		Entity.Valid = false;
	}
	return Entity;
}

DWORD PlayerFunctions::GetBaseAddr()
{
	if (DwClient)
	{
		return MemoryToolbox.Read<DWORD>(DwClient + m_dwLocalPlayer);
	}
	else
	{
		return 0;
	}
}

int PlayerFunctions::GetTeam(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<int>(BaseAddr + m_iTeamNum);
	}
	else
	{
		return 0;
	}
}

Vec3_t PlayerFunctions::GetVecOrigin(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<Vec3_t>(BaseAddr + m_vecOrigin);
	}
	else
	{
		return{ 0 };
	}
}

Vec3_t PlayerFunctions::GetPunchAngs(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<Vec3_t>(BaseAddr + m_vecPunch);
	}
	else
	{
		return{ 0 };
	}
}

Vec3_t PlayerFunctions::GetVecViewOrigin(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<Vec3_t>(BaseAddr + m_vecViewOffset);
	}
	else
	{
		return{ 0 };
	}
}

Vec3_t PlayerFunctions::GetViewAngles(DWORD EnginePointerAddr)
{
	if (EnginePointerAddr)
	{
		return MemoryToolbox.Read<Vec3_t>(EnginePointerAddr + m_dwViewAngles);
	}
	else
	{
		return{ 0 };
	}
}

void PlayerFunctions::SetViewAngles(DWORD EnginePointerAddr, Vec3_t AimAngles)
{
	if (EnginePointerAddr)
	{
		MemoryToolbox.Write<Vec3_t>(EnginePointerAddr + m_dwViewAngles, AimAngles);
	}
	else
	{
		return;
	}
}

int PlayerFunctions::GetShotsFired(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<int>(BaseAddr + m_iShotsFired);
	}
	else
	{
		return 0;
	}
}

int PlayerFunctions::GetCrosshairEnt(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<int>(BaseAddr + m_iCrossHairID);
	}
	else
	{
		return 0;
	}
}

Vec3_t PlayerFunctions::GetVecVelocity(DWORD BaseAddr)
{
	if (BaseAddr)
	{
		return MemoryToolbox.Read<Vec3_t>(BaseAddr + m_vecVelocity);
	}
	else
	{
		return{ 0 };
	}
}

DWORD PlayerFunctions::GetEnginePointerAddr()
{
	if (DwEngine)
	{
		return MemoryToolbox.Read<DWORD>(DwEngine + m_dwClientState);
	}
	else
	{
		return 0;
	}
}

Player_t PlayerFunctions::GetLocalPlayer()
{
	Player_t Player;
	Player.Valid = true;
	Player.BaseAddr = PlayerToolbox.GetBaseAddr();
	if (Player.BaseAddr)
	{
		Player.Team = PlayerToolbox.GetTeam(Player.BaseAddr);
		Player.VecOrigin = PlayerToolbox.GetVecOrigin(Player.BaseAddr);
		Player.PunchAngs = PlayerToolbox.GetPunchAngs(Player.BaseAddr);
		Player.VecViewOrigin = PlayerToolbox.GetVecViewOrigin(Player.BaseAddr);
		Player.CrosshairEnt = PlayerToolbox.GetCrosshairEnt(Player.BaseAddr);
		Player.ShotsFired = PlayerToolbox.GetShotsFired(Player.BaseAddr);
		Player.VecVelocity = PlayerToolbox.GetVecVelocity(Player.BaseAddr);
		Player.EnginePointerAddr = PlayerToolbox.GetEnginePointerAddr();
		Player.ViewAngles = PlayerToolbox.GetViewAngles(Player.EnginePointerAddr);
	}
	else
	{
		Player.Valid = false;
	}
	return Player;
}