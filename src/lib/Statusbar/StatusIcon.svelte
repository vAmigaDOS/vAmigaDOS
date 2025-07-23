<script lang="ts">
	import { darkTheme } from '$lib/stores';

	let {
		src = '',
		onclick = () => {},
		children
	}: {
		src?: string;
		onclick?: () => void;
		children?: import('svelte').Snippet;
	} = $props();
</script>

<button type="button" {onclick}>
	<img
		class="h-8 w-8 border-0 object-scale-down p-1.5 {$darkTheme ? 'invert' : ''} opacity-70"
		{src}
		alt="Status bar icon"
	/>
</button>
{@render children?.()}
