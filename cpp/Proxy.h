#include <emscripten.h>
#include <emscripten/wasm_worker.h>
#include <emscripten/bind.h>

#include "VAmiga.h"

using namespace emscripten;
using namespace vamiga;

typedef struct
{
    bool valid;
    u32 type;
    u32 d1;
    u32 d2;
    u32 d3;
    u32 d4;
} EmuMsg;

typedef struct
{
    u32 crc32;
    string title;
    string version;
    string released;
    string model;
    RomVendor vendor;
    bool patched;
    /*
    bool isAros;
    bool isDiag;
    bool isCommodore;
    bool isHyperion;
    bool isPatched;
    */
} RomInfo;

typedef struct
{
    u32 frameNr;
    u32 data;
    bool currLof;
    bool prevLof;
} TextureWrapper;

VAmiga *amiga = nullptr;


//
// Exception handling
//

long errorCode;
string what;

void prepareTry()
{
    errorCode = 0;
    what = "";
}

void save(AppError &error)
{
    errorCode = error.data;
    what = error.what();
}

#define TRY prepareTry(); try {
#define CATCH } catch (AppError &err) { save(err); throw; }


//
// Proxies
//

struct AgnusProxy
{
    AgnusProxy() { };

    u32 frameCount() const { TRY return (u32)amiga->agnus.getInfo().frame; CATCH }
};

struct AmigaProxy
{
    AmigaProxy();

    // Handling messages
    Message readMessage();
    void lockMsgQueue() { return amiga->lockMsgQueue(); }
    void unlockMsgQueue() { return amiga->unlockMsgQueue(); }

    // Handling exceptions
    int errorCode() { return ::errorCode; }
    string what() { return ::what; }

    // Operating
    void launch() { TRY amiga->launch(); CATCH }
    void wakeUp() { TRY amiga->wakeUp(); CATCH }

    // Configuring
    void configure(int option, int value) { TRY amiga->set((Opt)option, (i64)value); CATCH }
    void configureId(int option, int value, int id) { TRY amiga->set((Opt)option, (i64)value, (i64)id); CATCH }
    int getConfig(int option) { TRY return (int)amiga->get((Opt)option); CATCH }
    int getConfigId(int option, int id) { TRY return (int)amiga->get((Opt)option, id); CATCH }

    // Querying state
    void hardReset() { TRY amiga->hardReset(); CATCH }
    void softReset() { TRY amiga->softReset(); CATCH }
    bool poweredOn() { TRY return amiga->isPoweredOn(); CATCH }
    bool poweredOff() { TRY return amiga->isPoweredOff(); CATCH }
    bool isRunning() { TRY return amiga->isRunning(); CATCH }
    bool isPaused() { TRY return amiga->isPaused(); CATCH }
    bool isHalted() { TRY return amiga->isHalted(); CATCH }
    bool isWarping() { TRY return amiga->isWarping(); CATCH }
    bool isTracking() { TRY return amiga->isTracking(); CATCH }

    // Changing state
    void powerOn() { TRY amiga->powerOn(); CATCH }
    void powerOff() { TRY amiga->powerOff(); CATCH }
    void run() { TRY amiga->run(); CATCH }
    void pause() { TRY amiga->pause(); CATCH }
    void halt() { TRY amiga->halt(); CATCH }
    void warpOn() { TRY amiga->warpOn(); CATCH }
    void warpOff() { TRY amiga->warpOff(); CATCH }

    // Analyzing
    u32 cpuLoad() { TRY return u32(amiga->getStats().cpuLoad * 100.0); CATCH }

    // Handling media files
    int getFileType(const string &blob);
    
    // Juggling disks
    bool insertDisk(const string &blob, u8 drive);
    void ejectDisk(u8 drive) { TRY amiga->df[drive]->ejectDisk(); CATCH }

    // Juggling hard drives
    bool attachHardDrive(const string &blob, u8 drive);

    // Managing alarms
    void setAlarmAbs(int frames, int payload);
    void setAlarmRel(int frames, int payload);
};

struct AudioPortProxy
{
    AudioPortProxy() { };

    util::Buffer<float> leftChannel;
    util::Buffer<float> rightChannel;

    u32 audioFillLevel() { TRY return u32(amiga->audioPort.getStats().fillLevel * 100.0); CATCH }
    void setSampleRate(unsigned sample_rate) { TRY amiga->set(Opt::HOST_SAMPLE_RATE, sample_rate); CATCH }
    void updateAudio(int offset);
    u32 leftChannelBuffer();
    u32 rightChannelBuffer();
};

struct CPUProxy
{
    CPUProxy() { };

    u32 getClock() const { TRY return (u32)amiga->cpu.getInfo().clock; CATCH }
};

struct DeniseProxy
{
    DeniseProxy() { };

    // Textures
    TextureWrapper getEmulatorTexture();
};

struct DiskControllerProxy
{
    DiskControllerProxy();

    int getSelected() const { return amiga->paula.diskController.getInfo().selectedDrive; }
    int getState() const { return (int)amiga->paula.diskController.getInfo().state; }
};

struct DriveProxy
{
    int nr;

    DriveProxy(int nr) : nr(nr) { assert(nr >= 0 && nr <= 3); }

    bool isConnected() const { TRY return amiga->df[nr]->getInfo().isConnected; CATCH }
    bool hasDisk() const { TRY return amiga->df[nr]->getInfo().hasDisk; CATCH }
    bool hasModifiedDisk() const { TRY return amiga->df[nr]->getInfo().hasModifiedDisk; CATCH }
    bool hasUnmodifiedDisk() const { TRY return amiga->df[nr]->getInfo().hasUnmodifiedDisk; CATCH }
    bool hasProtectedDisk() const { TRY return amiga->df[nr]->getInfo().hasProtectedDisk; CATCH }
    bool hasUnprotectedDisk() const { TRY return amiga->df[nr]->getInfo().hasUnprotectedDisk; CATCH }
    int currentCyl() const { TRY return amiga->df[nr]->getInfo().head.cylinder; CATCH }
    bool motor() const { TRY return amiga->df[nr]->getInfo().motor; CATCH }

    void markDiskAsModified() { TRY amiga->df[nr]->setFlag(DiskFlags::MODIFIED, true); CATCH }
    void markDiskAsUnmodified() { TRY amiga->df[nr]->setFlag(DiskFlags::MODIFIED, false); CATCH }
    void toggleWriteProtection() { TRY amiga->df[nr]->setFlag(DiskFlags::PROTECTED, !amiga->df[nr]->getFlag(DiskFlags::PROTECTED)); CATCH }
};

struct EnumProxy
{
    EnumProxy() { };

    string ErrorCodeKey(int value)
    {
        printf("ErrorCodeKey\n");
        return FaultEnum::key(Fault(value));
    }
    string MsgTypeKey(int value) { return MsgEnum::key(Msg(value)); }
    string RetroShellKey(int value) { return RSKeyEnum::key(RSKey(value)); }
};

struct HardDriveProxy
{
    int nr;

    HardDriveProxy(int nr) : nr(nr) { assert(nr >= 0 && nr <= 3); }

    bool isConnected() const { TRY return amiga->hd[nr]->getInfo().isConnected; CATCH }
    bool isReading() const { TRY return amiga->hd[nr]->getInfo().state == HardDriveState::READING; CATCH }
    bool isWriting() const { TRY return amiga->hd[nr]->getInfo().state == HardDriveState::WRITING; CATCH }
    int currentCyl() const { TRY return amiga->hd[nr]->getInfo().head.cylinder; CATCH }
    int state() const { TRY return (int)amiga->hd[nr]->getInfo().state; CATCH }
};

struct JoystickProxy
{
    int joystick;

    JoystickProxy(int joystick) : joystick(joystick) { assert(joystick == 1 || joystick == 2); }
    ControlPortAPI &port() { return joystick == 1 ? amiga->controlPort1 : amiga->controlPort2; }

    void trigger(int action) { port().joystick.trigger(GamePadAction(action)); }
};

struct KeyboardProxy
{
    KeyboardProxy() { };

    void pressKey(u8 keycode) { TRY amiga->keyboard.press(keycode); CATCH }
    void releaseKey(u8 keycode) { TRY amiga->keyboard.release(keycode); CATCH }
};

struct MemoryProxy
{
    MemoryProxy() { };

    RomInfo analyzeRom(const string &blob, u32 len);

    bool hasRom() const { TRY return amiga->mem.getInfo().hasRom; CATCH }
    bool hasExt() const { TRY return amiga->mem.getInfo().hasExt; CATCH }

    bool loadRom(const string &blob, u32 len);
    bool loadExt(const string &blob, u32 len);

    void deleteRom() { TRY amiga->mem.deleteRom(); CATCH }
    void deleteWom() { TRY amiga->mem.deleteWom(); CATCH }
    void deleteExt() { TRY amiga->mem.deleteExt(); CATCH }

    u32 romFingerprint() const { TRY return amiga->mem.getRomTraits().crc; CATCH }
};

struct MouseProxy
{
    int mouse;

    MouseProxy(int mouse) : mouse(mouse) { assert(mouse == 1 || mouse == 2); }
    ControlPortAPI &port() { return mouse == 1 ? amiga->controlPort1 : amiga->controlPort2; }

    bool detectShakeAbs(double x, double y) { return port().mouse.detectShakeXY(x, y); }
    bool detectShakeRel(double x, double y) { return port().mouse.detectShakeDxDy(x, y); }
    void setXY(double x, double y) { port().mouse.setXY(x, y); }
    void setDxDy(double x, double y) { port().mouse.setDxDy(x, y); }
    void trigger(int action) { port().mouse.trigger((GamePadAction)action); }
};

struct PaulaProxy
{
    PaulaProxy() { };

    bool isMuted() const { TRY return amiga->audioPort.getInfo().isMuted; CATCH };
};

struct RetroShellProxy
{
    RetroShellProxy() { };

    string getText() { TRY return amiga->retroShell.text(); CATCH };
    int getCursorRel() { TRY return (int)amiga->retroShell.getInfo().cursorRel; CATCH }
    void press(RSKey key) { TRY amiga->retroShell.press(key); CATCH }
    void pressKey(char c) { TRY amiga->retroShell.press(c); CATCH }
    void pressUp() { TRY amiga->retroShell.press(RSKey::UP); CATCH }
    void pressDown() { TRY amiga->retroShell.press(RSKey::DOWN); CATCH }
    void pressLeft() { TRY amiga->retroShell.press(RSKey::LEFT); CATCH }
    void pressRight() { TRY amiga->retroShell.press(RSKey::RIGHT); CATCH }
    void pressHome() { TRY amiga->retroShell.press(RSKey::HOME); CATCH }
    void pressEnd() { TRY amiga->retroShell.press(RSKey::END); CATCH }
    void pressBackspace() { TRY amiga->retroShell.press(RSKey::BACKSPACE); CATCH }
    void pressDelete() { TRY amiga->retroShell.press(RSKey::DEL); CATCH }
    void pressCut() { TRY amiga->retroShell.press(RSKey::CUT); CATCH }
    void pressReturn() { TRY amiga->retroShell.press(RSKey::RETURN); CATCH }
    void pressTab() { TRY amiga->retroShell.press(RSKey::TAB); CATCH }
    void pressShiftReturn() { TRY amiga->retroShell.press(RSKey::RETURN, true); CATCH }
};