#include "MinecraftUtils.h"

Util::ProcessReader minecraft;

void initStuff()
{
	if (!minecraft.isOpen())
		minecraft.openProcess(L"Minecraft.Win10.DX11.exe");
}

void uninitStuff()
{
	if (minecraft.isOpen())
		minecraft.close();
}

float GetFPS()
{
	return minecraft.readFloat(FPS_OFFSET);
}

int GetEntitiesOnScreen()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1+0x18);
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x108);
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0x460);
	return minecraft.readInt(pointer4 + 0xA30);
}

void GetPlayerLookingAt(Vec3* pos)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	minecraft.readVec3(pointer2 + 0x284, pos);
}

HitResult GetHitResultType()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return (HitResult)minecraft.readInt(pointer2 + 0x270);
}

BlockFace GetHitFace()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return (BlockFace)minecraft.readInt(pointer2 + 0x274);
}

void GetHitBlockPosition(IntVec3* blockpos)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	minecraft.readIntVec3(pointer2 + 0x278, blockpos);
}

void GetHitEntity(Entity* entity)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x290);
	minecraft.readBytes(pointer3, entity, sizeof(Entity));
}

int GetHitEntityAttributesLength()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30); // Level
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x290); // Hit entity instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0xCB0); // Attributes instance
	DWORD_PTR pointer5 = minecraft.readPointer(pointer4 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer5); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter = 0;

	while (++counter < 32 && nextPointer != head)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}

	return counter - 1;
}

int GetHitEntityAttributeName(int index, char* attributeName)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30); // Level
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x290); // Hit entity instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0xCB0); // Attributes instance
	DWORD_PTR pointer5 = minecraft.readPointer(pointer4 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer5); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter;
	for (counter = 0; counter < index; counter++)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}

	int strLen = minecraft.readInt(nextPointer + 0x20);

	minecraft.readAsciiString(nextPointer + 0x10, attributeName, strLen < 32 ? strLen : 31);

	/*
	minecraft.readVec2(nextPointer + 0xB0, attributeValues);

	float min = attributeValues->y;
	attributeValues->y = attributeValues->x;
	attributeValues->x = min;
	*/

	return strLen < 32 ? strLen : 31;
}

void GetHitEntityAttributValues(int index, Vec2* attributeValues)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30); // Level
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x290); // Hit entity instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0xCB0); // Attributes instance
	DWORD_PTR pointer5 = minecraft.readPointer(pointer4 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer5); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter;
	for (counter = 0; counter < index; counter++)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}

	minecraft.readVec2(nextPointer + 0xB0, attributeValues);

	float min = attributeValues->y;
	attributeValues->y = attributeValues->x;
	attributeValues->x = min;
}

void GetHitEntityAttributes()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30); // Level
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x290); // Hit entity instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0xCB0); // Attributes instance
	DWORD_PTR pointer5 = minecraft.readPointer(pointer4 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer5); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter = 0;

	while (++counter < 32 && nextPointer != head)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}
}

long long GetLevelTime()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return minecraft.readLong(pointer2 + 0xA0);
}

long long GetLevelTimeOfDay()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return minecraft.readLong(pointer2 + 0xC4);
}

// Should be long but C# doesn't like long strings
int GetLevelNameLength() {
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return minecraft.readInt(pointer2 + 0x88);
}

BOOL IsLevelNameUTF8()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x78);

	int len = minecraft.readInt(pointer2 + 0x88);

	if (len < 16)
	{
		return minecraft.isUTF8(pointer2 + 0x78, len);
	}
	else
	{
		return minecraft.isUTF8(pointer3, len);
	}
}

void GetLevelName(char* string, int len)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);

	minecraft.readAsciiString(pointer2 + 0x78, string, len);
}

int GetLevelThunderCountdown()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return minecraft.readInt(pointer2 + 0x118);
}

int GetLevelWeatherCountdown()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return minecraft.readInt(pointer2 + 0x110);
}

float GetLevelWeatherIntensity()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x30);
	return minecraft.readFloat(pointer2 + 0x10C);
}

// Unused; was to be used to get the actual sky/block light values
short AverageValue(short a, short b, short c, short d) {
	if ((a == b && b == c && c == d))
		return a;

	if ((a == b && b == c) || (a == c && c == d) || (b == c && c == d))
		return ((a + b + c + d) >> 2) + 1;

	if (a == b || a == c || a == d || b == c || b == d || c == d)
		return ((a + b + c + d) >> 2) + 1;
}

void GetLightValues(IntVec2* light)
{
	DWORD_PTR pointer1 = minecraft.readPointer(MAGIC_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x8);
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0x10);
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0x28);
	DWORD_PTR pointer5 = minecraft.readPointer(pointer4 + 0x98);
	int lightVal = minecraft.readInt(pointer5 + 0xF4);

	light->x = lightVal & 0xFF;
	light->y = (lightVal >> 8) & 0xFF;
}

void GetPlayerEntity(Entity* player)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x38);
	minecraft.readBytes(pointer2, player, sizeof(Entity));
}

int GetPlayerAttributesLength()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x38); // Player
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0xCB0); // Attributes instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer4); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter = 0;

	while (++counter < 32 && nextPointer != head)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}

	return counter - 1;
}

int GetPlayerAttributeName(int index, char* attributeName)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x38); // Player
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0xCB0); // Attributes instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer4); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter;
	for (counter = 0; counter < index; counter++)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}

	int strLen = minecraft.readInt(nextPointer + 0x20);

	minecraft.readAsciiString(nextPointer + 0x10, attributeName, strLen < 32 ? strLen : 31);

	return strLen < 32 ? strLen : 31;
}

void GetPlayerAttributValues(int index, Vec2* attributeValues)
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET); // Static pointer
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x38); // Player
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0xCB0); // Attributes instance
	DWORD_PTR pointer4 = minecraft.readPointer(pointer3 + 0x18); // Linked list of attributes

	DWORD_PTR head = minecraft.readPointer(pointer4); // Head of linked list
	DWORD_PTR nextPointer = minecraft.readPointer(head); // Next pointer in linked list

	int counter;
	for (counter = 0; counter < index; counter++)
	{
		nextPointer = minecraft.readPointer(nextPointer);
	}

	minecraft.readVec2(nextPointer + 0xB0, attributeValues);

	float min = attributeValues->y;
	attributeValues->y = attributeValues->x;
	attributeValues->x = min;
}

short GetItemAmount()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x28);
	return minecraft.readShort(pointer2 + 0xB0);
}

short GetItemDamage()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x28);
	return minecraft.readShort(pointer2 + 0xB2);
}

short GetItemId()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR pointer2 = minecraft.readPointer(pointer1 + 0x28);
	DWORD_PTR pointer3 = minecraft.readPointer(pointer2 + 0xC0);
	return minecraft.readShort(pointer3 + 0x36);
}

bool IsInLevel()
{
	DWORD_PTR pointer1 = minecraft.readPointer(LEVEL_OFFSET);
	DWORD_PTR levelPtr = minecraft.readPointer(pointer1 + 0x30);

	// Check if level pointer is non-null
	if (levelPtr == 0)
		return false;

	levelPtr = minecraft.readPointer(levelPtr);
	if (minecraft.readLong(levelPtr) == 0)
		return false;

	DWORD_PTR playerPtr = minecraft.readPointer(pointer1 + 0x38);

	// Check if the player pointer is non-null
	if (playerPtr == 0)
		return false;

	playerPtr = minecraft.readPointer(playerPtr);
	if (minecraft.readLong(playerPtr) == 0)
		return false;

	return true;
}