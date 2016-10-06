#pragma once
#include <Windows.h>

#define UInt64 DWORD_PTR
#define UInt32 DWORD

enum EntityRenderModel : int {
	None,
	Chicken = 5,
	Cow = 6,
	Pig = 8,
	Sheep = 9,
	Wolf = 11,
	Villager = 12,
	Mushroom_Cow = 7,
	Squid = 36,
	Rabbit = 46,
	Bat = 10,
	Iron_Golem = 42,
	Snow_Golem = 44,
	Ocelot = 43,
	Horse = 48,
	Zombie = 14,
	Creeper = 22,
	Skeleton = 19,
	Spider = 20,
	Pig_Zombie = 15,
	Slime = 23,
	EnderMan = 24,
	Silverfish = 21,
	Cave_Spider = 20,
	Ghast = 17,
	LavaSlime = 16,
	Blaze = 18,
	Zombie_Villager = 39,
	Witch = 47,
	Stray = 55,
	Husk = 54,
	Tripod_Camera = 49,
	Player = 27,
	Primed_Tnt = 2,
	Falling_Block = 33,
	Experience_Orb = 40,
	Painting = 32,
	Minecart = 34,
	Large_Fireball = 37,
	Boat = 35,
	Lightning_Bolt = 41,
	Small_Fireball = 38
};

enum BlockFace {
	Bottom_Face,
	Top_Face,
	North_Face,
	South_Face,
	West_Face,
	East_Face,
	No_Face // Custom enum; not actually defined in-game
};

enum HitResult {
	Hit_Block,
	Hit_Entity,
	Hit_Unknown,
	Hit_None
};

struct Vec4 {
	float x, y, z, w;
};

struct Vec3 {
	float x, y, z;
};

struct Vec2 {
	float x, y;
};

struct IntVec4 {
	int x, y, z, w;
};

struct IntVec3 {
	int x, y, z;
};

struct IntVec2 {
	int x, y;
};

struct AABB {
	Vec3 lower, upper;
};

struct Entity
{
	UInt64 vtablePointer; //0x0000
	int unkInt0; //0x0008  ?
	Vec3 pos; //0x000C  Position
	Vec3 oldPos; //0x0018  Old Position?
	Vec3 somePos; //0x0024  Old Old Position?
	Vec3 vel; //0x0030  Velocity
	Vec2 rot; //0x003C Rotation
	Vec2 oldRot; //0x0044 Rotation Old?
	Vec3 unkVec3_0; //0x004C  ?
	UInt64 dataTable; //0x0058 Data Table
	UInt64 unkPtr0; //0x0060  Ptr 2
	UInt64 unkPtr1; //0x0068  Ptr 3
	int unkInt1; //0x0070 Always 255?  
	int unkInt2; //0x0074 Always 0? 
	int chunkX; //0x0078 ChunkX  
	int chunkZ; //0x007C ChunkZ 
	Vec3 unkVec3_1; //0x0080 ? 
	int unkInt5; //0x008C 
	UInt64 level; //0x0090 Level Instance 
	int unkInt6; //0x0098 
	Vec3 color; //0x009C Color 
	Vec3 unkVec3_2; //0x00A8 ? 
	AABB aabb; //0x00B4 AABB bounding box
	Vec3 unkVec3_3; //0x00CC ? 
	Vec3 unkVec3_4; //0x00D8 ? 
	Vec2 unkVec2_0; //0x00E4 ? 
	Vec3 unkVec3_5; //0x00EC ?
	int age; //0x00F8 Age?  
	int unkInt7; //0x00FC Usually 0, Changes when entity is hurt or healed   
	int unkInt8; //0x0100 Always 300? 
	Vec3 filler0; //0x0104
	int unkInt9; //0x0110 Always MAX_INT? 
	int unkInt10; //0x0114 Always MAX_INT? 
	int unkInt11; //0x0118 Always MAX_INT? 
	UInt64 filler1; //0x011C
	int renderModelId; //0x0124 Something to do with entity type 
	UInt64 rider0; //0x0128
	UInt64 rider1;
	UInt64 rider2;
	UInt64 rider3;
	UInt64 rider4;
	UInt64 rider5;
	UInt64 filler2; //0x0158
	UInt64 filler3;
	UInt64 filler4;
	UInt64 filler5;
	UInt64 filler6;
	UInt64 filler7;
	UInt64 filler8;
	int unkInt13; //0x0190 
	int unkInt14; //0x0194 
	int renderId; //0x0198 Render Id
};

struct AttributeInstance
{
	char str[32]; // at 0x10
	float current; // at 0xB4
	float max; // at 0xB0
};

//struct Entity
//{
//	DWORD_PTR vtablePointer; //0x0000
//	DWORD unkInt0; //0x0008  ?
//	Vec3 pos; //0x000C  Position
//	Vec3 oldPos; //0x0018  Old Position?
//	Vec3 somePos; //0x0024  Old Old Position?
//	Vec3 vel; //0x0030  Velocity
//	Vec2 rot; //0x003C Rotation
//	Vec2 oldRot; //0x0044 Rotation Old?
//	Vec3 unkVec3_0; //0x004C  ?
//	DWORD_PTR dataTable; //0x0058 Data Table
//	DWORD_PTR unkPtr0; //0x0060  Ptr 2
//	DWORD_PTR unkPtr1; //0x0068  Ptr 3
//	int unkInt1; //0x0070 Always 255?  
//	int unkInt2; //0x0074 Always 0? 
//	int chunkX; //0x0078 ChunkX  
//	int chunkZ; //0x007C ChunkZ 
//	Vec3 unkVec3_1; //0x0080 ? 
//	int unkInt5; //0x008C 
//	DWORD_PTR level; //0x0090 Level Instance 
//	int unkInt6; //0x0098 
//	Vec3 color; //0x009C Color 
//	Vec3 unkVec3_2; //0x00A8 ? 
//	AABB aabb; //0x00B4 AABB bounding box
//	Vec3 unkVec3_3; //0x00CC ? 
//	Vec3 unkVec3_4; //0x00D8 ? 
//	Vec2 unkVec2_0; //0x00E4 ? 
//	Vec3 unkVec3_5; //0x00EC ?
//	int age; //0x00F8 Age?  
//	int unkInt7; //0x00FC Usually 0, Changes when entity is hurt or healed   
//	int unkInt8; //0x0100 Always 300? 
//	char pad_0x0104[0xC]; //0x0104
//	int unkInt9; //0x0110 Always MAX_INT? 
//	int unkInt10; //0x0114 Always MAX_INT? 
//	int unkInt11; //0x0118 Always MAX_INT? 
//	char pad_0x011C[0x8]; //0x011C
//	int renderModelId; //0x0124 Something to do with entity type 
//	DWORD_PTR riders[6]; //0x0128 
//	char pad_0x0158[0x38]; //0x0158
//	int unkInt13; //0x0190 
//	int unkInt14; //0x0194 
//	int renderId; //0x0198 Render Id 
//	int unkInt16; //0x019C 
//	DWORD_PTR unkPtr2; //0x01A0 
//	char pad_0x01A8[0x10]; //0x01A8
//	int unkInt17; //0x01B8 
//	int unkInt18; //0x01BC 
//	float unkFloat0; //0x01C0 
//	char pad_0x01C4[0x8]; //0x01C4
//	float unkFloat1; //0x01CC 
//	char pad_0x01D0[0x4]; //0x01D0
//	int unkInt19; //0x01D4 
//	int unkInt20; //0x01D8 
//	int unkInt21; //0x01DC 
//	DWORD_PTR unkPtr3; //0x01E0
//	char pad_0x01E8[0x8]; //0x01E8
//	DWORD_PTR region; //0x01F0 Region Instance
//	char pad_0x01F8[0x4]; //0x01F8
//	int unkInt22; //0x01FC 
//	char pad_0x0200[0x10]; //0x0200
//	int unkInt23; //0x0210 
//	int invulnTime; //0x0214 Invulnerability 
//};