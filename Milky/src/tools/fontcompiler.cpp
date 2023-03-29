/*
 *  tools/fontcompiler.cpp
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

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "BasicTypes.h"
#include "Font.h"
#include "PPPathFactory.h"
#include "XMFile.h"

using namespace std;

const pp_uint32 fontWidth = 8;
const pp_uint32 fontHeight = 8;

pp_uint8 defaultFont[256*8] = {
0,0,0,0,0,0,0,0,
126,129,165,129,189,153,129,126,
126,255,219,255,195,231,255,126,
108,254,254,254,124,56,16,0,
16,56,124,254,124,56,16,0,
56,124,56,254,254,146,16,124,
0,16,56,124,254,124,56,124,
0,0,24,60,60,24,0,0,
255,255,231,195,195,231,255,255,
0,60,102,66,66,102,60,0,
255,195,153,189,189,153,195,255,
15,7,15,125,204,204,204,120,
60,102,102,102,60,24,126,24,
63,51,63,48,48,112,240,224,
127,99,127,99,99,103,230,192,
153,90,60,231,231,60,90,153,
128,224,248,254,248,224,128,0,
2,14,62,254,62,14,2,0,
24,60,126,24,24,126,60,24,
102,102,102,102,102,0,102,0,
127,219,219,123,27,27,27,0,
62,99,56,108,108,56,134,252,
0,0,0,0,126,126,126,0,
24,60,126,24,126,60,24,255,
24,60,126,24,24,24,24,0,
24,24,24,24,126,60,24,0,
0,24,12,254,12,24,0,0,
0,48,96,254,96,48,0,0,
0,0,192,192,192,254,0,0,
0,36,102,255,102,36,0,0,
0,24,60,126,255,255,0,0,
0,255,255,126,60,24,0,0,
0,0,0,0,0,0,0,0,
48,48,48,48,48,0,48,0,
108,108,108,0,0,0,0,0,
108,108,254,108,254,108,108,0,
24,126,192,124,6,252,24,0,
0,102,204,24,48,102,204,0,
56,108,56,118,220,204,118,0,
12,12,12,0,0,0,0,0,
24,48,96,96,96,48,24,0,
96,48,24,24,24,48,96,0,
0,102,60,255,60,102,0,0,
0,24,24,126,24,24,0,0,
0,0,0,0,0,24,48,0,
0,0,0,126,0,0,0,0,
0,0,0,0,0,0,24,0,
0,6,12,24,48,96,0,0,
124,198,214,214,214,198,124,0,
112,48,48,48,48,52,124,0,
248,12,12,120,192,192,252,0,
248,12,12,120,12,12,248,0,
192,204,204,204,254,12,12,0,
252,192,192,248,12,12,248,0,
192,192,192,252,198,198,124,0,
252,12,12,24,48,48,48,0,
56,108,108,124,198,198,124,0,
124,198,198,126,6,6,6,0,
0,0,24,0,0,24,0,0,
0,0,24,0,0,24,48,0,
0,12,24,48,24,12,0,0,
0,0,126,0,126,0,0,0,
0,48,24,12,24,48,0,0,
60,102,6,12,24,0,24,0,
60,102,219,219,206,96,60,0,
60,102,198,198,254,198,198,0,
252,198,198,252,198,198,252,0,
62,96,192,192,192,96,62,0,
252,198,198,198,198,198,252,0,
254,192,192,254,192,192,254,0,
254,192,192,254,192,192,192,0,
62,96,192,206,198,198,124,0,
198,198,198,254,198,198,198,0,
24,24,24,24,24,24,24,0,
6,6,6,6,6,6,124,0,
198,204,216,240,216,204,198,0,
192,192,192,192,192,192,254,0,
252,214,214,214,214,214,214,0,
248,204,198,198,198,198,198,0,
60,102,198,198,198,198,124,0,
252,198,198,198,252,192,192,0,
60,102,198,198,198,218,108,6,
252,198,198,198,252,198,195,0,
62,96,192,124,6,6,252,0,
126,24,24,24,24,24,24,0,
198,198,198,198,198,198,124,0,
198,198,198,198,198,108,56,0,
214,214,214,214,214,214,252,0,
198,198,198,124,198,198,198,0,
204,204,204,120,48,48,48,0,
254,6,12,24,48,96,254,0,
120,96,96,96,96,96,120,0,
0,96,48,24,12,6,0,0,
120,24,24,24,24,24,120,0,
24,60,102,195,0,0,0,0,
0,0,0,0,0,0,0,255,
48,48,48,0,0,0,0,0,
0,0,124,6,126,198,126,0,
192,192,252,198,198,198,252,0,
0,0,126,192,192,192,126,0,
6,6,126,198,198,198,126,0,
0,0,124,198,254,192,126,0,
60,96,248,96,96,96,96,0,
0,0,124,198,198,126,6,252,
192,192,252,198,198,198,198,0,
24,0,120,24,24,24,24,0,
12,0,60,12,12,12,12,248,
192,192,198,204,248,204,198,0,
48,48,48,48,48,48,28,0,
0,0,252,214,214,214,214,0,
0,0,252,198,198,198,198,0,
0,0,124,198,198,198,124,0,
0,0,252,198,198,252,192,192,
0,0,126,198,198,126,6,6,
0,0,252,198,192,192,192,0,
0,0,124,192,120,12,248,0,
48,48,252,48,48,48,28,0,
0,0,198,198,198,198,124,0,
0,0,198,198,198,108,56,0,
0,0,214,214,214,214,252,0,
0,0,198,198,124,198,198,0,
0,0,198,198,198,126,6,252,
0,0,254,12,56,96,254,0,
7,12,12,56,12,12,7,0,
24,24,24,24,24,24,24,24,
224,48,48,28,48,48,224,0,
118,220,0,0,0,0,0,0,
0,16,56,108,198,198,254,0,
255,128,128,128,128,128,128,128,
255,0,0,0,0,0,0,0,
255,1,1,1,1,1,1,1,
128,128,128,128,128,128,128,128,
1,1,1,1,1,1,1,1,
128,128,128,128,128,128,128,255,
0,0,0,0,0,0,0,255,
1,1,1,1,1,1,1,255,
128,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,128,
0,0,0,0,0,0,0,1,
128,192,224,240,248,252,254,255,
255,127,63,31,15,7,3,1,
255,255,192,192,192,192,192,192,
255,255,0,0,0,0,0,0,
255,255,3,3,3,3,3,3,
192,192,192,192,192,192,192,192,
3,3,3,3,3,3,3,3,
192,192,192,192,192,192,255,255,
0,0,0,0,0,0,255,255,
3,3,3,3,3,3,255,255,
192,192,0,0,0,0,0,0,
3,3,0,0,0,0,0,0,
0,0,0,0,0,0,192,192,
0,0,0,0,0,0,3,3,
0,0,0,85,0,0,0,0,
0,252,252,252,252,252,252,0,
0,126,126,126,126,126,126,0,
0,63,63,63,63,63,63,0,
0,31,31,31,31,31,31,0,
0,15,15,15,15,15,15,0,
0,7,7,7,7,7,7,0,
0,3,3,3,3,3,3,0,
0,1,1,1,1,1,1,0,
0,128,128,128,128,128,128,0,
0,192,192,192,192,192,192,0,
0,224,224,224,224,224,224,0,
0,240,240,240,240,240,240,0,
0,248,248,248,248,248,248,0,
126,126,126,126,126,126,126,126,
255,255,0,24,60,126,255,0,
255,255,0,0,0,0,24,24,
255,255,0,24,60,126,255,24,
255,255,0,66,102,90,66,66,
0,0,0,24,24,0,0,0,
0,192,192,192,192,192,192,192,
0,216,216,216,216,216,216,216,
0,219,219,219,219,219,219,219,
0,96,96,96,96,96,96,96,
0,108,108,108,108,108,108,108,
0,109,109,109,109,109,109,109,
0,128,128,128,128,128,128,128,
0,176,176,176,176,176,176,176,
0,182,182,182,182,182,182,182,
0,0,24,60,60,24,0,0,
0,0,248,24,248,24,24,24,
54,54,246,6,246,54,54,54,
54,54,54,54,54,54,54,54,
0,0,254,6,246,54,54,54,
54,54,246,6,254,0,0,0,
54,54,54,54,254,0,0,0,
24,24,248,24,248,0,0,0,
0,0,0,0,248,24,24,24,
24,24,24,24,31,0,0,0,
24,24,24,24,255,0,0,0,
0,0,0,0,255,24,24,24,
24,24,24,24,31,24,24,24,
0,0,0,0,255,0,0,0,
24,24,24,24,255,24,24,24,
24,24,31,24,31,24,24,24,
54,54,54,54,55,54,54,54,
54,54,55,48,63,0,0,0,
0,0,63,48,55,54,54,54,
54,54,247,0,255,0,0,0,
0,0,255,0,247,54,54,54,
54,54,55,48,55,54,54,54,
0,0,255,0,255,0,0,0,
54,54,247,0,247,54,54,54,
24,24,255,0,255,0,0,0,
54,54,54,54,255,0,0,0,
0,0,255,0,255,24,24,24,
0,0,0,0,255,54,54,54,
54,54,54,54,63,0,0,0,
24,24,31,24,31,0,0,0,
0,0,31,24,31,24,24,24,
0,0,0,0,63,54,54,54,
54,54,54,54,255,54,54,54,
24,24,255,24,255,24,24,24,
24,24,24,24,248,0,0,0,
0,0,0,0,31,24,24,24,
255,255,255,255,255,255,255,255,
0,0,0,0,255,255,255,255,
240,240,240,240,240,240,240,240,
15,15,15,15,15,15,15,15,
255,255,255,255,0,0,0,0,
0,0,118,220,200,220,118,0,
0,120,204,248,204,248,192,192,
0,252,204,192,192,192,192,0,
0,0,254,108,108,108,108,0,
252,204,96,48,96,204,252,0,
0,0,126,216,216,216,112,0,
0,102,102,102,102,124,96,192,
0,118,220,24,24,24,24,0,
252,48,120,204,204,120,48,252,
56,108,198,254,198,108,56,0,
56,108,198,198,108,108,238,0,
28,48,24,124,204,204,120,0,
0,0,126,219,219,126,0,0,
6,12,126,219,219,126,96,192,
56,96,192,248,192,96,56,0,
0,0,0,0,0,219,219,0, 									// 239
0,126,0,126,0,126,0,0, 									// 240
0x00,16+8+4+2+1,16,16,16,16,16+8+4+2+1,0x00,			// 241
0x00,255,0,0,0,0,255,0x00,								// 242
0x00,128+64+32+16+8,8,8,8,8,128+64+32+16+8,0x00,        // 243
// Small DOT
0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,				// 244
// ramp 2 wave part 2
0x00,0x00,0x00,0x40,0x80,0x00,0x00,0x00,				// 245
// ramp 2 wave part 1
0x04,0x0c,0x14,0x24,0x44,0x85,0x06,0x04,				// 246
// ramp wave part 2
0x00,0x00,0x00,0x80,0x40,0x00,0x00,0x00,				// 247
// ramp wave part 1
0x04,0x06,0x85,0x44,0x24,0x14,0x0c,0x04,				// 248
// squarewave part 2
0x00,0x00,0x00,0x02,0x02,0x02,0x02,0xfe,				// 249
// squarewave part 1
0xff,0x81,0x81,0x81,0x81,0x01,0x01,0x01,				// 250
// sinewave part 2
0x00,0x00,0x00,0x04,0x04,0x88,0x88,0x70,				// 251
// sinewave part 1
0x1c,0x22,0x22,0x41,0x41,0x00,0x00,0x00,				// 252
// arrow down
0x00,0x00,0x7f,0x3e,0x1c,0x08,0x00,0x00,				// 253
// arrow up
0x00,0x00,0x08,0x1c,0x3e,0x7f,0x00,0x00,				// 254
// checkbox marker:
0x01,1+2+128,128+64+4+2,64+32+8+4,32+16+8,16};			// 255

// pseudo syntax checkin ?x? where ? can be any combination of numbers
bool checkExtension(const char* ext, pp_uint32& width, pp_uint32& height)
{
	pp_int32 i;

	// skip period
	ext++;

	bool noNumber = false;	
	
	PPSystemString widthStr;
	for (i = 0; i < strlen(ext) && ext[i] != 'X'; i++)
	{
		widthStr.append(ext[i]);
		if (ext[i] < '0' || ext[i] > '9')
			noNumber = true;			
	}
	
	if (ext[i] != 'X')
		return false;
	
	if (noNumber)
		return false;

	ext+=i+1;
	
	if (!strlen(ext))
		return false;

	PPSystemString heightStr;
	for (i = 0; i < strlen(ext); i++)
	{
		heightStr.append(ext[i]);
		if (ext[i] < '0' || ext[i] > '9')
			noNumber = true;
	}
	
	if (noNumber)
		return false;
		
	width = atoi(widthStr);
	height = atoi(heightStr);	
	return true;
} 

int main(int argc, const char* argv[])
{
	bool writeRawImage = true;

	pp_int32 i = 0;
	
	// output
	fstream f("FontLibrary.cpp", ios::out);	
	
	f << "struct FontEntry\n"
		 "{\n	const char* name;\n"
		 "	const unsigned int width, height;\n"
		 "	const unsigned char* data;\n"		 
		 "};\n\n";
	
	// Input
	PPPath* path = PPPathFactory::createPathFromString("../../resources/fonts/");
	
	const PPPathEntry* entry = path->getFirstEntry();
	
	f << "FontEntry fontEntries[] = {\n";
	
	pp_uint32 numFonts = 0;
	
	while (entry)
	{
		if (!entry->isHidden() && entry->isFile())
		{				
			PPSystemString extension = entry->getName().getExtension();
			extension.toUpper();
			
			// Check for font file
			pp_uint32 width, height;
			if (checkExtension(extension, width, height))
			{
			
				PPSystemString currentFile = path->getCurrent();
				currentFile.append(entry->getName());
				//cout << currentFile << endl;

				f << "// Font no. " << numFonts << endl;
				f << "FontEntry(" << '"' << entry->getName().stripExtension() << '"' << ", " << width << ", " << height << ", " << endl;				
				
				XMFile input(currentFile);
				
				double dsize = ceil(width*height*256.0)/8.0f;
				pp_uint32 size = (pp_uint32)dsize;
				cout << "Size: " << size << endl;
				
				pp_uint8* data = new pp_uint8[size];
				
				bool flip = false;
				if (width == 8 && height == 8)
				{
					cout << "Font is 8x8 => x-flipping + adding special characters" << endl;
					// char 239-256 are coming from out default font
					// those are special characters necessary for
					// displaying special pattern data
					// and symbols
					memcpy(data, defaultFont, 256*8);
				
					input.read(data, 8, 239);
					
					memcpy(data + 0xc4*8, defaultFont + 0xc4*8, 8);
					flip = true;
				}
				else
				{
					memset(data, 0, size);
					input.read(data, 1, size);
				}
				
				// fill in stream
				pp_uint32 j = 0;
				for (pp_int32 i = 0; i < size; i++)
				{
					if (j == 0)
						f << '"';
				
					pp_uint32 src = data[i];

					pp_uint8 dst = 0;
					if (flip)
					{
						dst = (src&1)<<7;
						dst |= ((src&2)>>1)<<6;
						dst |= ((src&4)>>2)<<5;
						dst |= ((src&8)>>3)<<4;
						dst |= ((src&16)>>4)<<3;
						dst |= ((src&32)>>5)<<2;
						dst |= ((src&64)>>6)<<1;
						dst |= ((src&128)>>7);
					}
					else 
						dst = src;
				
					char hexStr[100];
					sprintf(hexStr, "\\x%x", dst);
				
					if (i == size - 1)
						f << hexStr << '"' << endl;
					else
						f << hexStr;
					
					j++;
					if (j == 64 && i != size - 1)
					{
						f << '"' << endl;
						j = 0;
					}
				}
				
				if (writeRawImage)				
				{
					
					PPSystemString file = currentFile.stripPath();
					
					file.append(".raw");
					
					cout << "Writing raw image: " << file << endl;
					
					char* rawImage = new char[256*width*height];
					
					Bitstream bitstream(data, size);
					
					for (pp_int32 j = 0; j < width*height*256; j++)
						rawImage[j] = bitstream.read(j) ? 255 : 0;
					
					fstream bfstream(file,ios::out|ios::binary);
			        bfstream.write(rawImage, 256*width*height);
          			bfstream.close();					
					
					delete[] rawImage;
					
				}
				
				delete data;
				
				numFonts++;
				
				f << ")," << endl;
			}
			else
			{
				cout << "Unrecognized extension for " << entry->getName() << endl;
			}
			
		}
		
	
		entry = path->getNextEntry();
	}

	f << "};" << endl << endl << "const unsigned int numFontsEntries = " << numFonts << ";" << endl;

	

	return 0;
}