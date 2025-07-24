<script lang="ts">
	import { onMount } from 'svelte';
	import { fade } from 'svelte/transition';
	import { goto } from '$app/navigation';
	import DiGrails from 'svelte-icons/di/DiGrails.svelte';
	import FaGithub from 'svelte-icons/fa/FaGithub.svelte';
	import FaBookOpen from 'svelte-icons/fa/FaBookOpen.svelte';
	import GiBatMask from 'svelte-icons/gi/GiBatMask.svelte';
	import MdPowerSettingsNew from 'svelte-icons/md/MdPowerSettingsNew.svelte';
	import GoLaw from 'svelte-icons/go/GoLaw.svelte';
	import { Layer } from '$lib/types';
	import MainPageLink from '$lib/Widgets/MainPageLink.svelte';
	import Impressum from '$lib/Impressum.svelte';
	import RomViewer from '$lib/RomViewer.svelte';
	import Showcases from '$lib/Showcases/Showcases.svelte';
	import { demos } from '$lib/Showcases/database';
	import { amiga, audio, layer, poweredOn, wasm, proxy, showSidebar } from '$lib/stores';

	let debug = ''; // 'border-2';
	let showRomViewer = false;
	let showImpressum = false;

	$: if ($layer === Layer.kickstart) {
		showRomViewer = true;
	}

	onMount(() => {
		console.log('TitleScreen::onMount()');

		// Prefetch some images
		new Image().src = 'footage/3ddemo-small.jpg';
		new Image().src = 'footage/batmanrises-small.jpg';
		new Image().src = 'footage/celtic-small.jpg';
		new Image().src = 'footage/china-small.jpg';
		new Image().src = 'footage/defender-small.jpg';
		new Image().src = 'footage/desertdreams-small.jpg';
		new Image().src = 'footage/diagrom-small.jpg';
		new Image().src = 'footage/eon-small.jpg';
		new Image().src = 'footage/inebriation-small.jpg';
		new Image().src = 'footage/kyrios-small.jpg';
		new Image().src = 'footage/paccer-small.jpg';
		new Image().src = 'footage/rinkadink-small.jpg';
		new Image().src = 'footage/sdi-small.jpg';
		new Image().src = 'footage/stateart-small.jpg';
		new Image().src = 'footage/sysinfo-small.jpg';
		new Image().src = 'footage/testkit-small.jpg';
		new Image().src = 'footage/traprunner-small.jpg';
	});

	async function switchOn() {
		await $audio.setup();
		$amiga.run();
	}

	async function runDemo() {
		await $audio.setup();
		$proxy.runShowcase(demos[0]);
	}

	function gotoGitHub() {
		window.location.href = 'https://github.com/dirkwhoffmann/vAmigaNet';
		// goto('https://dirkwhoffmann.github.io/vAmiga');
	}

	async function openShowcases() {
		console.log('openShowcases');
		await $audio.setup();
		$layer = $layer === Layer.showcases ? Layer.none : Layer.showcases;
		$showSidebar = false;
	}

	async function openRoms() {
		await $audio.setup();
		showRomViewer = true;
	}

	function openImpressum() {
		showImpressum = true;
	}
</script>

{#if !$poweredOn}
	<div class="modal" class:modal-open={showRomViewer}>
		<div class="modal-box h-2/3 w-2/3 max-w-none">
			<RomViewer bind:show={showRomViewer} />
		</div>
	</div>

	<div class="modal" class:modal-open={showImpressum}>
		<div class="modal-box">
			<Impressum bind:show={showImpressum} />
		</div>
	</div>

	<div class="flex h-full flex-col {debug}" transition:fade>
		<div in:fade={{ duration: 2000 }}>
			<div class="absolute h-full">
				<div class="h-full w-screen bg-gradient-to-t from-gray-200 to-gray-400"></div>
			</div>
			<div class="absolute flex h-full w-full items-center justify-center">
				<div class="flex flex-col items-center">
					<img
						class="h-[50vh] w-auto object-contain opacity-100"
						src="icons/hand-disk.png"
						alt="Background"
					/>
					<div class="mt-4 w-[50vw] font-josefin text-lg text-center text-gray-600">
						Welcome to the Amiga file system inspector!
					</div>
					<div class=" w-[50vw] font-josefin text-lg text-center text-gray-400">
						Drag and drop an ADF or HDF file anywhere on this window to browse files and
						explore folders.
					</div>
				</div>
			</div>
			<!--
        <div class="absolute h-full">
            <img class="h-full w-screen object-fill" src="splash-bg.jpg" alt="Background"/>
        </div>
        -->
		</div>
		{#if $layer === Layer.showcases}
			<Showcases />
		{:else}
			<div
				in:fade
				class="relative flex grow flex-col items-center justify-center bg-transparent {debug}"
			>
				{#if $layer === Layer.none}
					<div class={debug}>
						<div class="flex justify-center rounded">
							<img class="mt-3 h-24 p-2" src="va-icon.png" alt="vAmiga Icon" />
							<div class="p-6 text-white">
								<div class="flex">
									<div class="font-sofia-extra mr-2 text-7xl">
										vAmiga<span class="opacity-75">.net</span>
									</div>
								</div>
								<div class="font-sofia-semi pb-10 pl-2 text-xl">Version 2.0 beta 2 (250723)</div>
								<div class="flex space-x-5">
									<div class="tooltip tooltip-primary" data-tip="Power On">
										<button class="btn btn-lg btn-outline btn-square p-1.5" on:click={switchOn}>
											<MdPowerSettingsNew />
										</button>
									</div>
									<div class="tooltip tooltip-primary" data-tip="Run Demo">
										<button class="btn btn-lg btn-outline btn-square p-1.5" on:click={runDemo}>
											<GiBatMask />
										</button>
									</div>
									<div class="tooltip tooltip-primary" data-tip="Visit GitHub">
										<button class="btn btn-lg btn-outline btn-square p-1.5" on:click={gotoGitHub}>
											<FaGithub />
										</button>
									</div>
								</div>
							</div>
						</div>
					</div>
				{/if}
			</div>
		{/if}
		<div class="relative flex justify-center space-x-4 bg-gray-900/50 align-middle {debug}">
			<MainPageLink onclick={openRoms}>
				{#snippet icon()}
					<div>
						<FaBookOpen />
					</div>
				{/snippet}
				{#snippet description()}
					<div>Learn More</div>
				{/snippet}
			</MainPageLink>
			<MainPageLink onclick={gotoGitHub}>
				{#snippet icon()}
					<div>
						<FaGithub />
					</div>
				{/snippet}
				{#snippet description()}
					<div>Sources</div>
				{/snippet}
			</MainPageLink>
			<MainPageLink onclick={openImpressum}>
				{#snippet icon()}
					<div>
						<GoLaw />
					</div>
				{/snippet}
				{#snippet description()}
					<div>Impressum</div>
				{/snippet}
			</MainPageLink>
		</div>
	</div>
{/if}
