/*
 *  tracker/SectionGuitar.cpp
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

#include "SectionGuitar.h"
#include "Tracker.h"
#include "TrackerConfig.h"
#include "TabManager.h"
#include "ModuleEditor.h"
#include "SampleEditor.h"
#include "PianoControl.h"
#include "GuitarControl.h"
#include "PatternTools.h"
#include "SectionSamples.h"

#include "Screen.h"
#include "PPUIConfig.h"
#include "Button.h"
#include "CheckBox.h"
#include "MessageBoxContainer.h"
#include "TransparentContainer.h"
#include "ListBox.h"
#include "RadioGroup.h"
#include "Seperator.h"
#include "Slider.h"
#include "StaticText.h"
#include "PatternEditorControl.h"
#include "DialogBase.h"

#include "PlayerController.h"
#include "InputControlListener.h"

// OS Interface
#include "PPOpenPanel.h"
#include "PPSavePanel.h"

#include "ControlIDs.h"

// Class which responds to message box clicks
class DialogResponderGuitar : public DialogResponder
{
private:
	SectionGuitar& section;
	
public:
	DialogResponderGuitar(SectionGuitar& section) :
		section(section)
	{
	}
	
	virtual pp_int32 ActionOkay(PPObject* sender)
	{
		switch (reinterpret_cast<PPDialogBase*>(sender)->getID())
	/*	{
			case MESSAGEBOX_ZAPINSTRUMENT:
			{
				section.zapInstrument();
				break;
			}
		}
*/
		return 0;
	}	
};


void SectionGuitar::showSection(bool bShow)
{
	containerEntire->show(bShow);
}

SectionGuitar::SectionGuitar(Tracker& theTracker) :
	SectionAbstract(theTracker, NULL, new DialogResponderGuitar(*this)),
	containerEntire(NULL),
	pianoControl(NULL),
	visible(false)//,
{

}

SectionGuitar::~SectionGuitar()
{
}

pp_int32 SectionGuitar::handleEvent(PPObject* sender, PPEvent* event)
{
//	char buffer[100];
//	char buffer2[100];
//	ModuleEditor::TEditorInstrument* ins;

	PPScreen* screen = tracker.screen;
	ModuleEditor* moduleEditor = tracker.moduleEditor;

	if (event->getID() == eUpdateChanged)
	{
		tracker.updateWindowTitle();
	}
	else if (event->getID() == eCommand || event->getID() == eCommandRepeat)
	{
		switch (reinterpret_cast<PPControl*>(sender)->getID())
		{

			case BUTTON_GUITAR_VW_M:
			{
				getGuitarControl()->vw-=1;
				if (getGuitarControl()->vw <0 )
				     getGuitarControl()->vw +=12;
				update();
				break;
			}
			case BUTTON_GUITAR_VW_P:
			{
				getGuitarControl()->vw+=1;
				if (getGuitarControl()->vw >= 12 )
				     getGuitarControl()->vw -=12;
				update();
				break;
			}
			case BUTTON_SAMPLE_RELNOTENUM_OCTUP:
			{
				tracker.getSampleEditor()->increaseRelNoteNum(12);
				update();
				break;
			}

			case BUTTON_SAMPLE_RELNOTENUM_OCTDN:
			{
				tracker.getSampleEditor()->increaseRelNoteNum(-12);
				update();
				break;
			}

			case BUTTON_SAMPLE_RELNOTENUM_NOTEUP:
			{
				tracker.getSampleEditor()->increaseRelNoteNum(1);
				getGuitarControl()->vw+=1;
				update();
				break;
			}

			case BUTTON_SAMPLE_RELNOTENUM_NOTEDN:
			{
				tracker.getSampleEditor()->increaseRelNoteNum(-1);
				getGuitarControl()->vw-=1;
				update();
				break;
			}

			case BUTTON_PIANO_EDIT_G:
				if (event->getID() != eCommand)
					break;
					
				getPianoControl()->setMode(PianoControl::ModeEdit);
				getGuitarControl()->setMode(GuitarControl::ModeEdit);
				update();
				break;

			case BUTTON_PIANO_PLAY_G:
				if (event->getID() != eCommand)
					break;
				getPianoControl()->setMode(PianoControl::ModePlay);
				getGuitarControl()->setMode(GuitarControl::ModePlay);
				update();
				break;

			case BUTTON_SAMPLE_PLAY_STOP:
				tracker.playerController->stopInstrument(tracker.listBoxInstruments->getSelectedIndex()+1);
				//tracker.stopSong();
				break;

			case BUTTON_GITEDITOR_CLEAR:
				if (event->getID() != eCommand)
					break;

//				handleZapInstrument();
				break;
			
			// load instrument
/*	*/		case BUTTON_GITEDITOR_LOAD:
			{
				if (event->getID() != eCommand)
					break;

//				tracker.loadType(FileTypes::FileTypeSongAllInstruments);
				break;
			}

			// save instrument
			case BUTTON_GITEDITOR_SAVE:
			{
				if (event->getID() != eCommand)
					break;

//				tracker.saveType(FileTypes::FileTypeInstrumentXI);
				break;
			}

			// test instrument chooser
			case BUTTON_GITEDITOR_COPY:
			{
				if (event->getID() != eCommand)
					break;
/*
				sprintf(buffer, "Copy ins. %x to %x", tracker.listBoxInstruments->getSelectedIndex()+1, 1);
				sprintf(buffer2, "Copy smp. %x to %x", tracker.listBoxSamples->getSelectedIndex(), 0);

				tracker.initInstrumentChooser(INSTRUMENT_CHOOSER_COPY, "Copy ins", "Copy smp", "Copy instrument/sample"PPSTR_PERIODS, 
											  buffer, buffer2, 
											  tracker.listBoxInstruments->getSelectedIndex(), 
											  tracker.listBoxSamples->getSelectedIndex(),
											  tracker.tabManager->getSelectedTabIndex());
				screen->setModalControl(tracker.instrumentChooser);
	*/
				break;
			}

			// test instrument chooser
			case BUTTON_GITEDITOR_PASTE:
			{
				if (event->getID() != eCommand)
					break;

/*				sprintf(buffer, "Swap ins. %x with %x", tracker.listBoxInstruments->getSelectedIndex()+1, 1);
				sprintf(buffer2, "Swap smp. %x with %x", tracker.listBoxSamples->getSelectedIndex(), 0);

				tracker.initInstrumentChooser(INSTRUMENT_CHOOSER_SWAP, "Swap ins", "Swap smp", "Swap instrument/sample"PPSTR_PERIODS, 
											  buffer, buffer2, 
											  tracker.listBoxInstruments->getSelectedIndex(), 
											  tracker.listBoxSamples->getSelectedIndex(),
											  tracker.tabManager->getSelectedTabIndex());
				screen->setModalControl(tracker.instrumentChooser);
	*/
				break;
			}
		}
	}
	else if (event->getID() == eSelection)
	{
		switch (reinterpret_cast<PPControl*>(sender)->getID())
		{
			case GUITAR_CONTROL:
			case PIANO_CONTROL_G:
			{
				pp_int32 v = *((pp_int32*)event->getDataPtr());

				tracker.inputControlListener->sendNote(v);			
				break;
			}
		}
	}
	else if (event->getID() == eValueChanged)
	{
		switch (reinterpret_cast<PPControl*>(sender)->getID())
		{
		/*	case SLIDER_SAMPLE_VOLUME:
*/
			// sample table has been updated
			case PIANO_CONTROL_G:
			{
				tracker.setChanged();
				pp_int32 i = tracker.listBoxInstruments->getSelectedIndex();	
				moduleEditor->updateSampleTable(i, *(reinterpret_cast<const pp_uint8* const*>(event->getDataPtr())));
				break;
			}		
		}
	}
/*	else if (event->getID() == eUpdated)
	{
		switch (reinterpret_cast<PPControl*>(sender)->getID())
		{			
			case CONTAINER_ENVELOPES:
			{
				updateEnvelopeEditor(true, false);
				break;
			}
		}
	}
	*/
	return 0;
}

void SectionGuitar::init()
{
	init(0, tracker.MAXEDITORHEIGHT()-tracker.GUITARSECTIONDEFAULTHEIGHT());
}

void SectionGuitar::init(pp_int32 x, pp_int32 y)
{
	PPScreen* screen = tracker.screen;

	containerEntire = new PPTransparentContainer(CONTAINER_ENTIREINSSECTION, screen, this, 
												 PPPoint(0, 0), PPSize(screen->getWidth(), screen->getHeight()));

#ifndef __LOWRES__
	pp_int32 w4 = 165;
	pp_int32 w3 = 39;
	pp_int32 w = screen->getWidth();
	pp_int32 scale = 256*screen->getWidth() / 800;
	
	
	pp_int32 y4 = y;  //  +20


	
	PPContainer* guitarContainer = new PPContainer(CONTAINER_GUITAR, screen, this, PPPoint(106, y), PPSize(screen->getWidth()-136,114), false);
	containerEntire->addControl(guitarContainer);
//	guitarContainer->setColor(TrackerConfig::colorPatternEditorNote); //colorThemeMain);

	// piano test
	pp_int32 guitarWidth = screen->getWidth();
	if (guitarWidth > 956)
		guitarWidth = 956;
	
/**/		
	guitarControl = new GuitarControl(GUITAR_CONTROL, screen, this, PPPoint(107, y), PPSize(guitarWidth, 114), ModuleEditor::MAX_NOTE); 
	guitarControl->assureNoteVisible(12*2);
	guitarControl->setBorderColor(TrackerConfig::colorThemeMain);
	guitarContainer->addControl(guitarControl);
	
	PPContainer*	
	container = new PPContainer(CONTAINER_GUITAR_INFO4, screen, this, PPPoint(x, y), PPSize(w3,38), false);
	pp_int32 height = container->getSize().height;
	pp_int32 nx = x + container->getSize().width;
	

	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	PPButton* button = new PPButton(BUTTON_GITEDITOR_EXIT, screen, &tracker, PPPoint(x + 2, y + 2), PPSize(34, 34));
	button->setText("Exit");

	container->addControl(button);


	container = new PPContainer(CONTAINER_GUITAR_INFO7, screen, this, PPPoint(x, y+38), PPSize(w3,38), false);

	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	button = new PPButton(BUTTON_GUITAR_VW_M, screen, this, PPPoint(x + 2, y + 40), PPSize(17, 17));
	button->setText("<-");
	container->addControl(button);
	button = new PPButton(BUTTON_GUITAR_VW_P, screen, this, PPPoint(x + 19, y + 40), PPSize(17, 17));
	button->setText("->");
	container->addControl(button);
	
	// load & save 
	container = new PPContainer(CONTAINER_GUITAR_INFO5, screen, this, PPPoint(nx, y), PPSize(w3,39), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	button = new PPButton(BUTTON_GITEDITOR_CLEAR, screen, this, PPPoint(nx + 2, y + 2), PPSize(34, 11));
	button->setText("Clear");
	container->addControl(button);

	button = new PPButton(BUTTON_GITEDITOR_LOAD, screen, this, PPPoint(nx + 2, y + 2+12), PPSize(34, 11));
	button->setText("Load");
	container->addControl(button);

	button = new PPButton(BUTTON_GITEDITOR_SAVE, screen, this, PPPoint(nx + 2, y + 2+12*2), PPSize(34, 11));
	button->setText("Save");
	container->addControl(button);

	// copy & paste
	y+=container->getSize().height;

	container = new PPContainer(CONTAINER_GUITAR_INFO6, screen, this, PPPoint(nx, y), PPSize(w3,27), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	button = new PPButton(BUTTON_GITEDITOR_COPY, screen, this, PPPoint(nx + 2, y + 2), PPSize(34, 11));
	//button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Copy");
	container->addControl(button);

	button = new PPButton(BUTTON_GITEDITOR_PASTE, screen, this, PPPoint(nx + 2, y + 2+12), PPSize(34, 11));
	//button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Paste");
	container->addControl(button);

//	y+=container->getSize().height;	

	y = y4;

	y+=height;


	// relative note
	container = new PPContainer(CONTAINER_GUITAR_INFO2, screen, this, PPPoint(x, y+height), PPSize(104,36+2), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	y+=height;

	container->addControl(new PPStaticText(0, NULL, NULL, PPPoint(x + 4, y + 2), "Rel note:", true));	
	container->addControl(new PPStaticText(STATICTEXT_SAMPLE_RELNOTE_G, screen, this, PPPoint(x + 4 + 15*5 - 4, y + 2), "C-4", false));	

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_OCTUP, screen, this, PPPoint(x + 4, y + 1 + 12), PPSize( 50, 11));
	button->setText("Oct +");
	container->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_NOTEUP, screen, this, PPPoint(x + 4 + 50, y + 1 + 12), PPSize( 50, 11));
	button->setText("Note+");
	container->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_OCTDN, screen, this, PPPoint(x + 4, y + 1 + 12 + 12), PPSize( 50, 11));
	button->setText("Oct -");
	container->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_NOTEDN, screen, this, PPPoint(x + 4 + 50, y + 1 + 24), PPSize( 50, 11));
	button->setText("Note-");
	container->addControl(button);

	// piano
	y+=container->getSize().height;
	
	PPContainer* pianoContainer_G = new PPContainer(CONTAINER_PIANO_G, screen, this, PPPoint(0, y), PPSize(screen->getWidth(),25*2+SCROLLBUTTONSIZE+4), false);
	containerEntire->addControl(pianoContainer_G);
	pianoContainer_G->setColor(TrackerConfig::colorThemeMain);

	// piano test
	pp_int32 pianoWidth = screen->getWidth()-2-40;
	if (pianoWidth > 898)
		pianoWidth = 898;
	
	pp_int32 dx = 0;
		
	pianoControl = new PianoControl(PIANO_CONTROL_G, screen, this, PPPoint(1+40, y+1), PPSize(pianoWidth, 25*2+12), ModuleEditor::MAX_NOTE); 
	// show C-2
	pianoControl->assureNoteVisible(12*2);
	pianoControl->setBorderColor(TrackerConfig::colorThemeMain);
	pianoContainer_G->addControl(pianoControl);
	
	button = new PPButton(BUTTON_PIANO_PLAY_G, screen, this, PPPoint(2, y+1), PPSize(36, 20), false, true, false);
	button->setColor(TrackerConfig::colorThemeMain);
	button->setTextColor(PPUIConfig::getInstance()->getColor(PPUIConfig::ColorStaticText));
	button->setText("Play");

	pianoContainer_G->addControl(button);

	button = new PPButton(BUTTON_PIANO_EDIT_G, screen, this, PPPoint(2, y+1+20), PPSize(36, 20), false, true, false);
	button->setColor(TrackerConfig::colorThemeMain);
	button->setTextColor(PPUIConfig::getInstance()->getColor(PPUIConfig::ColorStaticText));
	button->setText("Edit");

	pianoContainer_G->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_PLAY_STOP, screen, this, PPPoint(2, y+1+2*20), PPSize(36, 20));
	button->setText("Stop");
	pianoContainer_G->addControl(button);		

#else
//  Low Resolution
	
	PPContainer* container = new PPContainer(CONTAINER_GUITAR_INFO1, screen, this, PPPoint(x, y), PPSize(116,34+4), false);
	containerEntire->addControl(container);
//	containerSampleSlider = container;
	container->setColor(TrackerConfig::colorThemeMain);

	pp_int32 width = container->getSize().width;
	pp_int32 height = container->getSize().height;

	// relative note
	container = new PPContainer(CONTAINER_GUITAR_INFO2, screen, this, PPPoint(0, y + height), PPSize(116,34+4), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	container->addControl(new PPStaticText(0, NULL, NULL, PPPoint(x + 4, y + 4 + 37), "Rel.note:", true));	
	container->addControl(new PPStaticText(STATICTEXT_SAMPLE_RELNOTE_G, screen, this, PPPoint(x + 4 + 16*5 - 4, y + 4 + 37), "C-4", false));	

	PPButton* button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_OCTUP, screen, this, PPPoint(x + 4, y + 3 + 36 + 12), PPSize(56, 10));
	button->setText("Octave up");
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	container->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_NOTEUP, screen, this, PPPoint(x + 4 + 55, y + 3 + 36 + 12), PPSize(54, 10));
	button->setText("Note up");
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	container->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_OCTDN, screen, this, PPPoint(x + 4, y + 3 + 36 + 12 + 11), PPSize(56, 10));
	button->setText("Octave dn");
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	container->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_RELNOTENUM_NOTEDN, screen, this, PPPoint(x + 4 + 55, y + 3 + 36 + 23), PPSize(54, 10));
	button->setText("Note dn");
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	container->addControl(button);

	// autovibrato etc.
	x+=width;

	const pp_int32 dy = 15;
	// sucks
	pp_int32 nx = x + container->getSize().width;

	// load & save 
	container = new PPContainer(CONTAINER_INSTRUMENTS_INFO5, screen, this, PPPoint(nx, y), PPSize(39,23), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	button = new PPButton(BUTTON_GITEDITOR_LOAD, screen, this, PPPoint(nx + 2, y + 2), PPSize(34, 9));
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Load");

	container->addControl(button);

	button = new PPButton(BUTTON_GITEDITOR_SAVE, screen, this, PPPoint(nx + 2, y + 2+10), PPSize(34, 9));
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Save");

	container->addControl(button);

	// copy & paste
	y+=container->getSize().height;

	container = new PPContainer(CONTAINER_GUITAR_INFO6, screen, this, PPPoint(nx, y), PPSize(39,23), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	button = new PPButton(BUTTON_GITEDITOR_COPY, screen, this, PPPoint(nx + 2, y + 2), PPSize(34, 9));
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Copy");

	container->addControl(button);

	button = new PPButton(BUTTON_GITEDITOR_PASTE, screen, this, PPPoint(nx + 2, y + 2+10), PPSize(34, 9));
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Paste");

	container->addControl(button);

	y+=container->getSize().height;

	// buoah
	container = new PPContainer(CONTAINER_GUITAR_INFO4, screen, this, PPPoint(nx, y), PPSize(39,30), false);
	containerEntire->addControl(container);
	container->setColor(TrackerConfig::colorThemeMain);

	button = new PPButton(MAINMENU_SMPEDIT, screen, &tracker, PPPoint(nx + 2, y + 2), PPSize(34, 12));
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Smp.Ed.");
	container->addControl(button);

	button = new PPButton(BUTTON_INSTRUMENTEDITOR_EXIT, screen, &tracker, PPPoint(nx + 2, y + 2+13), PPSize(34, 12));
	button->setText("Exit");
	container->addControl(button);
	
	// piano
	y+=container->getSize().height;
	
	PPContainer* pianoContainer_G = new PPContainer(CONTAINER_PIANO_G, screen, this, PPPoint(0, y), PPSize(screen->getWidth(),25+SCROLLBUTTONSIZE+4), false);
	containerEntire->addControl(pianoContainer_G);
	pianoContainer_G->setColor(TrackerConfig::colorThemeMain);

	// piano test
	pianoControl = new PianoControl(PIANO_CONTROL_G, screen, this, 
									PPPoint(1+32, y+1), PPSize(screen->getWidth()-2-32, 25+12), ModuleEditor::MAX_NOTE); 
	// show C-3
	pianoControl->assureNoteVisible(12*4);
	pianoControl->setBorderColor(TrackerConfig::colorThemeMain);
	pianoControl->setMode(PianoControl::ModePlay);
	
	pianoContainer_G->addControl(pianoControl);

	button = new PPButton(BUTTON_PIANO_PLAY_G, screen, this, PPPoint(1, y+1), PPSize(30, 12), false, true, false);
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setColor(TrackerConfig::colorThemeMain);
	button->setTextColor(PPUIConfig::getInstance()->getColor(PPUIConfig::ColorStaticText));
	button->setText("Play");

	pianoContainer_G->addControl(button);

	button = new PPButton(BUTTON_PIANO_EDIT_G, screen, this, PPPoint(1, y+1+12), PPSize(30, 12), false, true, false);
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setColor(TrackerConfig::colorThemeMain);
	button->setTextColor(PPUIConfig::getInstance()->getColor(PPUIConfig::ColorStaticText));
	button->setText("Edit");

	pianoContainer_G->addControl(button);

	button = new PPButton(BUTTON_SAMPLE_PLAY_STOP, screen, this, PPPoint(2, y+1+2*12), PPSize(28, 11));
	button->setFont(PPFont::getFont(PPFont::FONT_TINY));
	button->setText("Stop");
	pianoContainer_G->addControl(button);
	
#endif
	containerEntire->adjustContainerSize();
	screen->addControl(containerEntire);

	initialised = true;
	
	showSection(false);	
}

void SectionGuitar::realign()
{
	pp_uint32 maxShould = tracker.MAXEDITORHEIGHT();
	pp_uint32 maxIs = containerEntire->getLocation().y + containerEntire->getSize().height;
	
	if (maxIs != maxShould)
	{
		pp_int32 offset = maxShould - maxIs;
		containerEntire->move(PPPoint(0, offset));
	}
	
	PatternEditorControl* control = tracker.getPatternEditorControl();
	PPScreen* screen = tracker.screen;
	
	if (visible)
	{
		control->setSize(PPSize(screen->getWidth(),
							tracker.MAXEDITORHEIGHT()-tracker.GUITARSECTIONDEFAULTHEIGHT()-tracker.UPPERSECTIONDEFAULTHEIGHT()));
	}
	else
	{
		control->setSize(PPSize(screen->getWidth(),tracker.MAXEDITORHEIGHT()-tracker.UPPERSECTIONDEFAULTHEIGHT()));
	}
}

void SectionGuitar::show(bool bShow)
{
#ifdef __LOWRES__
	PPScreen* screen = tracker.screen;
	screen->pauseUpdate(true);
#endif
	SectionAbstract::show(bShow);
	
	visible = bShow;
	containerEntire->show(bShow);
	
	if (!initialised)
	{
		init();
	}

	if (initialised)
	{
		PatternEditorControl* control = tracker.getPatternEditorControl();
		
#ifndef __LOWRES__
		realign();
#endif
		
		if (bShow)
		{
			if (control)
			{
#ifdef __LOWRES__
				control->show(false);				
				replaceInstrumentListBoxes(true);
#endif
				tracker.hideInputControl();				
			}
			
			update(false);
		}
		else if (control)
		{
#ifdef __LOWRES__
			control->show(true);
			replaceInstrumentListBoxes(false);
#endif
		}
		
		showSection(bShow);
	}	

#ifdef __LOWRES__
	// If instrument section is shown (bShow = true)
	// set focus to the Instrumentlist container (instrument listbox)
	// but before disable screen updates to prevent flickering
	if (bShow)
	{
		screen->setFocus(screen->getControlByID(CONTAINER_INSTRUMENTLIST));
	}
	screen->pauseUpdate(false);
	if (!bShow)
	{
		screen->update();
	}
#endif
}

void SectionGuitar::notifyTabSwitch()
{
	if (isVisible())
		realign();

	resetPianoAssignment();
}

void SectionGuitar::notifySampleSelect(pp_int32 index)
{
	PianoControl* pianoControl = getPianoControl();				
	if (pianoControl)
		pianoControl->setSampleIndex(index);
	if (guitarControl)
		guitarControl->setSampleIndex(index);	
}

void SectionGuitar::update(bool repaint)
{
	if (!initialised || !visible)
		return;

	PPScreen* screen = tracker.screen;
	ModuleEditor* moduleEditor = tracker.moduleEditor;

	// relative note number
	PPContainer* container3 = static_cast<PPContainer*>(screen->getControlByID(CONTAINER_GUITAR_INFO2));

	char noteName[40];
	SampleEditor* sampleEditor = tracker.getSampleEditor();


	PatternTools::getNoteName(noteName, sampleEditor->getRelNoteNum() + 4*12 + 1);

	PPContainer* container6 = static_cast<PPContainer*>(screen->getControlByID(CONTAINER_GUITAR));
	static_cast<PPStaticText*>(container3->getControlByID(STATICTEXT_SAMPLE_RELNOTE_G))->setText(noteName);		

	pianoControl->setSampleTable(moduleEditor->getSampleTable(tracker.listBoxInstruments->getSelectedIndex()));


	PPContainer* container5 = static_cast<PPContainer*>(screen->getControlByID(CONTAINER_PIANO_G));
	{
		PPButton* editButton = static_cast<PPButton*>(container5->getControlByID(BUTTON_PIANO_EDIT_G));
		PPButton* playButton = static_cast<PPButton*>(container5->getControlByID(BUTTON_PIANO_PLAY_G));

		editButton->setPressed(false);
		playButton->setPressed(false);
		switch (getPianoControl()->getMode())
		{
			case PianoControl::ModeEdit:
				editButton->setPressed(true);
				break;
			case PianoControl::ModePlay:
				playButton->setPressed(true);
				break;
		}
	}
	screen->paintControl(container6, false);
	screen->paintControl(guitarControl, false);
	screen->paintControl(container5, false);

	screen->paintControl(container3, false);
	
	screen->paintControl(pianoControl, false);
	
	if (repaint)
		screen->update();
}

void SectionGuitar::updateAfterLoad()
{
	tracker.updateInstrumentsListBox(false);
	tracker.updateSamplesListBox(false);
	tracker.updateSampleEditorAndInstrumentSection(false);
}



void SectionGuitar::resetPianoAssignment()
{
	pianoControl->setSampleIndex(0);
	guitarControl->setSampleIndex(0);
}
//}

