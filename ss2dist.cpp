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
#include <iterator>
#include <valarray>
#include <climits>
#include <sstream>
#include <sys/stat.h>
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

	// for file check
	struct stat buffer;   

	// set object name
	input_file_name.assign(fileName);

	// print the name
	std::string title_file = "======> " + input_file_name + " <======";
	std::cout << "\n" << std::string(title_file.length(), '=') << std::endl;
	std::cout << title_file <<std::endl;
	std::cout << std::string(title_file.length(), '=') << "\n" << std::endl;

	// open file
	if( (stat (fileName, &buffer) == 0)){
		input_file.open(fileName, std::ios::binary);
	}
	else{
		printf("problem opening %s.  Aborting\n",fileName);
		exit(1);
	}

}
// FORTRAN record delimiter length... usually 4 bytes.  Can be 8!  Should set as a preprocessor option
const double	pi = 3.141592653589793238462643383279502884197169399375105820974944;
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
	printf("===============================================================!IMPROVE LATER!=============================================================== \n");
	for(int i=0;i<surface_count;i++){
		printf("Surface %5d\n",surface_numbers[i]);
		for(int j=0;j<surface_summary_length;j++){
			printf("%d ",surface_summaries[i][j]);
		}
		printf("\n");
	}

}

void SurfaceSource::GetTrack(track* this_track){

	// local vars
	size_t size 	= 1;
	size_t* sizes 	= new size_t [size];
	sizes[0] 		= 11*sizeof(double);
	
	// try reading it all in at once...
	if(!ReadRecord((void**) &this_track, sizes, size)){printf("ERROR READING TRACKS RECORD\n");std::exit(1);}

	// calculate missing zhat from the data
	if ((this_track[0].xhat*this_track[0].xhat+this_track[0].yhat*this_track[0].yhat)<1.0){
		this_track[0].zhat   = copysign(std::sqrt(1.0 - this_track[0].xhat*this_track[0].xhat -
		                                         this_track[0].yhat*this_track[0].yhat), this_track[0].bitarray);
	}
	else{
	    this_track[0].zhat = 0.0;
	}

}

//class histogram:
//
//	def __init__(self,bins):
//		self.bins		= 	copy.deepcopy(bins)  # bins are edges
//		self.n_bins		=	len(bins)
//		self.values		=	numpy.zeros((self.n_bins-1,))
//		self.sqvals		=	numpy.zeros((self.n_bins-1,))
//		self.counts		=	numpy.zeros((self.n_bins-1,))
//		self.err		=	numpy.zeros((self.n_bins-1,))
//
//	def add(self,bin_val,weight):
//
//		# check if in bounds
//		valid = True
//		if bin_val < self.bins[0] or bin_val > self.bins[-1]:
//			valid = False
//
//		# add weight to bin if between bins
//		if valid:
//			dex = next((i for i, x in enumerate(bin_val < self.bins) if x), False) - 1
//			self.values[dex] = self.values[dex] + weight
//			self.sqvals[dex] = self.sqvals[dex] + weight*weight
//			self.counts[dex] = self.counts[dex] + 1
//
//	def update(self):
//
//		# calculate error
//		for dex in range(0,self.n_bins-1):
//			N		= self.counts[dex]
//			sum_xi	= self.values[dex]
//			sum_xi2	= self.sqvals[dex]
//			if N==1:
//				self.err[dex] = 1.0
//			elif N > 1:
//				tally_err_sq =   1.0/(N-1) * ( N*sum_xi2/(sum_xi*sum_xi) - 1.0) 
//				if tally_err_sq > 0:
//					self.err[dex] = numpy.sqrt(tally_err_sq)
//				else:
//					self.err[dex] = 0.0
//			else:
//				self.err[dex] = 0.0


InputFile::~InputFile(){
	input_file.close();
}
InputFile::InputFile(const std::string& fileName){
	Init();
	OpenInputFile(fileName.c_str());
}
InputFile::InputFile(const char*        fileName){
	Init();
	OpenInputFile(fileName);
}
void InputFile::Init(){
	// init valarray vectors
	surface_plane .resize(4,0);
	surface_center.resize(3,0);
}
void InputFile::OpenInputFile(const char* fileName){

	// for file check
	struct stat buffer;   

	// set object name
	input_file_name.assign(fileName);

	// print the name
	std::string title_file = "======> " + input_file_name + " <======";
	std::cout << "\n" << std::string(title_file.length(), '=') << std::endl;
	std::cout << title_file <<std::endl;
	std::cout << std::string(title_file.length(), '=') << "\n" << std::endl;

	// open file
	if( (stat (fileName, &buffer) == 0)){
		input_file.open(fileName, std::ios::in);
	}
	else{
		printf("problem opening %s.  Aborting\n",fileName);
		exit(1);
	}
}
std::vector<std::string> InputFile::SplitString(const std::string &s, char delim) {
	std::stringstream sstream(s);
	std::string item;
	std::vector<std::string> tokens;
	while (getline(sstream, item, delim)) {
	    tokens.push_back(item);
	}
	return tokens;
}

void InputFile::Parse(){

	std::string line;
	std::vector<std::string> tokens;
	std::string item;
	char* delim = new char;
	strncpy(delim, " " , 1);

	while(std::getline(input_file,line)){
		tokens = SplitString(line, *delim);
		if     (tokens.size()==0){}
		else if(!strcmp(tokens[0].c_str(),"#")){}
		else{
			if (!strcmp(tokens[0].c_str(),"surface")){
				this_sc = atoi(tokens[1].c_str());
				printf("this_sc %ld\n",this_sc);
			}
			else if(!strcmp(tokens[0].c_str(),"center")){
				surface_center[0] = atof(tokens[1].c_str());
				surface_center[1] = atof(tokens[2].c_str());
				surface_center[2] = atof(tokens[3].c_str());
			}
			else if(!strcmp(tokens[0].c_str(),"E_bins")){
				for(long i=1;i<tokens.size();i++){
					E_bins.push_back(atof(tokens[i].c_str()));
				}
			}
			else if(!strcmp(tokens[0].c_str(),"theta_bins")){
				for(long i=1;i<tokens.size();i++){
					theta_bins.push_back(atof(tokens[i].c_str()));
				}
			}
			else if(!strcmp(tokens[0].c_str(),"phi_bins")){
				for(long i=1;i<tokens.size();i++){
					phi_bins.push_back(atof(tokens[i].c_str()));
				}
			}
			else if(!strcmp(tokens[0].c_str(),"x_params")){
				x_min	= atof(tokens[1].c_str());
				x_max	= atof(tokens[2].c_str());
				x_res	= atof(tokens[3].c_str());
				x_len	=  (x_max-x_min)/x_res;
			}
			else if(!strcmp(tokens[0].c_str(),"y_params")){
				y_min	= atof(tokens[1].c_str());
				y_max	= atof(tokens[2].c_str());
				y_res	= atof(tokens[3].c_str());
				y_len	=  (y_max-y_min)/y_res;
			}
		}
	}

	//this_sc = 10307;
	//E_bins.push_back(1e-11);
	//E_bins.push_back(1e-6);
	//E_bins.push_back(1.0);
	//E_bins.push_back(600.0);
	
	//x_min	= -425.0;
	//x_max	=  425.0;
	//x_len	=  1700;
	//x_res	= (x_max-x_min)/(x_len);

	//y_min	= -153.5;
	//y_max	=  286;
	//y_len	=  879;
	//y_res	= (x_max-x_min)/(x_len);
	
	
	//theta_bins.push_back(0.0);
	//theta_bins.push_back(90.0*pi/180.0);
	//phi_bins.push_back(0.0);
	//phi_bins.push_back(2.0*pi);
	
}
bool InputFile::GetSurface( SurfaceSource* ss , long this_sc ){

	// go through, look for the surface specified
	for(long i=0;i<ss[0].surface_count;i++){
		if( ss[0].surface_numbers[i] == this_sc ){
			// check if not a plane, get data
			switch( ss[0].surface_types[i] ){
				case 1 :   // p
					if(ss[0].surface_parameters_lengths[i]!=4){printf("Surface type '%s' marked as having %d parameters!",ss[0].surface_card[ss[0].surface_types[i]].symbol,ss[0].surface_parameters_lengths[i]);return false;}
					surface_plane[0] = ss[0].surface_parameters[i].value[0];
					surface_plane[1] = ss[0].surface_parameters[i].value[1];
					surface_plane[2] = ss[0].surface_parameters[i].value[2];
					surface_plane[3] = ss[0].surface_parameters[i].value[3]; 
					return true;
				case 2 :   // px
					if(ss[0].surface_parameters_lengths[i]!=1){printf("Surface type '%s' marked as having %d parameters!",ss[0].surface_card[ss[0].surface_types[i]].symbol,ss[0].surface_parameters_lengths[i]);return false;}
					surface_plane[0] = 1.0;
					surface_plane[1] = 0.0;
					surface_plane[2] = 0.0;
					surface_plane[3] = ss[0].surface_parameters[i].value[0];
					return true;
				case 3 :   // py
					if(ss[0].surface_parameters_lengths[i]!=1){printf("Surface type '%s' marked as having %d parameters!",ss[0].surface_card[ss[0].surface_types[i]].symbol,ss[0].surface_parameters_lengths[i]);return false;}
					surface_plane[0] = 0.0;
					surface_plane[1] = 1.0;
					surface_plane[2] = 0.0;
					surface_plane[3] = ss[0].surface_parameters[i].value[0];
					return true;
				case 4 :   // pz
					if(ss[0].surface_parameters_lengths[i]!=1){printf("Surface type '%s' marked as having %d parameters!",ss[0].surface_card[ss[0].surface_types[i]].symbol,ss[0].surface_parameters_lengths[i]);return false;}
					surface_plane[0] = 0.0;
					surface_plane[1] = 0.0;
					surface_plane[2] = 1.0;
					surface_plane[3] = ss[0].surface_parameters[i].value[0];
					return true;
				default: // surface not supported
					printf("Surface type '%s' not supported.  Only planes are!\n",ss[0].surface_card[ss[0].surface_types[i]].symbol); return false;
			}
		}
	}

	// not found
	printf("Surface %ld not found in the surface source...\n",this_sc);
	return false;

}


/*
MAIN FUNCTION
*/

int main(int argc, char* argv[]){

	//
	if (argc!=3) { printf("A wssa filename and a surface description file must be given.\n"); return 1;}

	// init some data
	track this_track;
	SurfaceSource ss(argv[1]);

	// load up WSSA file
	ss.PrintSizes();
	ss.ReadHeader();
	ss.PrintHeader();

	// load input file
	InputFile input(argv[2]);
	input.Parse();

	// constants
	int 	this_sc = 0;
	bool 	sphere = false;

	// init dist vector
	long E_len		= input.E_bins.end()     - input.E_bins.begin()     - 1;
	long theta_len	= input.theta_bins.end() - input.theta_bins.begin() - 1;
	long phi_len	= input.phi_bins.end()   - input.phi_bins.begin()   - 1;
	long dist_len	= E_len*theta_len*phi_len*input.x_len*input.y_len;
	printf("E_len %ld theta_len %ld phi_len %ld x_len %ld y_len %ld\n",E_len,theta_len,phi_len,input.x_len,input.y_len );
	printf("dist_len %ld \n",dist_len);
	std::vector<double> dist ( dist_len );

	// claculate strides for indexing
	long 	E_stride		=  theta_len*phi_len*input.y_len*input.x_len;
	long 	theta_stride	=  phi_len*input.y_len*input.x_len;
	long 	phi_stride		=  input.y_len*input.x_len;
	long 	y_stride		=  input.x_len;
	long 	x_stride		=  1;

	// calculate the other vectors from plane vectors
	std::valarray<double> surface_normal (3);
	std::valarray<double> surface_vec1   (3);
	std::valarray<double> surface_vec2   (3);
	surface_normal[0]=input.surface_plane[0];
	surface_normal[1]=input.surface_plane[1];
	surface_normal[2]=input.surface_plane[2];
	surface_vec1[0]=-input.surface_plane[1];
	surface_vec1[1]= input.surface_plane[0];
	surface_vec1[2]=0.0;
	surface_vec2[0]= 0.0;
	surface_vec2[1]= 0.0;
	surface_vec2[2]= 1.0;

	// init binning variables
	std::valarray<double> vec		(3);
	std::valarray<double> pos		(3);
	std::valarray<double> xfm_pos	(3);
	std::valarray<double> this_vec	(3);
	std::valarray<double> this_pos	(2);
	double this_E 		= 0.0;
	double this_wgt		= 0.0;
	double this_theta	= 0.0;
	double this_phi		= 0.0;
	double sense 		= 0.0;
	double max_wgt 		= 1e99;
	long E_dex			= 0;
	long x_dex			= 0;
	long y_dex			= 0;
	long theta_dex		= 0;
	long phi_dex		= 0;
	long array_dex		= 0;
	//
	double b			= 0;
	int j				= 0;
	int ipt				= 0;
	int nsf				= 0;
	//
	std::vector<double>::iterator E_dex2;
	std::vector<double>::iterator theta_dex2;
	std::vector<double>::iterator phi_dex2;

	// set loop length
	long N = ss.nrss;//std::min(ss.nrss,10000000000);

	// stuff for status printing
	long Nd = N%int(pow(10,floor (log10 (labs (N)))));
	long Ns =  (N-Nd)/10;
	printf("\nBINNING TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	// loop over tracks
	for(long i=0;i<N;i++){

		// print a new status
		if ( (i-Nd)%Ns == 0){printf("||||");fflush(stdout);}

		// get track
		ss.GetTrack(&this_track);

		// decode bitarray, onyl for mcnpx?
		b   = fabs(this_track.bitarray); // sign means what?
		j   = int(b / 2e8);              // collided?  history?
		ipt = int(b / 1e6 - j*2e2);      // particle type (1=n,2=p,3=e,4=mu-,9=proton,20=pi_+)
		nsf = int(b - ipt*1e6 - j*2e8);  // surface

		// mcnp6
		if (!strcmp("SF_00001",ss.id)){
			nsf=this_track.cs;
			ipt=1;  //have manually set, IS NOT READ HERE, SCRIPT WILL ASSUME ALL ARE NEUTRONS
		}

		// get data
		vec[0]=this_track.xhat;
		vec[1]=this_track.yhat;
		vec[2]=this_track.zhat;
		pos[0]=this_track.x;
		pos[1]=this_track.y;
		pos[2]=this_track.z;
		this_E 	  = this_track.erg;
		this_wgt  = this_track.wgt;

		// transform particle origin
		xfm_pos	= pos - input.surface_center;

		// calculate sense
		sense = (surface_normal*pos).sum() - input.surface_plane[3];  // use sense almost zero for on-plane particles since I don't think mcnpx prints which surface its on!

		if  ((ipt==1) & (fabs(sense)<=1e-5)){

			// transform vector to normal system
			this_vec[0] = (surface_vec1*vec).sum();
			this_vec[1] = (surface_vec2*vec).sum();
			this_vec[2] = (surface_normal*vec).sum();

			// transform position to surface coordinates using basis vectors specified
			this_pos[0] = (surface_vec1*xfm_pos).sum();
			this_pos[1] = (surface_vec2*xfm_pos).sum();
		
			// calc angular values
			this_theta  = acos(this_vec[2]);
			this_phi 	= atan2(this_vec[1],this_vec[0]);
			
			if (this_phi < 0.0){
				this_phi = 2.0*pi + this_phi;
			}
		

			// find the bins
			if (this_E > *input.E_bins.begin() & this_E < *(input.E_bins.end()-1)){
				E_dex2 	= std::lower_bound (input.E_bins.begin(), input.E_bins.end(), this_E);
				E_dex	= E_dex2-input.E_bins.begin()-1;
			}
			else{
				E_dex = INT_MAX;
			}
			if (this_pos[0] > input.x_min & this_pos[0] < input.x_max){
				x_dex 	= int((this_pos[0]-input.x_min)/input.x_res);
			}
			else{
				x_dex = INT_MAX;
			}
			if (this_pos[1] > input.y_min & this_pos[1] < input.y_max){
				y_dex 	=  int((this_pos[1]-input.y_min)/input.y_res);
			}
			else{
				y_dex = INT_MAX;
			}
			if (this_theta > *input.theta_bins.begin() & this_theta < *(input.theta_bins.end()-1)){
				theta_dex2 	= std::lower_bound (input.theta_bins.begin(), input.theta_bins.end(), this_theta);
				theta_dex	= theta_dex2-input.theta_bins.begin()-1;
			}
			else{
				theta_dex = INT_MAX;
			}
			if (this_phi > *input.phi_bins.begin() & this_phi < *(input.phi_bins.end()-1)){
				phi_dex2	= std::lower_bound (input.phi_bins.begin(), input.phi_bins.end(), this_phi);
				phi_dex		= phi_dex2-input.phi_bins.begin()-1;
			}
			else{
				phi_dex = INT_MAX;
			}
				
			// increment array
			if (E_dex < INT_MAX & theta_dex < INT_MAX & phi_dex < INT_MAX & y_dex < INT_MAX & x_dex < INT_MAX & this_wgt <= max_wgt) {
//				count = count+1
//				x_avg = x_avg + x_bins[x_dex]
//				x_dex_avg = x_dex_avg + x_dex
				array_dex = E_dex*E_stride + theta_dex*theta_stride + phi_dex*phi_stride + y_dex*y_stride + x_dex*x_stride;
				//printf("array_dex %ld E_dex %ld theta_dex %ld phi_dex %ld y_dex %ld x_dex %ld \n",array_dex,E_dex ,theta_dex,phi_dex, y_dex , x_dex);
				dist[ array_dex ] = dist[ array_dex] + this_wgt;
//				histograms_curr[theta_dex].add(this_E,this_wgt)
//				histograms_flux[theta_dex].add(this_E,this_wgt/this_vec[2]/surface_area)
//				histograms_wght[theta_dex].add(this_wgt,1)
			}
			else{
			//	if (E_dex >= len(E_bins)-1 and printflag and errorflag): 
			//		print "E = %6.4E index %i is outside bin boundaries" % (this_E,E_dex)
			//	if(theta_dex >= len(theta_bins)-1 and printflag and errorflag): 
			//		print "theta = %6.4E index %i is outside bin boundaries" % (this_theta,theta_dex)
			//	if(phi_dex >= len(phi_bins)-1 and printflag and errorflag): 
			//		print "phi = %6.4E index %i is outside bin boundaries" % (this_phi,phi_dex)
			//		print pos,vec
			//	if(y_dex >= len(y_bins)-1 and printflag and errorflag): 
			//		print "y = %6.4E index %i is outside bin boundaries" % (this_pos[1],y_dex)
			//		print pos,vec
			//	if(x_dex >= len(x_bins)-1 and printflag and errorflag):
			//		print "x = %6.4E index %i is outside bin boundaries" % (this_pos[0],x_dex)
			//	if(this_wgt > max_wgt and printflag and errorflag):
			//		print "wgt = %6.4E is greater than maximum specified weight %6.4E" % (this_wgt,max_wgt)
			}
		}
//	print "max weight",wgt_avg
//	# update the histograms to calculate error, must be done before nps division!
//	for i in range(0,len(theta_bins)-1):
//		histograms_curr[i].update()
//		histograms_flux[i].update()
//		histograms_wght[i].update()
//	### normalize dist to nps:
//	unit_area = (y_bins[1]-y_bins[0])*(x_bins[1]-x_bins[0])
//	surface_nps = abs(track.nps)
//	total_weight = 0.0
//	total_tracks = 0
//	# divide by nps
//	for i in range(0,len(theta_bins)-1):
//		total_tracks = total_tracks + numpy.sum(histograms_curr[i].counts)
//		total_weight = total_weight + numpy.sum(histograms_curr[i].values)
//		histograms_curr[i].values = histograms_curr[i].values / surface_nps
//		histograms_flux[i].values = histograms_flux[i].values / surface_nps
//	npstrack_ratio = surface_nps/total_tracks
//	# divide dists array
//	if fluxflag:
//		dist = dist / surface_nps / unit_area
//	else:
//		dist = dist / surface_nps


	//

	}
	printf("<X>   DONE.\n\n");

	// write output
	char* ofileName = new char [ int(floor(log10(input.this_sc)))+9 ];
	sprintf(ofileName,"%ld_dist.bin",input.this_sc);
	printf("writing output to %s \n\n",ofileName);
	std::ofstream output_file;
	output_file.open(ofileName, std::ios::binary);
	double fE_len 		 = (double)       E_len;
	double ftheta_len 	 = (double)       theta_len;
	double fphi_len 	 = (double)       phi_len;
	double fy_len 		 = (double) input.y_len;
	double fx_len 		 = (double) input.x_len;
	output_file.write((char*) &fE_len,		sizeof(double));
	output_file.write((char*) &ftheta_len,	sizeof(double));
	output_file.write((char*) &fphi_len,	sizeof(double));
	output_file.write((char*) &fy_len,		sizeof(double));
	output_file.write((char*) &fx_len,		sizeof(double));
	output_file.write((char*) &dist[0], dist_len*sizeof(double));
	output_file.close();

}
