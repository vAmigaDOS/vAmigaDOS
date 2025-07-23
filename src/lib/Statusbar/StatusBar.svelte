<script lang="ts">
	import { Layer } from '$lib/types';
	import FloppyDriveInfo from '$lib/Statusbar/FloppyDriveInfo.svelte';
	import HardDriveInfo from '$lib/Statusbar/HardDriveInfo.svelte';
	import Speedometer from '$lib/Statusbar/Speedometer.svelte';
	import BarBox from '$lib/Statusbar/BarBox.svelte';
	import {
		amiga,
		audio,
		dfConnected,
		dfCylinder,
		dfHasDisk,
		dfMotor,
		dfProtected,
		dfUnsaved,
		dfWriting,
		halted,
		hdConnected,
		hdCylinder,
		hdReading,
		hdUnsaved,
		hdWriting,
		layer,
		muted,
		showSidebar,
		track,
		warp
	} from '$lib/stores';
	import StatusIcon from './StatusIcon.svelte';
	import WarpControl from './WarpControl.svelte';
	import FileDialog from '$lib/Utils/FileDialog.svelte';

	/*
	interface Props {
		connected: boolean[];
	}
	let {
		connected = [false, false, false, false],
	}: Props = $props();
	*/

	// Bindings
	let speedometer: Speedometer;
	let fdialog: FileDialog;

	let muteIcon = $derived($muted || $warp);
	let debugIcon = $derived($track);
	let haltIcon = $derived($halted);
	
	const bg = 'bg-gradient-to-t from-primary to-primary/80';

	export function update(animationFrame: number, now: DOMHighResTimeStamp) {
		speedometer?.update(animationFrame, now);
	}

	function toggleSidebar(e: Event) {
		e.preventDefault();
		$audio.setup();
		$showSidebar = !$showSidebar;
		if (!$showSidebar) {
			$layer = Layer.none;
		}
	}

	function dfMenuAction(df: number, tag: number) {
        switch (tag) {
			case 0:
				fdialog.open().then(
					function (value) {
						
                        // Insert disk
						$amiga.insertDisk(value, df);

                        // Start emulation if necessary
                        if (!$amiga.running) $amiga.run();
					},
					function (error) {
						console.log('error', error);
					}
				);
				break;
			case 1:
				$amiga.ejectDisk(df);
				break;
			default:
				console.warn('Invalid menu item', tag);
		}
	}

	function hdMenuAction(df: number, tag: number) {
		switch (tag) {
			case 0:
				fdialog.open().then(
					function (value) {
						console.log('Attaching hard drive', value);
						$amiga.attachHardDrive(value, df);
					},
					function (error) {
						console.log('error', error);
					}
				);
				break;
			default:
				console.warn('Invalid menu item', tag);
		}
	}	
</script>

<FileDialog bind:this={fdialog}></FileDialog>

<div class="relative z-50 mb-1 flex h-8 {bg}">
	<BarBox>
		<button
			type="button"
			class="flex h-full w-16 justify-center"
			id="vamigaButton"
			onclick={toggleSidebar}
		>
			<img class="h-full" src="icons/vamigaIcon.png" alt="vAmiga Icon" />
		</button>
	</BarBox>
	<div class="flex grow border-0 border-red-500">
		{#each Array(4) as _, i}
			{#if $dfConnected[i]}
				<FloppyDriveInfo
                    tag={i}
					disk={$dfHasDisk[i]}
					cyl={$dfCylinder[i]}
					motor={$dfMotor[i]}
					writing={$dfWriting[i]}
					unsaved={$dfUnsaved[i]}
					wp={$dfProtected[i]}
                    select={(value: number) => dfMenuAction(i, value)}
				/>
			{/if}
		{/each}
		{#each Array(4) as _, i}
			{#if $hdConnected[i]}
				<HardDriveInfo
                    tag={i}
					cyl={$hdCylinder[i]}
					reading={$hdReading[i]}
					writing={$hdWriting[i]}
					unsaved={$hdUnsaved[i]}
                    select={(value: number) => hdMenuAction(i, value)}
				/>
			{/if}
		{/each}
	</div>
	{#if haltIcon}
		<StatusIcon src="icons/halt.png" />
	{/if}
	{#if debugIcon}
		<StatusIcon src="icons/debug.png" />
	{/if}
	{#if muteIcon}
		<StatusIcon src="icons/mute.png" />
	{/if}
	<WarpControl />
	<Speedometer bind:this={speedometer} />
</div>
