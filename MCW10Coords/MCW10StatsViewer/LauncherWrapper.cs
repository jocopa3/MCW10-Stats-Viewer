using System;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;
using System.Threading.Tasks;
using System.Text;

namespace MCW10StatsViewer
{ 
    public enum PACKAGE_EXECUTION_STATE
    {
        PES_UNKNOWN = 0, // The app is likely not running
        PES_RUNNING = 1, // The app is running
        PES_SUSPENDING = 2, // The app is being suspended
        PES_SUSPENDED = 3, // The app is suspended
        PES_TERMINATED = 4, // The app is terminated
        PES_ERROR = 5 // There was an error in getting the execution state
    }

    class StateChangeCallback
    {
        private delegate void Callback(int state);
        private Callback mInstance;
        private PACKAGE_EXECUTION_STATE currentState = PACKAGE_EXECUTION_STATE.PES_UNKNOWN;

        [DllImport("MCW10Backend.dll")]
        private static extern void SetStateChangeCallback(Callback fn);

        [DllImport("MCW10Backend.dll")]
        private static extern void UnregisterStateChanges();

        public StateChangeCallback(MainWindow someWindow)
        {
            mInstance = new Callback(Handler);
            SetStateChangeCallback(mInstance);
        }

        private void Handler(int state)
        {
            currentState = (PACKAGE_EXECUTION_STATE)state;
        }

        public PACKAGE_EXECUTION_STATE getState()
        {
            return currentState;
        }

        public void unregister()
        {
            UnregisterStateChanges();
        }
    }

    public partial class LauncherWrapper
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string path);

        public static void Initialize()
        {
            var path = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
            path = Path.Combine(path, Environment.Is64BitProcess ? "x64" : "x86");
            bool ok = SetDllDirectory(path);
            if (!ok) throw new System.ComponentModel.Win32Exception();
        }

        /*
         * Import functions from MCW10Backend.dll
         */

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetPlayerPos([MarshalAs(UnmanagedType.Struct)] ref Vec3 pos);

        public static void GetPlayerBlockPos(ref Vec3 pos)
        {
            GetPlayerPos(ref pos);
            pos.x = (float)Math.Floor(pos.x);
            pos.y = (float)Math.Floor(pos.y);
            pos.z = (float)Math.Floor(pos.z);
        }

        public static void GetPlayerChunkPos(ref Vec3 pos)
        {
            GetPlayerPos(ref pos);
            pos.x = (int)Math.Floor(pos.x) >> 4;
            pos.y = (int)Math.Floor(pos.y) >> 4;
            pos.z = (int)Math.Floor(pos.z) >> 4;
        }

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetPlayerLookingAt([MarshalAs(UnmanagedType.Struct)] ref Vec3 pos);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetLightValues([MarshalAs(UnmanagedType.Struct)] ref IntVec2 light);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern HitResult GetHitResultType();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern BlockFace GetHitFace();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetHitBlockPosition(ref IntVec3 blockpos);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetHitEntity(ref Entity entity);

        public static Entity entity = new Entity();

        public static void updateEntity()
        {
            GetHitEntity(ref entity);
        }

        public static Vec3 GetEntityPosition()
        {
            return entity.pos;
        }

        public static Vec3 GetEntityVelocity()
        {
            return entity.vel;
        }

        public static Vec2 GetEntityRotation()
        {
            return entity.rot;
        }
        
        public static float GetEntityPitch()
        {
            return entity.rot.x;
        }

        public static float GetEntityYaw()
        {
            return entity.rot.y;
        }
        
        public static IntVec3 GetEntityColor()
        {
            IntVec3 color = new IntVec3((int)(entity.color.x * 255), (int)(entity.color.y * 255), (int)(entity.color.z * 255));
            return color;
        }

        public static int GetEntityAge()
        {
            return entity.age;
        }

        public static AABB GetEntityAABB()
        {
            return entity.aabb;
        }

        public static EntityRenderModel GetEntityRenderModel()
        {
            return (EntityRenderModel)entity.renderModelId;
        }

        public static float GetEntityDistanceTravelled()
        {
            return entity.unkVec3_4.x;
        }

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetHitEntityAttributeName(int index, StringBuilder attributeName);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetHitEntityAttributesLength();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetHitEntityAttributValues(int index, ref Vec2 attributeValues);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern long GetLevelTime();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern long GetLevelTimeOfDay();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetLevelNameLength(); // Made an int because of issues with string builders

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool IsLevelNameUTF8();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetLevelName(ref byte arr, int len);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetLevelWeatherIntensity();

        /*
        public static WeatherType GetLevelWeather()
        {

        }
        */

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetLevelWeatherCountdown();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetLevelThunderCountdown();

        // Todo make entity class
        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetPlayerEntity(ref Entity player);

        public static Entity player = new Entity();

        public static void updatePlayer()
        {
            GetPlayerEntity(ref player);
        }

        public static Vec3 GetPlayerPosition()
        {
            return player.pos;
        }

        public static Vec3 GetPlayerBlockPosition()
        {
            Vec3 pos = player.pos;
            pos.x = (float)Math.Floor(pos.x);
            pos.y = (float)Math.Floor(pos.y);
            pos.z = (float)Math.Floor(pos.z);
            return pos;
        }

        public static Vec3 GetPlayerChunkPosition()
        {
            Vec3 pos = player.pos;
            pos.x = (int)Math.Floor(pos.x) >> 4;
            pos.y = (int)Math.Floor(pos.y) >> 4;
            pos.z = (int)Math.Floor(pos.z) >> 4;
            return pos;
        }

        public static Vec3 GetPlayerVelocity()
        {
            return player.vel;
        }

        public static Vec2 GetPlayerRotation()
        {
            return player.rot;
        }

        public static float GetPlayerPitch()
        {
            return player.rot.x;
        }

        public static float GetPlayerYaw()
        {
            return player.rot.y;
        }

        public static BlockFace GetPlayerFacingDirection()
        {
            float yaw = player.rot.y;
            yaw = yaw < 0 ? 360 - Math.Abs(yaw%360) : Math.Abs(yaw%360);
            yaw = (yaw - 45);
            yaw = yaw/90;
            switch((int)(yaw < 0 ? -1 : yaw))
            {
                case -1:
                    return BlockFace.South;
                case 0:
                    return BlockFace.West;
                case 1:
                    return BlockFace.North;
                case 2:
                    return BlockFace.East;
                case 3:
                    return BlockFace.South;
                case 4:
                    return BlockFace.West;
                default:
                    return BlockFace.None;
            }
        }

        public static IntVec3 GetPlayerColor()
        {
            IntVec3 color = new IntVec3((int)(player.color.x * 255), (int)(player.color.y * 255), (int)(player.color.z * 255));
            return color;
        }

        public static int GetPlayerAge()
        {
            return player.age;
        }

        public static AABB GetPlayerAABB()
        {
            return player.aabb;
        }

        public static EntityRenderModel GetPlayerRenderModel()
        {
            return (EntityRenderModel)player.renderModelId;
        }

        public static float GetPlayerDistanceTravelled()
        {
            return player.unkVec3_4.x;
        }

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetPlayerAttributesLength();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetPlayerAttributeName(int index, StringBuilder attributeName);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetPlayerAttributValues(int index, ref Vec2 attributeValues);

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short GetItemAmount();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short GetItemDamage();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern short GetItemId();

        public static DataValue GetItemIdEnum()
        {
            return (DataValue)GetItemId();
        }

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float GetFPS();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetEntitiesOnScreen();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool IsInLevel();

        [DllImport("MCW10Backend.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern PACKAGE_EXECUTION_STATE GetMinecraftExecutionState();

        public static string executionStateAsString(PACKAGE_EXECUTION_STATE state)
        {
            switch (state)
            {
                case PACKAGE_EXECUTION_STATE.PES_UNKNOWN:
                    return "Unknown";
                case PACKAGE_EXECUTION_STATE.PES_RUNNING:
                    return "Running";
                case PACKAGE_EXECUTION_STATE.PES_SUSPENDING:
                    return "Suspending";
                case PACKAGE_EXECUTION_STATE.PES_SUSPENDED:
                    return "Suspended";
                case PACKAGE_EXECUTION_STATE.PES_TERMINATED:
                    return "Terminated";
                case PACKAGE_EXECUTION_STATE.PES_ERROR:
                    return "Error";
                default:
                    return "Invalid State";
            }
        }
    }
}
