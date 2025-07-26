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
        MsgGuardUpdated,
        MsgBreakpointReached,
        MsgWatchpointReached,
        MsgCatchpointReached,
        MsgSwTrapReached,
        MsgCpuHalt,
        MsgCopperBpReached,
        MsgCopperWpReached,
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
        MsgSnapshotTaken,
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
        warpMode,

		MsgRshExport

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

    function assertDefined<T>(value: T | undefined | null): T {
        if (value === undefined || value === null) {
            console.warn("Value is undefined or null");
            throw new Error("Value is undefined or null");
        }
        return value;
    }

    export function processMsg(msg: any)
    {
        let type = msg.type?.valueOf();

        switch (type) {
            case $wasm.Msg.NONE.valueOf():
                $MsgNone++;
                break;

            case $wasm.Msg.CONFIG.valueOf():
                $MsgConfig++;
                $config = $config;
                updateStateVariables();
                break;

            case $wasm.Msg.POWER.valueOf():
                $MsgPower++;
                updateStateVariables();
                break;

            case $wasm.Msg.RUN.valueOf():
                $MsgRun++;
                updateStateVariables();
                break;

            case $wasm.Msg.PAUSE.valueOf():
                $MsgPause++;
                updateStateVariables();
                break;

            case $wasm.Msg.STEP.valueOf():
                $MsgStep++;
                break;

            case $wasm.Msg.RESET.valueOf():
                $MsgReset++;
                updateStateVariables();
                break;

            case $wasm.Msg.SHUTDOWN.valueOf():
                $MsgShutdown++;
                break;

            case $wasm.Msg.ABORT.valueOf():
                $MsgAbort++;
                break;

            case $wasm.Msg.WARP.valueOf():
                $MsgWarp++;
                updateStateVariables();
                break;

            case $wasm.Msg.TRACK.valueOf():
                $MsgTrack++;
                updateStateVariables();
                break;

            case $wasm.Msg.MUTE.valueOf():
                $MsgMute++;
                updateStateVariables();
                break;

            case $wasm.Msg.POWER_LED_ON.valueOf():
                $MsgPowerLedOn++;
                break;

            case $wasm.Msg.POWER_LED_DIM.valueOf():
                $MsgPowerLedDim++;
                break;

            case $wasm.Msg.POWER_LED_OFF.valueOf():
                $MsgPowerLedOff++;
                break;

            case $wasm.Msg.RSH_CLOSE.valueOf():
                $MsgConsoleClose++;
                break;

            case $wasm.Msg.RSH_UPDATE.valueOf():
                $MsgConsoleUpdate++;
                break;

            /*
            case $wasm.Msg.CONSOLE_DEBUGGER.valueOf():
                $MsgConsoleDebugger++;
                break;
           
            case $wasm.Msg.SCRIPT_DONE.valueOf():
                $MsgScriptDone++;
                break;

            case $wasm.Msg.SCRIPT_PAUSE.valueOf():
                $MsgScriptPause++;
                break;

            case $wasm.Msg.SCRIPT_ABORT.valueOf():
                $MsgScriptAbort++;
                break;

            case $wasm.Msg.SCRIPT_WAKEUP.valueOf():
                $MsgScriptWakeup++;
                break;
            */

            case $wasm.Msg.VIDEO_FORMAT.valueOf():
                $MsgVideoFormat++;
                break;

            case $wasm.Msg.OVERCLOCKING.valueOf():
                $MsgOverclocking++;
                break;

            case $wasm.Msg.BREAKPOINT_REACHED.valueOf():
                $MsgBreakpointReached++;
                break;

            case $wasm.Msg.GUARD_UPDATED.valueOf():
                $MsgGuardUpdated++;
                break;

            case $wasm.Msg.WATCHPOINT_REACHED.valueOf():
                $MsgWatchpointReached++;
                break;

            case $wasm.Msg.CATCHPOINT_REACHED.valueOf():
                $MsgCatchpointReached++;
                break;

            case $wasm.Msg.SWTRAP_REACHED.valueOf():
                $MsgSwTrapReached++;
                break;

            case $wasm.Msg.CPU_HALT.valueOf():
                $MsgCpuHalt++;
                updateStateVariables();
                break;

            case $wasm.Msg.COPPERBP_REACHED.valueOf():
                $MsgCopperBpReached++;
                break;

            case $wasm.Msg.COPPERWP_REACHED.valueOf():
                $MsgCopperWpReached++;
                break;

            case $wasm.Msg.VIEWPORT.valueOf():
                $MsgViewport++;
                break;

            case $wasm.Msg.MEM_LAYOUT.valueOf():
                $MsgMemLayout++;
                break;

            case $wasm.Msg.DRIVE_CONNECT.valueOf():
                $MsgDriveConnect++;
                updateStateVariables();
                break;

            case $wasm.Msg.DRIVE_SELECT.valueOf():
                $MsgDriveSelect++;
                break;

            case $wasm.Msg.DRIVE_READ.valueOf():
                $MsgDriveRead++;
                break;

            case $wasm.Msg.DRIVE_WRITE.valueOf():
                $MsgDriveWrite++;
                updateStateVariables();
                break;

            case $wasm.Msg.DRIVE_LED.valueOf():
                $MsgDriveLed++;
                break;

            case $wasm.Msg.DRIVE_MOTOR.valueOf():
                $MsgDriveMotor++;
                updateStateVariables();
                break;

            case $wasm.Msg.DRIVE_STEP.valueOf():
                $MsgDriveStep++;
                updateStateVariables();
                $audio.playStepSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.Msg.DRIVE_POLL.valueOf():
                $MsgDrivePoll++;
                updateStateVariables();
                $audio.playStepSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.Msg.DISK_INSERT.valueOf():
                $MsgDiskInsert++;
                updateStateVariables();
                $audio.playInsertSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.Msg.DISK_EJECT.valueOf():
                $MsgDiskEject++;
                updateStateVariables();
                $audio.playEjectSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.Msg.DISK_PROTECTED.valueOf():
                $MsgDiskProtected++;
                updateStateVariables();
                break;

            case $wasm.Msg.HDC_CONNECT.valueOf():
                $MsgHdcConnect++;
                updateStateVariables();
                break;

            case $wasm.Msg.HDC_STATE.valueOf():
                $MsgHdcState++;
                updateStateVariables();
                break;

            case $wasm.Msg.HDR_STEP.valueOf():
                $MsgHdrStep++;
                $audio.playClickSound(msg.drive.volume, msg.drive.pan);
                break;

            case $wasm.Msg.HDR_READ.valueOf():
                $MsgHdrRead++;
                updateStateVariables();
                break;

            case $wasm.Msg.HDR_WRITE.valueOf():
                $MsgHdrWrite++;
                updateStateVariables();
                break;

            case $wasm.Msg.HDR_IDLE.valueOf():
                $MsgHdrIdle++;
                updateStateVariables();
                break;

            case $wasm.Msg.CTRL_AMIGA_AMIGA.valueOf():
                $MsgCtrlAmigaAmiga++;
                break;

            case $wasm.Msg.SHAKING.valueOf():
                $MsgShaking++;
                break;

            case $wasm.Msg.SER_IN.valueOf():
                // console.log("SER_IN: ", String.fromCharCode(d1 & 0xFF));
                $MsgSerIn++;
                break;

            case $wasm.Msg.SER_OUT.valueOf():
                // console.log("SER_OUT: ", String.fromCharCode(d1 & 0xFF));
                $MsgSerOut++;
                break;

            case $wasm.Msg.SNAPSHOT_TAKEN.valueOf():
                $MsgSnapshotTaken++;
                break;

            case $wasm.Msg.SNAPSHOT_RESTORED.valueOf():
                $MsgSnapshotRestored++;
                updateStateVariables();
                break;

            case $wasm.Msg.RECORDING_STARTED.valueOf():
                $MsgRecordingStarted++;
                break;

            case $wasm.Msg.RECORDING_STOPPED.valueOf():
                $MsgRecordingStopped++;
                break;

            case $wasm.Msg.RECORDING_ABORTED.valueOf():
                $MsgRecordingAborted++;
                break;

            case $wasm.Msg.DMA_DEBUG.valueOf():
                $MsgDmaDebug++;
                $debugDma = msg.value;
                break;

            case $wasm.Msg.SRV_STATE.valueOf():
                $MsgSrvState++;
                break;

            case $wasm.Msg.SRV_RECEIVE.valueOf():
                $MsgSrvReceive++;
                break;

            case $wasm.Msg.SRV_SEND.valueOf():
                $MsgSrvSend++;
                break;

            case $wasm.Msg.ALARM.valueOf():
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

            case $wasm.Msg.RSH_EXPORT.valueOf():
                $MsgRshExport++;
            default:
                break;
        }
    }
</script>
