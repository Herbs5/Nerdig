/*
 *  tracker/ChordPaletteContainer.cpp
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
 *  ChordPaletteContainer.cpp
 *  MilkyTracker
 *
 *  Created by Peter Barth on 21.11.05.
 *
 */

#include "ChordPaletteContainer.h"
#include "PatternTools.h"

ChordPaletteContainer::ChordPaletteContainer(pp_int32 num) :
	palettes(NULL)
{
	palettes = new TChordPalette[num];
	
	memset(palettes, 0, num*sizeof(TChordPalette));
	
	numPalettes = num;
}

ChordPaletteContainer::~ChordPaletteContainer()
{
	delete[] palettes;
}

void ChordPaletteContainer::store(pp_int32 index, const TChordPalette& palette)
{
	if (index < 0 || index > numPalettes - 1)
		return;
		
	palettes[index] = palette;
}

const TChordPalette* ChordPaletteContainer::restore(pp_int32 index)
{
	if (index < 0 || index > numPalettes - 1)
		return NULL;

	return palettes + index;
}

PPString ChordPaletteContainer::encodePalette(const TChordPalette& palette)
{
	char buffer[10];
	
	// Convert number of chords
	PatternTools::convertToHex(buffer, 8, 2); // palette.numChords, 2);
	
	PPString str = buffer;
	
	for (pp_int32 i = 0; i < 8; i++)   //palette.numChords; i++)
	{
		// E
		PatternTools::convertToHex(buffer, palette.chords[i].E, 2);
		str.append(buffer);

		// a
		PatternTools::convertToHex(buffer, palette.chords[i].a, 2);
		str.append(buffer);

		// d
		PatternTools::convertToHex(buffer, palette.chords[i].d, 2);
		str.append(buffer);		
	
		// g
		PatternTools::convertToHex(buffer, palette.chords[i].g, 2);
		str.append(buffer);

		// b
		PatternTools::convertToHex(buffer, palette.chords[i].b, 2);
		str.append(buffer);

		// e
		PatternTools::convertToHex(buffer, palette.chords[i].e, 2);
		str.append(buffer);			
	}
	
	return str;
}

static pp_uint8 getNibble(const char* str)
{
	if (*str >= '0' && *str <= '9')
		return (*str - '0');
	if (*str >= 'A' && *str <= 'F')
		return (*str - 'A' + 10);
	if (*str >= 'a' && *str <= 'f')
		return (*str - 'a' + 10);
		
	return 0;
}

static pp_uint8 getByte(const char* str)
{
	return (getNibble(str)<<4) + getNibble(str+1);
}

TChordPalette ChordPaletteContainer::decodePalette(const PPString& str)
{
	TChordPalette palette;
	
	const char* ptr = str;
	
	palette.numChords = 8; // getByte(ptr);
	ptr+=2;
	
	for (pp_int32 i = 0; i < 8 ; i++)  //   palette.numChords; i++)
	{
		palette.chords[i].E = getByte(ptr);
		ptr+=2;
		palette.chords[i].a = getByte(ptr);
		ptr+=2;
		palette.chords[i].d = getByte(ptr);
		ptr+=2;
		palette.chords[i].g = getByte(ptr);
		ptr+=2;
		palette.chords[i].b = getByte(ptr);
		ptr+=2;
		palette.chords[i].e = getByte(ptr);
		ptr+=2;
	}
	
	return palette;
}
