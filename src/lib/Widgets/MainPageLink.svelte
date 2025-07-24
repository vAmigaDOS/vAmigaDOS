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
		class="flex h-6 w-6 border-0 flex-col items-center justify-center text-gray-600"
		{onclick}
	>
		{@render icon()}
	</button>
	<div class="font-josefin flex border-none text-xs text-gray-600">
		{@render description()}
	</div>
</div>
