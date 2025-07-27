<script lang="ts">
	import { Layer } from '$lib/types';
	import {
		amiga,
		dragItem,
		dragType,
		initialized,
		layer,
		proxy,
		retroShell,
		wasm
	} from '$lib/stores';

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

		console.log('kind: ', item.kind);

		const entry = item.webkitGetAsEntry?.();
		console.log('entry: ', entry);
		if (entry && entry.isDirectory) {
				console.log('User dropped in directory ${entry.name}');
				console.log('Clearing import directory...');
				$retroShell.remove_all('/import');
				$wasm.FS.mkdir('/import');
				console.log('Importing files...');
				await readDirectoryEntry(entry, '/import/' + entry.name);
				console.log('FS contents:', $wasm.FS.readdir('/import/' + entry.name));
				$retroShell.importFiles('/import', true, true);
				console.log('Done');
			return;
		} else if (entry && entry.isFile) {
			console.log('User dropped a file');
		} else {
			console.log('Unknown or unsupported item');
		}

		// Only proceed if the user has dragged in a file
		if (item.kind !== 'file') return;
		const file = item.getAsFile();
		if (!file) return;

		// Get the file name here
		const filename = file.name;
		console.log('Dropped file:', filename);

		// Get the file data
		let blob = await file.arrayBuffer();
		let uint8View = new Uint8Array(blob);
		console.log('Got file data');

		// Assume file is a File object
		// const buffer = await file.arrayBuffer();
		// const uint8Array = new Uint8Array(buffer);

		// Write into the WASM virtual filesystem
		let tmpName = filename;
		console.log('tmp name:', tmpName);
		console.log('WASM.FS', $wasm.FS);
		$wasm.FS.writeFile(tmpName, uint8View);
		console.log('Checking type...');

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

	async function readDirectoryEntry(entry: FileSystemEntry, targetPath: string) {
		if (entry.isDirectory) {
			$wasm.FS.mkdirTree(targetPath);

			const reader = (entry as FileSystemDirectoryEntry).createReader();

			const readAllEntries = async (): Promise<FileSystemEntry[]> => {
				const all: FileSystemEntry[] = [];

				// reader.readEntries returns up to 100 entries at a time
				while (true) {
					const entries: FileSystemEntry[] = await new Promise((resolve) =>
						reader.readEntries(resolve)
					);
					if (entries.length === 0) break;
					all.push(...entries);
				}

				return all;
			};

			const entries = await readAllEntries();

			for (const subEntry of entries) {
				const subPath = `${targetPath}/${subEntry.name}`;
				await readDirectoryEntry(subEntry, subPath); // recurse
			}
		} else if (entry.isFile) {
			const file = await new Promise<File>((resolve) =>
				(entry as FileSystemFileEntry).file(resolve)
			);
			await copyFileToWasm(file, targetPath);
		}
	}

	async function copyFileToWasm(file: File, targetPath: string) {
		const arrayBuffer = await file.arrayBuffer();
		const data = new Uint8Array(arrayBuffer);

		const dirPath = targetPath.substring(0, targetPath.lastIndexOf('/'));
		if (dirPath) {
			try {
				$wasm.FS.mkdirTree(dirPath);
			} catch (e) {
				// Might already exist
			}
		}

		$wasm.FS.writeFile(targetPath, data);
		console.log(`Saved ${targetPath} (${data.length} bytes)`);
	}

	async function handleDraggedRom(blob: Uint8Array) {
		$proxy.addRom(blob);
	}

	function handleDraggedDisk(blob: Uint8Array) {
		$amiga.insertDisk(blob, 0);
		$retroShell.importDf(0);
		$retroShell.type('info\n');
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
		$retroShell.type('info\n');
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
