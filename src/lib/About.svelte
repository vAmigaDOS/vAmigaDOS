<script lang="ts">
	import TextBox from './TextBox.svelte';

	function close() {
		console.log('close');
		show = false;
	}

	interface Props {
		show?: boolean;
	}

	let { show = $bindable(false) }: Props = $props();
</script>

<TextBox title="About" {show} {close}>
	<h2>What is vAmigaDOS?</h2>
	<p>
		vAmigaDOS is a web-based tool designed to inspect and interact with Amiga media files such as
		ADF and HDF disk images. It brings the classic Amiga command-line experience directly to your
		browser, no installation required. Getting started is simple: Just drag and drop an ADF or HDF
		file onto the browser window to launch a fully functional command shell. Once inside the shell,
		you can navigate directories and view file contents, create and delete files and folders, or run
		built-in file system diagnostics. It's an intuitive and efficient way to explore the inner
		structure of your Amiga disk images.
	</p>
	<h2>Technical Background</h2>
	<p>
		Technical background Behind the scenes, vAmigaDOS combines a modern Svelte frontend with the
		power of vAmiga compiled to WebAssembly (WASM). This setup allows it to emulate the Amiga file
		system and command interface entirely in the browser, delivering fast performance and seamless
		usability across platforms.
	</p>
</TextBox>
