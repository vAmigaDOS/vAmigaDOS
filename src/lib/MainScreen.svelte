<script lang="ts">
	import { Layer } from '$lib/types';
	import { amiga, dragItem, dragType, initialized, layer, proxy, retroShell, wasm } from '$lib/stores';

	interface Props {
		children?: import('svelte').Snippet;
	}
	let { children }: Props = $props();

	function handleDragEnter(event: DragEvent) {
		event.preventDefault();
		if (!event.dataTransfer) {
			return;
		}
	}

	function handleDragOver(event: DragEvent) {
		event.preventDefault();
	}

	function handleDragLeave(event: DragEvent) {}

	async function handleDragDrop(event: DragEvent) {
		event.preventDefault();

		// Only proceed if we've received any data
		if (!event.dataTransfer || !event.dataTransfer.items) return;

		// Only proceed if the user has dragged in a single item
		if (event.dataTransfer.items.length !== 1) return;
		let item = event.dataTransfer.items[0];

		// Only proceed if the user has dragged in a file
		if (item.kind !== 'file') return;
		const file = item.getAsFile();
		if (!file) return;

		// Get the file name here
		const filename = file.name;
		console.log("Dropped file:", filename);

		// Get the file data
		let blob = await file.arrayBuffer();
		let uint8View = new Uint8Array(blob);

		// Assume file is a File object
		const buffer = await file.arrayBuffer();
		const uint8Array = new Uint8Array(buffer);

		// Write into the WASM virtual filesystem
		let tmpName = filename;
		console.log("tmp name:", tmpName);
		console.log("WASM.FS", $wasm.FS);
		$wasm.FS.writeFile(tmpName, uint8View);
		console.log("Checking type...");

		// Check the file type
		switch ($amiga.getFileType(tmpName)) {
			case $wasm.FILETYPE_ROM:
				await handleDraggedRom(uint8View);
				return;
			case $wasm.FILETYPE_ADF:
			case $wasm.FILETYPE_EADF:
			case $wasm.FILETYPE_DMS:
			case $wasm.FILETYPE_EXE:
				handleDraggedDisk(uint8View);
				return;
			case $wasm.FILETYPE_HDF:
				handleDraggedHardDrive(uint8View);
				return;
			default:
				console.warn('Unsupported file format');
				return;
		}
	}

	async function handleDraggedRom(blob: Uint8Array) {
		$proxy.addRom(blob);
	}

	function handleDraggedDisk(blob: Uint8Array) {
		$amiga.insertDisk(blob, 0);
		$retroShell.importDf(0);
		$layer = Layer.shell;
		/*
		$dragItem = blob;
		$dragType = 'floppy';
		$layer = Layer.dropzone;
		*/
	}

	function handleDraggedHardDrive(blob: Uint8Array) {
		$amiga.attachHardDrive(blob, 0);
		$retroShell.importHd(0, 0);
		$layer = Layer.shell;
		/*
		$dragItem = blob;
		$dragType = 'harddrive';
		$layer = Layer.dropzone;
		*/
	}
</script>

<div
	class="flex h-screen w-screen flex-grow flex-col overflow-clip"
	role="region"
	ondragenter={handleDragEnter}
	ondragleave={handleDragLeave}
	ondragover={handleDragOver}
	ondrop={handleDragDrop}
>
	{#if $initialized}
		{@render children?.()}
	{/if}
</div>
