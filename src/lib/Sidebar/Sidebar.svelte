<script lang="ts">
	import { Layer } from '$lib/types';
	import SidebarButton from '$lib/Sidebar/SidebarButton.svelte';
	import { fade } from 'svelte/transition';
	import { layer, showSidebar } from '$lib/stores';

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
	let diskIcon = 'icons/disk.png';
	let shellIcon = 'icons/retroShellIcon.png';

	const shellTooltip = $derived(
		$layer === Layer.shell ? 'Close the command shell' : 'Open the command shell'
	);
</script>

{#if $showSidebar}
	<div transition:fade={{ duration }} class="">
		<div class="bg-base-100/0 absolute top-0 left-0 z-40 flex w-16 flex-col pl-2">
			<div class="mt-1.5 mb-2 flex flex-col space-y-1.5 border-0">
				<SidebarButton
					{select}
					item={{ id: 'export', icon: diskIcon }}
					active={false}
					tooltip="Export the file system"
				/>
				<SidebarButton
					{select}
					item={{ id: 'shell', icon: shellIcon }}
					active={$layer === Layer.shell}
					tooltip={shellTooltip}
				/>
			</div>
		</div>
	</div>
{/if}
