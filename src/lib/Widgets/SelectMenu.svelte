<script lang="ts">
	import type { Snippet } from 'svelte';
	import { MenuItem } from '$lib/types';
	import Menu from '$lib/Widgets/Menu.svelte';

	interface Props {
		items?: MenuItem[];
		isEnabled?: boolean;
		dropdownStyle?: string;
		listStyle?: string;
		selectedTag?: number;
		select: (value: number) => void;
		children?: Snippet;
	}
	let {
		items = [],
		isEnabled = true,
		dropdownStyle = '',
		listStyle = '',
		selectedTag = 0,
		select = () => {},
		children
	}: Props = $props();

	let checkmarkedItems = $derived.by(() => {
		let result = items;
		let selected = selectedTag;
		for (const item of result) {
			item.isSelected = item.tag == selected;
		}
		return result;
	});
</script>

{#key items}
{#key selectedTag}
<Menu items={checkmarkedItems} {isEnabled} checkmarks={true} {dropdownStyle} {listStyle} {select}>
	{@render children?.()}
</Menu>
{/key}
{/key}
