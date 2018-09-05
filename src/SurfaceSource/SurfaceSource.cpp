#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <valarray>
#include <climits>
#include <sstream>
#include <sys/stat.h>
#include <bitset>
#include "SurfaceSource.hpp"

/*
Adapated from https://wiki.calculquebec.ca/w/C%2B%2B_:_fichier_Fortran_binaire/en
*/
void SurfaceSource::Init(){
	// init paramters
	strncpy(id,		"        \0"																			,  9 );
	strncpy(kods,	"        \0"																			,  9 );
	strncpy(vers,	"     \0"																				,  6 );
	strncpy(lods,	"        \0"																			,  9 );
	strncpy(idtms,	"                   \0"																	, 20 );
	strncpy(probs,	"                   \0"																	, 20 );
	strncpy(aids,	"                                                                                \0"	, 81 );
	knods			= 0;
	np1				= 0;
	nrss			= 0;
	nrcd			= 0;
	njsw			= 0;
	niss			= 0;
	niwr			= 0;
	mipts			= 0;
	kjaq			= 0;
	surface_count	= 0;
	// init particle lookup table
	strncpy(particles[  1-1].name , "neutron\0"						, 8);
	strncpy(particles[  5-1].name , "anti-neutron\0"				,13);
	strncpy(particles[  2-1].name , "photon\0"						, 7);
	strncpy(particles[  3-1].name , "electron\0"					, 9);
	strncpy(particles[  8-1].name , "positron\0"					, 9);
	strncpy(particles[  4-1].name , "muon-\0" 						, 6);
	strncpy(particles[ 16-1].name , "muon+\0"				    	, 6);
	strncpy(particles[  6-1].name , "electron neutrino\0"			,18);
	strncpy(particles[ 17-1].name , "anti-electron neutrino\0"		,23);
	strncpy(particles[  7-1].name , "muon neutrino\0"				,14);
	strncpy(particles[ 18-1].name , "anti muon neutrino\0"			,19);
	strncpy(particles[  9-1].name , "proton\0"						, 7);
	strncpy(particles[ 19-1].name , "anti-proton\0"					,12);
	strncpy(particles[ 10-1].name , "lambda baryon\0"				,14);
	strncpy(particles[ 25-1].name , "anti lambda baryon\0"			,19);
	strncpy(particles[ 11-1].name , "positive sigma baryon\0"		,22);
	strncpy(particles[ 26-1].name , "anti positive sigma baryon\0"	,17);
	strncpy(particles[ 12-1].name , "negative sigma baryon\0"		,22);
	strncpy(particles[ 27-1].name , "anti negative sigma baryon\0"	,27);
	strncpy(particles[ 13-1].name , "cascade\0"						, 8);
	strncpy(particles[ 28-1].name , "anti cascade\0"				,13);
	strncpy(particles[ 14-1].name , "negative cascade\0"			,17);
	strncpy(particles[ 29-1].name , "positive cascade\0"			,17);
	strncpy(particles[ 15-1].name , "omega baryon\0"				,13);
	strncpy(particles[ 30-1].name , "anti omega baryon\0"			,18);
	strncpy(particles[ 20-1].name , "positive pion\0"				,14);
	strncpy(particles[ 35-1].name , "negative pion\0"				,14);
	strncpy(particles[ 21-1].name , "neutral pion\0"				,13);
	strncpy(particles[ 22-1].name , "positive kaon\0"				,14);
	strncpy(particles[ 36-1].name , "negative kaon\0"				,14);
	strncpy(particles[ 23-1].name , "kaon, short\0"					,12);
	strncpy(particles[ 24-1].name , "kaon, long\0"					,11);
	strncpy(particles[ 31-1].name , "deuteron\0"					, 9);
	strncpy(particles[ 32-1].name , "triton\0"						, 7);
	strncpy(particles[ 33-1].name , "helion\0"						, 7);
	strncpy(particles[ 34-1].name , "alpha\0"						, 6);
	strncpy(particles[ 37-1].name , "heavy ions\0"					,11);
	//
	strncpy(particles[  1-1].symbol , "n\0", 2);
	strncpy(particles[  5-1].symbol , "q\0", 2);
	strncpy(particles[  2-1].symbol , "p\0", 2);
	strncpy(particles[  3-1].symbol , "e\0", 2);
	strncpy(particles[  8-1].symbol , "f\0", 2);
	strncpy(particles[  4-1].symbol , "|\0", 2);
	strncpy(particles[ 16-1].symbol , "!\0", 2);
	strncpy(particles[  6-1].symbol , "u\0", 2);
	strncpy(particles[ 17-1].symbol , "<\0", 2);
	strncpy(particles[  7-1].symbol , "v\0", 2);
	strncpy(particles[ 18-1].symbol , ">\0", 2);
	strncpy(particles[  9-1].symbol , "h\0", 2);
	strncpy(particles[ 19-1].symbol , "g\0", 2);
	strncpy(particles[ 10-1].symbol , "l\0", 2);
	strncpy(particles[ 25-1].symbol , "b\0", 2);
	strncpy(particles[ 11-1].symbol , "+\0", 2);
	strncpy(particles[ 26-1].symbol , "_\0", 2);
	strncpy(particles[ 12-1].symbol , "-\0", 2);
	strncpy(particles[ 27-1].symbol , "~\0", 2);
	strncpy(particles[ 13-1].symbol , "x\0", 2);
	strncpy(particles[ 28-1].symbol , "c\0", 2);
	strncpy(particles[ 14-1].symbol , "y\0", 2);
	strncpy(particles[ 29-1].symbol , "w\0", 2);
	strncpy(particles[ 15-1].symbol , "o\0", 2);
	strncpy(particles[ 30-1].symbol , "@\0", 2);
	strncpy(particles[ 20-1].symbol , "/\0", 2);
	strncpy(particles[ 35-1].symbol , "*\0", 2);
	strncpy(particles[ 21-1].symbol , "z\0", 2);
	strncpy(particles[ 22-1].symbol , "k\0", 2);
	strncpy(particles[ 36-1].symbol , "?\0", 2);
	strncpy(particles[ 23-1].symbol , "%\0", 2);
	strncpy(particles[ 24-1].symbol , "^\0", 2);
	strncpy(particles[ 31-1].symbol , "d\0", 2);
	strncpy(particles[ 32-1].symbol , "t\0", 2);
	strncpy(particles[ 33-1].symbol , "s\0", 2);
	strncpy(particles[ 34-1].symbol , "a\0", 2);
	strncpy(particles[ 37-1].symbol , "#\0", 2);
	// init surface lookup table
	strncpy(surface_card[ 0].symbol , "XXX\0" , 4);
	strncpy(surface_card[ 1].symbol , "p  \0" , 4);
	strncpy(surface_card[ 2].symbol , "px \0" , 4);
	strncpy(surface_card[ 3].symbol , "py \0" , 4);
	strncpy(surface_card[ 4].symbol , "pz \0" , 4);
	strncpy(surface_card[ 5].symbol , "so \0" , 4);
	strncpy(surface_card[ 6].symbol , "s  \0" , 4);
	strncpy(surface_card[ 7].symbol , "sx \0" , 4);
	strncpy(surface_card[ 8].symbol , "sy \0" , 4);
	strncpy(surface_card[ 9].symbol , "sz \0" , 4);
	strncpy(surface_card[10].symbol , "c/x\0" , 4);
	strncpy(surface_card[11].symbol , "c/y\0" , 4);
	strncpy(surface_card[12].symbol , "c/z\0" , 4);
	strncpy(surface_card[13].symbol , "cx \0" , 4);
	strncpy(surface_card[14].symbol , "cy \0" , 4);
	strncpy(surface_card[15].symbol , "cz \0" , 4);
	strncpy(surface_card[16].symbol , "k/x\0" , 4);
	strncpy(surface_card[17].symbol , "k/y\0" , 4);
	strncpy(surface_card[18].symbol , "k/z\0" , 4);
	strncpy(surface_card[19].symbol , "kx \0" , 4);
	strncpy(surface_card[20].symbol , "ky \0" , 4);
	strncpy(surface_card[21].symbol , "kz \0" , 4);
	strncpy(surface_card[22].symbol , "sq \0" , 4);
	strncpy(surface_card[23].symbol , "gq \0" , 4);
	strncpy(surface_card[24].symbol , "tx \0" , 4);
	strncpy(surface_card[25].symbol , "ty \0" , 4);
	strncpy(surface_card[26].symbol , "tz \0" , 4);
	strncpy(surface_card[27].symbol , "x  \0" , 4);
	strncpy(surface_card[28].symbol , "y  \0" , 4);
	strncpy(surface_card[29].symbol , "z  \0" , 4);
	strncpy(surface_card[30].symbol , "box\0" , 4);
	strncpy(surface_card[31].symbol , "rpp\0" , 4);
	strncpy(surface_card[32].symbol , "sph\0" , 4);
	strncpy(surface_card[33].symbol , "rcc\0" , 4);
	strncpy(surface_card[34].symbol , "rec\0" , 4);
	strncpy(surface_card[35].symbol , "ell\0" , 4);
	strncpy(surface_card[36].symbol , "trc\0" , 4);
	strncpy(surface_card[37].symbol , "wed\0" , 4);
	strncpy(surface_card[38].symbol , "arb\0" , 4);
	strncpy(surface_card[39].symbol , "rhp\0" , 4);
	strncpy(surface_card[40].symbol , "hex\0" , 4);
	//
	surface_card[ 0].n_coefficients =  0;
	surface_card[ 1].n_coefficients =  4;
	surface_card[ 2].n_coefficients =  1;
	surface_card[ 3].n_coefficients =  1;
	surface_card[ 4].n_coefficients =  1;
	surface_card[ 5].n_coefficients =  1;
	surface_card[ 6].n_coefficients =  4;
	surface_card[ 7].n_coefficients =  2;
	surface_card[ 8].n_coefficients =  2;
	surface_card[ 9].n_coefficients =  2;
	surface_card[10].n_coefficients =  3;
	surface_card[11].n_coefficients =  3;
	surface_card[12].n_coefficients =  3;
	surface_card[13].n_coefficients =  1;
	surface_card[14].n_coefficients =  1;
	surface_card[15].n_coefficients =  1;
	surface_card[16].n_coefficients =  5;
	surface_card[17].n_coefficients =  5;
	surface_card[18].n_coefficients =  5;
	surface_card[19].n_coefficients =  3;
	surface_card[20].n_coefficients =  3;
	surface_card[21].n_coefficients =  3;
	surface_card[22].n_coefficients = 10;
	surface_card[23].n_coefficients = 10;
	surface_card[24].n_coefficients =  6;
	surface_card[25].n_coefficients =  6;
	surface_card[26].n_coefficients =  6;
	surface_card[27].n_coefficients =  0;
	surface_card[28].n_coefficients =  0;
	surface_card[29].n_coefficients =  0;
	surface_card[30].n_coefficients = 12;
	surface_card[31].n_coefficients =  6;
	surface_card[32].n_coefficients =  4;
	surface_card[33].n_coefficients =  7;
	surface_card[34].n_coefficients = 12;
	surface_card[35].n_coefficients =  7;
	surface_card[36].n_coefficients =  8;
	surface_card[37].n_coefficients = 12;
	surface_card[38].n_coefficients = 30;
	surface_card[39].n_coefficients = 15;
	surface_card[40].n_coefficients = 15;
	//
	strncpy(surface_card[ 0].description , "INDEXING ERROR                          \0", 41);
	strncpy(surface_card[ 1].description , "General plane                           \0", 41);
	strncpy(surface_card[ 2].description , "Plane normal to X-axis                  \0", 41);
	strncpy(surface_card[ 3].description , "Plane normal to Y-axis                  \0", 41);
	strncpy(surface_card[ 4].description , "Plane normal to Z-axis                  \0", 41);
	strncpy(surface_card[ 5].description , "Sphere centered at the origin           \0", 41);
	strncpy(surface_card[ 6].description , "General sphere                          \0", 41);
	strncpy(surface_card[ 7].description , "Sphere centered on X-axis               \0", 41);
	strncpy(surface_card[ 8].description , "Sphere centered on Y-axis               \0", 41);
	strncpy(surface_card[ 9].description , "Sphere centered on Z-axis               \0", 41);
	strncpy(surface_card[10].description , "Cylinder parallel to X-axis             \0", 41);
	strncpy(surface_card[11].description , "Cylinder parallel to Y-axis             \0", 41);
	strncpy(surface_card[12].description , "Cylinder parallel to Z-axis             \0", 41);
	strncpy(surface_card[13].description , "Cylinder on X-axis                      \0", 41);
	strncpy(surface_card[14].description , "Cylinder on Y-axis                      \0", 41);
	strncpy(surface_card[15].description , "Cylinder on Z-axis                      \0", 41);
	strncpy(surface_card[16].description , "Cone parallel to X-axis                 \0", 41);
	strncpy(surface_card[17].description , "Cone parallel to Y-axis                 \0", 41);
	strncpy(surface_card[18].description , "Cone parallel to Z-axis                 \0", 41);
	strncpy(surface_card[19].description , "Cone on X-axis                          \0", 41);
	strncpy(surface_card[20].description , "Cone on Y-axis                          \0", 41);
	strncpy(surface_card[21].description , "Cone on Z-axis                          \0", 41);
	strncpy(surface_card[22].description , "Quadric parallel to X-,Y-, or Z-axis    \0", 41);
	strncpy(surface_card[23].description , "Quadric not parallel to X-,Y-, or Z-axis\0", 41);
	strncpy(surface_card[24].description , "Torus parallel to X-axis                \0", 41);
	strncpy(surface_card[25].description , "Torus parallel to Y-axis                \0", 41);
	strncpy(surface_card[26].description , "Torus parallel to Z-axis                \0", 41);
	strncpy(surface_card[27].description , "Surface defined by points sym. about X  \0", 41);
	strncpy(surface_card[28].description , "Surface defined by points sym. about Y  \0", 41);
	strncpy(surface_card[29].description , "Surface defined by points sym. about Z  \0", 41);
	strncpy(surface_card[30].description , "Arbitrarily oriented orthogonal box     \0", 41);
	strncpy(surface_card[31].description , "Rectangular Parallelepiped              \0", 41);
	strncpy(surface_card[32].description , "Sphere                                  \0", 41);
	strncpy(surface_card[33].description , "Right Circular Cylinder                 \0", 41);
	strncpy(surface_card[34].description , "Right Elliptical Cylinder               \0", 41);
	strncpy(surface_card[35].description , "Ellipsoid                               \0", 41);
	strncpy(surface_card[36].description , "Truncated Right-angle Cone              \0", 41);
	strncpy(surface_card[37].description , "Wedge                                   \0", 41);
	strncpy(surface_card[38].description , "Arbitrary polydron                      \0", 41);
	strncpy(surface_card[39].description , "Right Hexagonal Prism                   \0", 41);
	strncpy(surface_card[40].description , "Right Hexagonal Prism - Same as RHP     \0", 41);
}

//
// CON/DE-STRUCTORS
//
SurfaceSource::~SurfaceSource(){
	input_file.close();
	output_file.close();
}
SurfaceSource::SurfaceSource(const std::string& fileName){
	Init();
	OpenWssaFile_Read(fileName.c_str());
}
SurfaceSource::SurfaceSource(const std::string& fileName1, const std::string& fileName2){
	Init();
	OpenWssaFile_Read( fileName1.c_str());
	OpenWssaFile_Write(fileName2.c_str());
}
SurfaceSource::SurfaceSource(const char*        fileName){
	Init();
	OpenWssaFile_Read(fileName);
}
SurfaceSource::SurfaceSource(const char*        fileName1, const char*        fileName2){
	Init();
	OpenWssaFile_Read(fileName1);
	OpenWssaFile_Write(fileName2);
}
SurfaceSource::SurfaceSource(const std::string& fileName, const int flag){
	Init();
	switch(flag){
		case RSSA_READ:  OpenWssaFile_Read( fileName.c_str());
		case RSSA_WRITE: OpenWssaFile_Write(fileName.c_str());
	}
}
SurfaceSource::SurfaceSource(const char*        fileName, const int flag){
	Init();
	switch(flag){
		case RSSA_READ:  OpenWssaFile_Read( fileName);
		case RSSA_WRITE: OpenWssaFile_Write(fileName);
	}
}

//
// FILE IO
//
bool SurfaceSource::OpenWssaFile_Read(const char* fileName){

	// for file check
	struct stat buffer;

	// set object name
	input_file_name.assign(fileName);

	// open file
	if( (stat (fileName, &buffer) == 0)){
		input_file.open(fileName, std::ios::in | std::ios::binary);
		return true;
	}
	else{
		printf("problem opening %s for reading.\n",fileName);
		return false;
	}

}
bool SurfaceSource::OpenWssaFile_Write(const char* fileName){

	// for file check
	struct stat buffer;

	// set object name
	output_file_name.assign(fileName);

	// open file, make sure it doens't already exist
	if( (stat (fileName, &buffer) != 0)){
		output_file.open(fileName, std::ios::out | std::ios::binary);
		return true;
	}
	else{
		printf("problem opening %s for writing.\n",fileName);
		return false;
	}

}

//
//  RECORD READS
//

bool SurfaceSource::ReadRecord(void** destination, size_t* size, size_t NumberOfEntries)
{
	int record_length0	= 0;
	int record_length1	= 0;
	int null					= 0;
	int length_read		= 0;
	int dist_to_end		= 0;

	if (input_file.good())
	{
		// read starting delimiter
		input_file.read((char*) &record_length0, RECORD_DELIMITER_LENGTH);
		//printf("READ RECORD LENGTH %d\n",record_length0);

		// read what's asked for
		for(int i=0;i<NumberOfEntries;i++){
			length_read = length_read + size[i];
			if(length_read>record_length0){
				printf("DATA REQUESTED (%d) OVERRAN RECORD LENGTH (%d)!\n",length_read,record_length0);
				return false;
			}
			else{
				input_file.read((char*) destination[i], size[i]);
			}
		}

		// go to the end of the record
		dist_to_end = record_length0-length_read;
		if( dist_to_end > 0 ){
			printf("--> skipping ahead %d bytes to end of record after %d entries\n",dist_to_end, NumberOfEntries);
			input_file.seekg(dist_to_end, std::ios::cur);
		}

		// read ending delimiter, assert
		input_file.read((char*) &record_length1, RECORD_DELIMITER_LENGTH);
		if(record_length0!=record_length1){
			printf("BEGINNING (%d) AND ENDING (%d) RECORD LENGTH DELIMITERS DO NOT MATCH\n",record_length0,record_length1);
			return false;
		}
		else{
			return true;
		}
	}
	else
	{
		return false;
	}

}
bool SurfaceSource::ReadSurfaceRecord0(int* numbers, int* types, int* lengths, surface* parameters)
{
	// internal variables
	int record_length0 = 0;
	int record_length1 = 0;

	// read record
	if (input_file.good())
	{
		input_file.read((char*) &record_length0, sizeof(record_length0));
		input_file.read((char*) numbers,	sizeof(int));
		input_file.read((char*) types,		sizeof(int));
		input_file.read((char*) lengths,	sizeof(int));
		input_file.read((char*) parameters,lengths[0]*sizeof(parameters[0].value[0]));
		input_file.read((char*) &record_length1, sizeof(record_length1));
		if(record_length0!=record_length1){
			printf("SurfaceRecord0 BEGINNING (%d) AND ENDING (%d) RECORD LENGTH DELIMITERS DO NOT MATCH\n",record_length0,record_length1);
			return false;
		}
		else{
			return true;
		}
	}
	else
	{
		return false;
	}

}
bool SurfaceSource::ReadSurfaceRecord1(int* numbers, int* types, int* lengths, surface* parameters, int* facets)
{

	// internal variables
	int record_length = 0;

	if (input_file.good())
	{
		input_file.read((char*) &record_length, sizeof(record_length));
		input_file.read((char*) numbers,	sizeof(int));
		input_file.read((char*) facets,		sizeof(int));
		input_file.read((char*) types,		sizeof(int));
		input_file.read((char*) lengths,	sizeof(int));
		input_file.read((char*) parameters,lengths[0]*sizeof(parameters[0].value[0]));
		input_file.seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
		return true;
	}
	else
	{
		return false;
	}

}
bool SurfaceSource::ReadSummaryRecord(int** summaries)
{
	int record_length0	= 0;
	int record_length1	= 0;
	int null			= 0;
	double dnull		= 0.0;
	int length_read		= 0;
	int dist_to_end		= 0;
	int this_record_legnth	= 0;
	if (input_file.good())
	{
		// read starting delimiter
		input_file.read((char*) &record_length0, RECORD_DELIMITER_LENGTH);
		//printf("RECORD LENGTH %d\n",record_length0);

		// read dumb leading (double) zero
		input_file.read((char*) &dnull, sizeof(double));
		length_read = sizeof(double);

		// read what's asked for
		for(int i=0;i<surface_count;i++){
			for(int j=0;j<surface_summary_length;j++){
				length_read = length_read + sizeof(int);
				if(length_read>record_length0){
					printf("SUMMARY DATA REQUESTED (%d) OVERRAN RECORD LENGTH (%d)!\n",length_read,record_length0);
					return false;
				}
				else{
					input_file.read((char*) &summaries[i][j], sizeof(int));
				}
			}
		}

		// go to the end of the record
		dist_to_end = record_length0-length_read;
		if( dist_to_end > 0 ){
			printf("--> skipping ahead %d bytes to end of record\n",dist_to_end);
			input_file.seekg(dist_to_end, std::ios::cur);
		}

		// read ending delimiter, assert
		input_file.read((char*) &record_length1, RECORD_DELIMITER_LENGTH);
		if(record_length0!=record_length1){
			printf("BEGINNING (%d) AND ENDING (%d) SUMMARY RECORD LENGTH DELIMITERS DO NOT MATCH\n",record_length0,record_length1);
			return false;
		}
		else{
			return true;
		}
	}
	else
	{
		return false;
	}

}

//
// RECORD WRITES
//
bool SurfaceSource::WriteRecord(void** source, size_t* size, size_t NumberOfEntries)
{
	int record_length0	= 0;
	for (int i=0;i<NumberOfEntries;i++){
		record_length0 += size[i];
	}
	//printf("WRITE RECORD LENGTH %d\n",record_length0);

	if (output_file.good())
	{
		// read starting delimiter
		output_file.write((char*) &record_length0, RECORD_DELIMITER_LENGTH);

		// write what's given
		for(int i=0;i<NumberOfEntries;i++){
			output_file.write((char*) source[i], size[i]);
		}
		// write ending delimiter
		output_file.write((char*) &record_length0, RECORD_DELIMITER_LENGTH);

		return true;

	}
	else
	{
		printf("OUTPUT FILE NOT GOOD.\n");
		return false;
	}


}


bool SurfaceSource::WriteSurfaceRecord0(int* numbers, int* types, int* lengths, surface* parameters)
{
	// internal variables
	int record_length = 3*sizeof(int) + lengths[0]*sizeof(parameters[0].value[0]);

	// write record
	if (output_file.good())
	{
		output_file.write((char*) &record_length, RECORD_DELIMITER_LENGTH);
		output_file.write((char*) numbers,	sizeof(int));
		output_file.write((char*) types,		sizeof(int));
		output_file.write((char*) lengths,	sizeof(int));
		output_file.write((char*) parameters,lengths[0]*sizeof(parameters[0].value[0]));
		output_file.write((char*) &record_length, RECORD_DELIMITER_LENGTH);
		return true;
	}
	else
	{
		return false;
	}

}
bool SurfaceSource::WriteSurfaceRecord1(int* numbers, int* types, int* lengths, surface* parameters, int* facets)
{

	// internal variables
	int record_length = 4*sizeof(int) + lengths[0]*sizeof(parameters[0].value[0]);

	if (output_file.good())
	{
		output_file.write((char*) &record_length, RECORD_DELIMITER_LENGTH);
		output_file.write((char*) numbers,	sizeof(int));
		output_file.write((char*) facets,		sizeof(int));
		output_file.write((char*) types,		sizeof(int));
		output_file.write((char*) lengths,	sizeof(int));
		output_file.write((char*) parameters,lengths[0]*sizeof(parameters[0].value[0]));
		output_file.write((char*) &record_length, RECORD_DELIMITER_LENGTH);
		return true;
	}
	else
	{
		return false;
	}

}

bool SurfaceSource::WriteSummaryRecord(int** summaries)
{
	int record_length0	= surface_summary_length*surface_count*sizeof(int)+sizeof(double);
	int null			= 0;
	double dnull		= 0.0;
	int length_write	= 0;

	if (output_file.good())
	{
		// write starting delimiter
		output_file.write((char*) &record_length0, RECORD_DELIMITER_LENGTH);

		// write stupid leading (double) zero
		output_file.write((char*) &dnull, sizeof(double));

		// write what's asked for
		for(int i=0;i<surface_count;i++){
			for(int j=0;j<surface_summary_length;j++){
				length_write = length_write + sizeof(int);
				output_file.write((char*) &summaries[i][j], sizeof(int));
			}
		}

		// write ending delimiter, assert
		output_file.write((char*) &record_length0, RECORD_DELIMITER_LENGTH);

		return true;
	}
	else
	{
		return false;
	}

}

//
// HIGHER LEVEL ROUTINES
//
void SurfaceSource::ReadHeader(){
	// HEADER FORMATTING
	//
	// record 1: id;
	// record 2: kods,vers,lods,idtms,probs,aids,knods;
	// record 3: np1,nrss,nrcd,njsw,niss;
	// record 4: niwr,mipts,kjaq;
	//
	// id		= The ID string, should be SF_00001 for MCNP6-made surface source, char8
	// kods		= code name, char8
	// vers		= code version, char5
	// lods		= LODDAT of code that wrote surface source file, char8
	// idtms	= IDTM of the surface source write run, char19
	// probs	= probid, problem id, char19
	// aids		= title string of the creation run, char80
	// knods	= ending dump number, int
	// np1		= total number of histories in SS write run, int
	// nrss		= the total number of tracks recorded, int
	// nrcd		= Number of values in a surface-source record, int
	// njsw		= Number of surfaces in JASW, int
	// niss		= Number of histories in input surface source, int
	// niwr		= Number of cells in RSSA file, int
	// mipts	= Source particle type, int
	// kjaq		= Flag for macrobody facets on source tape, int
	//
	//
	// the next njsw+niwr records describe the surfaces/cells in the SS
	//
	//
	// the last record is the SS summary vector

	// first record
	void** pointers = 	new void*	[20];
	size_t* sizes 	= 	new size_t	[20];
	size_t size = 1;
	pointers[0]	= (void**) &id;
	sizes[0]	= sizeof(id)-1;
	if(!ReadRecord(pointers, sizes, size)){printf("ERROR READING FIRST RECORD\n");std::exit(1);}

	// second record, first make array of pointers, then sizes
	size = 7;
	pointers[0]	= (void*) &kods;
	pointers[1]	= (void*) &vers;
	pointers[2]	= (void*) &lods;
	pointers[3]	= (void*) &idtms;
	pointers[4]	= (void*) &probs;
	pointers[5]	= (void*) &aids;
	pointers[6]	= (void*) &knods;
	sizes[0]	= sizeof(kods)-1;
	sizes[1]	= sizeof(vers)-1;
	sizes[2]	= sizeof(lods)-1;
	sizes[3]	= sizeof(idtms)-1;
	sizes[4]	= sizeof(probs)-1;
	sizes[5]	= sizeof(aids)-1;
	sizes[6]	= sizeof(knods);
	if(!ReadRecord(pointers, sizes, size)){printf("ERROR READING SECOND RECORD\n");std::exit(1);}

	// third record, first make array of pointers, then sizes
	size = 5;
	pointers[0]	= (void*) &np1;
	pointers[1]	= (void*) &nrss;
	pointers[2]	= (void*) &nrcd;
	pointers[3]	= (void*) &njsw;
	pointers[4]	= (void*) &niss;
	sizes[0]	= sizeof(np1);
	sizes[1]	= sizeof(nrss);
	sizes[2]	= sizeof(nrcd);
	sizes[3]	= sizeof(njsw);
	sizes[4]	= sizeof(niss);
	if(!ReadRecord(pointers, sizes, size)){printf("ERROR READING THIRD RECORD\n");std::exit(1);}

	// fourth record, first make array of pointers, then sizes
	size = 3+17;   // 17 zeros written after for whatever reason!
	pointers[0]	= (void*) &niwr;
	pointers[1]	= (void*) &mipts;
	pointers[2]	= (void*) &kjaq;
	int null = 0;
	for (int g=3;g<3+17;g++){pointers[g]=&null;}
	sizes[0]	= sizeof(niwr);
	sizes[1]	= sizeof(mipts);
	sizes[2]	= sizeof(kjaq);
	for (int g=3;g<3+17;g++){sizes[g]=sizeof(null);}
	if(!ReadRecord(pointers, sizes, size)){printf("ERROR READING FOURTH RECORD\n");std::exit(1);}

	// init arays for surface information
	surface_count				= njsw+niwr;
	surface_parameters			= new surface 	[surface_count];
	surface_parameters_lengths	= new int 		[surface_count];
	surface_numbers				= new int 		[surface_count];
	surface_types				= new int 		[surface_count];
	surface_facets				= new int 		[surface_count];
	surface_summaries			= new int* 		[surface_count];
	for(int i = 0 ; i < surface_count ; i++){
		for(int j = 0 ; j < 10 ; j++){
			surface_parameters	[i].value[j]	= 0;
		}
		surface_summaries[i] = new int [surface_summary_length];
		for(int k=0;k<surface_summary_length;k++){
			surface_summaries[i][k]=0;
		}
		surface_numbers			[i] 			= -1;
		surface_facets			[i] 			= -1;
	}


	// go on, copying surface/cell information from the next records until particle data starts
	for(int i = 0 ; i < surface_count ; i++){
		if( kjaq==0 | i>njsw-1 ) {
			ReadSurfaceRecord0(&surface_numbers[i],&surface_types[i],&surface_parameters_lengths[i],&surface_parameters[i]);
		}
		if( kjaq==1 & i<=njsw-1 ) {
			ReadSurfaceRecord1(&surface_numbers[i],&surface_types[i],&surface_parameters_lengths[i],&surface_parameters[i],&surface_facets[i]);
		}
	}

	// last record is the summary tables
	ReadSummaryRecord(surface_summaries);

}


void SurfaceSource::WriteHeader(){
	// HEADER FORMATTING
	//
	// record 1: id;
	// record 2: kods,vers,lods,idtms,probs,aids,knods;
	// record 3: np1,nrss,nrcd,njsw,niss;
	// record 4: niwr,mipts,kjaq;
	//
	// id		= The ID string, should be SF_00001 for MCNP6-made surface source, char8
	// kods		= code name, char8
	// vers		= code version, char5
	// lods		= LODDAT of code that wrote surface source file, char8
	// idtms	= IDTM of the surface source write run, char19
	// probs	= probid, problem id, char19
	// aids		= title string of the creation run, char80
	// knods	= ending dump number, int
	// np1		= total number of histories in SS write run, int
	// nrss		= the total number of tracks recorded, int
	// nrcd		= Number of values in a surface-source record, int
	// njsw		= Number of surfaces in JASW, int
	// niss		= Number of histories in input surface source, int
	// niwr		= Number of cells in RSSA file, int
	// mipts	= Source particle type, int
	// kjaq		= Flag for macrobody facets on source tape, int
	//
	//
	// the next njsw+niwr records describe the surfaces/cells in the SS
	//
	//
	// the last record is the SS summary vector

	// first record
	int largest_size = 3+17;
	void** pointers = 	new void*	[largest_size];
	size_t* sizes 	= 	new size_t	[largest_size];
	size_t size = 1;
	pointers[0]	= (void**) &id;
	sizes[0]	= sizeof(id)-1;
	if(!WriteRecord(pointers, sizes, size)){printf("ERROR WRITING FIRST RECORD\n");std::exit(1);}

	// second record, first make array of pointers, then sizes
	size = 7;
	pointers[0]	= (void*) &kods;
	pointers[1]	= (void*) &vers;
	pointers[2]	= (void*) &lods;
	pointers[3]	= (void*) &idtms;
	pointers[4]	= (void*) &probs;
	pointers[5]	= (void*) &aids;
	pointers[6]	= (void*) &knods;
	sizes[0]	= sizeof(kods)-1;
	sizes[1]	= sizeof(vers)-1;
	sizes[2]	= sizeof(lods)-1;
	sizes[3]	= sizeof(idtms)-1;
	sizes[4]	= sizeof(probs)-1;
	sizes[5]	= sizeof(aids)-1;
	sizes[6]	= sizeof(knods);
	if(!WriteRecord(pointers, sizes, size)){printf("ERROR WRITING SECOND RECORD\n");std::exit(1);}

	// third record, first make array of pointers, then sizes
	size = 5;
	pointers[0]	= (void*) &np1;
	pointers[1]	= (void*) &nrss;
	pointers[2]	= (void*) &nrcd;
	pointers[3]	= (void*) &njsw;
	pointers[4]	= (void*) &niss;
	sizes[0]	= sizeof(np1);
	sizes[1]	= sizeof(nrss);
	sizes[2]	= sizeof(nrcd);
	sizes[3]	= sizeof(njsw);
	sizes[4]	= sizeof(niss);
	if(!WriteRecord(pointers, sizes, size)){printf("ERROR WRITING THIRD RECORD\n");std::exit(1);}

	// fourth record, first make array of pointers, then sizes
	size = 3+17;
	pointers[0]	= (void*) &niwr;
	pointers[1]	= (void*) &mipts;
	pointers[2]	= (void*) &kjaq;
	int null = 0;
	for (int g=3;g<size;g++){pointers[g]=&null;}
	sizes[0]	= sizeof(niwr);
	sizes[1]	= sizeof(mipts);
	sizes[2]	= sizeof(kjaq);
	for (int g=3;g<size;g++){sizes[g]=sizeof(null);}
	if(!WriteRecord(pointers, sizes, size)){printf("ERROR WRITING FOURTH RECORD\n");std::exit(1);}

	// init arays for surface information
	//surface_count								= njsw+niwr;
	//surface_parameters					= new surface [surface_count];
	//surface_parameters_lengths	= new int 		[surface_count];
	//surface_numbers							= new int 		[surface_count];
	//surface_types								= new int 		[surface_count];
	//surface_facets							= new int 		[surface_count];
	//surface_summaries						= new int* 		[surface_count];
	//for(int i = 0 ; i < surface_count ; i++){
	//	for(int j = 0 ; j < 10 ; j++){
	//		surface_parameters	[i].value[j]	= 0;
	//	}
	//	surface_summaries[i] = new int [surface_summary_length];
	//	for(int k=0;k<surface_summary_length;k++){
	//		surface_summaries[i][k]=0;
	//	}
	//	surface_numbers			[i] 			= -1;
	//	surface_facets			[i] 			= -1;
	//}

	// go on, copying surface/cell information from the next records until particle data starts
	for(int i = 0 ; i < surface_count ; i++){
		if( kjaq==0 | i>njsw-1 ) {
			WriteSurfaceRecord0(&surface_numbers[i],&surface_types[i],&surface_parameters_lengths[i],&surface_parameters[i]);
		}
		if( kjaq==1 & i<=njsw-1 ) {
			WriteSurfaceRecord1(&surface_numbers[i],&surface_types[i],&surface_parameters_lengths[i],&surface_parameters[i],&surface_facets[i]);
		}
	}

	// last record is the summary tables
	WriteSummaryRecord(surface_summaries);
	output_file.flush();

}

void SurfaceSource::PrintSizes(){

	printf("== DATA SIZE INFORMATION == \n");
	printf("normal integers  :  %1ld bytes\n",	sizeof(knods));
	printf("floating points  :  %1ld bytes\n",	sizeof(surface_parameters[0].value[0]));
	printf("characters       :  %1ld bytes\n",	sizeof(id[0]));
	printf("\n");

}

void SurfaceSource::PrintHeader(){

	printf("=========================================== HEADER INFORMATION =========================================== \n");
	printf("RSSA ID string                                 :  %8s \n",	id);
	printf("code name                                      :  %8s \n",	kods);
	printf("code version                                   :  %5s \n",	vers);
	printf("LODDAT of code that wrote surface source file  :  %8s \n",	lods);
	printf("IDTM of the surface source write run           :  %19s\n",	idtms);
	printf("probid, problem id                             :  %19s\n",	probs);
	printf("title string of the creation run               :  %80s\n",	aids);
	printf("ending dump number                             :  %d\n",	knods);
	printf("total number of histories in SS write run      :  %lld\n",	np1);
	printf("the total number of tracks recorded            :  %lld\n",	nrss);
	printf("Number of values in a surface-source record    :  %d\n",	nrcd);
	printf("Number of surfaces in JASW                     :  %d\n",	njsw);
	printf("Number of histories in input surface source    :  %lld\n",	niss);
	printf("Number of cells in RSSA file                   :  %d\n",	niwr);
	printf("Source particle type                           :  %d\n",	mipts);
	printf("Flag for macrobody facets on source tape       :  %d\n",	kjaq);


	if(     kjaq == 0 ) {
		printf("\n============================================================ SURFACE INFORMATION ============================================================ \n");
		printf("creation-run surfaces |  surface  |  type  |  coefficients\n");
	}
	else if( kjaq == 1 ) {
		printf("\n============================================================ SURFACE INFORMATION ============================================================ \n");
		printf("creation-run surfaces |  surface  |  facet  |  type  |  coefficients\n");
	}
	for(int i = 0 ; i < surface_count ; i++){
		if(     kjaq == 0 ) {
			printf("                %5d      %5d       %s  ",i,surface_numbers[i],surface_card[surface_types[i]].symbol);
			for(int j = 0 ; j < surface_parameters_lengths[i] ; j++){
				printf(" % 10.8E",surface_parameters[i].value[j]);
			}
			printf("\n");
        }
		else if( kjaq == 1 ) {
			printf("                %5d      %5d        %1d          %s  ",i,surface_numbers[i],surface_facets[i],surface_card[surface_types[i]].symbol);
			for(int j = 0 ; j < surface_parameters_lengths[i] ; j++){
				printf(" % 10.8E",surface_parameters[i].value[j]);
			}
			printf("\n");
		}
	}
	printf("\n");




	printf("\n============================================================= SURFACE SUMMARIES ============================================================= \n");
	int a,b,c,d;
	for(int i=0;i<surface_count;i++){
		printf("--=== Surface %5d ===--\n",surface_numbers[i]);
		printf("%20s  %21s\n","TOTAL TRACKS","INDEPENDENT HISTORIES");
		printf("        %12d           %12d\n",surface_summaries[i][0],surface_summaries[i][1]);
		for(int j=0;j<mipts;j++){
			a = surface_summaries[i][(2+j*4)+0];
			b = surface_summaries[i][(2+j*4)+1];
			c = surface_summaries[i][(2+j*4)+2];
			d = surface_summaries[i][(2+j*4)+3];
			if (a+b+c+d > 0){
				printf("---- %s ----\n",particles[j].name);
				printf("%20s   %20s   %20s   %20s\n","TOTAL TRACKS","INDEPENDENT TRACKS","UNCOLLIDED","INDEP. UNCOLLIDED");
				printf("        %12d           %12d           %12d           %12d\n",a,b,c,d);
			}
		}
		printf("\n");
	}

}

void SurfaceSource::GetTrack(track* this_track){

	// local vars
	//size_t size 	= 1;
	size_t sizes 	= 11*sizeof(double);

	// try reading it all in at once...
	if(!ReadRecord((void**) &this_track, &sizes, 1)){printf("ERROR READING TRACKS RECORD\n");std::exit(1);}

	// calculate missing zhat from the data
	if ((this_track[0].xhat*this_track[0].xhat+this_track[0].yhat*this_track[0].yhat)<1.0){
		this_track[0].zhat   = copysign(std::sqrt(1.0 - this_track[0].xhat*this_track[0].xhat -
		                                         this_track[0].yhat*this_track[0].yhat), this_track[0].bitarray);
	}
	else{
	    this_track[0].zhat = 0.0;
	}

}

void SurfaceSource::PutTrack(double nps, double bitarray, double wgt, double erg, double tme, double x, double y, double z, double xhat, double yhat, double cs, double zhat){

	// local vars
	size_t sizes 	= 11*sizeof(double);  // only write the first 11, track in record doens't have zhat
	double this_track_data[12] = {nps, bitarray, wgt, erg, tme, x, y, z, xhat, yhat, cs, zhat};

	// try reading it all in at once...
	if(!WriteRecord((void**) this_track_data, &sizes, 1)){printf("ERROR WRITING TRACK RECORD\n");std::exit(1);}

}
void SurfaceSource::PutTrack(track* this_track){

	// local vars
	size_t sizes 	= 11*sizeof(double);  // only write the first 11, track in record doens't have zhat

	// try reading it all in at once...
	if(!WriteRecord((void**) &this_track, &sizes, 1)){printf("ERROR WRITING TRACK RECORD\n");std::exit(1);}

}
