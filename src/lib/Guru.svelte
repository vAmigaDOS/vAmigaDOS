<script lang="ts">
	import { wasm, enums, what, errno } from '$lib/stores';
	$: initialized = $wasm != undefined;

	let intervalId: ReturnType<typeof setInterval> | undefined = undefined;
	let active = false;
	let border = true;

	$: if ($errno && initialized && !active) {
		active = true;
		if (intervalId === undefined) {
			intervalId = setInterval(() => {
				border = !border;
			}, 800);
		}
	}
	$: borderColor = border ? 'border-red-600' : 'border-black';

	function onclick() {
		console.log('onclick');
		$errno = 0;
		active = false;
		clearInterval(intervalId);
		intervalId = undefined
	}
</script>

{#if active}
	<div
		{onclick}
		class="relative flex justify-center border-[10px] bg-black text-red-600 {borderColor}"
	>
		<div
			class="font-azeret flex h-24 w-full flex-col justify-center border-none text-center text-lg"
		>
			<p class="p-1">
				Emulator failure. Uncaught exception {$errno} ({$enums.ErrorCodeKey($errno)})
			</p>
			<p class="p-1">{$what}</p>
		</div>
	</div>
{/if}
