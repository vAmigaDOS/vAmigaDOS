<script lang="ts">
	import { writable } from 'svelte/store';

	import { browser } from '$app/environment';
	import { onMount } from 'svelte';
	import { db } from '$lib/Db/db';
	import { liveQuery } from 'dexie';
	import { Layer } from '$lib/types';
	import { initialized, amiga, dfConnected, kickstarts } from '$lib/stores';
	import { wasm, proxy, audio, config, gamepadManager } from '$lib/stores';
	import { layer, poweredOn, what, errno } from '$lib/stores';
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

	$: if (canvas) { resize(); }

	$: if ($initialized) {
		// Start render loop
		window.requestAnimationFrame(doAnimationFrame);
	}

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

	function action(sender: string, state: boolean) {
	
		switch (sender) {
			case 'shell':
				$layer = $layer == Layer.shell ? Layer.none : Layer.shell;
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
						console.log("Pausing...")
						$amiga.pause();
					} else {
						console.log("Running..:")
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
  <title>vAmiga.net</title>
</svelte:head>

<!--<body>-->
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
            <StatusBar bind:this={statusBar} on:push={push}/>
            <div bind:this={canvas} class="box relative grow border-none border-green-300 overflow-auto">
                <TitleScreen/>
                <Emulator bind:this={emulator}/>
                <RetroShell/>
                <Settings/>
                <DropZone/>
                <Sidebar {action}/>
            </div>
        </MainScreen>
	{/if}
</div>
<!--</body>-->
