#pragma once

#include <stdint.h>
#include "MemoryReader.h"
#include "ZenovaLauncher.h"

/*
 * Offsets
 */

#define LEVEL_OFFSET 0xFDDBD8 // 0.15.10
#define MAGIC_OFFSET 0xFDDBA8 // 0.15.10
#define FPS_OFFSET   0xFD5E30 // 0.15.10

/*
 * Internal functions
 */

void initStuff();
void uninitStuff();

/*
 * Exported functions
 */

 /************************/
 /*   Render Functions   */
 /************************/

EXPORT float GetFPS();
EXPORT int GetEntitiesOnScreen();

/************************/
/* Hit result functions */
/************************/

// Type of object the player is looking at
EXPORT HitResult GetHitResultType();

// If looking at a block, returns the face of the block
EXPORT BlockFace GetHitFace();

// Returns the position of the block on the world grid 
EXPORT void GetHitBlockPosition(IntVec3* blockpos);

// If looking at a block, returns the exact position the player is looking
// If looking at an entity, returns that entity's position
EXPORT void GetPlayerLookingAt(Vec3* pos); 

// Reads the entity struct corresponding to the entity the player is looking at
EXPORT void GetHitEntity(Entity* entity);

// Returns how many attributes the entity the player is looking at has
EXPORT int GetHitEntityAttributesLength();

// Puts the name of the attribute at the index in attributeName and returns its length as an int
EXPORT int GetHitEntityAttributeName(int index, char* attributeName);

// Puts the current/maximum value of the attribute at the index in attributeValues
EXPORT void GetHitEntityAttributValues(int index, Vec2* attributeValues);

EXPORT void GetLightValues(IntVec2* light);

EXPORT long long GetLevelTime();
EXPORT long long GetLevelTimeOfDay();
EXPORT BOOL IsLevelNameUTF8();
EXPORT int GetLevelNameLength();
EXPORT void GetLevelName(char* string, int len);
EXPORT float GetLevelWeatherIntensity();
EXPORT int GetLevelWeatherCountdown();
EXPORT int GetLevelThunderCountdown();

// Reads an entire player struct and stores it in the Entity pointer
EXPORT void GetPlayerEntity(Entity* player);
EXPORT int GetPlayerAttributesLength();
EXPORT int GetPlayerAttributeName(int index, char* attributeName);
EXPORT void GetPlayerAttributValues(int index, Vec2* attributeValues);

EXPORT short GetItemAmount();
EXPORT short GetItemDamage();
EXPORT short GetItemId();

EXPORT bool IsInLevel();