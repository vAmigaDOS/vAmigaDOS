<script lang="ts">
	import { Layer } from '$lib/types';
	import SidebarButton from '$lib/Sidebar/SidebarButton.svelte';
	import SidebarSection from '$lib/Sidebar/SidebarSection.svelte';
	import { fade } from 'svelte/transition';
	import SidebarPad from '$lib/Sidebar/SidebarPad.svelte';
	import {
		debugDma,
		layer,
		layout,
		port1,
		port2,
		poweredOn,
		running,
		showSidebar
	} from '$lib/stores';

	let {
		action = () => {}
	}: {
		action: (sender: string, state: boolean) => void;
	} = $props();

	let sel = $state('');
	let duration = 200;

	function select(sender: string, state: boolean) {
		sel = sender == sel ? '' : sender;
		action(sender, state);
	}
	const portIcons = [
		'icons/device-none.png',
		'icons/device-mouse.png',
		'icons/device-keyset-1.png',
		'icons/device-keyset-2.png',
		'icons/device-usb-1.png',
		'icons/device-usb-2.png'
	];
	let powerIcon = 'icons/powerIcon.png';
	let resetIcon = 'icons/resetIcon.png';
	let settingsIcon = 'icons/settingsIcon.png';
	let shellIcon = 'icons/retroShellIcon.png';
	let monitorIcon = 'icons/monitorIcon.png';
	let pauseIcon = $derived($running ? 'icons/pauseIcon.png' : 'icons/runIcon.png');
	let port1Icon = $derived(portIcons[$port1]);
	let port2Icon = $derived(portIcons[$port2]);
	let layoutIcon = $derived(
		$layout == 'full'
			? 'icons/layoutFullIcon.png'
			: $layout == 'aspect'
				? 'icons/layoutAspectIcon.png'
				: 'icons/layoutFitIcon.png'
	);
	const port1Items = [
		{ id: 'empty1', icon: portIcons[0] },
		{ id: 'mouse1', icon: portIcons[1] },
		{ id: 'keyset11', icon: portIcons[2] },
		{ id: 'keyset12', icon: portIcons[3] },
		{ id: 'gamepad11', icon: portIcons[4] }
	];
	const port2Items = [
		{ id: 'empty2', icon: portIcons[0] },
		{ id: 'mouse2', icon: portIcons[1] },
		{ id: 'keyset21', icon: portIcons[2] },
		{ id: 'keyset22', icon: portIcons[3] },
		{ id: 'gamepad21', icon: portIcons[4] }
	];
	const layoutItems = [
		{ id: 'aspect', icon: 'icons/layoutAspectIcon.png' },
		{ id: 'fit', icon: 'icons/layoutFitIcon.png' },
		{ id: 'full', icon: 'icons/layoutFullIcon.png' }
	];
</script>

{#if $showSidebar}
	<div transition:fade={{ duration }} class="">
		<div class="bg-base-100/0 absolute top-0 left-0 z-40 flex w-16 flex-col pl-2">
			<div class="mt-1.5 mb-2 flex flex-col space-y-1.5 border-0">
				<!--
				<SidebarButton {select} item={{ id: 'power', icon: powerIcon }} />
				{#if $poweredOn}
					<SidebarButton {select} item={{ id: 'pause', icon: pauseIcon }} />
					<SidebarButton {select} item={{ id: 'reset', icon: resetIcon }} />
				{/if}
				<SidebarPad />
				<SidebarSection
					{select}
					expanded={sel === 'port1'}
					item={{ id: 'port1', icon: port1Icon }}
					subitems={port1Items}
				/>
				<SidebarSection
					{select}
					expanded={sel === 'port2'}
					item={{ id: 'port2', icon: port2Icon }}
					subitems={port2Items}
				/>
				<SidebarPad />
				<SidebarButton
					{select}
					item={{ id: 'settings', icon: settingsIcon }}
					active={$layer === Layer.settings}
				/>
				-->
				<SidebarButton
					{select}
					item={{ id: 'shell', icon: shellIcon }}
					active={$layer === Layer.shell}
				/>
				<!--
				<SidebarPad />
				{#if $poweredOn}
					<SidebarSection
						{select}
						expanded={sel === 'layout'}
						item={{ id: 'layout', icon: layoutIcon }}
						subitems={layoutItems}
					/>
					<SidebarButton {select} item={{ id: 'monitor', icon: monitorIcon }} active={$debugDma} />
				{/if}
				-->
			</div>
		</div>
	</div>
{/if}
