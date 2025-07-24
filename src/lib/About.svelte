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
	<h2>Overview</h2>
	<p>
		This web-based tool is designed to inspect and interact Amiga media files such as ADF and HDF
		disk images. It brings the classic Amiga command-line experience directly to your browser, no
		installation required. Getting started is simple: Just drag and drop an ADF or HDF file onto the
		browser window to launch a fully functional command shell. Once inside the shell, you can
		navigate directories and view file contents, create and delete files and folders, or run
		built-in file system diagnostics. It's an intuitive and efficient way to explore the inner
		structure of your Amiga disk images.
	</p>
	<h2>Technical Background</h2>
	<p>
		Behind the scenes, a modern web-frontend implemented in Svelte interacts with the Amiga emulator
		<it>vAmiga</it> compiled to WebAssembly (WASM). This setup allows it to emulate the Amiga file system
		and command interface entirely in the browser, delivering fast performance and seamless usability
		across platforms.
	</p>
	<h2>What’s Next?</h2>
	<p>
		While the current version offers a text-based command interface, the long-term goal is to evolve
		it into a more visual and interactive tool. Future versions may include a graphical file
		manager, allowing users to browse disk contents in a familiar folder view, click on files, and
		perform common actions such as copying, renaming, or deleting.
	</p>
</TextBox>
