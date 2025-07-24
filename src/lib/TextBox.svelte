<script lang="ts">
	import { fade } from 'svelte/transition';

	interface Props {
		show?: boolean;
		title?: string;
		close?: () => void;
		children?: import('svelte').Snippet;
	}

	let { show = $bindable(false), title = '', close = () => {}, children }: Props = $props();

</script>

<div class="" transition:fade>
	<div class="font-josefin flex h-[500px] flex-col">
		<!-- Header -->
		<div class="w-full text-center text-3xl font-semibold text-white">{title}</div>
		<hr class="my-4 h-px border-0 bg-gray-200 dark:bg-gray-700" />

		<!-- Main -->
		<div class="grow overflow-auto border-blue-500 px-4">
			<div>
				<article in:fade class="prose mx-1">
					{@render children?.()}
				</article>
			</div>
		</div>

		<!-- Footer -->
		<hr class="my-4 h-px border-0 bg-gray-200 dark:bg-gray-700" />
		<div class="flex w-full justify-center">
			<button class="btn btn-primary" onclick={close}>Close</button>
		</div>
	</div>
</div>
