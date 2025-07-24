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
</script>

<div class="flex w-24 border-0 flex-col items-center justify-center">
	<button
		class="flex h-8 w-8 border-0 flex-col items-center justify-center text-gray-500"
		{onclick}
	>
		{@render icon()}
	</button>
	<div class="font-josefin flex border-none text-sm text-gray-500">
		{@render description()}
	</div>
</div>
