<script lang="ts">
	import type { Snippet } from 'svelte';
	import { MenuItem, MenuSeparator } from '$lib/types';
	import Checkmark from './Checkmark.svelte';

	interface Props {
		items?: MenuItem[];
		isEnabled?: boolean;
		checkmarks?: boolean;
		dropdownStyle?: string;
		listStyle?: string;
		select: (value: number) => void;
		children?: Snippet;
	}
	let {
		items = [],
		isEnabled = true,
		checkmarks = false,
		dropdownStyle = '',
		listStyle = '',
		select = () => {},
		children
	}: Props = $props();

	const selectedItems = $derived.by(() => {
		let it = items;
		return it.filter((item) => item.isSelected);
	});

	const action = (e: MouseEvent, value: number) => {
		e.preventDefault();

		// Force the dropdown to close
		if (document.activeElement) (document.activeElement as HTMLElement).blur();
		select(value);
	};

	function style(i: number) {
		return items[i].isEnabled ? '' : 'opacity-20';
	}
</script>

<div class="dropdown {dropdownStyle}">
	<!-- <div tabindex="0" role="button" class="btn m-1">Click</div> -->
	<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
	<label tabindex="0" class={isEnabled ? '' : 'pointer-events-none'}>
		{@render children?.()}
	</label>
	<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
	<!-- svelte-ignore a11y_label_has_associated_control -->
	<ul tabindex="0" class="dropdown-content menu bg-base-100 {listStyle}">
		<!--
		{#if selectedItems.length != 0}
			<li class="text-primary-content px-2 text-xs">
				{selectedItems.length} item{selectedItems.length > 1 ? 's' : ''} selected
			</li>
		{/if}
		-->
		{#key items}
			{#each items as item, i}
				{#if item instanceof MenuSeparator}
					<li class="divider"></li>
				{:else if !item.isHidden}
					<li>
						<button
							disabled={!item.isEnabled}
							class={item.isEnabled ? '' : 'hover:bg-base-100 opacity-40'}
							onclick={(e) => action(e, item.tag)}
						>
							{#if checkmarks}
								<Checkmark enabled={true} visible={item.isSelected} />
							{/if}
							{item.title}</button
						>
					</li>
				{/if}
			{/each}
		{/key}
	</ul>
</div>
