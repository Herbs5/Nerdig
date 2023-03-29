/*
 *  tracker/SectionGuitar.h
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
 *  SectionGuitar.h
 *  MilkyTracker
 *
 *  Created by Peter Barth on 15.04.05.
 *
 */

#ifndef SECTIONGUITAR__H
#define SECTIONGUITAR__H

#include "BasicTypes.h"
#include "Event.h"
#include "SectionAbstract.h"

class PPControl;
class PianoControl;
class GuitarControl;
class PPContainer;

class SectionGuitar : public SectionAbstract
{
private:
	PPContainer* containerEntire;
// 	PPContainer* containerSampleSlider;
//	PPContainer* containerInstrumentSlider;
	PianoControl* pianoControl;
	GuitarControl* guitarControl;
	bool visible;

	
protected:
	virtual void showSection(bool bShow);

public:
	SectionGuitar(Tracker& tracker);
	virtual ~SectionGuitar();

	// PPEvent listener
	virtual pp_int32 handleEvent(PPObject* sender, PPEvent* event);
	
	virtual void init();

	virtual void init(pp_int32 x, pp_int32 y);

	void realign();

	virtual void show(bool bShow);
	
//	void updateSampleSliders(bool repaint = true);
//	void updateInstrumentSliders(bool repaint = true);
	
	virtual void update(bool repaint = true);

	virtual void notifyTabSwitch();

	virtual void notifySampleSelect(pp_int32 index);

	void updateAfterLoad();

	void resetPianoAssignment();

	PianoControl* getPianoControl() { return pianoControl; }
	GuitarControl* getGuitarControl() { return guitarControl; }

	bool isVisible() const { return visible; }
	
	
private:

	// Responder should be friend
	friend class DialogResponderGuitar;	

	friend class Tracker;
};

#endif

