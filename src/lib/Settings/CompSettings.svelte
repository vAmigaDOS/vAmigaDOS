<script lang="ts">
    import { fade } from 'svelte/transition';
    import { MenuItem, Opt } from "$lib/types";
    import ConfigSection from './ConfigSection.svelte';
    import ConfigItem from '$lib/Settings/ConfigItem.svelte';
    import { config } from '$lib/stores';
</script>

<div in:fade>
    <ConfigSection name="Blitter">
        <ConfigItem
                name = "Accuracy"
                items={[
                    new MenuItem('Low', 0),
				    new MenuItem('Medium', 1),
				    new MenuItem('High', 2)
				    ]}
                select={(value) => $config.setNum(Opt.BLITTER_ACCURACY, value)}
                selectedTag={$config.getNum(Opt.BLITTER_ACCURACY)}
        />
    </ConfigSection>
    <ConfigSection name="Chipset Features">
        <ConfigItem
                name="Emulate TOD Bug"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.TODBUG, value)}
                selectedTag={$config.getNum(Opt.TODBUG)}
        />
        <ConfigItem
                name="Emulate Dropped Register Writes"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.PTR_DROPS, value)}
                selectedTag={$config.getNum(Opt.PTR_DROPS)}
        />
    </ConfigSection>
    <ConfigSection name="Timing">
        <ConfigItem
                name="Sync CIA Accesses With E-Clock"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.ECLOCK_SYNCING, value)}
                selectedTag={$config.getNum(Opt.ECLOCK_SYNCING)}
        />
    </ConfigSection>
    <ConfigSection name="Floppy Drives">
        <ConfigItem
                name="Drive Speed"
                items={[
				    new MenuItem('Original', 1),
				    new MenuItem('Accelerated (2 x)', 2),
				    new MenuItem('Accelerated (4 x)', 4),
				    new MenuItem('Accelerated (8 x)', 8),
				    new MenuItem('Infinite', -1),
			    ]}
                select={(value) => $config.setNum(Opt.DRIVE_SPEED, value)}
                selectedTag={$config.getNum(Opt.DRIVE_SPEED)}
        />
    </ConfigSection>
    <ConfigSection name="Sprites">
        <ConfigItem
                name="Sprite-Sprite Collision Detection"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.CLX_SPR_SPR, value)}
                selectedTag={$config.getNum(Opt.CLX_SPR_SPR)}
        />
        <ConfigItem
                name="Sprite-Playfield Collision Detection"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.CLX_SPR_PLF, value)}
                selectedTag={$config.getNum(Opt.CLX_SPR_PLF)}
        />
        <ConfigItem
                name="Playfield-Playfield Collision Detection"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.CLX_PLF_PLF, value)}
                selectedTag={$config.getNum(Opt.CLX_PLF_PLF)}
        />
    </ConfigSection>
    <ConfigSection name="Keyboard">
        <ConfigItem
                name="Transmit Keycodes Bit by Bit"
                items={[
                    new MenuItem('Yes', 1),
                    new MenuItem('No', 0)
                    ]}
                select={(value) => $config.setNum(Opt.ACCURATE_KEYBOARD, value)}
                selectedTag={$config.getNum(Opt.ACCURATE_KEYBOARD)}
        />
    </ConfigSection>
</div>
