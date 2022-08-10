#include "stdafx.h"
#pragma once


// 3 float storage structure(game coords)
struct Vec3_t
{

	float x;

	float y;

	float z;

};

//all functions relating to process interaction
struct ProcessFunctions
{

	static void EnableDebugPriv();

	DWORD GetModuleHandleByName(wchar_t* ModuleName, DWORD ProcID);

	HANDLE GetProcessHandleByName(wchar_t* ProcessName);

	bool ReadSettingsFile(std::string* data);

	void ParseSettings(std::string* data);

	bool ReadSettings();

};

//RPM wrapper
struct MemoryFunctions
{

	template <typename ReadType> ReadType Read(DWORD Address);

	template <typename WriteType> WriteType Write(DWORD Address, WriteType Data);

};


//functions that don't fit other categories
struct MiscFunctions
{

	float get3ddist(Vec3_t myCoords, Vec3_t enemyCoords);

	Vec3_t ClampAngle(Vec3_t SrcAngles);

	float GetRandomFloat(float min, float max);

	Vec3_t CalcAngle(Vec3_t PlayerPos, Vec3_t EnemyPos, Vec3_t PunchAngs, Vec3_t VecViewOrigin);

	Vec3_t SmoothAngle(Vec3_t SrcAngles, Vec3_t DestAngles, float SmoothAmount);

	Vec3_t VelocityComp(Vec3_t EnemyPos, Vec3_t EnemyVecVelocity, Vec3_t PlayerVecVelocity, float distance);

	void Click();

};

//storage for entity information
struct Entity_t
{

	DWORD BaseAddr;

	bool IsDead;

	DWORD BoneMatrixAddr;

	Vec3_t HeadPos;

	Vec3_t VecVelocity;

	int Team;

	bool Valid;

};


//storage for entities
struct EnityInfo_t
{
	Entity_t Entity[64];
};

//all functions relating to entity interaction
struct EntityFunctions
{

	DWORD GetBaseAddr(int PlayerNumber);

	bool IsDead(DWORD BaseAddr);

	int GetTeam(DWORD BaseAddr);

	Vec3_t GetVecVelocity(DWORD BaseAddr);

	DWORD GetBoneMatrixAddr(DWORD BaseAddr);

	Vec3_t GetBonePos(DWORD BoneMatrixAddr, int TargetBone);

	Entity_t GetEntity(int PlayerNumber);

};

//storage for local player info
struct Player_t
{
	DWORD BaseAddr;

	int Team;

	Vec3_t VecOrigin;

	bool Valid;

	Vec3_t PunchAngs;

	Vec3_t VecViewOrigin;

	int ShotsFired;

	int CrosshairEnt;

	Vec3_t VecVelocity;

	Vec3_t ViewAngles;

	DWORD EnginePointerAddr;

};

//storage for local player entity
struct PlayerInfo_t
{
	Player_t Player;
};

//all functions that relate to local-player interaction
struct PlayerFunctions
{

	DWORD GetBaseAddr();

	int GetTeam(DWORD BaseAddr);

	Vec3_t GetVecOrigin(DWORD BaseAddr);

	Player_t GetLocalPlayer();

	Vec3_t GetPunchAngs(DWORD BaseAddr);

	Vec3_t GetVecViewOrigin(DWORD BaseAddr);

	Vec3_t GetViewAngles(DWORD EnginePointerAddr);

	void SetViewAngles(DWORD EnginePointerAddr, Vec3_t AimAngles);

	int GetShotsFired(DWORD BaseAddr);

	int GetCrosshairEnt(DWORD BaseAddr);

	Vec3_t GetVecVelocity(DWORD BaseAddr);

	DWORD GetEnginePointerAddr();

};

//declaration for objects used by multiple .cpp files
extern HANDLE TargetProcess;
extern DWORD DwClient;
extern DWORD DwEngine;
extern ProcessFunctions ProcessToolbox;
extern MemoryFunctions MemoryToolbox;
extern EntityFunctions EntityToolbox;
extern PlayerFunctions PlayerToolbox;
extern MiscFunctions MiscToolbox;
extern EnityInfo_t EnityList;
extern PlayerInfo_t PlayerInfo;
extern float PitchMinPunch;
extern float PitchMaxPunch;
extern float YawMinPunch;
extern float YawMaxPunch;
extern int TargetBone;
extern float smoothamount;
extern int firstshotdelay;
extern int followupdelay;
extern int shotwait;
extern DWORD TrigVkeyCode;
extern DWORD AimVkeyCode;