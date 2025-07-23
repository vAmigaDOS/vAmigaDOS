<svelte:options accessors={true}/>

<script lang="ts">
    import { CRC32 } from "$lib/constants";
    import type { DataBaseItem } from '$lib/types';
    import { Layer } from '$lib/types';
    import { onMount } from 'svelte';
    import {
        agnus,
        amiga,
        audio,
        audioPort,
        config,
        cpu,
        debugDma,
        denise,
        df0,
        df1,
        df2,
        df3,
        dfConnected,
        dfCylinder,
        dfHasDisk,
        dfMotor,
        dfProtected,
        dfUnsaved,
        dfWriting,
        diskController,
        enums,
        hdConnected,
        hdReading,
        hdWriting,
        hdCylinder,
        halted,
        hd0,
        hd1,
        hd2,
        hd3,
        initialized,
        joystick1,
        joystick2,
        keyboard,
        keyset1,
        keyset2,
        layer,
        memory,
        mouse1,
        mouse2,
        MsgNone,
        MsgConfig,
        MsgPower,
        MsgRun,
        MsgPause,
        MsgStep,
        MsgReset,
        MsgShutdown,
        MsgAbort,
        MsgWarp,
        MsgTrack,
        MsgMute,
        MsgPowerLedOn,
        MsgPowerLedDim,
        MsgPowerLedOff,
        MsgConsoleClose,
        MsgConsoleUpdate,
        MsgConsoleDebugger,
        MsgScriptDone,
        MsgScriptPause,
        MsgScriptAbort,
        MsgScriptWakeup,
        MsgVideoFormat,
        MsgOverclocking,
        MsgBreakpointReached,
        MsgBreakpointUpdated,
        MsgWatchpointReached,
        MsgWatchpointUpdated,
        MsgCatchpointReached,
        MsgCatchpointUpdated,
        MsgSwTrapReached,
        MsgCpuHalt,
        MsgCopperBpReached,
        MsgCopperBpUpdated,
        MsgCopperWpReached,
        MsgCopperWpUpdated,
        MsgViewport,
        MsgMemLayout,
        MsgDriveConnect,
        MsgDriveSelect,
        MsgDriveRead,
        MsgDriveWrite,
        MsgDriveLed,
        MsgDriveMotor,
        MsgDriveStep,
        MsgDrivePoll,
        MsgDiskInsert,
        MsgDiskEject,
        MsgDiskProtected,
        MsgHdcConnect,
        MsgHdcState,
        MsgHdrStep,
        MsgHdrRead,
        MsgHdrWrite,
        MsgHdrIdle,
        MsgCtrlAmigaAmiga,
        MsgShaking,
        MsgSerIn,
        MsgSerOut,
        MsgAutoSnapshotTaken,
        MsgUserSnapshotTaken,
        MsgSnapshotRestored,
        MsgRecordingStarted,
        MsgRecordingStopped,
        MsgRecordingAborted,
        MsgDmaDebug,
        MsgSrvState,
        MsgSrvReceive,
        MsgSrvSend,
        MsgAlarm,
        muted,
        paula,
        poweredOn,
        wasm,
        retroShell,
        romcrc,
        running,
        track,
        warp,
        warpMode
    } from "$lib/stores";
    import { db } from '$lib/Db/db';
    import { Opt } from "./types";

    onMount(() => {
        console.log('Proxy::onMount()');

        // Prepare to receive messages
        $wasm.processMsg = processMsg;

        onRuntimeInitialized();
    });

    export async function runShowcase(showcase: DataBaseItem)
    {
        await $audio.setup();
        try {
            console.log('Showcase:', showcase.title);
            $amiga.powerOff();
            installRoms(showcase.roms);
            $config.set(Opt.CHIP_RAM, showcase.memory[0]);
            $config.set(Opt.SLOW_RAM, showcase.memory[1]);
            $config.set(Opt.FAST_RAM, showcase.memory[2]);
            $config.set(Opt.DF1, showcase.adf.length > 1);
            for (let i = 0; i < showcase.adf.length; i++) {
                await insert(showcase.adf[i], i);
            }
            $config.set(Opt.WARP_MODE, showcase.warp);
            console.log('Launchine emulator...');
            $amiga.run();

            // Set GUI timer to manage disk changes
            if (showcase.title == 'Absolute Inebriation') {
                console.log("Scheduling disk change (inebriation)");
                // Change disk after 3000 frames
                $amiga.setAlarmAbs(3000, 1);
            }
            if (showcase.title == 'Eon') {
                console.log("Scheduling disk change (eon)");
                // Change disk after 7600 frames
                $amiga.setAlarmAbs(7600, 2);
            }

            console.log('Done');

        } catch (exception) {
            console.log('CATCHED ' + exception);
            throw exception;
        }
    }

    export function reportException()
    {
        console.error('Exception ' + $amiga.errorCode() + ': ' + $amiga.what());
    }

    export async function insert(name: string, drive: number)
    {
        try {
            console.log('Fetching adf/' + name);
            let response = await fetch('adf/' + name);
            let blob = await response.arrayBuffer();
            let uint8View = new Uint8Array(blob);
            $amiga.insertDisk(uint8View, drive);
            console.log('Disk inserted');
        } catch (exc) {
            reportException();
        }
    }

    export async function installRom(crc32: number)
    {
        if (crc32 == 0) {
            $memory.deleteRom();
            $memory.deleteExt();
            $romcrc = crc32;
            return true;
        }
        try {
            const item = await db.roms.get(crc32);
            if (!item) {
                return false;
            }

            if (item?.rom) {
                $memory.loadRom(item!.rom, item!.rom!.length);
                $romcrc = crc32;
            } else {
                $memory.deleteRom();
            }
            if (item?.ext) {
                $memory.loadExt(item!.ext, item!.ext!.length);
            } else {
                $memory.deleteExt();
            }
            return true;
        } catch (error) {
            console.log(`installRom failed: `, error);
            return false;
        }
    }

    export async function installRoms(crcs: number[])
    {
        // console.log('Installing Rom: ', crcs);
        for (const crc of crcs) {

            const success = await installRom(crc);
            if (success) return true;
        }
        return false;
    }

    export async function addRom(blob: Uint8Array) {
        let info = $memory.analyzeRom(blob, blob.byteLength);

        if (info.crc32) {
            try {
                const t = info.title;

                const id = await db.roms.add({
                    crc32: info.crc32,
                    title: info.title,
                    version: info.version,
                    released: info.released,
                    model: info.model,
                    isAros: info.vendor == $wasm.RomVendor.AROS,
                    isDiag: info.vendor == $wasm.RomVendor.DIAG,
                    isCommodore: info.vendor == $wasm.RomVendor.COMMODORE,
                    isHyperion: info.vendor == $wasm.RomVendor.HYPERION,
                    isUnknown: info.vendor == $wasm.RomVendor.UNKNOWN,
                    isPatched: info.isPatched,
                    rom: blob,
                    ext: null,
                    extStart: 0
                });

                console.log(`${t} successfully added with id ${id}`);
            } catch (error) {
                console.log(`Failed to add Kickstart`, error);
                throw error;
            }
            $layer = Layer.kickstart;
        }
    }

    export async function installAros()
    {
        await installRom(CRC32.Aros);
    }

    export async function installDiagRom()
    {
        await installRom(CRC32.DiagROM);
    }

    export function runtimeInitialized()
    {
        onRuntimeInitialized();
    }

    export function onRuntimeInitialized()
    {
        console.log('Creating proxies...');
        $agnus = new $wasm.AgnusProxy();
        $audioPort = new $wasm.AudioPortProxy();
        $cpu = new $wasm.CPUProxy();
        $denise = new $wasm.DeniseProxy();
        $df0 = new $wasm.DriveProxy(0);
        $df1 = new $wasm.DriveProxy(1);
        $df2 = new $wasm.DriveProxy(2);
        $df3 = new $wasm.DriveProxy(3);
        $diskController = new $wasm.DiskControllerProxy();
        $enums = new $wasm.EnumProxy();
        $hd0 = new $wasm.HardDriveProxy(0);
        $hd1 = new $wasm.HardDriveProxy(1);
        $hd2 = new $wasm.HardDriveProxy(2);
        $hd3 = new $wasm.HardDriveProxy(3);
        $joystick1 = new $wasm.JoystickProxy(1);
        $joystick2 = new $wasm.JoystickProxy(2);
        $keyboard = new $wasm.KeyboardProxy();
        $memory = new $wasm.MemoryProxy();
        $mouse1 = new $wasm.MouseProxy(1);
        $mouse2 = new $wasm.MouseProxy(2);
        $paula = new $wasm.PaulaProxy();
        $retroShell = new $wasm.RetroShellProxy();
        $amiga = new $wasm.AmigaProxy();

        // Start the emulator
        console.log('Launching the emulator...');
        $amiga.launch();

        // Apply some default settings
        console.log('Configuring the emulator...');
        $amiga.configure($wasm.OPT_AGNUS_REVISION, $wasm.AGNUS_ECS_2MB);

        $keyset1 = {
            ArrowLeft: $wasm.PULL_LEFT,
            ArrowRight: $wasm.PULL_RIGHT,
            ArrowUp: $wasm.PULL_UP,
            ArrowDown: $wasm.PULL_DOWN,
            ControlRight: $wasm.PRESS_FIRE,
            Space: $wasm.PRESS_FIRE
        };

        $keyset2 = {
            KeyS: $wasm.PULL_LEFT,
            KeyD: $wasm.PULL_RIGHT,
            KeyE: $wasm.PULL_UP,
            KeyX: $wasm.PULL_DOWN,
            KeyC: $wasm.RELEASE_FIRE
        };

        (async () => {
            console.log('Installing Roms...');

            // Install AROS
            const defaultRoms = [
                3304125791, // Kickstart 1.3
                2798523958, // Kickstart 1.2
                3283989056, // Kickstart 2.04
                1062194186 // Aros
            ];

            installRoms(defaultRoms);

            console.log('Initialization completed');
            $initialized = true;
        })();

        // Trigger an exception (for debugging, only)
        // $amiga.configure($wasm.OPT_AGNUS_REVISION, 42);
    }

    export function updateStateVariables()
    {
        if (!$initialized) return;

        const state = $diskController.getState();
        const sel = $diskController.getSelected();
        const write = state == $wasm.DRIVE_DMA_WRITE;

        $poweredOn = $amiga.poweredOn();
        $running = $amiga.isRunning();
        $warp = $amiga.isWarping();
        $warpMode = $config.getNum(Opt.WARP_MODE);
        $track = $amiga.isTracking();
        $muted = $paula.isMuted() || $warp;
        $halted = $amiga.isHalted();

        $dfConnected = [$df0.isConnected(), $df1.isConnected(), $df2.isConnected(), $df3.isConnected()];
        $dfHasDisk = [$df0.hasDisk(), $df1.hasDisk(), $df2.hasDisk(), $df3.hasDisk()];
        $dfMotor = [$df0.motor(), $df1.motor(), $df2.motor(), $df3.motor()];
        $dfWriting = [sel == 0 && write, sel == 1 && write, sel == 2 && write, sel == 3 && write];
        $dfUnsaved = [$df0.hasModifiedDisk(), $df1.hasModifiedDisk(), $df2.hasModifiedDisk(), $df3.hasModifiedDisk()];
        $dfProtected = [$df0.hasProtectedDisk(), $df1.hasProtectedDisk(), $df2.hasProtectedDisk(), $df3.hasProtectedDisk()];
        $dfCylinder = [$df0.currentCyl(), $df1.currentCyl(), $df2.currentCyl(), $df3.currentCyl()];

        $hdConnected = [$hd0.isConnected(), $hd1.isConnected(), $hd2.isConnected(), $hd3.isConnected()];
        $hdCylinder = [$hd0.currentCyl(), $hd1.currentCyl(), $hd2.currentCyl(), $hd3.currentCyl()];
        $hdReading = [$hd0.isReading(), $hd1.isReading(), $hd2.isReading(), $hd3.isReading()];
        $hdWriting = [$hd0.isWriting(), $hd1.isWriting(), $hd2.isWriting(), $hd3.isWriting()];
    }

    export function processMsg(msg: any)
    {
        let type = msg.type?.value;
    
        switch (type) {
            case $wasm.MSG_NONE:
                $MsgNone++;
                break;

            case $wasm.MSG_CONFIG:
                $MsgConfig++;
                $config = $config;
                updateStateVariables();
                break;

            case $wasm.MSG_POWER:
                $MsgPower++;
                updateStateVariables();
                break;

            case $wasm.MSG_RUN:
                $MsgRun++;
                updateStateVariables();
                break;

            case $wasm.MSG_PAUSE:
                $MsgPause++;
                updateStateVariables();
                break;

            case $wasm.MSG_STEP:
                $MsgStep++;
                break;

            case $wasm.MSG_RESET:
                $MsgReset++;
                updateStateVariables();
                break;

            case $wasm.MSG_SHUTDOWN:
                $MsgShutdown++;
                break;

            case $wasm.MSG_ABORT:
                $MsgAbort++;
                break;

            case $wasm.MSG_WARP:
                $MsgWarp++;
                updateStateVariables();
                break;

            case $wasm.MSG_TRACK:
                $MsgTrack++;
                updateStateVariables();
                break;

            case $wasm.MSG_MUTE:
                $MsgMute++;
                updateStateVariables();
                break;

            case $wasm.MSG_POWER_LED_ON:
                $MsgPowerLedOn++;
                break;

            case $wasm.MSG_POWER_LED_DIM:
                $MsgPowerLedDim++;
                break;

            case $wasm.MSG_POWER_LED_OFF:
                $MsgPowerLedOff++;
                break;

            case $wasm.MSG_CONSOLE_CLOSE:
                $MsgConsoleClose++;
                break;

            case $wasm.MSG_CONSOLE_UPDATE:
                $MsgConsoleUpdate++;
                break;

            case $wasm.MSG_CONSOLE_DEBUGGER:
                $MsgConsoleDebugger++;
                break;

            case $wasm.MSG_SCRIPT_DONE:
                $MsgScriptDone++;
                break;

            case $wasm.MSG_SCRIPT_PAUSE:
                $MsgScriptPause++;
                break;

            case $wasm.MSG_SCRIPT_ABORT:
                $MsgScriptAbort++;
                break;

            case $wasm.MSG_SCRIPT_WAKEUP:
                $MsgScriptWakeup++;
                break;

            case $wasm.MSG_VIDEO_FORMAT:
                $MsgVideoFormat++;
                break;

            case $wasm.MSG_OVERCLOCKING:
                $MsgOverclocking++;
                break;

            case $wasm.MSG_BREAKPOINT_REACHED:
                $MsgBreakpointReached++;
                break;

            case $wasm.MSG_BREAKPOINT_UPDATED:
                $MsgBreakpointUpdated++;
                break;

            case $wasm.MSG_WATCHPOINT_REACHED:
                $MsgWatchpointReached++;
                break;

            case $wasm.MSG_WATCHPOINT_UPDATED:
                $MsgWatchpointUpdated++;
                break;

            case $wasm.MSG_CATCHPOINT_REACHED:
                $MsgCatchpointReached++;
                break;

            case $wasm.MSG_CATCHPOINT_UPDATED:
                $MsgCatchpointUpdated++;
                break;

            case $wasm.MSG_SWTRAP_REACHED:
                $MsgSwTrapReached++;
                break;

            case $wasm.MSG_CPU_HALT:
                $MsgCpuHalt++;
                updateStateVariables();
                break;

            case $wasm.MSG_COPPERBP_REACHED:
                $MsgCopperBpReached++;
                break;

            case $wasm.MSG_COPPERBP_UPDATED:
                $MsgCopperBpUpdated++;
                break;

            case $wasm.MSG_COPPERWP_REACHED:
                $MsgCopperWpReached++;
                break;

            case $wasm.MSG_COPPERWP_UPDATED:
                $MsgCopperWpUpdated++;
                break;

            case $wasm.MSG_VIEWPORT:
                $MsgViewport++;
                break;

            case $wasm.MSG_MEM_LAYOUT:
                $MsgMemLayout++;
                break;

            case $wasm.MSG_DRIVE_CONNECT:
                $MsgDriveConnect++;
                updateStateVariables();
                break;

            case $wasm.MSG_DRIVE_SELECT:
                $MsgDriveSelect++;
                break;

            case $wasm.MSG_DRIVE_READ:
                $MsgDriveRead++;
                break;

            case $wasm.MSG_DRIVE_WRITE:
                $MsgDriveWrite++;
                updateStateVariables();
                break;

            case $wasm.MSG_DRIVE_LED:
                $MsgDriveLed++;
                break;

            case $wasm.MSG_DRIVE_MOTOR:
                $MsgDriveMotor++;
                updateStateVariables();
                break;

            case $wasm.MSG_DRIVE_STEP:
                $MsgDriveStep++;
                updateStateVariables();
                $audio.playStepSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.MSG_DRIVE_POLL:
                $MsgDrivePoll++;
                updateStateVariables();
                $audio.playStepSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.MSG_DISK_INSERT:
                $MsgDiskInsert++;
                updateStateVariables();
                $audio.playInsertSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.MSG_DISK_EJECT:
                $MsgDiskEject++;
                updateStateVariables();
                $audio.playEjectSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.MSG_DISK_PROTECTED:
                $MsgDiskProtected++;
                updateStateVariables();
                break;

            case $wasm.MSG_HDC_CONNECT:
                $MsgHdcConnect++;
                updateStateVariables();
                break;

            case $wasm.MSG_HDC_STATE:
                $MsgHdcState++;
                updateStateVariables();
                break;

            case $wasm.MSG_HDR_STEP:
                $MsgHdrStep++;
                $audio.playClickSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.MSG_HDR_READ:
                $MsgHdrRead++;
                updateStateVariables();
                break;

            case $wasm.MSG_HDR_WRITE:
                $MsgHdrWrite++;
                updateStateVariables();
                break;

            case $wasm.MSG_HDR_IDLE:
                $MsgHdrIdle++;
                updateStateVariables();
                break;

            case $wasm.MSG_CTRL_AMIGA_AMIGA:
                $MsgCtrlAmigaAmiga++;
                break;

            case $wasm.MSG_SHAKING:
                $MsgShaking++;
                break;

            case $wasm.MSG_SER_IN:
                // console.log("SER_IN: ", String.fromCharCode(d1 & 0xFF));
                $MsgSerIn++;
                break;

            case $wasm.MSG_SER_OUT:
                // console.log("SER_OUT: ", String.fromCharCode(d1 & 0xFF));
                $MsgSerOut++;
                break;

            case $wasm.MSG_AUTO_SNAPSHOT_TAKEN:
                $MsgAutoSnapshotTaken++;
                break;

            case $wasm.MSG_USER_SNAPSHOT_TAKEN:
                $MsgUserSnapshotTaken++;
                break;

            case $wasm.MSG_SNAPSHOT_RESTORED:
                $MsgSnapshotRestored++;
                updateStateVariables();
                break;

            case $wasm.MSG_RECORDING_STARTED:
                $MsgRecordingStarted++;
                break;

            case $wasm.MSG_RECORDING_STOPPED:
                $MsgRecordingStopped++;
                break;

            case $wasm.MSG_RECORDING_ABORTED:
                $MsgRecordingAborted++;
                break;

            case $wasm.MSG_DMA_DEBUG:
                $MsgDmaDebug++;
                $debugDma = msg.value;
                break;

            case $wasm.MSG_SRV_STATE:
                $MsgSrvState++;
                break;

            case $wasm.MSG_SRV_RECEIVE:
                $MsgSrvReceive++;
                break;

            case $wasm.MSG_SRV_SEND:
                $MsgSrvSend++;
                break;

            case $wasm.MSG_ALARM:
                $MsgAlarm++;
                console.log("Alarm received: ", msg.value);

                if (msg.value == 1) {
                    console.log("Inserting disk 2...");
                    insert('AbsoluteInebriation2.adf', 0);
                }
                if (msg.value == 2) {
                    console.log("Inserting disk 2...");
                    insert('Eon2.adf', 0);
                }
                break;

            default:
                break;
        }
    }
</script>
