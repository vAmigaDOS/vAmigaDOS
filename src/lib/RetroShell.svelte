<script lang="ts">
	import { fade } from 'svelte/transition';
	import { Layer } from '$lib/types';
	import { layer, MsgConsoleClose, MsgConsoleUpdate, retroShell } from '$lib/stores';

	// Textual contents of the console window
	let value = '';

	// Bindings
	let textarea = $state<HTMLTextAreaElement | null>(null);

	// Message handlers
	$effect(() => {
		if ($MsgConsoleClose) {
			$layer = Layer.none;
		}
	});
	$effect(() => {
		if ($MsgConsoleUpdate) {
			let rel = $retroShell.getCursorRel();
			if (textarea) {
				textarea.value = $retroShell.getText();
				textarea.focus();
				textarea.setSelectionRange(textarea.value.length + rel - 1, textarea.value.length + rel);
				textarea.scrollTop = textarea.scrollHeight;
			}
		}
	});

	function onkeydown(e: KeyboardEvent) {
		e.preventDefault();

		if (e.ctrlKey) {
			switch (e.key) {
				case 'k':
					$retroShell.pressCut();
					break;

				case 'a':
					$retroShell.pressHome();
					break;

				default:
					break;
			}
			return;
		}

		switch (e.key) {
			case 'ArrowUp':
				$retroShell.pressUp();
				break;
			case 'ArrowDown':
				$retroShell.pressDown();
				break;
			case 'ArrowLeft':
				$retroShell.pressLeft();
				break;
			case 'ArrowRight':
				$retroShell.pressRight();
				break;
			case 'Home':
				$retroShell.pressHome();
				break;
			case 'End':
				$retroShell.pressEnd();
				break;
			case 'Backspace':
				$retroShell.pressBackspace();
				break;
			case 'Delete':
				$retroShell.pressDelete();
				break;
			case 'Enter':
				e.shiftKey ? $retroShell.pressShiftReturn() : $retroShell.pressReturn();
				break;
			case 'Tab':
				$retroShell.pressTab();
				break;
			default:
				if (e.key.length == 1) {
					$retroShell.pressKey(e.key.charCodeAt(0));
				}
		}
	}
</script>

{#if $layer === Layer.shell}
	<div class="absolute top-0 left-0 flex h-full w-full overflow-auto" transition:fade>
		<div class="h-full grow overflow-auto bg-gray-500/85">
			<textarea
				bind:this={textarea}
				readonly
				style="resize: none; font-variant-ligatures: none"
				class="font-azeret h-full w-full bg-transparent p-2 text-base text-white focus:border-transparent focus:ring-0 focus:outline-none"
				{onkeydown}
			></textarea>
		</div>
	</div>
{/if}
