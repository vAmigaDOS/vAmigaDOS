<script lang="ts">
	let {
		icon,
		description,
		onclick = () => {}
	}: {
		icon: import('svelte').Snippet;
		description: import('svelte').Snippet;
		onclick?: () => void;
	} = $props();

	const iconSize = 'h-10 w-10'
	const fontColor = 'text-gray-300'
</script>

<div class="flex w-24 border-0 flex-col items-center justify-center">
	<button
		class="flex {iconSize} border-0 flex-col items-center justify-center {fontColor}"
		{onclick}
	>
		{@render icon()}
	</button>
	<div class="font-josefin flex border-none {fontColor}">
		{@render description()}
	</div>
</div>
