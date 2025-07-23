#include "Proxy.h"
#include "MediaFile.h"
#include "Memory.h"
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <exception>

//
// Enum classes
//

EMSCRIPTEN_BINDINGS(EnumClasses)
{

    enum_<Msg>("Msg")
        .value("NONE", Msg::NONE)
        .value("CONFIG", Msg::CONFIG)
        .value("POWER", Msg::POWER)
        .value("RUN", Msg::RUN)
        .value("PAUSE", Msg::PAUSE)
        .value("STEP", Msg::STEP)
        .value("RESET", Msg::RESET)
        .value("SHUTDOWN", Msg::SHUTDOWN)
        .value("ABORT", Msg::ABORT)
        .value("WARP", Msg::WARP)
        .value("TRACK", Msg::TRACK)
        .value("MUTE", Msg::MUTE)
        .value("EASTER_EGG", Msg::EASTER_EGG)
        .value("POWER_LED_ON", Msg::POWER_LED_ON)
        .value("POWER_LED_DIM", Msg::POWER_LED_DIM)
        .value("POWER_LED_OFF", Msg::POWER_LED_OFF)
        .value("RSH_CLOSE", Msg::RSH_CLOSE)
        .value("RSH_UPDATE", Msg::RSH_UPDATE)
        .value("RSH_SWITCH", Msg::RSH_SWITCH)
        .value("RSH_WAIT", Msg::RSH_WAIT)
        .value("RSH_ERROR", Msg::RSH_ERROR)
        .value("VIDEO_FORMAT", Msg::VIDEO_FORMAT)
        .value("OVERCLOCKING", Msg::OVERCLOCKING)
        .value("GUARD_UPDATED", Msg::GUARD_UPDATED)
        .value("BREAKPOINT_REACHED", Msg::BREAKPOINT_REACHED)
        .value("WATCHPOINT_REACHED", Msg::WATCHPOINT_REACHED)
        .value("CATCHPOINT_REACHED", Msg::CATCHPOINT_REACHED)
        .value("SWTRAP_REACHED", Msg::SWTRAP_REACHED)
        .value("CPU_HALT", Msg::CPU_HALT)
        .value("EOL_REACHED", Msg::EOL_REACHED)
        .value("EOF_REACHED", Msg::EOF_REACHED)
        .value("BEAMTRAP_REACHED", Msg::BEAMTRAP_REACHED)
        .value("BEAMTRAP_UPDATED", Msg::BEAMTRAP_UPDATED)
        .value("COPPERBP_REACHED", Msg::COPPERBP_REACHED)
        .value("COPPERBP_UPDATED", Msg::COPPERBP_UPDATED)
        .value("COPPERWP_REACHED", Msg::COPPERWP_REACHED)
        .value("COPPERWP_UPDATED", Msg::COPPERWP_UPDATED)
        .value("VIEWPORT", Msg::VIEWPORT)
        .value("MEM_LAYOUT", Msg::MEM_LAYOUT)
        .value("DRIVE_CONNECT", Msg::DRIVE_CONNECT)
        .value("DRIVE_SELECT", Msg::DRIVE_SELECT)
        .value("DRIVE_READ", Msg::DRIVE_READ)
        .value("DRIVE_WRITE", Msg::DRIVE_WRITE)
        .value("DRIVE_LED", Msg::DRIVE_LED)
        .value("DRIVE_MOTOR", Msg::DRIVE_MOTOR)
        .value("DRIVE_STEP", Msg::DRIVE_STEP)
        .value("DRIVE_POLL", Msg::DRIVE_POLL)
        .value("DISK_INSERT", Msg::DISK_INSERT)
        .value("DISK_EJECT", Msg::DISK_EJECT)
        .value("DISK_PROTECTED", Msg::DISK_PROTECTED)
        .value("HDC_CONNECT", Msg::HDC_CONNECT)
        .value("HDC_STATE", Msg::HDC_STATE)
        .value("HDR_STEP", Msg::HDR_STEP)
        .value("HDR_READ", Msg::HDR_READ)
        .value("HDR_WRITE", Msg::HDR_WRITE)
        .value("HDR_IDLE", Msg::HDR_IDLE)
        .value("MON_SETTING", Msg::MON_SETTING)
        .value("CTRL_AMIGA_AMIGA", Msg::CTRL_AMIGA_AMIGA)
        .value("SHAKING", Msg::SHAKING)
        .value("SER_IN", Msg::SER_IN)
        .value("SER_OUT", Msg::SER_OUT)
        .value("SNAPSHOT_TAKEN", Msg::SNAPSHOT_TAKEN)
        .value("SNAPSHOT_RESTORED", Msg::SNAPSHOT_RESTORED)
        .value("WORKSPACE_LOADED", Msg::WORKSPACE_LOADED)
        .value("WORKSPACE_SAVED", Msg::WORKSPACE_SAVED)
        .value("RECORDING_STARTED", Msg::RECORDING_STARTED)
        .value("RECORDING_STOPPED", Msg::RECORDING_STOPPED)
        .value("RECORDING_ABORTED", Msg::RECORDING_ABORTED)
        .value("DMA_DEBUG", Msg::DMA_DEBUG)
        .value("SRV_STATE", Msg::SRV_STATE)
        .value("SRV_RECEIVE", Msg::SRV_RECEIVE)
        .value("SRV_SEND", Msg::SRV_SEND)
        .value("ALARM", Msg::ALARM);

    /*
    enum_<Opt>("Opt")
        .value("HOST_REFRESH_RATE", Opt::HOST_REFRESH_RATE)
        .value("HOST_SAMPLE_RATE", Opt::HOST_SAMPLE_RATE)
        .value("HOST_FRAMEBUF_WIDTH", Opt::HOST_FRAMEBUF_WIDTH)
        .value("HOST_FRAMEBUF_HEIGHT", Opt::HOST_FRAMEBUF_HEIGHT)

        .value("AMIGA_VIDEO_FORMAT", Opt::AMIGA_VIDEO_FORMAT)
        .value("AMIGA_WARP_BOOT", Opt::AMIGA_WARP_BOOT)
        .value("AMIGA_WARP_MODE", Opt::AMIGA_WARP_MODE)
        .value("AMIGA_VSYNC", Opt::AMIGA_VSYNC)
        .value("AMIGA_SPEED_BOOST", Opt::AMIGA_SPEED_BOOST)
        .value("AMIGA_RUN_AHEAD", Opt::AMIGA_RUN_AHEAD)

        .value("AMIGA_SNAP_AUTO", Opt::AMIGA_SNAP_AUTO)
        .value("AMIGA_SNAP_DELAY", Opt::AMIGA_SNAP_DELAY)
        .value("AMIGA_SNAP_COMPRESSOR", Opt::AMIGA_SNAP_COMPRESSOR)

        .value("AMIGA_WS_COMPRESSION", Opt::AMIGA_WS_COMPRESSION)

        .value("AGNUS_REVISION", Opt::AGNUS_REVISION)
        .value("AGNUS_PTR_DROPS", Opt::AGNUS_PTR_DROPS)

        .value("DENISE_REVISION", Opt::DENISE_REVISION)
        .value("DENISE_VIEWPORT_TRACKING", Opt::DENISE_VIEWPORT_TRACKING)
        .value("DENISE_HIDDEN_SPRITES", Opt::DENISE_HIDDEN_SPRITES)
        .value("DENISE_HIDDEN_LAYERS", Opt::DENISE_HIDDEN_LAYERS)
        .value("DENISE_HIDDEN_LAYER_ALPHA", Opt::DENISE_HIDDEN_LAYER_ALPHA)
        .value("DENISE_CLX_SPR_SPR", Opt::DENISE_CLX_SPR_SPR)
        .value("DENISE_CLX_SPR_PLF", Opt::DENISE_CLX_SPR_PLF)
        .value("DENISE_CLX_PLF_PLF", Opt::DENISE_CLX_PLF_PLF)

        .value("MON_PALETTE", Opt::MON_PALETTE)
        .value("MON_BRIGHTNESS", Opt::MON_BRIGHTNESS)
        .value("MON_CONTRAST", Opt::MON_CONTRAST)
        .value("MON_SATURATION", Opt::MON_SATURATION)
        .value("MON_CENTER", Opt::MON_CENTER)
        .value("MON_HCENTER", Opt::MON_HCENTER)
        .value("MON_VCENTER", Opt::MON_VCENTER)
        .value("MON_ZOOM", Opt::MON_ZOOM)
        .value("MON_HZOOM", Opt::MON_HZOOM)
        .value("MON_VZOOM", Opt::MON_VZOOM)
        .value("MON_ENHANCER", Opt::MON_ENHANCER)
        .value("MON_UPSCALER", Opt::MON_UPSCALER)
        .value("MON_BLUR", Opt::MON_BLUR)
        .value("MON_BLUR_RADIUS", Opt::MON_BLUR_RADIUS)
        .value("MON_BLOOM", Opt::MON_BLOOM)
        .value("MON_BLOOM_RADIUS", Opt::MON_BLOOM_RADIUS)
        .value("MON_BLOOM_BRIGHTNESS", Opt::MON_BLOOM_BRIGHTNESS)
        .value("MON_SCANLINES", Opt::MON_SCANLINES)
        .value("MON_SCANLINE_BRIGHTNESS", Opt::MON_SCANLINE_BRIGHTNESS)
        .value("MON_SCANLINE_WEIGHT", Opt::MON_SCANLINE_WEIGHT)
        .value("MON_DISALIGNMENT", Opt::MON_DISALIGNMENT)
        .value("MON_DISALIGNMENT_H", Opt::MON_DISALIGNMENT_H)
        .value("MON_DISALIGNMENT_V", Opt::MON_DISALIGNMENT_V)
        .value("MON_FLICKER", Opt::MON_FLICKER)
        .value("MON_FLICKER_WEIGHT", Opt::MON_FLICKER_WEIGHT)

        .value("DMA_DEBUG_ENABLE", Opt::DMA_DEBUG_ENABLE)
        .value("DMA_DEBUG_MODE", Opt::DMA_DEBUG_MODE)
        .value("DMA_DEBUG_OPACITY", Opt::DMA_DEBUG_OPACITY)
        .value("DMA_DEBUG_CHANNEL0", Opt::DMA_DEBUG_CHANNEL0)
        .value("DMA_DEBUG_CHANNEL1", Opt::DMA_DEBUG_CHANNEL1)
        .value("DMA_DEBUG_CHANNEL2", Opt::DMA_DEBUG_CHANNEL2)
        .value("DMA_DEBUG_CHANNEL3", Opt::DMA_DEBUG_CHANNEL3)
        .value("DMA_DEBUG_CHANNEL4", Opt::DMA_DEBUG_CHANNEL4)
        .value("DMA_DEBUG_CHANNEL5", Opt::DMA_DEBUG_CHANNEL5)
        .value("DMA_DEBUG_CHANNEL6", Opt::DMA_DEBUG_CHANNEL6)
        .value("DMA_DEBUG_CHANNEL7", Opt::DMA_DEBUG_CHANNEL7)
        .value("DMA_DEBUG_COLOR0", Opt::DMA_DEBUG_COLOR0)
        .value("DMA_DEBUG_COLOR1", Opt::DMA_DEBUG_COLOR1)
        .value("DMA_DEBUG_COLOR2", Opt::DMA_DEBUG_COLOR2)
        .value("DMA_DEBUG_COLOR3", Opt::DMA_DEBUG_COLOR3)
        .value("DMA_DEBUG_COLOR4", Opt::DMA_DEBUG_COLOR4)
        .value("DMA_DEBUG_COLOR5", Opt::DMA_DEBUG_COLOR5)
        .value("DMA_DEBUG_COLOR6", Opt::DMA_DEBUG_COLOR6)
        .value("DMA_DEBUG_COLOR7", Opt::DMA_DEBUG_COLOR7)

        .value("LA_PROBE0", Opt::LA_PROBE0)
        .value("LA_PROBE1", Opt::LA_PROBE1)
        .value("LA_PROBE2", Opt::LA_PROBE2)
        .value("LA_PROBE3", Opt::LA_PROBE3)
        .value("LA_ADDR0", Opt::LA_ADDR0)
        .value("LA_ADDR1", Opt::LA_ADDR1)
        .value("LA_ADDR2", Opt::LA_ADDR2)
        .value("LA_ADDR3", Opt::LA_ADDR3)

        .value("VID_WHITE_NOISE", Opt::VID_WHITE_NOISE)

        .value("CPU_REVISION", Opt::CPU_REVISION)
        .value("CPU_DASM_REVISION", Opt::CPU_DASM_REVISION)
        .value("CPU_DASM_SYNTAX", Opt::CPU_DASM_SYNTAX)
        .value("CPU_DASM_NUMBERS", Opt::CPU_DASM_NUMBERS)
        .value("CPU_OVERCLOCKING", Opt::CPU_OVERCLOCKING)
        .value("CPU_RESET_VAL", Opt::CPU_RESET_VAL)

        .value("RTC_MODEL", Opt::RTC_MODEL)

        .value("MEM_CHIP_RAM", Opt::MEM_CHIP_RAM)
        .value("MEM_SLOW_RAM", Opt::MEM_SLOW_RAM)
        .value("MEM_FAST_RAM", Opt::MEM_FAST_RAM)
        .value("MEM_EXT_START", Opt::MEM_EXT_START)
        .value("MEM_SAVE_ROMS", Opt::MEM_SAVE_ROMS)
        .value("MEM_SLOW_RAM_DELAY", Opt::MEM_SLOW_RAM_DELAY)
        .value("MEM_SLOW_RAM_MIRROR", Opt::MEM_SLOW_RAM_MIRROR)
        .value("MEM_BANKMAP", Opt::MEM_BANKMAP)
        .value("MEM_UNMAPPING_TYPE", Opt::MEM_UNMAPPING_TYPE)
        .value("MEM_RAM_INIT_PATTERN", Opt::MEM_RAM_INIT_PATTERN)

        .value("DC_SPEED", Opt::DC_SPEED)
        .value("LOCK_DSKSYNC", Opt::DC_LOCK_DSKSYNC)
        .value("AUTO_DSKSYNC", Opt::DC_AUTO_DSKSYNC)

        .value("DRIVE_CONNECT", Opt::DRIVE_CONNECT)
        .value("DRIVE_TYPE", Opt::DRIVE_TYPE)
        .value("DRIVE_MECHANICS", Opt::DRIVE_MECHANICS)
        .value("DRIVE_RPM", Opt::DRIVE_RPM)
        .value("DRIVE_SWAP_DELAY", Opt::DRIVE_SWAP_DELAY)
        .value("DRIVE_PAN", Opt::DRIVE_PAN)
        .value("DRIVE_STEP_VOLUME", Opt::DRIVE_STEP_VOLUME)
        .value("DRIVE_POLL_VOLUME", Opt::DRIVE_POLL_VOLUME)
        .value("DRIVE_INSERT_VOLUME", Opt::DRIVE_INSERT_VOLUME)
        .value("DRIVE_EJECT_VOLUME", Opt::DRIVE_EJECT_VOLUME)

        .value("HDC_CONNECT", Opt::HDC_CONNECT)

        .value("HDR_TYPE", Opt::HDR_TYPE)
        .value("HDR_PAN", Opt::HDR_PAN)
        .value("HDR_STEP_VOLUME", Opt::HDR_STEP_VOLUME)

        .value("SER_DEVICE", Opt::SER_DEVICE)
        .value("SER_VERBOSE", Opt::SER_VERBOSE)

        .value("BLITTER_ACCURACY", Opt::BLITTER_ACCURACY)

        .value("CIA_REVISION", Opt::CIA_REVISION)
        .value("CIA_TODBUG", Opt::CIA_TODBUG)
        .value("CIA_ECLOCK_SYNCING", Opt::CIA_ECLOCK_SYNCING)
        .value("CIA_IDLE_SLEEP", Opt::CIA_IDLE_SLEEP)

        .value("KBD_ACCURACY", Opt::KBD_ACCURACY)

        .value("MOUSE_PULLUP_RESISTORS", Opt::MOUSE_PULLUP_RESISTORS)
        .value("MOUSE_HAKE_DETECTION", Opt::MOUSE_SHAKE_DETECTION)
        .value("MOUSE_MOUSE_VELOCITY", Opt::MOUSE_VELOCITY)

        .value("JOY_AUTOFIRE", Opt::JOY_AUTOFIRE)
        .value("JOY_AUTOFIRE_BURSTS", Opt::JOY_AUTOFIRE_BURSTS)
        .value("JOY_AUTOFIRE_BULLETS", Opt::JOY_AUTOFIRE_BULLETS)
        .value("JOY_AUTOFIRE_DELAY", Opt::JOY_AUTOFIRE_DELAY)

        .value("AUD_PAN0", Opt::AUD_PAN0)
        .value("AUD_PAN1", Opt::AUD_PAN1)
        .value("AUD_PAN2", Opt::AUD_PAN2)
        .value("AUD_PAN3", Opt::AUD_PAN3)
        .value("AUD_VOL0", Opt::AUD_VOL0)
        .value("AUD_VOL1", Opt::AUD_VOL1)
        .value("AUD_VOL2", Opt::AUD_VOL2)
        .value("AUD_VOL3", Opt::AUD_VOL3)
        .value("AUD_VOLL", Opt::AUD_VOLL)
        .value("AUD_VOLR", Opt::AUD_VOLR)
        .value("AUD_FILTER_TYPE", Opt::AUD_FILTER_TYPE)
        .value("AUD_BUFFER_SIZE", Opt::AUD_BUFFER_SIZE)
        .value("AUD_SAMPLING_METHOD", Opt::AUD_SAMPLING_METHOD)
        .value("AUD_ASR", Opt::AUD_ASR)
        .value("AUD_FASTPATH", Opt::AUD_FASTPATH)
        .value("DIAG_BOARD", Opt::DIAG_BOARD)
        .value("SRV_PORT", Opt::SRV_PORT)
        .value("SRV_PROTOCOL", Opt::SRV_PROTOCOL)
        .value("SRV_AUTORUN", Opt::SRV_AUTORUN)
        .value("SRV_VERBOSE", Opt::SRV_VERBOSE);
*/

    enum_<RomVendor>("RomVendor")
        .value("COMMODORE", RomVendor::COMMODORE)
        .value("AROS", RomVendor::AROS)
        .value("HYPERION", RomVendor::HYPERION)
        .value("DEMO", RomVendor::DEMO)
        .value("DIAG", RomVendor::DIAG)
        .value("EMUTOS", RomVendor::EMUTOS)
        .value("OTHER", RomVendor::OTHER);
}

//
// Structures
//

EMSCRIPTEN_BINDINGS(Structures)
{
    value_object<DriveMsg>("DriveMsg")
        .field("nr", &DriveMsg::nr)
        .field("value", &DriveMsg::value)
        .field("volume", &DriveMsg::volume)
        .field("pan", &DriveMsg::pan);

    value_object<Message>("Message")
        .field("type", &Message::type)
        .field("value", &Message::value)
        .field("value2", &Message::value2)
        .field("drive", &Message::drive);

    /*
    value_object<RomTraits>("RomTraits")
        .field("crc", &RomTraits::crc)
        .field("title", &RomTraits::title)
        .field("revision", &RomTraits::revision)
        .field("released", &RomTraits::released)
        .field("model", &RomTraits::model)
        .field("vendor", &RomTraits::vendor)
        .field("boot", &RomTraits::boot)
        .field("patched", &RomTraits::patched)
        .field("relocated", &RomTraits::relocated);
    */
    
    value_object<TextureWrapper>("TextureWrapper")
        .field("frameNr", &TextureWrapper::frameNr)
        .field("data", &TextureWrapper::data)
        .field("currLof", &TextureWrapper::currLof)
        .field("prevLof", &TextureWrapper::prevLof);

    value_object<RomInfo>("RomInfo")
        .field("crc32", &RomInfo::crc32)
        .field("title", &RomInfo::title)
        .field("version", &RomInfo::version)
        .field("released", &RomInfo::released)
        .field("model", &RomInfo::model)
        .field("vendor", &RomInfo::vendor);
    /*
    .field("isAros", &RomInfo::isAros)
    .field("isDiag", &RomInfo::isDiag)
    .field("isCommodore", &RomInfo::isCommodore)
    .field("isHyperion", &RomInfo::isHyperion)
    .field("isPatched", &RomInfo::isPatched);
    */
}

//
// Enum Proxy
//

EMSCRIPTEN_BINDINGS(EnumProxy)
{
    class_<EnumProxy>("EnumProxy")
        .constructor<>()
        .function("ErrorCodeKey", &EnumProxy::ErrorCodeKey)
        .function("MsgTypeKey", &EnumProxy::MsgTypeKey)
        .function("RetroShellKey", &EnumProxy::RetroShellKey);
}

//
// Agnus proxy
//

EMSCRIPTEN_BINDINGS(AgnusProxy)
{
    class_<AgnusProxy>("AgnusProxy")
        .constructor<>()
        .function("frameCount", &AgnusProxy::frameCount);
}

//
// Amiga Proxy
//

AmigaProxy::AmigaProxy()
{
    TRY

    amiga = new VAmiga();

    // DEPRECATED (REMOVE ASAP)
    /*
    printf("Configuring...\n");
    amiga->set(Opt::AUD_VOLL, 100);
    amiga->set(Opt::AUD_VOLR, 100);
    amiga->set(Opt::AUD_VOL, 0, 100);
    amiga->set(Opt::AUD_PAN, 0, 0);

    amiga->set(Opt::CHIP_RAM, 1024);
    amiga->set(Opt::SLOW_RAM, 512);
    amiga->set(Opt::AGNUS_REVISION, AGNUS_OCS);

    amiga->set(Opt::DRIVE_CONNECT, 1, true);
    */
    CATCH
}

Message AmigaProxy::readMessage()
{
    Message msg;

    if (!amiga->getMsg(msg))
    {
        msg.type = Msg(0);
    } else {
        // printf("Message: %s [%llx]\n", MsgEnum::key(msg.type), msg.value);
    }
    return msg;
}

int AmigaProxy::getFileType(const string &blob)
{
    TRY
    printf("AmigaProxy::getFileType(%s)\n", blob.c_str());
    return (int)MediaFile::type(blob);
    CATCH
}

bool AmigaProxy::insertDisk(const string &blob, u8 drive)
{
    TRY

    auto data = (u8 *)blob.data();
    auto length = (isize)blob.length();

    if (auto file = MediaFile::make(data, length, FileType::ADF); file) {
    
        amiga->df[drive]->insertMedia(*file, false);
        return true;
    }
    if (auto file = MediaFile::make(data, length, FileType::EADF); file) {
    
        amiga->df[drive]->insertMedia(*file, false);
        return true;
    }
    if (auto file = MediaFile::make(data, length, FileType::EXE); file) {
    
        amiga->df[drive]->insertMedia(*file, false);
        return true;
    }
    if (auto file = MediaFile::make(data, length, FileType::DMS); file) {
    
        amiga->df[drive]->insertMedia(*file, false);
        return true;
    }
    return false;

    CATCH
}

bool AmigaProxy::attachHardDrive(const string &blob, u8 drive)
{
    TRY

    auto data = (u8 *)blob.data();
    auto length = (isize)blob.length();

    if (auto file = MediaFile::make(data, length, FileType::HDF); file) {
    
        amiga->hd[drive]->attach(*file);
        return true;
    }
    return false;

    CATCH
}

void AmigaProxy::setAlarmAbs(int frames, int payload)
{
    TRY
    Cycle trigger = (Cycle)frames * PAL::CLK_FREQUENCY / 50;
    amiga->put(Cmd::ALARM_ABS, AlarmCmd{trigger, payload});
    CATCH
};

void AmigaProxy::setAlarmRel(int frames, int payload)
{
    TRY
    Cycle trigger = (Cycle)frames * PAL::CLK_FREQUENCY / 50;
    amiga->put(Cmd::ALARM_REL, AlarmCmd{trigger, payload});
    CATCH
};

EMSCRIPTEN_BINDINGS(AmigaProxy)
{
    class_<AmigaProxy>("AmigaProxy")
        .constructor<>()
        .function("readMessage", &AmigaProxy::readMessage)
        .function("lockMsgQueue", &AmigaProxy::lockMsgQueue)
        .function("unlockMsgQueue", &AmigaProxy::unlockMsgQueue)

        .function("launch", &AmigaProxy::launch)
        .function("wakeUp", &AmigaProxy::wakeUp)
        .function("errorCode", &AmigaProxy::errorCode)

        .function("what", &AmigaProxy::what)

        .function("configure", &AmigaProxy::configure)
        .function("configureId", &AmigaProxy::configureId)
        .function("getConfig", &AmigaProxy::getConfig)
        .function("getConfigId", &AmigaProxy::getConfigId)

        .function("hardReset", &AmigaProxy::hardReset)
        .function("softReset", &AmigaProxy::softReset)

        .function("poweredOn", &AmigaProxy::poweredOn)
        .function("poweredOff", &AmigaProxy::poweredOff)
        .function("isRunning", &AmigaProxy::isRunning)
        .function("isPaused", &AmigaProxy::isPaused)
        .function("isHalted", &AmigaProxy::isHalted)
        .function("isWarping", &AmigaProxy::isWarping)
        .function("isTracking", &AmigaProxy::isTracking)
        // isReady
        .function("powerOn", &AmigaProxy::powerOn)
        .function("powerOff", &AmigaProxy::powerOff)
        .function("run", &AmigaProxy::run)
        .function("pause", &AmigaProxy::pause)
        .function("halt", &AmigaProxy::halt)
        .function("warpOn", &AmigaProxy::warpOn)
        .function("warpOff", &AmigaProxy::warpOff)

        .function("cpuLoad", &AmigaProxy::cpuLoad)

        .function("getFileType", &AmigaProxy::getFileType)
        .function("insertDisk", &AmigaProxy::insertDisk)
        .function("ejectDisk", &AmigaProxy::ejectDisk)
        .function("attachHardDrive", &AmigaProxy::attachHardDrive)

        .function("setAlarmAbs", &AmigaProxy::setAlarmAbs)
        .function("setAlarmRel", &AmigaProxy::setAlarmRel);
}

//
// AudioPort proxy
// 

void AudioPortProxy::updateAudio(int offset)
{
    assert(offset == 0 || offset == leftChannel.size / 2);

    float *left = leftChannel.ptr + offset;
    float *right = rightChannel.ptr + offset;
    amiga->audioPort.copyStereo(left, right, leftChannel.size / 2);
}

u32 AudioPortProxy::leftChannelBuffer()
{
    if (leftChannel.size == 0)
        leftChannel.init(2048, 0);
    return (u32)leftChannel.ptr;
}

u32 AudioPortProxy::rightChannelBuffer()
{
    if (rightChannel.size == 0)
        rightChannel.init(2048, 0);
    return (u32)rightChannel.ptr;
}

EMSCRIPTEN_BINDINGS(AudioPortProxy)
{
    class_<AudioPortProxy>("AudioPortProxy")
        .constructor<>()
        .function("setSampleRate", &AudioPortProxy::setSampleRate)
        .function("updateAudio", &AudioPortProxy::updateAudio)
        .function("leftChannelBuffer", &AudioPortProxy::leftChannelBuffer)
        .function("rightChannelBuffer", &AudioPortProxy::rightChannelBuffer)
        .function("audioFillLevel", &AudioPortProxy::audioFillLevel);
}

//
// CPU proxy
//

EMSCRIPTEN_BINDINGS(CPUProxy)
{
    class_<CPUProxy>("CPUProxy")
        .constructor<>()
        .function("getClock", &CPUProxy::getClock);
}

//
// Denise proxy
//

TextureWrapper DeniseProxy::getEmulatorTexture()
{
    TextureWrapper result;

    isize nr;
    bool lof;
    bool prevlof;
    const u32 *data;

    // auto &buffer = amiga->denise.pixelEngine.getStableBuffer();
    data = amiga->videoPort.getTexture(&nr, &lof, &prevlof);

    /*
    result.frameNr = (u32)buffer.nr;
    result.currLof = buffer.lof;
    result.prevLof = buffer.prevlof;
    result.data = (u32)buffer.pixels.ptr;
    */
    result.frameNr = nr;
    result.currLof = lof;
    result.prevLof = prevlof;
    result.data = (u32)data;

    return result;
}

EMSCRIPTEN_BINDINGS(DeniseProxy)
{
    class_<DeniseProxy>("DeniseProxy")
        .constructor<>()
        // .function("noise", &DeniseProxy::noise)
        .function("getEmulatorTexture", &DeniseProxy::getEmulatorTexture);
}

//
// Drive Proxy
//

EMSCRIPTEN_BINDINGS(DriveProxy)
{
    class_<DriveProxy>("DriveProxy")
        .constructor<int>()
        .function("isConnected", &DriveProxy::isConnected)
        .function("hasDisk", &DriveProxy::hasDisk)
        .function("hasModifiedDisk", &DriveProxy::hasModifiedDisk)
        .function("hasUnmodifiedDisk", &DriveProxy::hasUnmodifiedDisk)
        .function("hasProtectedDisk", &DriveProxy::hasProtectedDisk)
        .function("hasUnprotectedDisk", &DriveProxy::hasUnprotectedDisk)
        .function("currentCyl", &DriveProxy::currentCyl)
        .function("motor", &DriveProxy::motor)
        .function("markDiskAsModified", &DriveProxy::markDiskAsModified)
        .function("markDiskAsUnmodified", &DriveProxy::markDiskAsUnmodified)
        .function("toggleWriteProtection", &DriveProxy::toggleWriteProtection);
}

//
// HardDrive Proxy
//

EMSCRIPTEN_BINDINGS(HardDriveProxy)
{
    class_<HardDriveProxy>("HardDriveProxy")
        .constructor<int>()
        .function("isConnected", &HardDriveProxy::isConnected)
        .function("currentCyl", &HardDriveProxy::currentCyl)
        .function("isReading", &HardDriveProxy::isReading)
        .function("isWriting", &HardDriveProxy::isWriting)
        .function("state", &HardDriveProxy::state)
        .function("state", &HardDriveProxy::state);
}

//
// Joystick proxy
//

EMSCRIPTEN_BINDINGS(JoystickProxy)
{
    class_<JoystickProxy>("JoystickProxy")
        .constructor<int>()
        .function("trigger", &JoystickProxy::trigger);
}

//
// Keyboard proxy
//

EMSCRIPTEN_BINDINGS(KeyboardProxy)
{
    class_<KeyboardProxy>("KeyboardProxy")
        .constructor<>()
        .function("pressKey", &KeyboardProxy::pressKey)
        .function("releaseKey", &KeyboardProxy::releaseKey);
}

//
// Memory proxy
//

RomInfo
MemoryProxy::analyzeRom(const string &blob, u32 len)
{
    TRY

    u32 crc32 = util::crc32((const u8 *)blob.data(), len);
    auto traits = Memory::getRomTraits(crc32);

    RomInfo info{};
    info.crc32 = traits.crc;
    info.title = traits.title;
    info.version = traits.revision;
    info.released = traits.released;
    info.model = traits.model;
    info.vendor = traits.vendor;
    info.patched = traits.patched;
    return info;

    CATCH
}

bool MemoryProxy::loadRom(const string &blob, u32 len)
{
    TRY
        amiga->mem.loadRom((u8 *)blob.c_str(), len);
    // return amiga->mem.hasRom();
    return amiga->mem.getInfo().hasRom;
    CATCH
}

bool MemoryProxy::loadExt(const string &blob, u32 len)
{
    TRY
        amiga->mem.loadExt((u8 *)blob.c_str(), len);
    // return amiga->mem.hasExt();
    return amiga->mem.getInfo().hasExt;
    CATCH
}

EMSCRIPTEN_BINDINGS(MemoryProxy)
{
    class_<MemoryProxy>("MemoryProxy")
        .constructor<>()
        .function("analyzeRom", &MemoryProxy::analyzeRom)
        .function("loadRom", &MemoryProxy::loadRom)
        .function("loadExt", &MemoryProxy::loadExt)
        .property("hasRom", &MemoryProxy::hasRom)
        .property("hasExt", &MemoryProxy::hasExt)
        .function("deleteRom", &MemoryProxy::deleteRom)
        .function("deleteWom", &MemoryProxy::deleteWom)
        .function("deleteExt", &MemoryProxy::deleteExt)
        .function("romFingerprint", &MemoryProxy::romFingerprint);
}

//
// Mouse proxy
//

EMSCRIPTEN_BINDINGS(MouseProxy)
{
    class_<MouseProxy>("MouseProxy")
        .constructor<int>()
        .function("detectShakeAbs", &MouseProxy::detectShakeAbs)
        .function("detectShakeRel", &MouseProxy::detectShakeRel)
        .function("setXY", &MouseProxy::setXY)
        .function("setDxDy", &MouseProxy::setDxDy)
        .function("trigger", &MouseProxy::trigger);
}

//
// Disk controller proxy
//

DiskControllerProxy::DiskControllerProxy()
{
}

EMSCRIPTEN_BINDINGS(DiskControllerProxy)
{
    class_<DiskControllerProxy>("DiskControllerProxy")
        .constructor<>()
        .function("getSelected", &DiskControllerProxy::getSelected)
        .function("getState", &DiskControllerProxy::getState);
}

//
// Paula proxy
//

EMSCRIPTEN_BINDINGS(PaulaProxy)
{
    class_<PaulaProxy>("PaulaProxy")
        .constructor<>()
        .function("isMuted", &PaulaProxy::isMuted);
}

//
// RetroShell proxy
//

EMSCRIPTEN_BINDINGS(RetroShellProxy)
{
    class_<RetroShellProxy>("RetroShellProxy")
        .constructor<>()
        .function("getText", &RetroShellProxy::getText)
        .function("getCursorRel", &RetroShellProxy::getCursorRel)
        .function("pressUp", &RetroShellProxy::pressUp)
        .function("pressDown", &RetroShellProxy::pressDown)
        .function("pressLeft", &RetroShellProxy::pressLeft)
        .function("pressRight", &RetroShellProxy::pressRight)
        .function("pressHome", &RetroShellProxy::pressHome)
        .function("pressEnd", &RetroShellProxy::pressEnd)
        .function("pressBackspace", &RetroShellProxy::pressBackspace)
        .function("pressDelete", &RetroShellProxy::pressDelete)
        .function("pressCut", &RetroShellProxy::pressCut)
        .function("pressReturn", &RetroShellProxy::pressReturn)
        .function("pressTab", &RetroShellProxy::pressTab)
        .function("pressShiftReturn", &RetroShellProxy::pressShiftReturn)
        .function("pressKey", &RetroShellProxy::pressKey);
}

EMSCRIPTEN_BINDINGS(Constants)
{
    constant("HPIXELS", (int)HPIXELS);
    constant("VPIXELS", (int)VPIXELS);
    constant("TPP", (int)TPP);
}

EMSCRIPTEN_BINDINGS(Keys)
{
    // AgnusRevision
    constant("AGNUS_OCS_OLD", (int)AgnusRevision::OCS_OLD);
    constant("AGNUS_OCS", (int)AgnusRevision::OCS);
    constant("AGNUS_ECS_1MB", (int)AgnusRevision::ECS_1MB);
    constant("AGNUS_ECS_2MB", (int)AgnusRevision::ECS_2MB);

    // BankMap
    constant("BANK_MAP_A500", (int)BankMap::A500);
    constant("BANK_MAP_A1000", (int)BankMap::A1000);
    constant("BANK_MAP_A2000A", (int)BankMap::A2000A);
    constant("BANK_MAP_A2000B", (int)BankMap::A2000B);

    // CIARevision
    constant("CIA_MOS_8520_DIP", (int)CIARev::MOS_8520_DIP);
    constant("CIA_MOS_8520_PLCC", (int)CIARev::MOS_8520_PLCC);

    // CPURevision
    constant("CPU_68000", (int)CPURev::CPU_68000);
    constant("CPU_68010", (int)CPURev::CPU_68010);
    constant("CPU_68EC020", (int)CPURev::CPU_68EC020);

    // DeniseRevision
    constant("DENISE_OCS", (int)DeniseRev::OCS);
    constant("DENISE_ECS", (int)DeniseRev::ECS);

    // DriveMechanics
    constant("MECHANICS_NONE", (int)DriveMechanics::NONE);
    constant("MECHANICS_A1010", (int)DriveMechanics::A1010);

    // DriveState
    constant("DRIVE_DMA_OFF", (int)DriveDmaState::OFF);
    constant("DRIVE_DMA_WAIT", (int)DriveDmaState::WAIT);
    constant("DRIVE_DMA_READ", (int)DriveDmaState::READ);
    constant("DRIVE_DMA_WRITE", (int)DriveDmaState::WRITE);
    constant("DRIVE_DMA_FLUSH", (int)DriveDmaState::FLUSH);

    // FileType
    constant("FILETYPE_UNKNOWN", (int)FileType::UNKNOWN);
    constant("FILETYPE_SNAPSHOT", (int)FileType::SNAPSHOT);
    constant("FILETYPE_SCRIPT", (int)FileType::SCRIPT);
    constant("FILETYPE_ADF", (int)FileType::ADF);
    constant("FILETYPE_HDF", (int)FileType::HDF);
    constant("FILETYPE_EADF", (int)FileType::EADF);
    constant("FILETYPE_IMG", (int)FileType::IMG);
    constant("FILETYPE_DMS", (int)FileType::DMS);
    constant("FILETYPE_EXE", (int)FileType::EXE);
    constant("FILETYPE_DIR", (int)FileType::DIR);
    constant("FILETYPE_ROM", (int)FileType::ROM);

    // FloppyDriveType
    constant("DRIVE_DD_35", (int)FloppyDriveType::DD_35);
    constant("DRIVE_HD_35", (int)FloppyDriveType::HD_35);
    constant("DRIVE_DD_525", (int)FloppyDriveType::DD_525);

    // GamePad action
    constant("PULL_UP", (int)GamePadAction::PULL_UP);
    constant("PULL_DOWN", (int)GamePadAction::PULL_DOWN);
    constant("PULL_LEFT", (int)GamePadAction::PULL_LEFT);
    constant("PULL_RIGHT", (int)GamePadAction::PULL_RIGHT);
    constant("PRESS_FIRE", (int)GamePadAction::PRESS_FIRE);
    constant("PRESS_FIRE2", (int)GamePadAction::PRESS_FIRE2);
    constant("PRESS_FIRE3", (int)GamePadAction::PRESS_FIRE3);
    constant("PRESS_LEFT", (int)GamePadAction::PRESS_LEFT);
    constant("PRESS_MIDDLE", (int)GamePadAction::PRESS_MIDDLE);
    constant("PRESS_RIGHT", (int)GamePadAction::PRESS_RIGHT);
    constant("RELEASE_X", (int)GamePadAction::RELEASE_X);
    constant("RELEASE_Y", (int)GamePadAction::RELEASE_Y);
    constant("RELEASE_XY", (int)GamePadAction::RELEASE_XY);
    constant("RELEASE_FIRE", (int)GamePadAction::RELEASE_FIRE);
    constant("RELEASE_FIRE2", (int)GamePadAction::RELEASE_FIRE2);
    constant("RELEASE_FIRE3", (int)GamePadAction::RELEASE_FIRE3);
    constant("RELEASE_LEFT", (int)GamePadAction::RELEASE_LEFT);
    constant("RELEASE_MIDDLE", (int)GamePadAction::RELEASE_MIDDLE);
    constant("RELEASE_RIGHT", (int)GamePadAction::RELEASE_RIGHT);

    // Msg
    constant("MSG_NONE", (int)Msg::NONE);
    constant("MSG_CONFIG", (int)Msg::CONFIG);
    constant("MSG_POWER", (int)Msg::POWER);
    constant("MSG_RUN", (int)Msg::RUN);
    constant("MSG_PAUSE", (int)Msg::PAUSE);
    constant("MSG_STEP", (int)Msg::STEP);
    constant("MSG_RESET", (int)Msg::RESET);
    constant("MSG_SHUTDOWN", (int)Msg::SHUTDOWN);
    constant("MSG_ABORT", (int)Msg::ABORT);
    constant("MSG_WARP", (int)Msg::WARP);
    constant("MSG_TRACK", (int)Msg::TRACK);
    constant("MSG_MUTE", (int)Msg::MUTE);
    constant("MSG_EASTER_EGG", (int)Msg::EASTER_EGG);
    constant("MSG_POWER_LED_ON", (int)Msg::POWER_LED_ON);
    constant("MSG_POWER_LED_DIM", (int)Msg::POWER_LED_DIM);
    constant("MSG_POWER_LED_OFF", (int)Msg::POWER_LED_OFF);
    constant("MSG_CONSOLE_CLOSE", (int)Msg::RSH_CLOSE);
    constant("MSG_CONSOLE_UPDATE", (int)Msg::RSH_UPDATE);
    constant("MSG_CONSOLE_SWITCH", (int)Msg::RSH_SWITCH);
    constant("MSG_CONSOLE_WAIT", (int)Msg::RSH_WAIT);
    constant("MSG_CONSOLE_ERROR", (int)Msg::RSH_ERROR);
    constant("MSG_VIDEO_FORMAT", (int)Msg::VIDEO_FORMAT);
    constant("MSG_OVERCLOCKING", (int)Msg::OVERCLOCKING);
    constant("MSG_GUARD_UPDATED", (int)Msg::GUARD_UPDATED);
    constant("MSG_BREAKPOINT_REACHED", (int)Msg::BREAKPOINT_REACHED);
    constant("MSG_WATCHPOINT_REACHED", (int)Msg::WATCHPOINT_REACHED);
    constant("MSG_CATCHPOINT_REACHED", (int)Msg::CATCHPOINT_REACHED);
    constant("MSG_SWTRAP_REACHED", (int)Msg::SWTRAP_REACHED);
    constant("MSG_CPU_HALT", (int)Msg::CPU_HALT);
    constant("MSG_EOL_REACHED", (int)Msg::EOL_REACHED);
    constant("MSG_EOF_REACHED", (int)Msg::EOF_REACHED);
    constant("MSG_BEAMTRAP_REACHED", (int)Msg::BEAMTRAP_REACHED);
    constant("MSG_BEAMTRAP_UPDATED", (int)Msg::BEAMTRAP_UPDATED);
    constant("MSG_COPPERBP_REACHED", (int)Msg::COPPERBP_REACHED);
    constant("MSG_COPPERBP_UPDATED", (int)Msg::COPPERBP_UPDATED);
    constant("MSG_COPPERWP_REACHED", (int)Msg::COPPERWP_REACHED);
    constant("MSG_COPPERWP_UPDATED", (int)Msg::COPPERWP_UPDATED);
    constant("MSG_VIEWPORT", (int)Msg::VIEWPORT);
    constant("MSG_MEM_LAYOUT", (int)Msg::MEM_LAYOUT);
    constant("MSG_DRIVE_CONNECT", (int)Msg::DRIVE_CONNECT);
    constant("MSG_DRIVE_SELECT", (int)Msg::DRIVE_SELECT);
    constant("MSG_DRIVE_READ", (int)Msg::DRIVE_READ);
    constant("MSG_DRIVE_WRITE", (int)Msg::DRIVE_WRITE);
    constant("MSG_DRIVE_LED", (int)Msg::DRIVE_LED);
    constant("MSG_DRIVE_MOTOR", (int)Msg::DRIVE_MOTOR);
    constant("MSG_DRIVE_STEP", (int)Msg::DRIVE_STEP);
    constant("MSG_DRIVE_POLL", (int)Msg::DRIVE_POLL);
    constant("MSG_DISK_INSERT", (int)Msg::DISK_INSERT);
    constant("MSG_DISK_EJECT", (int)Msg::DISK_EJECT);
    constant("MSG_DISK_PROTECTED", (int)Msg::DISK_PROTECTED);
    constant("MSG_HDC_CONNECT", (int)Msg::HDC_CONNECT);
    constant("MSG_HDC_STATE", (int)Msg::HDC_STATE);
    constant("MSG_HDR_STEP", (int)Msg::HDR_STEP);
    constant("MSG_HDR_READ", (int)Msg::HDR_READ);
    constant("MSG_HDR_WRITE", (int)Msg::HDR_WRITE);
    constant("MSG_HDR_IDLE", (int)Msg::HDR_IDLE);
    constant("MSG_MON_SETTING", (int)Msg::MON_SETTING);
    constant("MSG_CTRL_AMIGA_AMIGA", (int)Msg::CTRL_AMIGA_AMIGA);
    constant("MSG_SHAKING", (int)Msg::SHAKING);
    constant("MSG_SER_IN", (int)Msg::SER_IN);
    constant("MSG_SER_OUT", (int)Msg::SER_OUT);
    constant("MSG_SNAPSHOT_TAKEN", (int)Msg::SNAPSHOT_TAKEN);
    constant("MSG_SNAPSHOT_RESTORED", (int)Msg::SNAPSHOT_RESTORED);
    constant("MSG_WORKSPACE_LOADED", (int)Msg::WORKSPACE_LOADED);
    constant("MSG_WORKSPACE_SAVED", (int)Msg::WORKSPACE_SAVED);
    constant("MSG_RECORDING_STARTED", (int)Msg::RECORDING_STARTED);
    constant("MSG_RECORDING_STOPPED", (int)Msg::RECORDING_STOPPED);
    constant("MSG_RECORDING_ABORTED", (int)Msg::RECORDING_ABORTED);
    constant("MSG_DMA_DEBUG", (int)Msg::DMA_DEBUG);
    constant("MSG_SRV_STATE", (int)Msg::SRV_STATE);
    constant("MSG_SRV_RECEIVE", (int)Msg::SRV_RECEIVE);
    constant("MSG_SRV_SEND", (int)Msg::SRV_SEND);
    constant("MSG_ALARM", (int)Msg::ALARM);

    // Options
    constant("OPT_HOST_REFRESH_RAT", (int)Opt::HOST_REFRESH_RATE);
    constant("OPT_HOST_SAMPLE_RATE", (int)Opt::HOST_SAMPLE_RATE);
    constant("OPT_HOST_FRAMEBUF_WIDTH", (int)Opt::HOST_FRAMEBUF_WIDTH);
    constant("OPT_HOST_FRAMEBUF_HEIGHT", (int)Opt::HOST_FRAMEBUF_HEIGHT);

    constant("OPT_AMIGA_VIDEO_FORMAT", (int)Opt::AMIGA_VIDEO_FORMAT);
    constant("OPT_AMIGA_WARP_BOOT", (int)Opt::AMIGA_WARP_BOOT);
    constant("OPT_AMIGA_WARP_MODE", (int)Opt::AMIGA_WARP_MODE);
    constant("OPT_AMIGA_VSYNC", (int)Opt::AMIGA_VSYNC);
    constant("OPT_AMIGA_SPEED_BOOST", (int)Opt::AMIGA_SPEED_BOOST);
    constant("OPT_AMIGA_RUN_AHEAD", (int)Opt::AMIGA_RUN_AHEAD);

    constant("OPT_AMIGA_SNAP_AUTO", (int)Opt::AMIGA_SNAP_AUTO);
    constant("OPT_AMIGA_SNAP_DELAY", (int)Opt::AMIGA_SNAP_DELAY);
    constant("OPT_AMIGA_SNAP_COMPRESSOR", (int)Opt::AMIGA_SNAP_COMPRESSOR);

    constant("OPT_AMIGA_WS_COMPRESSION", (int)Opt::AMIGA_WS_COMPRESSION);

    constant("OPT_AGNUS_REVISION", (int)Opt::AGNUS_REVISION);
    constant("OPT_AGNUS_PTR_DROPS", (int)Opt::AGNUS_PTR_DROPS);

    constant("OPT_DENISE_REVISION", (int)Opt::DENISE_REVISION);
    constant("OPT_DENISE_VIEWPORT_TRACKING", (int)Opt::DENISE_VIEWPORT_TRACKING);
    constant("OPT_DENISE_VIEWPORT_TRACKING", (int)Opt::DENISE_VIEWPORT_TRACKING);
    constant("OPT_DENISE_HIDDEN_SPRITES", (int)Opt::DENISE_HIDDEN_SPRITES);
    constant("OPT_DENISE_HIDDEN_LAYERS", (int)Opt::DENISE_HIDDEN_LAYERS);
    constant("OPT_DENISE_HIDDEN_LAYER_ALPHA", (int)Opt::DENISE_HIDDEN_LAYER_ALPHA);
    constant("OPT_DENISE_CLX_SPR_SPR", (int)Opt::DENISE_CLX_SPR_SPR);
    constant("OPT_DENISE_CLX_SPR_PLF", (int)Opt::DENISE_CLX_SPR_PLF);
    constant("OPT_DENISE_CLX_PLF_PLF", (int)Opt::DENISE_CLX_PLF_PLF);

    constant("OPT_MON_PALETTE", (int)Opt::MON_PALETTE);
    constant("OPT_MON_BRIGHTNESS", (int)Opt::MON_BRIGHTNESS);
    constant("OPT_MON_CONTRAST", (int)Opt::MON_CONTRAST);
    constant("OPT_MON_SATURATION", (int)Opt::MON_SATURATION);
    constant("OPT_MON_CENTER", (int)Opt::MON_CENTER);
    constant("OPT_MON_HCENTER", (int)Opt::MON_HCENTER);
    constant("OPT_MON_VCENTER", (int)Opt::MON_VCENTER);
    constant("OPT_MON_ZOOM", (int)Opt::MON_ZOOM);
    constant("OPT_MON_HZOOM", (int)Opt::MON_HZOOM);
    constant("OPT_MON_VZOOM", (int)Opt::MON_VZOOM);
    constant("OPT_MON_ENHANCER", (int)Opt::MON_ENHANCER);
    constant("OPT_MON_UPSCALER", (int)Opt::MON_UPSCALER);
    constant("OPT_MON_BLUR", (int)Opt::MON_BLUR);
    constant("OPT_MON_BLUR_RADIUS", (int)Opt::MON_BLUR_RADIUS);
    constant("OPT_MON_BLOOM", (int)Opt::MON_BLOOM);
    constant("OPT_MON_BLOOM_RADIUS", (int)Opt::MON_BLOOM_RADIUS);
    constant("OPT_MON_BLOOM_BRIGHTNESS", (int)Opt::MON_BLOOM_BRIGHTNESS);
    constant("OPT_MON_SCANLINES", (int)Opt::MON_SCANLINES);
    constant("OPT_MON_SCANLINES_BRIGHTNESS", (int)Opt::MON_SCANLINE_BRIGHTNESS);
    constant("OPT_MON_SCANLINES_WEIGHT", (int)Opt::MON_SCANLINE_WEIGHT);
    constant("OPT_MON_DISALIGNMENT", (int)Opt::MON_DISALIGNMENT);
    constant("OPT_MON_DISALIGNMENT_H", (int)Opt::MON_DISALIGNMENT_H);
    constant("OPT_MON_DISALIGNMENT_V", (int)Opt::MON_DISALIGNMENT_V);
    constant("OPT_MON_FLICKER", (int)Opt::MON_FLICKER);
    constant("OPT_MON_FLICKER_WEIGHT", (int)Opt::MON_FLICKER_WEIGHT);

    constant("OPT_DMA_DEBUG_ENABLE", (int)Opt::DMA_DEBUG_ENABLE);
    constant("OPT_DMA_DEBUG_MODE", (int)Opt::DMA_DEBUG_MODE);
    constant("OPT_DMA_DEBUG_OPACITY", (int)Opt::DMA_DEBUG_OPACITY);
    constant("OPT_DMA_DEBUG_CHANNEL0", (int)Opt::DMA_DEBUG_CHANNEL0);
    constant("OPT_DMA_DEBUG_CHANNEL1", (int)Opt::DMA_DEBUG_CHANNEL1);
    constant("OPT_DMA_DEBUG_CHANNEL2", (int)Opt::DMA_DEBUG_CHANNEL2);
    constant("OPT_DMA_DEBUG_CHANNEL3", (int)Opt::DMA_DEBUG_CHANNEL3);
    constant("OPT_DMA_DEBUG_CHANNEL4", (int)Opt::DMA_DEBUG_CHANNEL4);
    constant("OPT_DMA_DEBUG_CHANNEL5", (int)Opt::DMA_DEBUG_CHANNEL5);
    constant("OPT_DMA_DEBUG_CHANNEL6", (int)Opt::DMA_DEBUG_CHANNEL6);
    constant("OPT_DMA_DEBUG_CHANNEL7", (int)Opt::DMA_DEBUG_CHANNEL7);
    constant("OPT_DMA_DEBUG_COLOR0", (int)Opt::DMA_DEBUG_COLOR0);
    constant("OPT_DMA_DEBUG_COLOR1", (int)Opt::DMA_DEBUG_COLOR1);
    constant("OPT_DMA_DEBUG_COLOR2", (int)Opt::DMA_DEBUG_COLOR2);
    constant("OPT_DMA_DEBUG_COLOR3", (int)Opt::DMA_DEBUG_COLOR3);
    constant("OPT_DMA_DEBUG_COLOR4", (int)Opt::DMA_DEBUG_COLOR4);
    constant("OPT_DMA_DEBUG_COLOR5", (int)Opt::DMA_DEBUG_COLOR5);
    constant("OPT_DMA_DEBUG_COLOR6", (int)Opt::DMA_DEBUG_COLOR6);
    constant("OPT_DMA_DEBUG_COLOR7", (int)Opt::DMA_DEBUG_COLOR7);

    constant("OPT_LA_PROBE0", (int)Opt::LA_PROBE0);
    constant("OPT_LA_PROBE1", (int)Opt::LA_PROBE1);
    constant("OPT_LA_PROBE2", (int)Opt::LA_PROBE3);
    constant("OPT_LA_ADDR0", (int)Opt::LA_ADDR0);
    constant("OPT_LA_ADDR1", (int)Opt::LA_ADDR1);
    constant("OPT_LA_ADDR2", (int)Opt::LA_ADDR2);
    constant("OPT_LA_ADDR3", (int)Opt::LA_ADDR3);

    constant("OPT_VID_WHITE_NOISE", (int)Opt::VID_WHITE_NOISE);

    constant("OPT_CPU_REVISION", (int)Opt::CPU_REVISION);
    constant("OPT_CPU_DASM_REVISION", (int)Opt::CPU_DASM_REVISION);
    constant("OPT_CPU_DASM_SYNTAX", (int)Opt::CPU_DASM_SYNTAX);
    constant("OPT_CPU_DASM_NUMBERS", (int)Opt::CPU_DASM_NUMBERS);
    constant("OPT_CPU_OVERCLOCKING", (int)Opt::CPU_OVERCLOCKING);
    constant("OPT_CPU_RESET_VAL", (int)Opt::CPU_RESET_VAL);

    constant("OPT_RTC_MODEL", (int)Opt::RTC_MODEL);

    constant("OPT_MEM_CHIP_RAM", (int)Opt::MEM_CHIP_RAM);
    constant("OPT_MEM_SLOW_RAM", (int)Opt::MEM_SLOW_RAM);
    constant("OPT_MEM_FAST_RAM", (int)Opt::MEM_FAST_RAM);
    constant("OPT_MEM_EXT_START", (int)Opt::MEM_EXT_START);
    constant("OPT_MEM_SAVE_ROMS", (int)Opt::MEM_SAVE_ROMS);
    constant("OPT_MEM_SLOW_RAM_DELAY", (int)Opt::MEM_SLOW_RAM_DELAY);
    constant("OPT_MEM_SLOW_RAM_MIRROR", (int)Opt::MEM_SLOW_RAM_MIRROR);
    constant("OPT_MEM_BANKMAP", (int)Opt::MEM_BANKMAP);
    constant("OPT_MEM_UNMAPPING_TYPE", (int)Opt::MEM_UNMAPPING_TYPE);
    constant("OPT_MEM_RAM_INIT_PATTERN", (int)Opt::MEM_RAM_INIT_PATTERN);

    constant("OPT_DC_SPEED", (int)Opt::DC_SPEED);
    constant("OPT_DC_LOCK_DSKSYNC", (int)Opt::DC_LOCK_DSKSYNC);
    constant("OPT_DC_AUTO_DSKSYNC", (int)Opt::DC_AUTO_DSKSYNC);

    constant("OPT_DRIVE_CONNECT", (int)Opt::DRIVE_CONNECT);
    constant("OPT_DRIVE_TYPE", (int)Opt::DRIVE_TYPE);
    constant("OPT_DRIVE_MECHANICS", (int)Opt::DRIVE_MECHANICS);
    constant("OPT_DRIVE_RPM", (int)Opt::DRIVE_RPM);
    constant("OPT_DRIVE_SWAP_DELAY", (int)Opt::DRIVE_SWAP_DELAY);
    constant("OPT_DRIVE_PAN", (int)Opt::DRIVE_PAN);
    constant("OPT_DRIVE_STEP_VOLUME", (int)Opt::DRIVE_STEP_VOLUME);
    constant("OPT_DRIVE_POLL_VOLUME", (int)Opt::DRIVE_POLL_VOLUME);
    constant("OPT_DRIVE_INSERT_VOLUME", (int)Opt::DRIVE_INSERT_VOLUME);
    constant("OPT_DRIVE_EJECT_VOLUME", (int)Opt::DRIVE_EJECT_VOLUME);

    constant("OPT_HDC_CONNECT", (int)Opt::HDC_CONNECT);

    constant("OPT_HDR_TYPE", (int)Opt::HDR_TYPE);
    constant("OPT_HDR_PAN", (int)Opt::HDR_PAN);
    constant("OPT_HDR_STEP_VOLUME", (int)Opt::HDR_STEP_VOLUME);

    constant("OPT_SER_DEVICE", (int)Opt::SER_DEVICE);
    constant("OPT_SER_VERBOSE", (int)Opt::SER_VERBOSE);

    constant("OPT_BLITTER_ACCURACY", (int)Opt::BLITTER_ACCURACY);

    constant("OPT_CIA_REVISION", (int)Opt::CIA_REVISION);
    constant("OPT_CIA_TODBUG", (int)Opt::CIA_TODBUG);
    constant("OPT_CIA_ECLOCK_SYNCING", (int)Opt::CIA_ECLOCK_SYNCING);
    constant("OPT_CIA_IDLE_SLEEP", (int)Opt::CIA_IDLE_SLEEP);

    constant("OPT_KBD_ACCURACY", (int)Opt::KBD_ACCURACY);

    constant("OPT_MOUSE_PULLUP_RESISTORS", (int)Opt::MOUSE_PULLUP_RESISTORS);
    constant("OPT_MOUSE_HAKE_DETECTION", (int)Opt::MOUSE_SHAKE_DETECTION);
    constant("OPT_MOUSE_MOUSE_VELOCITY", (int)Opt::MOUSE_VELOCITY);

    constant("OPT_JOY_AUTOFIRE", (int)Opt::JOY_AUTOFIRE);
    constant("OPT_JOY_AUTOFIRE_BURSTS", (int)Opt::JOY_AUTOFIRE_BURSTS);
    constant("OPT_JOY_AUTOFIRE_BULLETS", (int)Opt::JOY_AUTOFIRE_BULLETS);
    constant("OPT_JOY_AUTOFIRE_DELAY", (int)Opt::JOY_AUTOFIRE_DELAY);

    constant("OPT_AUD_PAN0", (int)Opt::AUD_PAN0);
    constant("OPT_AUD_PAN1", (int)Opt::AUD_PAN1);
    constant("OPT_AUD_PAN2", (int)Opt::AUD_PAN2);
    constant("OPT_AUD_PAN3", (int)Opt::AUD_PAN3);
    constant("OPT_AUD_VOL0", (int)Opt::AUD_VOL0);
    constant("OPT_AUD_VOL1", (int)Opt::AUD_VOL1);
    constant("OPT_AUD_VOL2", (int)Opt::AUD_VOL2);
    constant("OPT_AUD_VOL3", (int)Opt::AUD_VOL3);
    constant("OPT_AUD_VOLL", (int)Opt::AUD_VOLL);
    constant("OPT_AUD_VOLR", (int)Opt::AUD_VOLR);
    constant("OPT_AUD_FILTER_TYPE", (int)Opt::AUD_FILTER_TYPE);
    constant("OPT_AUD_BUFFER_SIZE", (int)Opt::AUD_BUFFER_SIZE);
    constant("OPT_AUD_SAMPLING_METHOD", (int)Opt::AUD_SAMPLING_METHOD);
    constant("OPT_AUD_ASR", (int)Opt::AUD_ASR);
    constant("OPT_AUD_FASTPATH", (int)Opt::AUD_FASTPATH);

    constant("OPT_DIAG_BOARD", (int)Opt::DIAG_BOARD);

    constant("OPT_SRV_PORT", (int)Opt::SRV_PORT);
    constant("OPT_SRV_PROTOCOL", (int)Opt::SRV_PROTOCOL);
    constant("OPT_SRV_AUTORUN", (int)Opt::SRV_AUTORUN);
    constant("OPT_SRV_VERBOSE", (int)Opt::SRV_VERBOSE);

    // Palette
    constant("PALETTE_COLOR", (int)Palette::COLOR);
    constant("PALETTE_BLACK_WHITE", (int)Palette::BLACK_WHITE);
    constant("PALETTE_PAPER_WHITE", (int)Palette::PAPER_WHITE);
    constant("PALETTE_GREEN", (int)Palette::GREEN);
    constant("PALETTE_AMBER", (int)Palette::AMBER);
    constant("PALETTE_SEPIA", (int)Palette::SEPIA);

    // RamInitPattern
    constant("RAM_INIT_ALL_ZEROES", (int)RamInitPattern::ALL_ZEROES);
    constant("RAM_INIT_ALL_ONES", (int)RamInitPattern::ALL_ONES);
    constant("RAM_INIT_RANDOMIZED", (int)RamInitPattern::RANDOMIZED);

    // RTCRevision
    constant("RTC_NONE", (int)RTCRevision::NONE);
    constant("RTC_OKI", (int)RTCRevision::OKI);
    constant("RTC_RICOH", (int)RTCRevision::RICOH);

    // SerialPortDevice
    constant("SPD_NONE", (int)SerialPortDevice::NONE);
    constant("SPD_NULLMODEM", (int)SerialPortDevice::NULLMODEM);
    constant("SPD_LOOPBACK", (int)SerialPortDevice::LOOPBACK);

    // UnmappedMemory
    constant("UNMAPPED_FLOATING", (int)UnmappedMemory::FLOATING);
    constant("UNMAPPED_ALL_ZEROES", (int)UnmappedMemory::ALL_ZEROES);
    constant("UNMAPPED_ALL_ONES", (int)UnmappedMemory::ALL_ONES);

    // VideoFormat
    constant("TV_PAL", (int)TV::PAL);
    constant("TV_NTSC", (int)TV::NTSC);
 
};
