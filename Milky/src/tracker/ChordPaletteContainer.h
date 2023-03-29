/*
 *  tracker/ChordPaletteContainer.h
 *
 *  Copyright 2009 Peter Barth
 *
 *  This file is part of Milkytracker.
 *
 *  Milkytracker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Milkytracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Milkytracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 *  ChordPaletteContainer.h
 *  MilkyTracker
 *
 *  Created by Peter Barth on 21.11.05.
 *
 */

#ifndef CHORDPALETTECONTAINER__H
#define CHORDPALETTECONTAINER__H

#include "BasicTypes.h"

// Verx simple chord palette, limited chords
struct TChordPalette
{
	// Should be more than enough
	pp_uint8 numChords;
	PPChord chords[256];
};

class ChordPaletteContainer 
{
private:
	TChordPalette* palettes;
	pp_int32 numPalettes;

public:
	ChordPaletteContainer(pp_int32 num);
	~ChordPaletteContainer();
	
	void store(pp_int32 index, const TChordPalette& palette);
	const TChordPalette* restore(pp_int32 index);

	static PPString encodePalette(const TChordPalette& palette);
	static TChordPalette decodePalette(const PPString& str);
};

#endif
