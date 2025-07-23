<script lang="ts">
	import { darkTheme, invert, dfHasDisk } from '$lib/stores';
	import BarBox from '$lib/Statusbar/BarBox.svelte';
	import Menu from '$lib/Widgets/Menu.svelte';
	import { MenuItem } from '$lib/types';

	interface Props {
		tag?: number;
		cyl?: number;
		disk?: boolean;
		motor?: boolean;
		writing?: boolean;
		unsaved?: boolean;
		wp?: boolean;
		select?: (value: number) => void;
	}
	let {
		tag = 0,
		cyl = 0,
		disk = true,
		motor = false,
		writing = false,
		unsaved = false,
		wp = false,
		select = () => {}
	}: Props = $props();

	const gray = '';
	const green = 'bg-gradient-to-b from-green-700 to-green-500';
	const red = 'bg-gradient-to-b from-red-700 to-red-500';
	const diskIcon = $derived(wp ? 'icons/disk-protected.png' : 'icons/disk.png');
	const driveIcon = 'icons/floppy-drive.png';
	const src = $derived(disk ? diskIcon : driveIcon);
	const bg = $derived(motor ? (writing ? red : green) : gray);
	const opc = $derived(unsaved ? 'opacity-40' : 'opacity-70');
	const textcol = $derived($darkTheme ? 'text-gray-300' : 'text-black');

	const items = $derived.by(() => {
		let result = [new MenuItem('Insert', 0), new MenuItem('Eject', 1)];

		result[0].isEnabled = true;
		result[0] = result[0];

		result[1].isEnabled = disk;
		result[1] = result[1];

		return result;
	});
</script>

<Menu {items} {tag} {select} listStyle="menu menu-compact rounded p-0 text-sm w-32">
	<BarBox {bg}>
		<img
			class="h-full border-0 object-scale-down py-1.5 pr-1 {$invert} {opc}"
			{src}
			alt="Floppy icon"
		/>
		<div class="w-6 items-center border-0 pl-1 text-left text-sm leading-none {textcol}">
			{cyl}
		</div>
	</BarBox>
</Menu>
