<script lang="ts">
	import { Layer } from '$lib/types';
	import Speedometer from '$lib/Statusbar/Speedometer.svelte';
	import BarBox from '$lib/Statusbar/BarBox.svelte';
	import { amiga, audio, halted, layer, muted, showSidebar, track, warp } from '$lib/stores';
	import FileDialog from '$lib/Utils/FileDialog.svelte';

	// Bindings
	let speedometer: Speedometer;
	let fdialog: FileDialog;

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

	function toggleShell(e: Event) {
		$layer = $layer == Layer.shell ? Layer.none : Layer.shell;
	}

</script>

<FileDialog bind:this={fdialog}></FileDialog>

<div class="relative z-50 mb-1 flex h-8 {bg}">
	<BarBox>
		<button
			type="button"
			class="flex h-full w-16 justify-center"
			id="vamigaButton"
			onclick={toggleShell}
		>
			<img class="h-full" src="icons/vamigaIcon.png" alt="vAmiga Icon" />
		</button>
	</BarBox>
	<div class="flex grow border-0 border-red-500">
	</div>
</div>
