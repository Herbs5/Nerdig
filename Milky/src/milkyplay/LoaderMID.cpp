/*
 * Copyright (c) 2009, The MilkyTracker Team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name of the <ORGANIZATION> nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  LoaderMID.cpp
 *  MilkyPlay Module Loader: midifile
  
 */
#include "Loaders.h"
#include "midifile.h"

const char* LoaderMID::identifyModule(const mp_ubyte* buffer)
{
	//return "something";

	/* check for the RMID variation on SMF */

	if (memcmp(buffer, "RIFF", 4) == 0)
	{
#ifdef VERBOSE		
		printf("identified riff");
#endif		
		if (memcmp(buffer+4, "RMID", 4) != 0)
		{
#ifdef VERBOSE			
			printf(" riff is no midi");
#endif
			return NULL;
		}
	}

	if (memcmp(buffer, "MThd", 4) != 0)
	{
		return NULL;
	}
#ifdef VERBOSE	
	printf("identified M");
#endif	
	return "MID";
	// This will be checked again at load in midifile.c

}



mp_sint32 LoaderMID::load(XMFileBase& f, XModule* module)
{
//	printf("  loading take place ");
	mp_sint32 i,j;
//	mp_ubyte buffer[128];
//	mp_uint32 samplePtrs[31];

	module->cleanUp();    // or reuse Instruments by merging only 

	// this will make code much easier to read
	TXMHeader*		header = &module->header;
	TXMInstrument*	instr  = module->instr;
	TXMSample*		smp	   = module->smp;
	TXMPattern*		phead  = module->phead;	


	//     MidiFile
	
	struct midi_instrument
	{		
			int  id;
			int	channel;
			int   number;
			midi_instrument() : id(0), channel(0), number(0) {} 
	};
//	midi_instrument MI[32];
	
	int midi_file_event_velocity, atall = 0;
	int Note_, tracks, Velocity_; //,  last_channel = 0;
	float midi_event_time, event_time_before;
	MidiFile_t midi_file;	
	MidiFileTrack_t Track;
	MidiFileEvent_t midi_event;
	int num_channl = 0, num_instr;	

	struct Trackinfo
	{
		int	first_channel;
		int	last_channel;	
		int   smalest_note;
		int	biggest_note;
		int	mchannel;
		int	num_playing;
		int	akkord;
		char *   minstrument;
		int	differnt_notes[128];
		int	is_playing[128];	
		int	playingNote[22];
//		midi_instrument MI;
		
    // Default constructor.
		Trackinfo()
		: first_channel(0), last_channel(0),  smalest_note(0),	biggest_note(1), mchannel(0)
		{}
    };	

//Trackinfo_t TI = new Trackinfo; 
	Trackinfo TI[64];  // for now 32 tracks limit, i never saw bigger midi files
	int	tchannel[128];
	// 		i'd like to know
	//  	what measure infos do we have?
	//
	//		first and last event time  i.o.w. length
	//		number of differnt notes of each track   =>
	//				melody may have more notes than a rythm 
	//
	//
	//		can i divide tracks in pattern
	//				smalest devider  ?
	//				power of 2 devider	?
	//
	//		track smalest note
	//		track biggest note
	//		akkords ?  need more chanals per track
	//

	midi_file = MidiFile_load((char*)f. getFileName() );
	if (midi_file == NULL)
	{
		printf("Midi File not loaded");
		return -7;	
	}
	else
	{
#ifdef VERBOSE	
		printf("Midi File loaded");
#endif
	}

	int resol = MidiFile_getResolution( midi_file);
#ifdef VERBOSE	
	printf(" \n Resolution  %i  \n ", resol );        //  96   128
#endif	
//  same like	
//	int TpB = MidiFile_getNumberOfTicksPerBeat( midi_file );
//	printf("Ticksperbeat  %i  \n ", TpB );   //  That should be ticks per quarternote

#ifdef VERBOSE						
	float nFpS = MidiFile_getNumberOfFramesPerSecond( midi_file);
	printf(" NumberOfFramesPerSecond %f  \n ",  nFpS );
#endif						
 //   division: delta timing value: positive = units/beat; negative = smpte compatible units (?)
	MidiFileDivisionType_t  eDt = MidiFile_getDivisionType( midi_file);
#ifdef VERBOSE
	if(eDt==MIDI_FILE_DIVISION_TYPE_PPQ)
		printf("          MIDI_FILE_DIVISION_TYPE_PPQ        \n  ");
	else
	printf("          MIDI_FILE_DIVISION_TYPE_SMPTE        \n  ");	
#endif	
	float last_beat = 0.0;
	long last_tick =0;
	MidiFileEvent_t last_event = MidiFile_getLastEvent(midi_file);
	while(! MidiFileEvent_isNoteEndEvent(last_event))
	{
			last_event = MidiFileEvent_getPreviousEvent(last_event);
	}
	if (last_event != NULL) 
	{
		last_tick = MidiFileEvent_getTick(last_event);
		last_beat = MidiFile_getBeatFromTick(midi_file, last_tick );
#ifdef VERBOSE		
		printf("  \n  last beat %f   \n  ", last_beat  );
#endif		
	}
	else
		return -7;
#ifdef VERBOSE
		printf("Midifile is empty                  ");
#endif		
 				
	TI[0].last_channel=0;
	int trackcount = MidiFile_getNumberOfTracks( midi_file);	
	
	for(tracks =0 ; tracks <= trackcount; tracks++ )
	{
		TI[tracks].mchannel=0;
		TI[tracks].num_playing=0;
		int nnotes=0;
		Track = MidiFile_getTrackByNumber( midi_file, tracks, 0);
		for( int no = 1; no < 125; no++)
		{ 
			TI[tracks].is_playing[no]=0;  
			TI[tracks].differnt_notes[no] = 0;
		 }
		TI[tracks].smalest_note = 124;
		TI[tracks].biggest_note = 0;
		midi_event = MidiFileTrack_getFirstEvent(Track);
		if (MidiFileEvent_isNoteStartEvent(midi_event))
		{
			Note_= MidiFileNoteOnEvent_getNote(midi_event);
			TI[tracks].is_playing[Note_] =1;
			TI[tracks].differnt_notes[Note_] = 1;
			TI[tracks].smalest_note = Note_;
			TI[tracks].biggest_note = Note_;
			nnotes=1;
		}
		for (; midi_event != NULL; midi_event = MidiFileEvent_getNextEventInTrack(midi_event))
		{
			
			if (MidiFileEvent_isNoteStartEvent(midi_event))
			{
				if(!TI[tracks].mchannel)
				{
					if(tracks && atall)
						TI[tracks].first_channel= TI[tracks - 1].last_channel+1;
					else	
						TI[tracks].first_channel= 0;

					TI[tracks].mchannel = MidiFileNoteOnEvent_getChannel( midi_event);
					if(TI[tracks].mchannel)
					{
#ifdef VERBOSE					
						printf(" \n Midi Track %i has  MidiChannel %i and trackchannel %i \n", tracks, TI[tracks].mchannel, TI[tracks].first_channel );
#endif						
						;
					}
					else TI[tracks].mchannel=-1;
					
				}
				if(!atall) atall=1;
				Note_= MidiFileNoteOnEvent_getNote(midi_event);
				if(Note_>TI[tracks].biggest_note)
					TI[tracks].biggest_note=Note_;
				if(Note_<TI[tracks].smalest_note)
					TI[tracks].smalest_note = Note_;	
				if(!TI[tracks].is_playing[ Note_])   //  ! allready on so it's another
				{
					TI[tracks].playingNote[TI[tracks].num_playing++]=Note_;
					if(TI[tracks].akkord <TI[tracks].num_playing)
						TI[tracks].akkord = TI[tracks].num_playing;
				}
				TI[tracks].is_playing[ Note_]++;	
				TI[tracks].differnt_notes[Note_] = 1;
				nnotes++;
				
			}
			else if (MidiFileEvent_isNoteEndEvent(midi_event))
			{
				int Note_= MidiFileNoteEndEvent_getNote(midi_event);
				if(TI[tracks].is_playing[ Note_])
				{
#ifdef VERBOSE					
					if(TI[tracks].is_playing[ Note_]>=2)
						printf(" \n note %i pressed %i times", Note_,TI[tracks].is_playing[ Note_] );
#endif						
					TI[tracks].is_playing[ Note_] = 0;
					for(int jj=0;jj< TI[tracks].num_playing; jj++)
					{
						if( TI[tracks].playingNote[ jj ] == Note_ )
						{
								for(int jjn=jj;jjn< TI[tracks].num_playing -1 ; jjn++)
								{
										TI[tracks].playingNote[ jjn ] = TI[tracks].playingNote[ jjn +1 ];
								}
								TI[tracks].num_playing--;
						}
					}
				}
			}
		}
		int differ=0;
		for( int no = 12; no < 120; no++)
		{ 
			if( TI[tracks].differnt_notes[no] == 1)
				differ++;
		}
		if(TI[tracks].smalest_note >= 120)  // no notes
		{
			TI[tracks].smalest_note = 0;
			if(tracks)
			{
					TI[tracks].first_channel = TI[tracks -1 ].first_channel;
					TI[tracks].last_channel = TI[tracks - 1].last_channel ;		
			}
#ifdef VERBOSE			
			printf(" \n  Track %i has no notes", tracks );
#endif			
		}
		else
		{	
			num_channl+= TI[tracks].akkord;  // channl  is a channel in the new module
			num_instr++; 
			TI[tracks].last_channel = num_channl-1;
#ifdef VERBOSE			
			printf(" \n  Track %is biggest  Akkord  has   %i    Notes %i Notes at all", tracks, TI[tracks].akkord, nnotes );		
			printf(" \n             biggest Note is %i, smalest is %i ,   %i different  \n",  TI[tracks].biggest_note,  TI[tracks].smalest_note , differ );
			printf(" last_channel  %i \n", TI[tracks].last_channel);    //%lu
#endif			
		}
		
	}
//	printf(" \n came here  error later");
	char* songname=getFileNameFromPath((char*)f. getFileName());
	memcpy(header->name,songname,20);   //strlen(songname));

	header->tempo = 4  ; 
	header->speed = resol/2;  //140;				// default tempo  bpm?
	
	header->patnum = (int) (last_beat / 8) ; // number of patterns
	float rest = fmod(last_beat,8);
	float restteil = (last_beat - rest ) / last_beat;
	
	//header->patnum = (int) (last_tick / 16/ resol) ;
//	int patnum=header->patnum / 4;
	header->mainvol = 188;  //XModule::vol64to255(128);
	

	printf(" \n  %i channels, %i instruments", num_channl, num_instr );
//	header->flags = module->MODULE_OLDS3MVOLSLIDES | module->MODULE_STMARPEGGIO;
	
	header->insnum = num_instr;			// number of instruments
	header->smpnum = 0;							// number of samples
	header->channum = num_channl+1;
	
	
	///////////////////////	

	
	header->ordnum = header->patnum;
	float beatperrow =  0.12500f;  // last_beat* restteil) / header->patnum/ 64;
	
	//printf("\n beatperpattern  %f " , beatperrow);
	//beatperrow /= 64;
//	printf("\n beatperrow  %f " , beatperrow);
	//  allocate                           the modul
	for (i = 0; i < header->patnum+5;i++) 
	{
		header->ord[i] = i; 

		phead[i].rows = 64;
		phead[i].effnum = 2;
		phead[i].channum = (mp_ubyte)header->channum;
		
		phead[i].patternData = new mp_ubyte[phead[i].rows*header->channum*6];
		
		// out of memory?
		if (phead[i].patternData == NULL)
		{
//			delete[] srcPattern;
			return -7;
		}
		
		memset(phead[i].patternData,0,phead[i].rows*header->channum*6);

	}
	
	int  on = 0, rowi;
	float  beat;
	long tack;
	int tchannelcount = 0;
	int tchann=0;
	for( int no = 1; no < 125; no++)
	{  				
		tchannel[no] = 0;
	}
	for(tracks =0 ; tracks <= trackcount-1; tracks++ )
	{
		if(TI[tracks].smalest_note >= 1 ) 
		{
			tchannelcount++;
	//		printf(" \n came here  trtack %i  ", tracks);

			TI[tracks].num_playing = 0;
				
			for( int no = 1; no < 125; no++)
			{ 
				TI[tracks].is_playing[no]=0;  
			}
			Track = MidiFile_getTrackByNumber( midi_file, tracks, 0);
			for (midi_event = MidiFileTrack_getFirstEvent(Track); midi_event != NULL; midi_event = MidiFileEvent_getNextEventInTrack(midi_event))
			{
				tack=	MidiFileEvent_getTick(midi_event);	
				beat = MidiFile_getBeatFromTick(midi_file, tack);
				on=0;
				
				switch (  MidiFileEvent_getType( midi_event))
				{
					//case MIDI_FILE_EVENT_TYPE_KEY_PRESSURE:
					//case MIDI_FILE_EVENT_TYPE_CONTROL_CHANGE:
					case MIDI_FILE_EVENT_TYPE_PROGRAM_CHANGE:
					{
							int cha = MidiFileProgramChangeEvent_getChannel(midi_event);
							int inst = MidiFileProgramChangeEvent_getNumber(midi_event);
							// for now i handle the usual case:  one instrume per midisongchannel  
							TI[tracks].minstrument = (char *) tran_inst(inst);
							printf("\n ProgramChange  midichannel %i  instr %i ID %i = %s   \n  ", cha, inst, tracks, TI[tracks].minstrument);
							// break;   //    crash  ???
					}
				
					case MIDI_FILE_EVENT_TYPE_NOTE_ON:
		//		if (MidiFileEvent_isNoteStartEvent(midi_event))
					{
						Note_= MidiFileNoteOnEvent_getNote(midi_event);
						Velocity_ = MidiFileNoteOnEvent_getVelocity(midi_event);

						if(!TI[tracks].is_playing[ Note_])   //  not allready on so it's one more
						{
								TI[tracks].playingNote[TI[tracks].num_playing++]=Note_;
								tchannel[Note_]= TI[tracks].num_playing;						
								tchann = tchannel[Note_];
								on=1;
						}
						TI[tracks].is_playing[ Note_]++;	

						//on=1;
						break;
					}
					case MIDI_FILE_EVENT_TYPE_NOTE_OFF:
					//else if (MidiFileEvent_isNoteEndEvent(midi_event))
					{
						int Note_= MidiFileNoteEndEvent_getNote(midi_event);
						if(TI[tracks].is_playing[ Note_])
						{
							TI[tracks].is_playing[ Note_] = 0;
							tchann = tchannel[Note_];
							for(int jj=0;jj< TI[tracks].num_playing; jj++)
							{
								if( TI[tracks].playingNote[ jj ] == Note_ )
								{
										for(int jjn=jj;jjn< TI[tracks].num_playing -1 ; jjn++)
										{
											TI[tracks].playingNote[ jjn ] = TI[tracks].playingNote[ jjn +1 ];
										}
										
								}
							}
							TI[tracks].num_playing--;
							tchannel[Note_]=0;
						}
						on = -1;
						if( TI[tracks].num_playing <= -1) 			TI[tracks].num_playing=0;
					}
				} // switch eventtype
				
						
				/*
					rowi =(int) beat % 16;    
					rowi *=  4;
					float rwoi = fmod( beat,  16);
					rowi =  (int)rwoi * 4;*/
				if( on != 0 )
				{	
					float bea = beat * restteil + 1;				
					i	= (int) (bea / 8);//	
	
					int rowbefore= i * 64 ;
					float rwo = bea / beatperrow;
					rowi =  (int)  ( rwo - rowbefore  );
					
					if(rowi >= 64)   rowi = 63;
					else if(rowi < 0) rowi = 0;
					
		//			patternData=phead[i].rows*header->channum*6
		//			int slotSize = 6          pattern->effnum*2 + 2;
		//			int offset = (slotSize*pattern->channum)*row + (tr_channel*slotSize);
					int  tr_channel, offs  ;
					tr_channel = TI[tracks]. first_channel   + tchann -1;
					
		//			if(tr_channel>=TI[tracks].first_channel&& tr_channel<=TI[tracks].last_channel)	
				//	{
					offs = (header->channum* rowi + tr_channel) *6;
					
					if (on >= 1)
					{
						phead[i].patternData[offs]=Note_;
						phead[i].patternData[offs+1]=tracks+1 ;//TI[tracks].minstrument;
						phead[i].patternData[offs+2] = 0x0C;
						phead[i].patternData[offs+3] = Velocity_* 2 - 1 ;
					}
					else if	(on <= -1)
					{
						phead[i].patternData[offs]=121;
						phead[i].patternData[offs+1]=tracks +1;//TI[tracks].mchannel;
					}
					
					on=0;

/*					phead[i].patternData[cnt+4]=nEff;
					phead[i].patternData[cnt+5]=nOp;
			*/	
				} //if(on)
				
		
			}   //  event		
		} 
	}//			trackcount

 MidiFile_free(midi_file);
/*
XIInstrument::load(const SYSCHAR* fileName)
 bool ModuleEditor::insertXIInstrument(mp_sint32 index, const XIInstrument* ins)
 
 #ifdef VERBOSE
*/ 

//	delete[] srcPattern;

//	strcpy(header->tracker,"MidiFile");
	
//	module->setDefaultPanning();
	
//	module->postProcessSamples();
	
	return 0;
}

