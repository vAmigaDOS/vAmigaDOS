<script lang="ts">
	import { onMount } from 'svelte';
	import { fade } from 'svelte/transition';
	import FaGithub from 'svelte-icons/fa/FaGithub.svelte';
	import FaBookOpen from 'svelte-icons/fa/FaBookOpen.svelte';
	import GoLaw from 'svelte-icons/go/GoLaw.svelte';
	import { Layer } from '$lib/types';
	import Logo from '$lib/Widgets/Logo.svelte';
	import MainPageLink from '$lib/Widgets/MainPageLink.svelte';
	import Impressum from '$lib/Impressum.svelte';
	import About from '$lib/About.svelte';
	import { layer, poweredOn } from '$lib/stores';

	let debug = ''; // 'border-2';
	let showAbout = false;
	let showImpressum = false;

	onMount(() => {
		console.log('TitleScreen::onMount()');
	});

	function gotoGitHub() {
		window.location.href = 'https://github.com/vAmigaDOS/vAmigaDOS';
	}

	function openAbout() {
		showAbout = true;
	}

	function openImpressum() {
		showImpressum = true;
	}
</script>

{#if !$poweredOn}
	<div class="modal" class:modal-open={showAbout}>
		<div class="modal-box">
			<About bind:show={showAbout} />
		</div>
	</div>
	<div class="modal" class:modal-open={showImpressum}>
		<div class="modal-box">
			<Impressum bind:show={showImpressum} />
		</div>
	</div>

	<div class="z-50 flex h-full w-screen flex-col border-0 border-cyan-400 {debug}" transition:fade>
		<div in:fade={{ duration: 2000 }}>
			<div class="absolute h-full w-full bg-gradient-to-t from-gray-500 via-white to-white"></div>
			<div class="absolute flex h-full w-full items-center justify-center">
				<div class="flex flex-col items-center">
					<img
						class="h-[50vh] w-auto object-contain opacity-100"
						src="icons/hand-disk.png"
						alt="Background"
					/>
					<div class="font-josefin mt-4 w-[50vw] text-center text-2xl text-white">
						Welcome to the Amiga file system inspector!
					</div>
					<!--<div class="mt-0 w-[50vw] font-josefin text-sm text-center text-white">
						v0.9 alpha (build 250724)
					</div>-->
					<div class=" font-josefin w-[50vw] text-center text-lg text-gray-200">
						Drag and drop an ADF or HDF file anywhere on this window to browse files and explore
						folders.
					</div>
					<div class="font-josefin text-lg m-4 w-[50vw] text-center text-indigo-200">
						This version is a pre-release. Please help improving this software by
						<a class="underline" href="https://github.com/vAmigaDOS/vAmigaDOS/issues">filing a bug report</a> on GitHub.
					</div>
				</div>
			</div>
		<!--
        <div class="absolute h-full">
            <img class="h-full w-screen object-fill" src="splash-bg.jpg" alt="Background"/>
        </div>
        -->
		</div>
		<div class="absolute bottom-6 left-6 flex flex-col space-y-3 border-0 {debug}">
			<MainPageLink onclick={openAbout}>
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
					<div>Source Code</div>
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
		<div class="absolute right-6 bottom-6 flex flex-col space-y-3 border-0 {debug}">
			<Logo></Logo>
		</div>
	</div>
{/if}
