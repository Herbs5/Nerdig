/*
 *  tracker/GlobalChordConfig.cpp
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
 *  GlobalChordConfig.cpp
 *  MilkyTracker
 *
 *  Created by Peter Barth on 21.11.05.
 *
 */

#include "GlobalChordConfig.h"
#include "Tracker.h"

//  aktuelle 8 Chords  und Rest
const PPChord& GlobalChordConfig::getChord(GlobalChords whichChord) const
{
	switch (whichChord)
	{
		case Chord_Tonika:
	//		return Tonika;
		case Chord_Dominante:
	//		return _Dominante;
		case Chord_Subdominante:
	//		return _Subdominante;
		case Chord_7:
	//		return _7;
		case Chord_m7:
	//		return _m7;
		case Chord_Moll:
	//		return _Moll;
		// Cursor line
		case Chord_Moll_7:
	//		return _Moll_7;

		case Chord_Moll_m7:
	//		return _Moll_m7;
		case Chord_Dominante2:
	//		return _Dominante2;
		case Chord_Subdominante2:
	//		return _Subdominante2;
		case Chord_Dominante3:
	//		return _Dominante3;
		case Chord_Subdominante3:
	//		return _Subdominante3;

		case Chord_Dominante4:
	//		return _Dominante4;

		case Chord_Subdominante4:
	//		return _Subdominante4;
			
		case Chord_79:
	//		return _79;			
		case Chord_sus2:
	//		return _sus2;
		case Chord_a:
	//		return ;

		case Chord_b:
	//		return ;

		case Chord_c:
	//		return ;
		case Chord_d:
	//		return;

		default:
			return dummy;
	}
}

const char* GlobalChordConfig::getChordReadableDescription(GlobalChords whichChord) const
{
	switch (whichChord)
	{
		// Pattern chord identfiers
		case Chord_Tonika:
			return "Tonika";
		case Chord_Dominante:
			return "1_Dominante";
		case Chord_Subdominante:
			return "1_Subdominante";
		case Chord_7:
			return "_7";
		case Chord_m7:
			return "_m7";
		case Chord_Moll:
			return "_Moll";
		case Chord_Moll_7:
			return "_Moll_7";
		case Chord_Moll_m7:
			return "_Moll_m7";
		case Chord_Dominante2:
			return "_Dominante2";
		case Chord_Subdominante2:
			return "_Subdominante2";
		case Chord_Dominante3:
			return "_Dominante3";
		case Chord_Subdominante3:
			return "_Subdominante3";

		case Chord_Dominante4:
			return "_Dominante4";
		case Chord_Subdominante4:
			return "_Subdominante4";
		case Chord_79:
			return "_79";
		case Chord_sus2:
			return "_sus2";
		case Chord_a:
			return "_a";
		case Chord_b:
			return "_b";

		case Chord_c:
			return "_c";
		case Chord_d:
			return "_d";

		default:
			return NULL;
	}
}

void GlobalChordConfig::setChord(GlobalChords whichChord, const PPChord& chord)
{
	switch (whichChord)
	{
		// Pattern chord identfiers
		case Chord_Tonika:
			//Tonika = chord;
			break;
		case Chord_Dominante:
//			_Dominante = chord;
			break;
		case Chord_Subdominante:
//			_Subdominante = chord;
			break;
/*		case Chord_7:
			_7 = chord;
			break;
		case Chord_m7:
			_m7 = chord;
			break;

		case Chord_Moll:
			_Moll = chord;
			break;
		case Chord_Moll_7:
			_Moll_7 = chord;
			break;
		case Chord_Moll_m7:
			_Moll_m7 = chord;
			break;
		case Chord_Dominante2:
			_Dominante2 = chord;
			break;
		case Chord_Subdominante2:
			_Subdominante2 = chord;
		case Chord_Dominante3:
			_Dominante3 = chord;
			break;
		case Chord_Subdominante3:
			_Subdominante3 = chord;
			break;
		
		case Chord_Dominante4:
			_Dominante4 = chord;
			break;

		case Chord_Subdominante4:
			_Subdominante4 = chord;
			break;
			
		case Chord_79:
			_79 = chord;
			break;

		case Chord_sus2:
			_sus2 = chord;
			break;
			
		case Chord_a:
			_a = chord;
			break;

		case Chord_b:
			_b = chord;
			break;

		case Chord_c:
			_c = chord;
			break;

		case Chord_d:
			_d = chord;
			break;				*/
	}	
}

