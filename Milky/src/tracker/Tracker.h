/*
 *  tracker/Tracker.h
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
 *  You could have received a copy of the GNU General Public License
 *  along with Milkytracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TRACKER__H
#define TRACKER__H

#include "BasicTypes.h"
#include "Event.h"
#include "EditModes.h"
#include "FileTypes.h"

#define INPUTCONTAINERHEIGHT_DEFAULT	(25+SCROLLBUTTONSIZE+4)
#define INPUTCONTAINERHEIGHT_EXTENDED	(25+SCROLLBUTTONSIZE+4+13)

template<class Type>
class PPSimpleVector;

class PPScreen;
class ModuleEditor;
class PatternEditor;
class SampleEditor;
class EnvelopeEditor;
class PlayerController;
class PlayerMaster;
class TabManager;
class PatternEditorControl;
class PPListBox;
class PPStaticText;
class EnvelopeEditorControl;
class PianoControl;
class GuitarControl;
class PeakLevelControl;
class ScopesControl;
class SampleEditorControl;
class PPContainer;
class PPMessageBoxContainer;
class TrackerSettingsDatabase;
class PPDictionaryKey;
class PPFont;

// OS Interfaces
class PPSavePanel;
class PPOpenPanel;

// Section forwards
class SectionAbstract;
class SectionTranspose;
class SectionAdvancedEdit;
class SectionDiskMenu;
class SectionHDRecorder;
class SectionSettings;
class SectionInstruments;
class SectionSamples;
class SectionGuitar;
class SectionQuickOptions;
class SectionOptimize;
class SectionAbout;

class PatternEditorControl;
class InputControlListener;
class PPDialogBase;
class DialogResponder;
class ToolInvokeHelper;

struct TMixerSettings;

// Key binding forwards
class Tracker;
typedef void (Tracker::*TTrackerKeyBindingHandler)();

template<class Type>
class PPKeyBindings;

class Tracker : public EventListenerInterface
{
private:
	// I've replaced some constants
#ifndef __LOWRES__
	pp_int32 SCOPESHEIGHT();
	pp_int32 CURRENTSCOPESHEIGHT();
	pp_int32 UPPERSECTIONDEFAULTHEIGHTWOINS()	{ return 54+64; }
	pp_int32 UPPERSECTIONDEFAULTHEIGHT()		{ return ((UPPERSECTIONDEFAULTHEIGHTWOINS())+(CURRENTSCOPESHEIGHT())); }
	pp_int32 INSTRUMENTSECTIONDEFAULTHEIGHT()	{ return 206; }
	pp_int32 GUITARSECTIONDEFAULTHEIGHT()	{ return 180; }
	pp_int32 SAMPLESECTIONDEFAULTHEIGHT();
	pp_int32 TABHEADERHEIGHT()					{ return 16; }
	pp_int32 MAXEDITORHEIGHT();
#else
	pp_int32 SCOPESHEIGHT()						{ return 64; }
	pp_int32 SCOPESWIDTH()						{ return 320-26; }
	pp_int32 UPPERSECTIONDEFAULTHEIGHT()		{ return (64+16); }
	pp_int32 INSTRUMENTSECTIONDEFAULTHEIGHT()	{ return 188; }
	pp_int32 GUITARSECTIONDEFAULTHEIGHT()	{ return 170; }	
	pp_int32 SAMPLESECTIONDEFAULTHEIGHT()		{ return 184; }
	pp_int32 NUMSUBMENUS()						{ return 6; }
	pp_int32 MAXEDITORHEIGHT();
#endif
	
	pp_uint8* muteChannels;
	
	TabManager* tabManager;
	PlayerController* playerController;
	PlayerMaster* playerMaster;
	ModuleEditor* moduleEditor;
	class PlayerLogic* playerLogic;
	class RecorderLogic* recorderLogic;
	
	PatternEditorControl* patternEditorControl;
	PeakLevelControl* peakLevelControl;
	ScopesControl* scopesControl;
	PPStaticText* playTimeText;
	
	// - Sections --------------------------------------------------------------
	class SectionSwitcher* sectionSwitcher;
	
	PPSimpleVector<SectionAbstract>* sections;
	SectionTranspose* sectionTranspose;
	SectionAdvancedEdit* sectionAdvancedEdit;
	SectionDiskMenu* sectionDiskMenu;
	SectionHDRecorder* sectionHDRecorder;
	SectionSettings* sectionSettings;
	SectionInstruments* sectionInstruments;
	SectionGuitar* sectionGuitar;
	SectionSamples* sectionSamples;
	SectionQuickOptions* sectionQuickOptions;
	SectionOptimize* sectionOptimize;
	SectionAbout* sectionAbout;
	
	InputControlListener* inputControlListener;
	
	DialogResponder* responder;
	PPDialogBase* dialog;
	ToolInvokeHelper* toolInvokeHelper;
	
	PPScreen* screen;

	TrackerSettingsDatabase* settingsDatabase;
	TrackerSettingsDatabase* settingsDatabaseCopy;

	PPSystemString currentFileName;
	bool lastState;

	// - Tracker mode ----------------------------------------------------------
	EditModes editMode;

	// - Misc. settings --------------------------------------------------------
	bool extendedOrderlist;	
	bool followSong;
	bool caughtMouseInUpperLeftCorner;	
	bool useClassicBrowser;
	
	// - Keyboard bindings -----------------------------------------------------
	PPKeyBindings<TTrackerKeyBindingHandler>* eventKeyDownBindings;
	PPKeyBindings<TTrackerKeyBindingHandler>* eventKeyDownBindingsMilkyTracker;
	PPKeyBindings<TTrackerKeyBindingHandler>* eventKeyDownBindingsFastTracker;

	// - settings --------------------------------------------------------------
	void buildDefaultSettings();
	void applySettingByKey(PPDictionaryKey* theKey, TMixerSettings& settings, pp_uint32 version);

	static void getMixerSettingsFromDatabase(TMixerSettings& mixerSettings, 
											 TrackerSettingsDatabase& currentSettings);

	void applySettings(TrackerSettingsDatabase* newSettings, 
					   TrackerSettingsDatabase* currentSettings = NULL, 
					   bool applyMixerSettings = true,
					   bool allowMixerRestart = true);
	
	// - global controls -------------------------------------------------------
	PPListBox* listBoxOrderList;
	PPListBox* listBoxInstruments;
	PPListBox* listBoxSamples;

	// - build UI parts --------------------------------------------------------
	void initSectionOrderlist(pp_int32 x, pp_int32 y);
	void initSectionSpeed(pp_int32 x, pp_int32 y);
	void initSectionPattern(pp_int32 x, pp_int32 y);
	void initSectionMainOptions(pp_int32 x, pp_int32 y);
	void initListboxesSection(pp_int32 x, pp_int32 y);
	
	// - instrument selector dialog --------------------------------------------
	void initAdvEdit();

	void initInstrumentChooser(pp_int32 id, const PPString& buttonText, const PPString& buttonText2, const PPString& caption, 
							   const PPString& userString1, const PPString& userString2,  
							   pp_int32 srcSelection = -1, pp_int32 srcSelection2 = -1, pp_int32 srcSelection3 = -1);
	void updateInstrumentChooser(bool repaint = true);

	PPMessageBoxContainer* instrumentChooser;

	// - message boxes ---------------------------------------------------------
	PPMessageBoxContainer* messageBoxContainerGeneric;

	enum MessageBoxTypes
	{
		MessageBox_OK,
		MessageBox_YESNO,
		MessageBox_YESNOCANCEL
	};
	void showMessageBox(pp_int32 id, const PPString& caption, MessageBoxTypes type, bool update = true);
	void showMessageBoxSized(pp_int32 id, const PPString& caption, MessageBoxTypes type, pp_int32 width = -1, pp_int32 height = -1, bool update = true);
	void showQuitMessageBox(const char* caption, const char* cpationOk, const char* captionCancel);

	// - software input panel --------------------------------------------------
	PPContainer* inputContainerCurrent;
	PPContainer* inputContainerDefault;
	PPContainer* inputContainerExtended;
	void initInputContainerDefault(pp_int32 x, pp_int32 y);
	void initInputContainerExtended(pp_int32 x, pp_int32 y);
	
	enum SIPs
	{
		SIPDefault,
		SIPExtended
	};
	void setInputControl(SIPs sip);
	void moveInputControls(pp_uint32 deltay);
	void hideInputControl(bool bHide = true);

	// - GUI refreshing --------------------------------------------------------
	void updateAboutToggleButton(pp_int32 id, bool b, bool repaint = true);

	void updateSongTitle(bool repaint = true);

#ifdef __LOWRES__
	void updateJamMenuOrder(bool repaint = true);
	void updateJgitMenuOrder(bool repaint = true);
#endif
	void updateOrderlist(bool repaint = true);
	void updateSongLength(bool repaint = true);
	void updateSongRepeat(bool repaint = true);
	
	void updateBPM(bool repaint = true);
	bool updateSpeed(bool repaint = true);
	void updatePatternAddAndOctave(bool repaint = true);

	void updatePatternIndex(bool repaint = true);
	void updatePatternLength(bool repaint = true);
	void updatePattern(bool repaint = true);

	void updateSamplesListBox(bool repaint = true);
	void updateInstrumentsListBox(bool repaint = true);

	void updateSongInfo(bool repaint = true);
	
	void updateTabTitle();
	void updateWindowTitle();
	void updateWindowTitle(const PPSystemString& fileName);

	bool updatePianoControl(PianoControl* pianoControl);
	bool updateGuitarControl(GuitarControl* guitarControl);
	bool updatePeakLevelControl();
	bool updatePlayTime();

	void updateSampleEditor(bool repaint = true, bool force = false);
	void updateSampleEditorAndInstrumentSection(bool repaint = true);
	
	void updateAfterLoad(bool loadResult, bool wasPlaying, bool wasPlayingPattern);
	void updateAfterTabSwitch();

	// - show hide GUI sections ------------------------------------------------
	// Show/hide main section (song settings + main menu)
	void showSongSettings(bool show);
	void showMainOptions(bool show);
	
#ifdef __LOWRES__
	void selectScopesControl(pp_int32 ctrlType);
	void updateScopesControlButtons();
	void toggleJamMenuPianoSize();
	void toggleJgitMenuGuitarSize();
	void toggleGuitarMode();
	void flipInstrumentListBoxes();
#endif

	void showMainMenu(bool show, bool showInstrumentSelector);
	
	void showScopes(bool visible, pp_uint32 style);
	// - misc. -----------------------------------------------------------------
	pp_int32 lastPos, lastRow;
	pp_int32 lastSpeed, lastBPM, lastMainVol;
	void resetStateMemories()
	{
		lastPos = lastRow = -1;
		lastSpeed = lastBPM = lastMainVol = -1;
	}
	
	bool updateSongPosition(pp_int32 pos = -1, pp_int32 row = -1, bool fast = false);
	// this always repaints, so no bool return value
	void updateRecordButton(PPContainer* container, const PPColor& pColor);
	void doFollowSong();
	
	PatternEditorControl* getPatternEditorControl() { return patternEditorControl; }
	void updatePatternEditorControl(bool repaint = true, bool fast = false);
	PatternEditor* getPatternEditor();
	SampleEditor* getSampleEditor();
	EnvelopeEditor* getEnvelopeEditor();

	pp_int32 getOrderListBoxIndex();
	void setOrderListIndex(pp_int32 index);
	bool isEditingCurrentOrderlistPattern();
	pp_int32 getInstrumentToPlay(pp_int32 note, PlayerController*& playerController);
	
	void setNumChannels(pp_int32 numChannels, bool repaint = true);

	bool isActiveEditing();

	void ensureSongStopped(bool bResetMainVolume, bool suspend);
	void ensureSongPlaying(bool continuePlaying);
	void initPlayback();
	
	bool shouldFollowSong();
	
	bool getFollowSong();
	void setFollowSong(bool b, bool repaint = true);
	
	bool getProspectiveMode();
	void setProspectiveMode(bool b, bool repaint = true);
	
	bool getCursorWrapAround();
	void setCursorWrapAround(bool b, bool repaint = true);

	void setLiveSwitch(bool b, bool repaint = true);
	
	void updateSongRow(bool checkFollowSong = true);
	
	void selectInstrument(pp_int32 instrument);
	void fillInstrumentListBox(PPListBox* listBox, ModuleEditor* moduleEditor = NULL);
	void fillSampleListBox(PPListBox* listBox, pp_int32 insIndex, ModuleEditor* moduleEditor = NULL);
	void fillModuleListBox(PPListBox* listBox);
	
	void setChanged();

	bool checkForChanges(ModuleEditor* module = NULL);
	bool checkForChangesOpenModule();
	
	bool swapAndCopyHandler(pp_int32 messageBoxID, pp_int32 messageBoxButtonID);
	void handleQuit();
	bool messageBoxEventListener(pp_int32 messageBoxID, pp_int32 messageBoxButtonID);

	void rearrangePatternEditorControl();
	void rearrangePatternEditorControlOrInstrumentContainer();

	void expandOrderlist(bool b);

	void flipSpeedSection();
	
	void enableInstrument(bool b);

	void commitListBoxChanges();	

	// Signal waitstate
	void signalWaitState(bool b);
	
	// - Load/Save -------------------------------------------------------------
	FileTypes currentSaveFileType;
	PPSavePanel* savePanel;
	EventListenerInterface* fileSystemChangedListener;

	FileTypes getCurrentSelectedSampleSaveType();

	struct TPrepareLoadingParameters
	{
		FileTypes eType;
		PPSystemString filename;
		PPSystemString preferredFilename;
		bool suspendPlayer;
		bool repaint;
		bool res;
		PPString lastError;
		bool wasPlaying;
		bool wasPlayingPattern;	
		bool abortLoading;
		bool deleteFile;
		bool didOpenTab;
		
		TPrepareLoadingParameters() :
			abortLoading(false),
			deleteFile(false),
			didOpenTab(false)
		{
		}
	} loadingParameters;

	static pp_uint32 fileTypeToHint(FileTypes type);

	void prepareLoadSaveUI();
	void finishLoadSaveUI();

	bool loadGenericFileType(const PPSystemString& fileName);

	bool prepareLoading(FileTypes eType, 
						const PPSystemString& fileName, 
						bool suspendPlayer, 
						bool repaint, bool 
						saveCheck);

	bool finishLoading();

	bool loadTypeFromFile(FileTypes eType, 
						  const PPSystemString& fileName, 
						  bool suspendPlayer = true, 
						  bool repaint = true, 
						  bool saveCheck = true);

	bool loadTypeWithDialog(FileTypes eType, 
							bool suspendPlayer = true, 
							bool repaint = true);
	void loadType(FileTypes eType);
	
	bool prepareSavingWithDialog(FileTypes eType);
	bool saveTypeWithDialog(FileTypes eType, EventListenerInterface* fileSystemChangedListener = NULL);
	bool saveCurrentModuleAsSelectedType();
	void saveType(FileTypes eType);
	void save();
	void saveAs();
	void handleSaveProceed();
	void handleSaveCancel();
	
	void buildMODSaveErrorWarning(pp_int32 error);
	void estimateSongLength(bool signalWait = false);

public:
	Tracker();

	virtual ~Tracker();
	
	bool numlock;
	
	void setScreen(PPScreen* screen) { this->screen = screen; }
	PPScreen* getScreen() { return this->screen; };

	void initUI();
	
	void sendKey(EEventDescriptor event, pp_uint16 vk, pp_uint16 sc, pp_uint16 chr);

	void setModuleNumChannels(pp_uint32 numChannels);

	// pp_int32 griff1[6], griff2[6], griff3[6], griff4[6], griff5[6], griff6[6], griff7[6], griff8[6];
	
	pp_int32 griff[9][6];
	// PPEvent listener
	pp_int32 handleEvent(PPObject* sender, PPEvent* event);
	
	PPSize getWindowSizeFromDatabase();
	bool getFullScreenFlagFromDatabase();
	pp_int32 getScreenScaleFactorFromDatabase();
	bool getShowSplashFlagFromDatabase();

	void showSplash();
	void hideSplash();

	// Tracker startup
	void startUp(bool forceNoSplash = false);
		
	// Tracker shutdown
	bool shutDown();
	
	void saveModule(const PPSystemString& fileName);

	void sendNoteDown(pp_int32 note, pp_int32 volume = -1);
	void sendNoteUp(pp_int32 note);

private:
	void switchEditMode(EditModes mode);

	// Process keyboard events according to current edit mode
	void processShortcuts(PPEvent* event);
	
	// Process messagebox shortcuts (RETURN & ESC)
	bool processMessageBoxShortcuts(PPEvent* event);

	void processShortcutsMilkyTracker(PPEvent* event);
	
	void selectNextOrder(bool wrap = false);
	void selectPreviousOrder(bool wrap = false);

	void selectNextInstrument();
	void selectPreviousInstrument();
	
	void processShortcutsFastTracker(PPEvent* event);

	// - Keyboard bindings -----------------------------------------------------
	
	GuitarControl* get_GuitarControl();
	void eventKeyDownBinding_MemGriff_1();
	void eventKeyDownBinding_MemGriff_2();
	void eventKeyDownBinding_MemGriff_3();
	void eventKeyDownBinding_MemGriff_4();
	void eventKeyDownBinding_MemGriff_5();
	void eventKeyDownBinding_MemGriff_6();
	void eventKeyDownBinding_MemGriff_7();
	void eventKeyDownBinding_MemGriff_8();
	
	void eventKeyDownBinding_Griff_1();
	void eventKeyDownBinding_Griff_2();
	void eventKeyDownBinding_Griff_3();
	void eventKeyDownBinding_Griff_4();
	void eventKeyDownBinding_Griff_5();
	void eventKeyDownBinding_Griff_6();
	void eventKeyDownBinding_Griff_7();
	void eventKeyDownBinding_Griff_8();
	
	void eventKeyDownBinding_Numlock();
	void eventKeyDownBinding_E_String();	
	void eventKeyDownBinding_a_String();
	void eventKeyDownBinding_d_String();
	void eventKeyDownBinding_g_String();
	void eventKeyDownBinding_b_String();
	void eventKeyDownBinding_e_String();

	
	pp_int16 currentKeyCode;
	
	bool executeBinding(const PPKeyBindings<TTrackerKeyBindingHandler>* bindings, pp_uint16 keyCode);
	void initKeyBindings();	
	
	void eventKeyDownBinding_OpenTab();
	void eventKeyDownBinding_CloseTab();
	
	void eventKeyDownBinding_SwitchToNextTab();
	void eventKeyDownBinding_SwitchToPreviousTab();

	void setOctave(pp_uint32 octave);

	void eventKeyDownBinding_SelectOctave1();
	void eventKeyDownBinding_SelectOctave2();
	void eventKeyDownBinding_SelectOctave3();
	void eventKeyDownBinding_SelectOctave4();
	void eventKeyDownBinding_SelectOctave5();
	void eventKeyDownBinding_SelectOctave6();
	void eventKeyDownBinding_SelectOctave7();
	void eventKeyDownBinding_SelectOctave8();
	
	void eventKeyDownBinding_PlaySong();
	void eventKeyDownBinding_PlayPattern();
	void eventKeyDownBinding_PlayPosition();
	void eventKeyDownBinding_PlayPatternFromFIRSTQUARTER();
	void eventKeyDownBinding_PlayPatternFromSECONDQUARTER();
	void eventKeyDownBinding_PlayPatternFromTHIRDQUARTER();
	void eventKeyDownBinding_PlayRow();
	void eventKeyDownBinding_PlayTrace();
	void eventKeyDownBinding_Stop();
	void eventKeyDownBinding_Edit();	

	void eventKeyDownBinding_UnmuteAll();

	void eventKeyDownBinding_Open();
	void eventKeyDownBinding_Save();
	void eventKeyDownBinding_SaveAs();
	
	void eventKeyDownBinding_NextPattern();
	void eventKeyDownBinding_PreviousPattern();
	
	// - Invoke sections
	void eventKeyDownBinding_InvokeMainScreen();
	void eventKeyDownBinding_InvokeSectionInstruments();
	void eventKeyDownBinding_InvokeSectionGuitar();
	void eventKeyDownBinding_InvokeSectionSamples();
	void eventKeyDownBinding_InvokeSectionSettings();
	void eventKeyDownBinding_InvokeSectionTranspose();
	void eventKeyDownBinding_InvokeSectionAdvancedEdit();
	void eventKeyDownBinding_InvokeSectionDiskMenu();
	void eventKeyDownBinding_InvokeSectionHDRecorder();
	void eventKeyDownBinding_InvokeSectionQuickOptions();
	void eventKeyDownBinding_InvokeSectionOptimize();
	void eventKeyDownBinding_InvokeSectionAbout();
	
	// Some handy shortcuts
	void eventKeyDownBinding_ToggleFT2Edit();
	void eventKeyDownBinding_ToggleFollowSong();
	void eventKeyDownBinding_ToggleProspectiveMode();
	void eventKeyDownBinding_ToggleCursorWrapAround();
	void eventKeyDownBinding_ToggleLiveSwitch();
	void eventKeyDownBinding_ToggleRecordKeyOff();
	void eventKeyDownBinding_ToggleScopes();
	
	void eventKeyDownBinding_InvokePatternToolVolumeScalePattern();
	void eventKeyDownBinding_InvokePatternToolVolumeScaleTrack();
	void eventKeyDownBinding_InvokePatternToolVolumeScaleSelection();

	void eventKeyDownBinding_InvokeQuickChooseInstrument();
	
	// - Some FT2 standard bindings
	void eventKeyDownBinding_TransposeCurInsTrackDown();
	void eventKeyDownBinding_TransposeCurInsTrackUp();

	void eventKeyDownBinding_TransposeCurInsPatternDown();
	void eventKeyDownBinding_TransposeCurInsPatternUp();

	void eventKeyDownBinding_TransposeCurInsBlockDown();
	void eventKeyDownBinding_TransposeCurInsBlockUp();

	void eventKeyDownBinding_TransposeAllInsTrackDown();
	void eventKeyDownBinding_TransposeAllInsTrackUp();

	void eventKeyDownBinding_TransposeAllInsPatternDown();
	void eventKeyDownBinding_TransposeAllInsPatternUp();

	void eventKeyDownBinding_TransposeAllInsBlockDown();
	void eventKeyDownBinding_TransposeAllInsBlockUp();
	
	void eventKeyDownBinding_ExitApplication();

private:
	// - friend classes --------------------------------------------------------
	friend class SectionAbstract;
	friend class SectionUpperLeft;
	friend class SectionSettings;
	friend class SectionTranspose;
	friend class SectionAdvancedEdit;
	friend class SectionDiskMenu;
	friend class SectionReplacedInstrumentListBoxes;
	friend class SectionInstruments;
	friend class SectionGuitar;
	friend class SectionSamples;
	friend class SectionHDRecorder;
	friend class SectionQuickOptions;
	friend class SectionOptimize;
	friend class SectionAbout;

	friend class InputControlListener;
	friend class SampleLoadChannelSelectionHandler;
	friend class ZapInstrumentHandler;
	friend class ToolInvokeHelper;
	friend class SaveProceedHandler;
	friend class PanningSettingsContainer;
	
	friend class TabManager;
	friend class PlayerLogic;
	friend class RecorderLogic;
	friend class Zapper;
	friend class SectionSwitcher;
};

#endif
