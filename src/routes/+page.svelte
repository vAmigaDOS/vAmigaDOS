<script lang="ts">
	import JSZip from 'jszip';
	import { browser } from '$app/environment';
	import { onMount } from 'svelte';
	import { db } from '$lib/Db/db';
	import { liveQuery } from 'dexie';
	import { Layer } from '$lib/types';
	import { initialized, amiga, kickstarts, retroShell } from '$lib/stores';
	import { MsgRshExport } from '$lib/stores';
	import { wasm, proxy, audio, config, gamepadManager } from '$lib/stores';
	import { layer, what, errno } from '$lib/stores';
	import { layout, showSidebar } from '$lib/stores';
	import { canvasWidth, canvasHeight, aspectWidth, aspectHeight } from '$lib/stores';
	import { port1, port2 } from '$lib/stores';
	import '@splidejs/svelte-splide/css';
	import Sidebar from '$lib/Sidebar/Sidebar.svelte';
	import TitleScreen from '$lib/TitleScreen.svelte';
	import Emulator from '$lib/Emulator.svelte';
	import Settings from '$lib/Settings/Settings.svelte';
	import StatusBar from '$lib/Statusbar/StatusBar.svelte';
	import RetroShell from '$lib/RetroShell.svelte';
	import MainScreen from '$lib/MainScreen.svelte';
	import DropZone from '$lib/DropZone/DropZone.svelte';
	import Guru from '$lib/Guru.svelte';
	import Proxy from '$lib/Proxy.svelte';
	import Audio from '$lib/Globals/Audio.svelte';
	import Config from '$lib/Globals/Config.svelte';
	import GamepadManager from '$lib/Globals/GamepadManager.svelte';

	// Animation frame counter
	let animationFrame = 0;

	// Component references
	let emulator: Emulator;
	let canvas: Element;
	let statusBar: StatusBar;

	// Timeout for debouncing the resize event
	let timeout: number;

	// Connect to Dexie DB
	let myQuery = liveQuery(() => (browser ? db.roms.orderBy('title').toArray() : []));
	myQuery.subscribe((value) => {
		$kickstarts = value;
	});

	onMount(() => {
		console.log('+page::onMount()');

		window.addEventListener('resize', function () {
			clearTimeout(timeout);
			timeout = setTimeout(handleResizeEvent, 250);
		});

		window.addEventListener('error', handleUncatchedError);
		window.addEventListener('unhandledrejection', handleUncatchedRejection);
	});

	$effect(() => {
		if (canvas) {
			resize();
		}
	});

	$effect(() => {
		if ($initialized) {
			// Start render loop
			window.requestAnimationFrame(doAnimationFrame);
		}
	});

	function doAnimationFrame(now: DOMHighResTimeStamp) {
		animationFrame++;

		if (emulator) {
			emulator.doAnimationFrame(animationFrame, now);
		}
		if (statusBar) {
			statusBar.update(animationFrame, now);
		}

		window.requestAnimationFrame(doAnimationFrame);
	}

	function resize() {
		// console.log('resize');
		if (canvas != undefined) {
			// Copy the current size of emulator element
			$canvasWidth = canvas.clientWidth;
			$canvasHeight = canvas.clientHeight;

			// Compute the biggest possible 4:3 box
			$aspectWidth = $canvasWidth;
			$aspectHeight = ($aspectWidth * 3) / 4;
			if ($aspectHeight > $canvasHeight) {
				$aspectHeight = $canvasHeight;
				$aspectWidth = ($aspectHeight * 4) / 3;
			}

			// emulator.updateRect($layout);
			// console.log('dimensions: ', $canvasWidth, ', ', $canvasHeight);
			// console.log('aspect dimensions: ', $aspectWidth, ', ', $aspectHeight);
		}
	}

	function handleResizeEvent() {
		resize();
	}

	function handleUncatchedError(event: ErrorEvent) {
		// if (event.error instanceof WebAssembly.Exception) {
		if ($amiga.what() !== 0) {
			$what = $amiga.what();
			$errno = $amiga.errorCode();
		}
	}

	function handleUncatchedRejection(event: PromiseRejectionEvent) {
		// if (event.reason instanceof WebAssembly.Exception) {
		if ($amiga.what() !== 0) {
			$what = $amiga.what();
			$errno = $amiga.errorCode();
		}
	}

	async function saveToHost(defaultName: string, accept: string[], data: Uint8Array | string) {
		const handle = await window.showSaveFilePicker({
			suggestedName: defaultName,
			types: [
				{
					description: 'Binary File',
					accept: { 'application/octet-stream': accept }
				}
			]
		});

		const writable = await handle.createWritable();
		await writable.write(data); // Uint8Array or string
		await writable.close();
	}

	async function saveExportDirectoryToHost() {
		const dirHandle = await window.showDirectoryPicker();

		await copyEmscriptenDirToHandle('/export', dirHandle);
	}

	async function copyEmscriptenDirToHandle(
		virtualPath: string,
		dirHandle: FileSystemDirectoryHandle
	) {
		const entries = $wasm.FS.readdir(virtualPath);

		for (const entry of entries) {
			if (entry === '.' || entry === '..') continue;

			const fullPath = `${virtualPath}/${entry}`;
			const stat = $wasm.FS.stat(fullPath);

			if ($wasm.FS.isDir(stat.mode)) {
				// Create subdirectory on the host
				const subDirHandle = await dirHandle.getDirectoryHandle(entry, { create: true });
				await copyEmscriptenDirToHandle(fullPath, subDirHandle); // recurse
			} else {
				// It's a file
				const fileHandle = await dirHandle.getFileHandle(entry, { create: true });
				const writable = await fileHandle.createWritable();
				const data = $wasm.FS.readFile(fullPath); // returns Uint8Array
				await writable.write(data);
				await writable.close();
			}
		}
	}

	async function zipAndDownloadExportDir(wasmName: string, hostName: string) {
		const zip = new JSZip();

		addEmscriptenDirToZip(zip.folder(hostName)!, wasmName);

		const blob = await zip.generateAsync({ type: 'blob' });

		// Trigger download
		const a = document.createElement('a');
		a.href = URL.createObjectURL(blob);
		a.download = hostName + '.zip';
		a.click();
		URL.revokeObjectURL(a.href);
	}

	function downloadFile(wasmName: string, hostName: string) {
		try {
			const data = $wasm.FS.readFile(wasmName);

			const blob = new Blob([data]);
			const url = URL.createObjectURL(blob);

			const a = document.createElement('a');
			a.href = url;
			a.download = hostName;
			a.click();

			URL.revokeObjectURL(url);
		} catch (e) {
			console.error(`Failed to download ${wasmName}:`, e);
		}
	}

	function addEmscriptenDirToZip(zipFolder: JSZip, virtualPath: string) {
		const entries = $wasm.FS.readdir(virtualPath);

		for (const entry of entries) {
			if (entry === '.' || entry === '..') continue;

			const fullPath = `${virtualPath}/${entry}`;
			const stat = $wasm.FS.stat(fullPath);

			if ($wasm.FS.isDir(stat.mode)) {
				const subZip = zipFolder.folder(entry)!;
				addEmscriptenDirToZip(subZip, fullPath);
			} else {
				const data = $wasm.FS.readFile(fullPath); // Uint8Array
				zipFolder.file(entry, data);
			}
		}
	}

	$effect(() => {
		let cnt = $MsgRshExport;
		if (cnt == 0) return;

		try {
			const wasmName = $amiga.getPayload(0);
			const hostName = $amiga.getPayload(1);
			console.log('Exporting... ', wasmName, hostName);

			const stat = $wasm.FS.stat(wasmName);

			if ($wasm.FS.isDir(stat.mode)) {
				console.log(wasmName, ' is a directory');
				zipAndDownloadExportDir(wasmName, hostName);
			} else if ($wasm.FS.isFile(stat.mode)) {
				console.log(wasmName, ' is a file');
				downloadFile(wasmName, hostName);
			}

			/*
			const info = $wasm.FS.analyzePath(wasmName);

			if (!info.exists) {
				console.warn(wasmName, ' does not exist');
				console.log($wasm.FS.readdir('/'));
			} else if (info.isDir) {
				console.log(wasmName, ' is a directory');
				zipAndDownloadExportDir(wasmName, hostName);
			} else if (info.isFile) {
				console.log(wasmName, ' is a file');
			} else {
				console.log('Huh?', info);
			}
			*/

			// Read file from the browser file system
			// const data = $wasm.FS.readFile(wasmName);
			// console.log('Reading export data... ');
			// console.log($wasm.FS.readdir('/'));
			// console.log($wasm.FS.analyzePath('/export'));

			// Save file to the host file system
			// saveToHost(proposedName, ['.bin'], data);
			// saveExportDirectoryToHost();
		} catch (err) {
			console.error('Export failed: ', err);
			if (err instanceof Error) {
				console.error('Name:', err.name);
				console.error('Message:', err.message);
			}
			// console.error('Stack:', err.stack);
			console.log('FS.analyzePath:', $wasm.FS.analyzePath('/export'));
			console.log('FS.readdir /:', $wasm.FS.readdir('/'));
			console.log('FS.readdir /export:', $wasm.FS.readdir('/export'));
		}
	});

	function action(sender: string, state: boolean) {
		switch (sender) {
			case 'export':
				// DEPRECATED
				console.log('export');
				try {
					console.log('Exporting file system...');
					$retroShell.exportBlocks('blob');
					console.log('Reading data...');
					console.log($wasm.FS.readdir('/'));
					console.log($wasm.FS.analyzePath('/blob'));
					const data = $wasm.FS.readFile('/blob');
					console.log('data: ', data);
					console.log('Save to host file system...');
					saveToHostFileSystem('', data);
					console.log(`File system successfully exported`);
				} catch (err) {
					console.error('Export failed!');
					console.error('Error object:', err);
					console.log('FS.analyzePath:', $wasm.FS.analyzePath('/blob'));
					console.log('FS contents:', $wasm.FS.readdir('/'));
				}
				break;
			case 'shell':
				$layer = $layer == Layer.shell ? Layer.none : Layer.shell;
				$showSidebar = false;
				break;
			case 'settings':
				$layer = $layer == Layer.settings ? Layer.none : Layer.settings;
				break;
			case 'monitor':
				if ($amiga.getConfig($wasm.OPT_DMA_DEBUG_ENABLE)) {
					emulator.textureRect!.zoomIn();
					$amiga.configure($wasm.OPT_DMA_DEBUG_ENABLE, 0);
				} else {
					emulator.textureRect!.zoomOut();
					$amiga.configure($wasm.OPT_DMA_DEBUG_ENABLE, 1);
				}
				break;
			case 'pause':
				try {
					if ($amiga.isRunning()) {
						console.log('Pausing...');
						$amiga.pause();
					} else {
						console.log('Running..:');
						$amiga.run();
					}
				} catch (exc) {
					console.log(exc);
					$wasm.reportException();
				}
				break;
			case 'power':
				try {
					if ($amiga.poweredOn()) {
						$amiga.powerOff();
						$amiga.hardReset();
					} else {
						$amiga.run();
					}
				} catch (exc) {
					console.log(exc);
					$wasm.reportException();
				}
				break;
			case 'reset':
				try {
					$amiga.hardReset();
				} catch (exc) {
					console.log(exc);
					$wasm.reportException();
				}
				break;
			case 'fit':
				$layout = 'fit';
				break;
			case 'aspect':
				$layout = 'aspect';
				break;
			case 'full':
				$layout = 'full';
				break;
			case 'empty1':
				$port1 = 0;
				break;
			case 'mouse1':
				$port1 = 1;
				if ($port2 == 1) $port2 = 0;
				break;
			case 'keyset11':
				$port1 = 2;
				if ($port2 == 2) $port2 = 0;
				break;
			case 'keyset12':
				$port1 = 3;
				if ($port2 == 3) $port2 = 0;
				break;
			case 'gamepad11':
				$port1 = 4;
				if ($port2 == 4) $port2 = 0;
				break;
			case 'empty2':
				$port2 = 0;
				break;
			case 'mouse2':
				$port2 = 1;
				if ($port1 == 1) $port1 = 0;
				break;
			case 'keyset21':
				$port2 = 2;
				if ($port1 == 2) $port1 = 0;
				break;
			case 'keyset22':
				$port2 = 3;
				if ($port1 == 3) $port1 = 0;
				break;
			case 'gamepad21':
				$port2 = 4;
				if ($port1 == 4) $port1 = 0;
				break;
			default:
				console.log('Unhandled sender: ' + sender);
		}
	}

	function push(event: CustomEvent<{ sender: string }>) {
		/*
		const sender = event.detail.sender;
		console.log('Status bar: ', sender);

		$showSidebar = !$showSidebar;

		if (!$showSidebar) {
			$layer = Layer.none;
		}
		*/
	}
</script>

<svelte:head>
	<title>vAmigaDOS</title>
</svelte:head>

<div class="h-screen overflow-y-auto scroll-smooth bg-black text-white">
	<Guru />
	{#if $wasm}
		<Proxy bind:this={$proxy} />
	{/if}
	{#if $initialized}
		<Config bind:this={$config} />
		<Audio bind:this={$audio} />
		<GamepadManager bind:this={$gamepadManager} />
		<MainScreen>
			<StatusBar bind:this={statusBar} on:push={push} />
			<div bind:this={canvas} class="box relative grow overflow-auto border-none border-green-300">
				<TitleScreen />
				<Emulator bind:this={emulator} />
				<RetroShell />
				<Settings />
				<DropZone />
				<Sidebar {action} />
			</div>
		</MainScreen>
	{/if}
</div>
