using System;
using System.Runtime.InteropServices;

namespace MCW10StatsViewer
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct Vec3
    {
        public float x, y, z;

        public Vec3(float X, float Y, float Z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        public override string ToString()
        {
            return Math.Round(x, 3) + " / " + Math.Round(y, 3) + " / " + Math.Round(z, 3);
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct Vec2
    {
        public float x, y;

        public Vec2(float X, float Y)
        {
            x = X;
            y = Y;
        }

        public override string ToString()
        {
            return (x < 1.0E38 ? Math.Round(x, 3).ToString() : "Infinity") + " / " + (y < 1.0E38 ? Math.Round(y, 3).ToString() : "Infinity");
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct IntVec2
    {
        public int x, y;

        public IntVec2(int X, int Y)
        {
            x = X;
            y = Y;
        }

        public override string ToString()
        {
            return x + " / " + y;
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct IntVec3
    {
        public int x, y, z;

        public IntVec3(int X, int Y, int Z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        public override string ToString()
        {
            return x + " / " + y + " / " + z;
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct AABB
    {
        public Vec3 lower, upper;
        public override string ToString()
        {
            return lower + " : " + upper;
        }
    }

    public enum HitResult
    {
        Block,
        Entity,
        Unknown,
        None
    }

    public enum BlockFace
    {
        Bottom,
        Top,
        North,
        South,
        West,
        East,
        None // Custom enum; not actually defined in-game
    }

    public enum WeatherType
    {
        Clear,
        Rainstorm,
        Thunderstorm
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct Entity
    {
        public UInt64 vtablePointer; //0x0000
        public int unkInt0; //0x0008  ?
        public Vec3 pos; //0x000C  Position
        public Vec3 oldPos; //0x0018  Old Position?
        public Vec3 somePos; //0x0024  Old Old Position?
        public Vec3 vel; //0x0030  Velocity
        public Vec2 rot; //0x003C Rotation
        public Vec2 oldRot; //0x0044 Rotation Old?
        public Vec3 unkVec3_0; //0x004C  ?
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
        public Vec3 color; //0x009C Color 
        Vec3 unkVec3_2; //0x00A8 ? 
        public AABB aabb; //0x00B4 AABB bounding box
        Vec3 unkVec3_3; //0x00CC ? 
        public Vec3 unkVec3_4; //0x00D8 ? 
        Vec2 unkVec2_0; //0x00E4 ? 
        Vec3 unkVec3_5; //0x00EC ?
        public int age; //0x00F8 Age?  
        int unkInt7; //0x00FC Usually 0, Changes when entity is hurt or healed   
        int unkInt8; //0x0100 Always 300? 
        Vec3 filler0; //0x0104
        int unkInt9; //0x0110 Always MAX_INT? 
        int unkInt10; //0x0114 Always MAX_INT? 
        int unkInt11; //0x0118 Always MAX_INT? 
        UInt64 filler1; //0x011C
        public int renderModelId; //0x0124 Something to do with entity type 
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
        public int renderId; //0x0198 Render Id 
    }

    public static class EntityUtils
    {
        public static bool isMob(EntityRenderModel type)
        {
            return !(type == EntityRenderModel.None || type == EntityRenderModel.Minecart || type == EntityRenderModel.Boat || type == EntityRenderModel.Painting || type == EntityRenderModel.Falling_Block || type == EntityRenderModel.Lightning_Bolt);
        }

        public static string getAttributeDescription(string attribute)
        {
            switch(attribute)
            {
                case "Absorption": return "Extra health indicated with yellow hearts";
                case "AttackDamage": return "The base level of damage a particular entity can do";
                case "Exhaustion": return "Depletes saturation or hunger by one point every time it flips back to 0";
                case "Experience": return "Percentage indicating how much experience is needed to level-up";
                case "FollowRange": return "The max distance in which an entity will follow another entity";
                case "Health": return "Total amount of hitpoints. An entity dies when this reaches 0";
                case "Hunger": return "Amount of hunger points an entity has";
                case "JumpStrength": return "Affects how high a horse can jump";
                case "KnockbackResistance": return "Reduces knockback from being hit by a certain percentage";
                case "Level": return "What level a player is";
                case "MovementSpeed": return "How fast the entity can move (measured in blocks per game tick)";
                case "Saturation": return "Extra hunger points; hunger is only depleted when saturation is 0";
                case "SpawnReinforcements": return "Percent probability a zombie will spawn more zombies when provoked";
                default: return "";
            }
        }
    }

    public enum TimeOfDay : int
    {
        Unknown = -1,
        Day = 0,
        Sunset = 11615,
        Night = 13000,
        Sunrise = 23000,
        DayLength = 24000
    }

    public static class LevelUtils
    {
        public static TimeOfDay TimeAsEnum(long time)
        {
            time %= (long)TimeOfDay.DayLength;

            if (time < (long)TimeOfDay.Sunset)
                return TimeOfDay.Day;

            if (time < (long)TimeOfDay.Night)
                return TimeOfDay.Sunset;

            if (time < (long)TimeOfDay.Sunrise)
                return TimeOfDay.Night;

            if (time < (long)TimeOfDay.DayLength)
                return TimeOfDay.Sunrise;

            return TimeOfDay.Unknown;
        }
    }
}