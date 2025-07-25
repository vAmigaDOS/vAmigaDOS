<script lang="ts">
	import { darkTheme } from '$lib/stores';

	interface Props {
		item?: { id: string; icon: string };
		alt?: string;
		toggle?: boolean;
		active?: boolean;
		tooltip?: string;
		select: (sender: string, state: boolean) => void;
	}
	let {
		item = { id: '', icon: '' },
		alt = 'Icon',
		toggle = false,
		active = false,
		tooltip = '',
		select = () => {}
	}: Props = $props();

	let ttStyle = $state('');
	let onoff = false;

	function click(e: Event) {
		e.preventDefault();
		onoff = toggle ? !onoff : true;
		select((e.target as HTMLElement).id, onoff);
	}
	
	function onmouseenter(e: Event) {
		console.log('onmouseenter');
		setTimeout(() => {
			ttStyle = 'tooltip tooltip-right tooltip-primary';
		}, 700);
	}

	function onmouseleave(e: Event) {
		console.log('onmouseleave');
		ttStyle = '';
	}
</script>

<div class="relative" {onmouseenter} {onmouseleave} role="region">
	<div class={ttStyle}>
		{#if ttStyle}
		<div class="tooltip-content">
		    <div class="p-2 font-josefin">{tooltip}</div>
		</div>
		{/if}
		<button
			type="button"
			class="btn btn-lg btn-primary btn-square p-1.5 {active ? 'btn-active' : ''}"
			onclick={click}
		>
			<img class={$darkTheme ? 'invert' : ''} id={item.id} src={item.icon} {alt} />
		</button>
	</div>
</div>
