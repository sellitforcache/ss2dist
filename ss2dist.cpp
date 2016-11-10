
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
#include "ss2dist.h"
 
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
SurfaceSource::~SurfaceSource(){
	input_file.close();
}
SurfaceSource::SurfaceSource(const std::string& fileName){
	Init();
	OpenWssaFile(fileName.c_str());
}
SurfaceSource::SurfaceSource(const char*        fileName){
	Init();
	OpenWssaFile(fileName);
}

void SurfaceSource::OpenWssaFile(const char* fileName){

	// set object name
	input_file_name.assign(fileName);

	// print the name
	std::string title_file = "======> " + input_file_name + " <======";
	std::cout << "\n" << std::string(title_file.length(), '=') << std::endl;
	std::cout << title_file <<std::endl;
	std::cout << std::string(title_file.length(), '=') << "\n" << std::endl;

	// open file
	if(input_file.is_open()){
		printf("!!! File '%s' is already open.",fileName);
	}
	else{
		input_file.open(fileName, std::ios::binary);
	}
}

// FORTRAN record delimiter length... usually 4 bytes.  Can be 8!  Should set as a preprocessor option
const int RECORD_DELIMITER_LENGTH = 4;
bool SurfaceSource::ReadRecord(void** destination, size_t* size, size_t NumberOfEntries)
{
	int record_length0	= 0;
	int record_length1	= 0;
	int null			= 0;
	int length_read		= 0;
	int dist_to_end		= 0;

	if (input_file.good())
	{
		// read starting delimiter
		input_file.read((char*) &record_length0, RECORD_DELIMITER_LENGTH);
		//printf("RECORD LENGTH %d\n",record_length0);

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
			//printf("--> skipping ahead %d bytes to end of record\n",dist_to_end);
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
	int record_length = 0;

	// read record
	if (input_file.good())
	{
		input_file.read((char*) &record_length, sizeof(record_length));
		input_file.read((char*) numbers,	sizeof(int));
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
	int length_read		= 0;
	int dist_to_end		= 0;

	if (input_file.good())
	{
		// read starting delimiter
		input_file.read((char*) &record_length0, RECORD_DELIMITER_LENGTH);
		//printf("RECORD LENGTH %d\n",record_length0);

		// read what's asked for
		for(int i=0;i<surface_count;i++){
			for(int j=0;j<surface_summary_length;j++){
				length_read = length_read + sizeof(int);
				if(length_read>record_length0){
					printf("DATA REQUESTED (%d) OVERRAN RECORD LENGTH (%d)!\n",length_read,record_length0);
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
			//printf("--> skipping ahead %d bytes to end of record\n",dist_to_end);
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
	void** pointers = 	new void*	[15];
	size_t* sizes 	= 	new size_t	[15];
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
	size = 3;
	pointers[0]	= (void*) &niwr;
	pointers[1]	= (void*) &mipts;
	pointers[2]	= (void*) &kjaq;
	sizes[0]	= sizeof(niwr);
	sizes[1]	= sizeof(mipts);
	sizes[2]	= sizeof(kjaq);
	if(!ReadRecord(pointers, sizes, size)){printf("ERROR READING FOURTH RECORD\n");std::exit(1);}

	// init arays for surface information
	surface_summary_length		= 2+4*mipts;
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

void SurfaceSource::PrintSizes(){

	printf("== DATA SIZE INFORMATION == \n");
	printf("normal integers  :  %1ld bytes\n",	sizeof(knods));
	printf("floating points  :  %1ld bytes\n",	sizeof(surface_parameters[0].value[0]));
	printf("characters       :  %1ld bytes\n",	sizeof(id[0]));
	printf("\n");

}

void SurfaceSource::PrintHeader(){

	printf("=========================================== HEADER INFORMATION =========================================== \n");
	printf("WSSA ID string                                 :  %8s \n",	id);
	printf("code name                                      :  %8s \n",	kods);
	printf("code version                                   :  %5s \n",	vers);
	printf("LODDAT of code that wrote surface source file  :  %8s \n",	lods);
	printf("IDTM of the surface source write run           :  %19s\n",	idtms);
	printf("probid, problem id                             :  %19s\n",	probs);
	printf("title string of the creation run               :  %80s\n",	aids);
	printf("ending dump number                             :  %d\n",	knods);
	printf("total number of histories in SS write run      :  %d\n",	np1);
	printf("the total number of tracks recorded            :  %d\n",	nrss);
	printf("Number of values in a surface-source record    :  %d\n",	nrcd);
	printf("Number of surfaces in JASW                     :  %d\n",	njsw);
	printf("Number of histories in input surface source    :  %d\n",	niss);
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
			printf("%                %5d      %5d        %1d          %s  ",i,surface_numbers[i],surface_facets[i],surface_card[surface_types[i]].symbol);
			for(int j = 0 ; j < surface_parameters_lengths[i] ; j++){
				printf(" % 10.8E",surface_parameters[i].value[j]);
			}
			printf("\n");
		}
	}
	printf("\n");


	printf("\n============================================================= SURFACE SUMMARIES ============================================================= \n");
	printf("===============================================================!IMPROVE LATER!=============================================================== \n");
	for(int i=0;i<surface_count;i++){
		printf("Surface %5d\n",surface_numbers[i]);
		for(int j=0;j<surface_summary_length;j++){
			printf("%d ",surface_summaries[i][j]);
		}
		printf("\n");
	}

};

void SurfaceSource::GetTrack(track* this_track){

	size_t size = 11;
	void** pointers = 	new void*	[size];
	size_t* sizes 	= 	new size_t	[size];
	pointers[ 0]	= (void*) &this_track[0].nps;
	pointers[ 1]	= (void*) &this_track[0].bitarray;
	pointers[ 2]	= (void*) &this_track[0].wgt;
	pointers[ 3]	= (void*) &this_track[0].erg;
	pointers[ 4]	= (void*) &this_track[0].tme;
	pointers[ 5]	= (void*) &this_track[0].x;
	pointers[ 6]	= (void*) &this_track[0].y;
	pointers[ 7]	= (void*) &this_track[0].z;
	pointers[ 8]	= (void*) &this_track[0].xhat;
	pointers[ 9]	= (void*) &this_track[0].yhat;
	pointers[10]	= (void*) &this_track[0].cs;
	sizes[ 0]		= sizeof(this_track[0].nps			);
	sizes[ 1]		= sizeof(this_track[0].bitarray		);
	sizes[ 2]		= sizeof(this_track[0].wgt			);
	sizes[ 3]		= sizeof(this_track[0].erg			);
	sizes[ 4]		= sizeof(this_track[0].tme			);
	sizes[ 5]		= sizeof(this_track[0].x			);
	sizes[ 6]		= sizeof(this_track[0].y			);
	sizes[ 7]		= sizeof(this_track[0].z			);
	sizes[ 8]		= sizeof(this_track[0].xhat			);
	sizes[ 9]		= sizeof(this_track[0].yhat			);
	sizes[10]		= sizeof(this_track[0].cs			);
	if(!ReadRecord(pointers, sizes, size)){printf("ERROR READING TRACKS RECORD\n");std::exit(1);}

	// calculate missing zhat from the data
	if ((this_track[0].xhat*this_track[0].xhat+this_track[0].yhat*this_track[0].yhat)<1.0){
		this_track[0].zhat   = copysign(std::sqrt(1.0 - this_track[0].xhat*this_track[0].xhat -
		                                         this_track[0].yhat*this_track[0].yhat), this_track[0].bitarray);
	}
	else{
	    this_track[0].zhat = 0.0;
	}

}




class histogram:

	def __init__(self,bins):
		self.bins		= 	copy.deepcopy(bins)  # bins are edges
		self.n_bins		=	len(bins)
		self.values		=	numpy.zeros((self.n_bins-1,))
		self.sqvals		=	numpy.zeros((self.n_bins-1,))
		self.counts		=	numpy.zeros((self.n_bins-1,))
		self.err		=	numpy.zeros((self.n_bins-1,))

	def add(self,bin_val,weight):

		# check if in bounds
		valid = True
		if bin_val < self.bins[0] or bin_val > self.bins[-1]:
			valid = False

		# add weight to bin if between bins
		if valid:
			dex = next((i for i, x in enumerate(bin_val < self.bins) if x), False) - 1
			self.values[dex] = self.values[dex] + weight
			self.sqvals[dex] = self.sqvals[dex] + weight*weight
			self.counts[dex] = self.counts[dex] + 1

	def update(self):

		# calculate error
		for dex in range(0,self.n_bins-1):
			N		= self.counts[dex]
			sum_xi	= self.values[dex]
			sum_xi2	= self.sqvals[dex]
			if N==1:
				self.err[dex] = 1.0
			elif N > 1:
				tally_err_sq =   1.0/(N-1) * ( N*sum_xi2/(sum_xi*sum_xi) - 1.0) 
				if tally_err_sq > 0:
					self.err[dex] = numpy.sqrt(tally_err_sq)
				else:
					self.err[dex] = 0.0
			else:
				self.err[dex] = 0.0


/*
MAIN FUNCTION
*/

int main(int argc, char* argv[]){

	// init some data
	track this_track;
	SurfaceSource ss(argv[1]);

	// load up WSSA file
	ss.PrintSizes();
	ss.ReadHeader();
	ss.PrintHeader();


	//
	this_sc == 10307
	sphere = False
	E_bins   = numpy.array([1e-11,to_energy(2.0),600])
	x_bins   = numpy.linspace(-425,425,1701)
	y_bins   = numpy.linspace(-153.5,286.0,880)
	theta_bins = numpy.array([0,90])*numpy.pi/180.0
	phi_bins = numpy.linspace(0,2*numpy.pi,2) 
	dist     = numpy.zeros((  len(E_bins)-1 , len(theta_bins)-1 , len(phi_bins)-1 , len(y_bins)-1 , len(x_bins)-1 ),dtype=numpy.float64)
	//  surface plane parameters
	surface_plane   = numpy.array([ 1.0, 0.0, 0.0, 2.9847115E+03])   # plane, GLOBAL coordinates
	surface_center  = numpy.array([ 2.9847115E+03, -226.3105 , 0.0 ])
	surface_normal  = numpy.array([surface_plane[0],surface_plane[1],surface_plane[2]]) 
	surface_normal_rot = surface_normal 
	surface_vec1    = numpy.array([-surface_plane[1],surface_plane[0] ,  0.0])
	surface_vec2    = numpy.array([0.0,0.0,1.0])

	for(int i=0;i<ss.nrss;i++){
		ss.GetTrack(&this_track);

	for i in progress(range(1,min(ss.nrss,int(1e10)))):    #max on BOA-bender 499,672,557?!
		
		### get track global position/direction
		track = ss.next_track()

		### decode bitarray
		b   = abs(track.bitarray)      # sign means what?
		j   = int(b / 2e8)             # collided?  history?
		ipt = int(b / 1e6 - j*2e2)     # particle type (1=n,2=p,3=e,4=mu-,9=proton,20=pi_+)
		nsf = int(b - ipt*1e6 - j*2e8) # surface
		
		### get data
		vec = numpy.array([track.u,track.v,track.w])
		pos = numpy.array([track.x,track.y,track.z])
		this_E 	  = track.erg
		this_wgt  = track.wgt
		
		# transform particle origin
		xfm_pos	= numpy.subtract(pos,surface_center)

		### mcnp6
		if 'SF_00001' in ss.kod:
			nsf=track.cs
			ipt=1  #have manually set, IS NOT READ HERE, SCRIPT WILL ASSUME ALL ARE NEUTRONS

		### calculate sense
		if sphere:
			surface_normal	= xfm_pos / numpy.linalg.norm(xfm_pos)
			D 				= numpy.dot(surface_normal,xfm_pos)
			surface_plane	= numpy.array([surface_normal[0],surface_normal[1],surface_normal[2],D])
			sense = surface_plane[0]*xfm_pos[0] + surface_plane[1]*xfm_pos[1] + surface_plane[2]*xfm_pos[2] - surface_plane[3]  # use sense almost zero for on-plane particles since I don't think mcnpx prints which surface its on!
		else:
			sense = surface_plane[0]*pos[0] + surface_plane[1]*pos[1] + surface_plane[2]*pos[2] - surface_plane[3]  # use sense almost zero for on-plane particles since I don't think mcnpx prints which surface its on!


		if  (ipt==1) and (abs(sense)<=1e-5): # (nsf==this_sc): #

			if sphere:
				surface_vec1	= surface_normal[2]/abs(surface_normal[2]) * numpy.subtract(numpy.array([0.0,0.0,D/surface_normal[2]]),xfm_pos)
				surface_vec1	= surface_vec1 / numpy.linalg.norm(surface_vec1)
				surface_vec2	= numpy.cross(surface_normal,surface_vec1)
				this_theta		= numpy.arccos(numpy.dot(surface_normal,vec))
				this_phi		= numpy.arctan2(numpy.dot(surface_vec2,vec),numpy.dot(surface_vec1,vec))
				sphere_vec1		= numpy.array([1.0,0.0,0.0])
				sphere_vec2		= numpy.array([0.0,1.0,0.0])
				sphere_vec3		= numpy.array([0.0,0.0,1.0])
				sphere_theta	= numpy.arccos(surface_normal[2])
				sphere_phi		= numpy.arctan2(surface_normal[1],surface_normal[0])
				if sphere_phi < 0.0:
					sphere_phi = 2.0*numpy.pi + sphere_phi
				this_pos        = numpy.array([sphere_phi,sphere_theta])
				this_vec = numpy.array([numpy.dot(surface_vec1,vec),numpy.dot(surface_vec2,vec),numpy.dot(surface_normal_rot,vec)])
			else:
				### transform vector to normal system
				this_vec = numpy.array([numpy.dot(surface_vec1,vec),numpy.dot(surface_vec2,vec),numpy.dot(surface_normal_rot,vec)])

				### transform position to surface coordinates using basis vectors specified
				this_pos = numpy.array([numpy.dot(surface_vec1,xfm_pos),numpy.dot(surface_vec2,xfm_pos)])
		
				### calc angular values
				this_theta  = numpy.arccos(this_vec[2])
				this_phi = numpy.arctan2(this_vec[1],this_vec[0])
			
			if this_phi < 0.0:
				this_phi = 2.0*numpy.pi + this_phi
		
			### find the bins
			if (this_E > E_bins[0] and this_E < E_bins[-1]):
				E_dex 	=  numpy.nonzero(this_E      < E_bins  )[0][0]-1
			else:
				E_dex = sys.maxint
			if (this_pos[0] > x_bins[0] and this_pos[0] < x_bins[-1]):
				x_dex 	=  numpy.nonzero(this_pos[0] < x_bins  )[0][0]-1
			else:
				x_dex= sys.maxint
			if (this_pos[1] > y_bins[0] and this_pos[1] < y_bins[-1]):	
				y_dex 	=  numpy.nonzero(this_pos[1] < y_bins  )[0][0]-1
			else:
				y_dex= sys.maxint
			if (this_theta > theta_bins[0] and this_theta < theta_bins[-1]):
				theta_dex	=  numpy.nonzero(this_theta     < theta_bins )[0][0]-1
			else:
				theta_dex= sys.maxint
			if (this_phi > phi_bins[0] and this_phi < phi_bins[-1]):	
				phi_dex	=  numpy.nonzero(this_phi    < phi_bins)[0][0]-1
			else:
				phi_dex=sys.maxint
				
			### increment array
			if (E_dex < len(E_bins)-1) and (theta_dex < len(theta_bins)-1) and (phi_dex < len(phi_bins)-1) and (y_dex < len(y_bins)-1) and (x_dex < len(x_bins)-1 and this_wgt <= max_wgt) :
				count = count+1
				x_avg = x_avg + x_bins[x_dex]
				x_dex_avg = x_dex_avg + x_dex
				dist[E_dex][theta_dex][phi_dex][y_dex][x_dex] = dist[E_dex][theta_dex][phi_dex][y_dex][x_dex] + this_wgt
				histograms_curr[theta_dex].add(this_E,this_wgt)
				histograms_flux[theta_dex].add(this_E,this_wgt/this_vec[2]/surface_area)
				histograms_wght[theta_dex].add(this_wgt,1)
			else:
				if (E_dex >= len(E_bins)-1 and printflag and errorflag): 
					print "E = %6.4E index %i is outside bin boundaries" % (this_E,E_dex)
				if(theta_dex >= len(theta_bins)-1 and printflag and errorflag): 
					print "theta = %6.4E index %i is outside bin boundaries" % (this_theta,theta_dex)
				if(phi_dex >= len(phi_bins)-1 and printflag and errorflag): 
					print "phi = %6.4E index %i is outside bin boundaries" % (this_phi,phi_dex)
					print pos,vec
				if(y_dex >= len(y_bins)-1 and printflag and errorflag): 
					print "y = %6.4E index %i is outside bin boundaries" % (this_pos[1],y_dex)
					print pos,vec
				if(x_dex >= len(x_bins)-1 and printflag and errorflag):
					print "x = %6.4E index %i is outside bin boundaries" % (this_pos[0],x_dex)
				if(this_wgt > max_wgt and printflag and errorflag):
					print "wgt = %6.4E is greater than maximum specified weight %6.4E" % (this_wgt,max_wgt)
	print "max weight",wgt_avg
	# update the histograms to calculate error, must be done before nps division!
	for i in range(0,len(theta_bins)-1):
		histograms_curr[i].update()
		histograms_flux[i].update()
		histograms_wght[i].update()
	### normalize dist to nps:
	unit_area = (y_bins[1]-y_bins[0])*(x_bins[1]-x_bins[0])
	surface_nps = abs(track.nps)
	total_weight = 0.0
	total_tracks = 0
	# divide by nps
	for i in range(0,len(theta_bins)-1):
		total_tracks = total_tracks + numpy.sum(histograms_curr[i].counts)
		total_weight = total_weight + numpy.sum(histograms_curr[i].values)
		histograms_curr[i].values = histograms_curr[i].values / surface_nps
		histograms_flux[i].values = histograms_flux[i].values / surface_nps
	npstrack_ratio = surface_nps/total_tracks
	# divide dists array
	if fluxflag:
		dist = dist / surface_nps / unit_area
	else:
		dist = dist / surface_nps



		std::vector<int> data = { 1, 1, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6 };
    	auto lower = std::lower_bound(data.begin(), data.end(), 4);
    	auto upper = std::upper_bound(data.begin(), data.end(), 4);
    	std::copy(lower, upper, std::ostream_iterator<int>(std::cout, " "));



	}
}