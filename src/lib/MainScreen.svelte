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

		// Get the file system entry
		const entry = item.webkitGetAsEntry?.();
		// console.log('entry: ', entry);
		if (!entry) return;

		if (entry.isDirectory) {
			await handleDraggedDirectory(item, entry);
			return;
		}

		switch ($amiga.getFileType(entry.name)) {
			case $wasm.FILETYPE_ADF:
			case $wasm.FILETYPE_EADF:
			case $wasm.FILETYPE_DMS:
			case $wasm.FILETYPE_EXE:
				await handleDraggedDisk(item, entry);
				break;

			case $wasm.FILETYPE_HDF:
				await handleDraggedHardDrive(item, entry);
				break;

			default:
				await handleDraggedFile(item, entry);
		}
	}

	async function handleDraggedDirectory(item: DataTransferItem, entry: FileSystemEntry) {
		console.log('User dropped in directory ${entry.name}');
		$retroShell.remove_all('/import');
		$wasm.FS.mkdir('/import');
		await readDirectoryEntry(entry, '/import/' + entry.name);
		$retroShell.type(`import /import/${entry.name}\n`);
	}

	async function handleDraggedFile(item: DataTransferItem, entry: FileSystemEntry) {
		console.log(`User dropped in file ${entry.name}`);
		const file = item.getAsFile();
		if (file) {
			$retroShell.remove_all('/import');
			$wasm.FS.mkdir('/import');
			await readDirectoryEntry(entry, '/import/' + entry.name);
			$retroShell.type(`import /import/${entry.name}\n`);
		}
	}

	async function handleDraggedDisk(item: DataTransferItem, entry: FileSystemEntry) {
		const file = item.getAsFile();
		if (file) {
			let buffer = await file.arrayBuffer();
			let uint8View = new Uint8Array(buffer);
			$amiga.insertDisk(uint8View, 0);
			$retroShell.importDf(0);
			$retroShell.type('info\n');
			$layer = Layer.shell;
		}
	}

	async function handleDraggedHardDrive(item: DataTransferItem, entry: FileSystemEntry) {
		const file = item.getAsFile();
		if (file) {
			let buffer = await file.arrayBuffer();
			let uint8View = new Uint8Array(buffer);
			$amiga.attachHardDrive(uint8View, 0);
			$retroShell.importHd(0, 0);
			$retroShell.type('info\n');
			$layer = Layer.shell;
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
