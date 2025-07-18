// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "VideoPort.h"
#include "Agnus.h"
#include "Denise.h"

namespace vamiga {

VideoPort::VideoPort(Amiga &ref) : SubComponent(ref)
{
    // Create random noise data
    noise.alloc(2 * PIXELS);
    for (isize i = 0; i < noise.size; i++) {
        noise[i] = rand() % 2 ? Texture::black : Texture::white;
    }

    // Setup the white-noise framebuffer (redirect the data source)
    whiteNoise.pixels.dealloc();
    whiteNoise.pixels.ptr = noise.ptr;
    whiteNoise.pixels.size = PIXELS;

    // Setup the blank framebuffer
    for (isize i = 0; i < blank.pixels.size; i++) {
        blank.pixels.ptr[i] = 0xFF000000;
    }
};

VideoPort::~VideoPort()
{
    // Don't free the white-noise framebuffer as it points to managed data
    whiteNoise.pixels.ptr = nullptr;
    whiteNoise.pixels.size = 0;
}

void
VideoPort::_dump(Category category, std::ostream &os) const
{
    using namespace util;

    if (category == Category::Config) {

        dumpConfig(os);
    }

    if (category == Category::State) {

    }
}

i64
VideoPort::getOption(Opt option) const
{
    switch (option) {

        case Opt::VID_WHITE_NOISE:   return config.whiteNoise;

        default:
            fatalError;
    }
}

void
VideoPort::checkOption(Opt opt, i64 value)
{
    switch (opt) {

        case Opt::VID_WHITE_NOISE:

            return;

        default:
            throw AppError(Fault::OPT_UNSUPPORTED);
    }
}

void
VideoPort::setOption(Opt opt, i64 value)
{
    checkOption(opt, value);

    switch (opt) {

        case Opt::VID_WHITE_NOISE:

            config.whiteNoise = (bool)value;
            return;

        default:
            fatalError;
    }
}

void 
VideoPort::cacheInfo(VideoPortInfo &result) const
{

}

void 
VideoPort::cacheStats(VideoPortStats &result) const
{

}

const Texture &
VideoPort::getTexture(isize offset) const
{
    if (isPoweredOn()) {

        auto &result = denise.pixelEngine.getStableBuffer(offset);
        info.latestGrabbedFrame = result.nr;
        return result;
    }
    if (config.whiteNoise) {

        whiteNoise.pixels.ptr = noise.ptr + (rand() % PIXELS);
        whiteNoise.nr++;
        whiteNoise.prevlof = whiteNoise.lof;
        whiteNoise.lof = !whiteNoise.lof;

        return whiteNoise;
    }

    return blank;
}

void 
VideoPort::buffersWillSwap()
{
    // Check if the texture has been grabbed
    auto grabbed = info.latestGrabbedFrame;
    auto current = denise.pixelEngine.getStableBuffer().nr;

    if (grabbed < current) {

        stats.droppedFrames++;
        debug(VID_DEBUG, "Frame %lld dropped (total: %ld latest: %lld)\n", 
            current, stats.droppedFrames, grabbed);
    }
}

void
VideoPort::findInnerArea(isize &x1, isize &x2, isize &y1, isize &y2) const
{
    auto &buffer = denise.pixelEngine.getStableBuffer();
    
    // Start with a large box
    x1 = 4 * HBLANK_CNT;
    x2 = 4 * PAL::HPOS_MAX;
    y1 = agnus.isPAL() ? PAL::VBLANK_CNT : NTSC::VBLANK_CNT;
    y2 = agnus.isPAL() ? PAL::VPOS_CNT_SF : NTSC::VPOS_CNT_SF;
    
    // Get a reference pixel from the border
    auto border = buffer.pixels[y1 * HPIXELS + x1];
    
    // Shrink the box as much as possible
    auto emptyRow = [&](isize row) {
        
        for (isize x = x1; x < x2; x++) {
            if (buffer.pixels[row * HPIXELS + x] != border) { return false; }
        }
        return true;
    };
    auto emptyCol = [&](isize col) {
        
        for (isize y = y1; y < y2; y++) {
            if (buffer.pixels[y * HPIXELS + col] != border) { return false; }
        }
        return true;
    };
    while (x2 > 0  && emptyCol(x2)) { x2 -= 1; }
    while (x1 < x2 && emptyCol(x1)) { x1 += 1; }
    while (y2 > 0  && emptyRow(y2)) { y2 -= 1; }
    while (y1 < y2 && emptyRow(y1)) { y1 += 1; }
    // printf("Shrinked box: (%ld,%ld) - (%ld,%ld)\n", x1, y1, x2, y2);

    // Return a zero rect if the box is invalid
    if (x2 <= x1 || y2 <= y1) { x1 = x2 = y1 = y2 = 0; }
}

void
VideoPort::findInnerAreaNormalized(double &x1, double &x2, double &y1, double &y2) const
{
    isize ix1, ix2, iy1, iy2;
    findInnerArea(ix1, ix2, iy1, iy2);
    
    x1 = double(ix1) / HPIXELS;
    x2 = double(ix2) / HPIXELS;
    y1 = double(iy1) / VPIXELS;
    y2 = double(iy2) / VPIXELS;
}

}
