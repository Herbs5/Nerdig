/*
 *  tracker/GlobalChordConfig.h
 *
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
 *  GlobalChordConfig.h
 *  MilkyTracker
 *
 *  Created by Peter Barth on 21.11.05.
 *
 */

#ifndef GLOBALCHORDCONFIG__H
#define GLOBALCHORDCONFIG__H

#include "BasicTypes.h"
#include "Singleton.h"

class GlobalChordConfig : public PPSingleton<GlobalChordConfig>
{
private:
	PPChord dummy;
	
public:
	enum GlobalChords
	{
		// Pattern chord identfiers
		Chord_Tonika = 0,
		Chord_Dominante,
		Chord_Subdominante,
		Chord_7,
		Chord_m7,
		Chord_Moll,
		Chord_Moll_7,
		Chord_Moll_m7,
		Chord_Dominante2,
		Chord_Subdominante2,
		Chord_Dominante3,
		Chord_Subdominante3,
		Chord_Dominante4,
		Chord_Subdominante4,
		Chord_79,
		Chord_sus2,
		Chord_a,
		Chord_b,
		Chord_c,
		Chord_d,

		// Unused, endmarker
		ChordLast = Chord_b + 24 // remember to subtract one from the last value 
								 //	every time you add another chord to the list
	};

public:
	const PPChord& getChord(GlobalChords whichChord) const;
	const char* getChordReadableDescription(GlobalChords whichChord) const;

	void setChord(GlobalChords whichChord, const PPChord& chord);
	
	friend class PPSingleton<GlobalChordConfig>;
};

#endif

