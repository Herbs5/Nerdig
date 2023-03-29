#ifndef GUITARCONTROL__H
#define GUITARCONTROL__H

#include "BasicTypes.h"
#include "Control.h"
#include "Event.h"

class GuitarControl : public PPControl, public EventListenerInterface
{
public:
	enum Modes
	{
		ModeEdit = 0,
		ModePlay = 1,
		ModeStep =2
	};
	pp_int32 vw, qvw, stemm;
	pp_int32 usedS[6], griff[6];  
	pp_int32 currentString;
	pp_int32 channel;
	bool large;
	enum ChoModes
	{
		ChoChoose = 0,
		ChoModify = 1,
		ChoPlay =2,
		ChoBG =3
	};
	ChoModes chomode;
//	PPSystemString lastChordFile;

private:

	pp_int32 KEYWIDTH();
	
	const pp_uint8 NUMNOTES;

	bool border;
	PPColor ourOwnBorderColor;
	const PPColor* borderColor;

	PPControl* caughtControl;
	bool controlCaughtByLMouseButton, controlCaughtByRMouseButton;

	pp_int32 startPos;
	pp_int32 visibleWidth;
	pp_int32 visibleHeight;

	pp_int32 sampleIndex;

	pp_uint8* nbu;

	struct KeyState
	{
		bool pressed;
		bool muted;
		int chanel;
		int	  rang;
		bool visab;
		
		KeyState() :
			pressed(false),
			muted(false),
			chanel(0),
			rang(0),
			visab(false)
		{
		}
	};
	
	KeyState* keyState;

	Modes mode;

	pp_int32 currentSelectedNote;
	
public:
	GuitarControl(pp_int32 id, 
				 PPScreen* parentScreen, 
				 EventListenerInterface* eventListener, 
				 const PPPoint& location, 
				 const PPSize& size, 
				 pp_uint8 numNotes,
				 bool border = false);

	~GuitarControl();

//	void noteevent(pp_int32 );
	void setMode(Modes mode) { this->mode = mode; }
	Modes getMode() const { return mode; }

	void setBorderColor(const PPColor& color) { this->borderColor = &color; }

	void setSampleTable(const pp_uint8* nbu);
	void toggleBackground();

	void setSampleIndex(pp_int32 index) { sampleIndex = index; }

	void pressNote(pp_int32 note, bool pressed, bool muted = false, pp_int32 chan = 0,  bool visa = false);

	void rangNote(pp_int32 note, pp_int32 inc);
	bool getNoteState(pp_int32 note) const;

	void assureNoteVisible(pp_int32 note);
	void  refresh();
	void noteevent(pp_int32 note);
	void akkord(pp_int32 string );
	// from PPControl
	virtual void paint(PPGraphicsAbstract* graphics);	
	virtual pp_int32 dispatchEvent(PPEvent* event);
	virtual pp_int32 handleEvent(PPObject* sender, PPEvent* event);

	virtual void setLocation(const PPPoint& location);
	virtual void setSize(const PPSize& size);

private:
	pp_int32 getMaxWidth();

	pp_int32 positionToNote(PPPoint cp, pp_int32 drag);
};


#endif

/*             GuitarControl.h
 * 
 * made from tracker/PianoControl.h
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
