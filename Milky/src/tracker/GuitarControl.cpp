
#include "GuitarControl.h"

#include "Screen.h"
#include "GraphicsAbstract.h"
#include "Font.h"
#include "Tools.h"
#include "PatternTools.h"
#include "Tracker.h"
#include "TrackerConfig.h"
#include "PatternEditorControl.h"
#include "Container.h"
#include "ControlIDs.h"
#include "blackKeys.h"


static const int offsets[] =
{
		4,
		-1,
		-5,
		-10,
		-15,
		-20
};


static const int lcircle[] =
{
	0,
	-5,
	2,
	-3,
	4,
	-1,
	6,
	1,
	-4,
	3,
	-2,
	5,
};

pp_int32 Background = 11;

pp_int32 GuitarControl::KEYWIDTH()
{
//	return XMAX()/piano
return 0;
}

GuitarControl::GuitarControl(pp_int32 id, 
						   PPScreen* parentScreen, 
						   EventListenerInterface* eventListener, 
						   const PPPoint& location, 
						   const PPSize& size, 
						   pp_uint8 numNotes,
						   bool border) :
	PPControl(id, parentScreen, eventListener, location, size),
	border(border),
	NUMNOTES(numNotes),
	borderColor(&ourOwnBorderColor),
	mode(ModeEdit)
{
	// default color
	ourOwnBorderColor.set(192, 192, 192);

	startPos = 0;
	vw = 0;
	stemm = 60;
	
	caughtControl = NULL;	
	controlCaughtByLMouseButton = controlCaughtByRMouseButton = false;

	nbu = new pp_uint8[NUMNOTES];
	memset(nbu, 0, NUMNOTES);

	keyState = new KeyState[NUMNOTES];

	sampleIndex = 0;
	large = false;
	channel = 0;
	for (pp_int32 s = 0; s < 6; s++)
	{
		usedS[s] = 0;
		griff[s] = 0;
	}
	chomode = ChoChoose; 
 
}

GuitarControl::~GuitarControl()
{
	delete[] nbu;
	
	delete[] keyState;
}


void GuitarControl::setLocation(const PPPoint& location)
{
	PPControl::setLocation(location);
}

void  GuitarControl::refresh()
{
	parentScreen->paintControl(this);
}

void GuitarControl::toggleBackground()
{
	
	if(Background == 11)
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys0[s];
		}
	else if(Background == 10)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys1[s];
		}
	else if(Background == 9)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys2[s];
		}
	else if(Background == 8)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys3[s];
		}
	else if(Background == 7)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys4[s];
		}
	else if(Background == 6)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys5[s];
		}				
	else if(Background == 5)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys6[s];
		}	
	else if(Background == 4)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys7[s];
		}		
	else if(Background == 3)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys8[s];
		}			
	else if(Background == 2)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys9[s];
		}			
	else if(Background == 1)		
		for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeys10[s];
		}				
	else if(Background == 0)
	{
		 Background=12;
		 for (pp_int32 s = 0; s < 12; s++)
		{
			blackKeys[s] = blackKeyso[s];
		}
	}
	Background--;	
}

void GuitarControl::paint(PPGraphicsAbstract* g)
{
	if (!isVisible())		return;
	g->setRect(location.x, location.y, location.x + size.width, location.y + size.height);

	pp_int32 bundW =size.width  / 24, bund;
	pp_int32 saiteH = size.height / 6, saite;
	pp_int32 note, nota;  

	PPRect hals;

	g->setColor(233,244,225);
	g->fill();
//nn	keyState[note].visab=false;	//g->setPixel(	55,55);			

 	for (pp_int32 s = 0; s < 6; s++)
	{
		usedS[s]=0;
		hals.y1 = location.y + s * saiteH;
		hals.y2 = hals.y1 + saiteH;
		
		//  background pattern
		for (pp_int32 b = 24; b >=0; b--)
		{
			note = stemm + offsets[s] + b;
			
			if(blackKeys[ note - abs(note/12)*12])
			{
				hals.x1 = location.x + (b - vw+qvw) * bundW ;
				if(hals.x1 >location.x + bundW *24) hals.x1-= bundW * 24;
				if(hals.x1 < location.x) hals.x1+= bundW * 24;
				if(hals.x1 <location.x + bundW *22)
				{
					hals.x2 = hals.x1 + bundW ;
					g->setColor(4,4,44);
					g->setRect(hals);
					g->fill();
				}
			}
		}
		if(this->channel)   //   akkord stroke control
		{
			hals.x1=bundW/5 +  location.x + 22  * bundW ;
			hals.x2 = hals.x1 + bundW - bundW/5 ;
			if(s==2 || s==3)
				g->setColor(122,112,142);
			else	
				g->setColor(172,152,81);
			g->setRect(hals);
			g->fill();
			
		}
		if(griff[s])  //   akkord
		{
			nota = griff[s];
			hals.x1=bundW/3 +  location.x + (nota -stemm - offsets[s])  * bundW ;
			//hals.y1;
			hals.x2 = hals.x1 + bundW/3 ;
			//hals.y2;
			if(blackKeys[ nota -qvw +vw -abs((nota -qvw +vw)/12)*12])
				g->setColor(166,122,222);
			else	
				g->setColor(22,22,77);
			g->setRect(hals);
			g->fill();
			
		//  akkord notewise control
			hals.x1=bundW/5 +  location.x + 23  * bundW ;
			hals.y1+=2;
			hals.x2 = hals.x1 + bundW - bundW/5 ;
			hals.y2-=2;
			g->setColor(122,152,131);
			g->setRect(hals);
			g->fill();

		}
		
		//  actuel note, pressed ore played:
		if(this->channel)   //   
		{
			for (pp_int32 b = 24; b >=0; b--)
			{
				note = stemm + offsets[s] + b;
				if(keyState[note].chanel==channel+s)
				{
					hals.x1=bundW/3 +  location.x + b  * bundW ;
					//hals.y1++;
					hals.x2 = hals.x1 + bundW/3 ;
					//hals.y2--;

					if(blackKeys[ note -qvw +vw -abs((note -qvw +vw)/12)*12])
						g->setColor(144,194,104);
					else	
						g->setColor(111,94,4);
					g->setRect(hals);
					g->fill();
					continue;
				}
			}
		}
		else
		{
			for (pp_int32 b = 24; b >=0; b--)
			{
				if(!usedS[s])
				{
					note = stemm + offsets[s] + b;
					if (keyState[note].pressed  && ! keyState[note].muted)
					{		
						if(!keyState[note].visab && keyState[note].rang <=7) 
						{
							usedS[s]=note;
							keyState[note].visab=true;  // show note only on one string
							hals.x1=bundW/3 +  location.x + b  * bundW ;
							//hals.y1+=3;
							hals.x2 = hals.x1 + bundW/3 ;
							//hals.y2-=3;
//goog					if(keyState[note].chanel==3)
							if(blackKeys[ note -qvw +vw -abs((note -qvw +vw)/12)*12])
								g->setColor(144,194,104);
							else	
								g->setColor(111,94,4);
							g->setRect(hals);
							g->fill();
			//		}
						}	
					}	
				}
			}
		}
	}
	
		

	hals.x1 = location.x + bundW-2 ;
	hals.y1 = location.y ;
	hals.x2 = hals.x1 + 4 ;
	hals.y2 = hals.y1 + saiteH * 6;
	g->setColor(144,144,146);
	g->setRect(hals);
	g->fill();
	g->setColor(199,199,201);
	g->drawVLine( hals.y1 +1, hals.y2 -2, hals.x1 +2);
}


void GuitarControl::noteevent(pp_int32 note)
{
	
		pp_int32 v = (1 << 16) + currentSelectedNote;
		PPEvent e(eSelection, &v, sizeof(v));
		eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &e);

	currentSelectedNote = note;
	keyState[note].pressed = true;
	if(this->channel)
	{
	//	keyState[note].chanel==channel+ currentString;
		PPEvent se(eSelection, &currentString, sizeof(currentString));
		eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &se);
	}
	PPEvent ef(eSelection, &note, sizeof(note));
	eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &ef);
}

void GuitarControl::akkord( pp_int32 string )
{
	if( string < 2)  //  stroke top down
	{
		for (pp_int32 s = 0; s < 6; s++)
		{
			if( griff[s] )
			{
				pp_int32 note = griff[s];
				currentString =s;
				noteevent( note );
			}
		}
	}
	else if( string < 4)  //  stroke at once
	{
//		PatternEditorControl* pEdi = Tracker->getPatternEditorControl();
PatternEditorControl* pEdiCo = static_cast<PatternEditorControl*>(parentScreen->getControlByID(PATTERN_EDITOR));	
		pEdiCo->setRowAdvance(false);
		pp_int32 Row ;
		Row = pEdiCo->getRow();
		for (pp_int32 s = 0; s < 6; s++)
		{
			if( griff[s] )
			{
				pp_int32 note = griff[s];
				currentString =s;
				noteevent( note );
				
	/*			if(s<5)
				{
					pEdiCo->setRow(Row  -1, true);	

				}*/
			}
		}
		pEdiCo->setRowAdvance(true);
		pEdiCo->setRow(Row  +1 , true);	

	}
	else if( string < 6)  //  stroke top down
	{
		for (pp_int32 s = 5; s > -1; s--)
		{
			if( griff[s] )
			{
				pp_int32 note = griff[s];
				currentString =s;
				noteevent( note );
			}
		}
	}
}


pp_int32 GuitarControl::dispatchEvent(PPEvent* event)
{ 
	if (eventListener == NULL)
		return -1;

	//if (!visible)
	//	return 0;

	switch (event->getID())
	{
		case eLMouseDown:
		{
			PPPoint* p = (PPPoint*)event->getDataPtr();

				pp_int32 note =  positionToNote(*p,0);

				if (note != -1)
				{
					if(chomode==ChoChoose || chomode==ChoPlay)
					{
								
						switch (mode)
						{
							case ModeEdit:
							{
								nbu[note] = (pp_uint8)sampleIndex & 0xf;
								PPEvent e(eValueChanged, &nbu, sizeof(pp_uint8*));	
								eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &e);
								break;
							}

							case ModePlay:
							{
								if(note - stemm - offsets[currentString] == 23 && griff[currentString])
									{
										note = griff[currentString];
										noteevent( note );
									}
								else if(note - stemm - offsets[currentString] == 22 && griff[currentString])	
										akkord(currentString);
								else noteevent( note );
								break;
							}
						}
					}
					else if(chomode==ChoModify)
					{
						if(this->channel)
						{
							if(note - stemm - offsets[currentString] == 23)
								griff[currentString] = 0;
							else
								griff[currentString] = note;
						}		
					}
					else if(chomode==ChoBG)
					{
						if(blackKeys[ note -qvw +vw -abs((note -qvw +vw)/12)*12])
//							if(blackKeys[note - abs(note/12)*12] )
//								blackKeys[note - abs(note/12)*12] = false;
								blackKeys[ note -qvw +vw -abs((note -qvw +vw)/12)*12]=false;
							else	
								blackKeys[ note -qvw +vw -abs((note -qvw +vw)/12)*12]=true;
//								blackKeys[note - abs(note/12)*12] = true;

					}
					
					parentScreen->paintControl(this);
				}

			break;
		}

		case eRMouseDown:
		{
			PPPoint* p = (PPPoint*)event->getDataPtr();

			pp_int32 note =  positionToNote(*p,0);

			if (note != -1)
			{
				if(this->channel)
				{
					if(note - stemm - offsets[currentString] == 23)
						griff[currentString] = 0;
					else
						griff[currentString] = note;
				}
				parentScreen->paintControl(this);
			}
			break;
		}
		case eRMouseUp:
		{
			controlCaughtByRMouseButton = false;
			if (caughtControl && !controlCaughtByLMouseButton && !controlCaughtByRMouseButton)
			{
				caughtControl->dispatchEvent(event);
				caughtControl = NULL;			
				break;
			}
			break;
		}

		case eLMouseUp:
			if (caughtControl == NULL)
			{
				pp_int32 note = currentSelectedNote;

				if (note != -1)
				{
					
					switch (mode)
					{
						case ModePlay:
						{
							pp_int32 v = (1 << 16) + currentSelectedNote;
							keyState[currentSelectedNote].pressed = false;
							PPEvent e(eSelection, &v, sizeof(v));
							eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &e);
							break;
						}
					}
					
					parentScreen->paintControl(this);
				}
				break;
			}
			
			controlCaughtByLMouseButton = false;
			if (!controlCaughtByLMouseButton && !controlCaughtByRMouseButton)
			{
				caughtControl->dispatchEvent(event);
				caughtControl = NULL;
			}
			break;

		case eLMouseDrag:
		{
			if (caughtControl && controlCaughtByLMouseButton)
			{
				caughtControl->dispatchEvent(event);
				break;
			}
			
			pp_int32 note = positionToNote(*(PPPoint*)event->getDataPtr(),1);
			if(note - stemm - offsets[currentString] == 22 && griff[currentString])	
					break;
			if(note - stemm - offsets[currentString] == 23 && griff[currentString])
				note = griff[currentString];
//			parentScreen->paintControl(this);

			if (note != -1)
			{
				
				switch (mode)
				{
					case ModeEdit:
					{
						nbu[note] = (pp_uint8)sampleIndex & 0xf;
						
						PPEvent e(eValueChanged, &nbu, sizeof(pp_uint8*));	
						
						eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &e);
						break;
					}
					
					case ModePlay:
					{
						if (note == currentSelectedNote)
							break;
					
						//  note off  ??
//						pp_int32 v = (1 << 16) + currentSelectedNote;
	//					PPEvent e(eSelection, &v, sizeof(v));
		//				eventListener->handleEvent(reinterpret_cast<PPObject*>(this), &e);
						
						noteevent( note );	
						break;
					}
				}
				
				parentScreen->paintControl(this);
			}

			break;
		}

		case eRMouseDrag:
		{
			if (caughtControl && controlCaughtByRMouseButton)
				caughtControl->dispatchEvent(event);
			break;
		}
		
		case eRMouseRepeat:
		{
			if (caughtControl && controlCaughtByRMouseButton)
				caughtControl->dispatchEvent(event);
			break;
		}
		
		case eMouseWheelMoved:
		{
	//		TMouseWheelEventParams* params = (TMouseWheelEventParams*)event->getDataPtr();
	/*		if (params->delta > 0 && hScrollbar)
			{
				PPEvent e(eBarScrollUp);
				handleEvent(reinterpret_cast<PPObject*>(hScrollbar), &e);
			}
		*/	
			event->cancel();
			break;
		}
		
		default:
			if (caughtControl == NULL)
				break;

			caughtControl->dispatchEvent(event);
			break;

	}

	return 0;
}

pp_int32 GuitarControl::handleEvent(PPObject* sender, PPEvent* event)
{	
	parentScreen->paintControl(this);

	return 0;
}

void GuitarControl::setSize(const PPSize& size)
{
	PPControl::setSize(size);

}

void GuitarControl::setSampleTable(const pp_uint8* nbu)
{
	if (nbu == NULL)
	{
		memset(this->nbu, 0, NUMNOTES);
		return;
	}

	memcpy(this->nbu, nbu, NUMNOTES);
}

void GuitarControl::pressNote(pp_int32 note, bool pressed, bool muted,  pp_int32 chan, bool visa )
{
	if (note >= 0 && note < NUMNOTES)
	{
		keyState[note].pressed = pressed;
		keyState[note].muted = muted;
		keyState[note].chanel = chan;
		keyState[note].visab = visa;
	}
}

void GuitarControl::rangNote(pp_int32 note, pp_int32 inc)
{
	if(inc)
		keyState[note].rang+= inc;
	else
	    keyState[note].rang = 0;
}

bool GuitarControl::getNoteState(pp_int32 note) const
{
	if (note >= 0 && note < NUMNOTES)
		return keyState[note].pressed;
	return false;
}

void GuitarControl::assureNoteVisible(pp_int32 note)
{
return;
}

pp_int32 GuitarControl::positionToNote(PPPoint cp, int drag)
{
	if (sampleIndex < 0)
		return -1;

	cp.x -= location.x + 1;
	cp.y -= location.y + 1;
		
	if (cp.x < 0 || cp.x >= size.width || cp.y < 0 || cp.y >= size.height)
		return -1;
	
	pp_int32 note;	
	pp_int32 bundW = (size.width ) / 24;
	pp_int32 saiteH = size.height / 6;		
	if(drag)
	{
		pp_int32 hioff = saiteH/4, wioff = bundW/4;
		pp_int32 s1 = (cp.y+hioff)/saiteH, s2 = (cp.y-hioff)/saiteH;
		pp_int32 b1 = (cp.x+wioff)/bundW, b2 = (cp.x-wioff)/bundW ;
		if(s1 == s2 && b1 == b2)
			note  = stemm + offsets[s1] + b1;
		else
		{
			s1 = cp.y/saiteH; 
			b1 = cp.x/bundW;
			note  = stemm + offsets[s1] + b1;
			if(note - stemm - offsets[currentString] == 23 && griff[currentString])
				;
			else	
				return -1;	
		}
		currentString=s1;
	}
	else if(!drag)
	{
		pp_int32 s = cp.y/saiteH, b = cp.x/bundW;
		note  = stemm + offsets[s] + b;
		currentString=s;
	}
				
					
	if (note < 0 || note >= NUMNOTES)
		return -1;
// PatternEditorControl* pEdi = static_cast<PatternEditorControl*>(parentScreen->getControlByID(PATTERN_EDITOR));	
//pEdi->setChannel(s+1,0);
//pEdi->unmuteAll();
//container->
	return note;
}

/*
 *  tracker/GuitarControl.cpp
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
