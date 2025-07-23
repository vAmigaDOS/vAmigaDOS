<script lang="ts">
	import { fade } from 'svelte/transition';
	import { MenuItem, Opt } from '$lib/types';
	import { config, MsgConfig, wasm } from '$lib/stores';
	import ConfigSection from './ConfigSection.svelte';
	import ConfigItem from './ConfigItem.svelte';

</script>

<div in:fade>
	<ConfigSection name="Sound Synthesis">
		<ConfigItem
				name="Interpolation Method"
				items={[
                    new MenuItem('None', 0),
                    new MenuItem('Nearest', 1),
                    new MenuItem('Linear', 2)
                    ]}
				select={(value) => $config.setNum(Opt.SAMPLING_METHOD, value)}
				selectedTag={$config.getNum(Opt.SAMPLING_METHOD)}
		/>
		<ConfigItem
				name="Audio Filter"
				items={[
                    new MenuItem('None', 0),
                    new MenuItem('A500', 1),
                    new MenuItem('A1000', 2),
                    new MenuItem('A1200', 3),
                    new MenuItem('Legacy Filter', 4),
                    new MenuItem('Low-pass only', 5),
                    new MenuItem('LED filter only', 6),
                    new MenuItem('High-pass only', 7)
                    ]}
				select={(value) => $config.set(Opt.FILTER_TYPE, value)}
				selectedTag={$config.get(Opt.FILTER_TYPE)}
		/>
	</ConfigSection>
	<ConfigSection name="Speakers">
		<ConfigItem
			name="Left Speaker Volume"
			selectedTag={$config.get(Opt.AUDVOLL)}
			select={(value) => $config.set(Opt.AUDVOLL, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Right Speaker Volume"
			selectedTag={$config.get(Opt.AUDVOLR)}
			select={(value) => $config.set(Opt.AUDVOLR, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Paula Channel 0 Volume"
			selectedTag={$config.get(Opt.AUDVOL0)}
			select={(value) => $config.set(Opt.AUDVOL0, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Paula Channel 1 Volume"
			selectedTag={$config.get(Opt.AUDVOL1)}
			select={(value) => $config.set(Opt.AUDVOL1, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Paula Channel 2 Volume"
			selectedTag={$config.get(Opt.AUDVOL2)}
			select={(value) => $config.set(Opt.AUDVOL2, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Paula Channel 3 Volume"
			selectedTag={$config.get(Opt.AUDVOL3)}
			select={(value) => $config.set(Opt.AUDVOL3, value)}
			min={0}
			max={100}
		/>
	</ConfigSection>
	<ConfigSection name="Drive Sounds">
		<ConfigItem
			name="Head Step Volume"
			selectedTag={$config.get(Opt.STEP_VOLUME)}
			select={(value) => $config.set(Opt.STEP_VOLUME, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Poll Disk Volume"
			selectedTag={$config.get(Opt.POLL_VOLUME)}
			select={(value) => $config.set(Opt.POLL_VOLUME, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Insert Disk Volume"
			selectedTag={$config.get(Opt.INSERT_VOLUME)}
			select={(value) => $config.set(Opt.INSERT_VOLUME, value)}
			min={0}
			max={100}
		/>
		<ConfigItem
			name="Eject Disk Volume"
			selectedTag={$config.get(Opt.EJECT_VOLUME)}
			select={(value) => $config.set(Opt.EJECT_VOLUME, value)}
			min={0}
			max={100}
		/>
	</ConfigSection>
</div>
