/*
 *  tracker/ChordExportImport.h
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
 *  ChordExportImport.h
 *  MilkyTracker
 *
 *  Created by Peter Barth on Sun Mar 13 2005.
 *
 */
 
#ifndef __CHORDEXPORTIMPORT_H__
#define __CHORDEXPORTIMPORT_H__

#include "BasicTypes.h"
#include "ChordPaletteContainer.h"

class ChordExportImport
{
private:
	PPSystemString fileName;

public:
	ChordExportImport(const PPSystemString& fileName) :
		fileName(fileName)
	{
	}
	
	bool importChordPalette(TChordPalette& palette);
	bool exportChordPalette(const TChordPalette& palette);	
};

#endif
