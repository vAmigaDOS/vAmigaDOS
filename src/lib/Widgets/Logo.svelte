<script lang="ts">
	interface Props {
		shadow?: boolean;
	}

	let { shadow = false }: Props = $props();
</script>

<div class="pointer-events-none">
	<!--<div class="z-50 absolute bottom-0 right-0 p-4">-->
	<div class="w-full border-0">
		<div class="flex items-center">
			<img class="mt-0 h-14 p-0" src="va-icon.png" alt="vAmiga Icon" />
			<!--<button class="btn btn-lg btn-outline btn-square p-1.5" on:click={switchOn}></button>-->
			<!--<button class="btn btn-lg"><img class="mt-3 h-24 p-2" src="va-icon.png" alt="vAmiga Icon"/></button>-->
			<div class="flex flex-col pl-2 border-0">
				<div class="font-sofia-extra leading-none text-sm text-gray-400 border-0">Powered by</div>
				<div class="font-sofia-extra leading-none text-4xl text-gray-400  border-0">vAmiga</div>
			</div>
		</div>
	</div>
</div>
