<script lang="ts">
	import type { ActionEvent, MenuItem } from '$lib/types';
	import GoInfo from 'svelte-icons/go/GoInfo.svelte';
	import GiPadlock from 'svelte-icons/gi/GiPadlock.svelte';
	import Slider from '$lib/Widgets/Slider.svelte';
	import SelectMenu from '$lib/Widgets/SelectMenu.svelte';
	import Chevron from '$lib/Widgets/Chevron.svelte';

	interface Props {
		name?: string;
		min?: number;
		max?: number;
		locked?: boolean;
		selectedTag?: number;
		items?: MenuItem[];
		select: (value: number) => void;
		info?: (value: number) => void;
	}

	let {
		name = '',
		min = 0,
		max = 0,
		locked = false,
		selectedTag = 0,
		items = [],
		select = () => {},
		info = undefined
	}: Props = $props();

	const opac = $derived(locked ? 'opacity-50' : '');

	const displayName = $derived.by(() => {
        for (const item of items) {
            if (item.tag == selectedTag) return item.title;
        }
        return '???';
    }); 

	const infoAction = (e: Event) => {
		console.log('infoAction', e);
		e.preventDefault();
	};

</script>

<div class="px-0 py-0.5">
	<div class="flex w-full items-center justify-between space-x-1 text-xl">
		<div class="bg-primary flex h-12 w-full grow overflow-hidden border-0">
			<div
				class="w-full border-0 text-xl {opac} text-primary-content flex items-center justify-between"
			>
				<div class="mx-2 flex grow overflow-hidden border-0 whitespace-nowrap">{name}</div>
			</div>
		</div>
		<div
			class="bg-primary flex h-12 w-20 items-center justify-center border-0 {locked
				? ''
				: 'hidden'}"
		>
			<button class="text-primary-content h-7 w-7 {opac}">
				<GiPadlock />
			</button>
		</div>
		<div
			class="bg-primary flex h-12 w-20 items-center justify-center border-0 {info ? '' : 'hidden'}"
		>
			<button class="text-primary-content h-7 w-7 {opac}" onclick={infoAction}>
				<GoInfo />
			</button>
		</div>
		<div class="bg-primary h-12 border-0">
			{#if min === max}
				<SelectMenu
					isEnabled={!locked}
					items={items}
					select={(value: number) => { selectedTag = value; select(selectedTag)}}
                    {selectedTag}
					listStyle="w-[18rem] mt-2 text-xl bg-accent text-accent-content"
				>
					{#if locked}
						<button
							class="btn btn-primary {opac} w-[18rem] rounded-none border-0 text-xl font-normal"
							>{displayName}</button
						>
					{:else}
						<button class="btn btn-primary w-[18rem] rounded-none border-0 text-xl font-normal">
							<Chevron>{displayName}</Chevron>
						</button>
					{/if}
				</SelectMenu>
			{:else}
				<Slider {min} {max} {locked} value={selectedTag} {select}>
					<button class="btn btn-primary {opac} w-[18rem] rounded-none border-0 text-xl font-normal"
						>{selectedTag}</button
					>
				</Slider>
			{/if}
		</div>
	</div>
</div>
