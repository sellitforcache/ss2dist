
#include <fstream>
#include <vector>
#include <string>
#include "ss2dist.h"
 
/*
Adapated from https://wiki.calculquebec.ca/w/C%2B%2B_:_fichier_Fortran_binaire/en
*/
SurfaceSource::Init(){
	// set the lengths of the caracter arrays
	id		= new char [];
	kods	= new char [];
	vers	= new char [];
	lods	= new char [];
	idtms 	= new char [];
	probs	= new char [];
	aids	= new char [];
	// init surface lookup table
	strncpy(surface_card[ 0].symbol , "XXX" , 3);
	strncpy(surface_card[ 1].symbol , "p  " , 3);
	strncpy(surface_card[ 2].symbol , "px " , 3);
	strncpy(surface_card[ 3].symbol , "py " , 3);
	strncpy(surface_card[ 4].symbol , "pz " , 3);
	strncpy(surface_card[ 5].symbol , "so " , 3);
	strncpy(surface_card[ 6].symbol , "s  " , 3);
	strncpy(surface_card[ 7].symbol , "sx " , 3);
	strncpy(surface_card[ 8].symbol , "sy " , 3);
	strncpy(surface_card[ 9].symbol , "sz " , 3);
	strncpy(surface_card[10].symbol , "c/x" , 3);
	strncpy(surface_card[11].symbol , "c/y" , 3);
	strncpy(surface_card[12].symbol , "c/z" , 3);
	strncpy(surface_card[13].symbol , "cx " , 3);
	strncpy(surface_card[14].symbol , "cy " , 3);
	strncpy(surface_card[15].symbol , "cz " , 3);
	strncpy(surface_card[16].symbol , "k/x" , 3);
	strncpy(surface_card[17].symbol , "k/y" , 3);
	strncpy(surface_card[18].symbol , "k/z" , 3);
	strncpy(surface_card[19].symbol , "kx " , 3);
	strncpy(surface_card[20].symbol , "ky " , 3);
	strncpy(surface_card[21].symbol , "kz " , 3);
	strncpy(surface_card[22].symbol , "sq " , 3);
	strncpy(surface_card[23].symbol , "gq " , 3);
	strncpy(surface_card[24].symbol , "tx " , 3);
	strncpy(surface_card[25].symbol , "ty " , 3);
	strncpy(surface_card[26].symbol , "tz " , 3);
	strncpy(surface_card[27].symbol , "x  " , 3);
	strncpy(surface_card[28].symbol , "y  " , 3);
	strncpy(surface_card[29].symbol , "z  " , 3);
	strncpy(surface_card[30].symbol , "box" , 3);
	strncpy(surface_card[31].symbol , "rpp" , 3);
	strncpy(surface_card[32].symbol , "sph" , 3);
	strncpy(surface_card[33].symbol , "rcc" , 3);
	strncpy(surface_card[34].symbol , "rec" , 3);
	strncpy(surface_card[35].symbol , "ell" , 3);
	strncpy(surface_card[36].symbol , "trc" , 3);
	strncpy(surface_card[37].symbol , "wed" , 3);
	strncpy(surface_card[38].symbol , "arb" , 3);
	strncpy(surface_card[39].symbol , "rhp" , 3);
	strncpy(surface_card[40].symbol , "hex" , 3);
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
	strncpy(surface_card[ 0].description , "INDEXING ERROR                          ", 40);
	strncpy(surface_card[ 1].description , "General plane                           ", 40);
	strncpy(surface_card[ 2].description , "Plane normal to X-axis                  ", 40);
	strncpy(surface_card[ 3].description , "Plane normal to Y-axis                  ", 40);
	strncpy(surface_card[ 4].description , "Plane normal to Z-axis                  ", 40);
	strncpy(surface_card[ 5].description , "Sphere centered at the origin           ", 40);
	strncpy(surface_card[ 6].description , "General sphere                          ", 40);
	strncpy(surface_card[ 7].description , "Sphere centered on X-axis               ", 40);
	strncpy(surface_card[ 8].description , "Sphere centered on Y-axis               ", 40);
	strncpy(surface_card[ 9].description , "Sphere centered on Z-axis               ", 40);
	strncpy(surface_card[10].description , "Cylinder parallel to X-axis             ", 40);
	strncpy(surface_card[11].description , "Cylinder parallel to Y-axis             ", 40);
	strncpy(surface_card[12].description , "Cylinder parallel to Z-axis             ", 40);
	strncpy(surface_card[13].description , "Cylinder on X-axis                      ", 40);
	strncpy(surface_card[14].description , "Cylinder on Y-axis                      ", 40);
	strncpy(surface_card[15].description , "Cylinder on Z-axis                      ", 40);
	strncpy(surface_card[16].description , "Cone parallel to X-axis                 ", 40);
	strncpy(surface_card[17].description , "Cone parallel to Y-axis                 ", 40);
	strncpy(surface_card[18].description , "Cone parallel to Z-axis                 ", 40);
	strncpy(surface_card[19].description , "Cone on X-axis                          ", 40);
	strncpy(surface_card[20].description , "Cone on Y-axis                          ", 40);
	strncpy(surface_card[21].description , "Cone on Z-axis                          ", 40);
	strncpy(surface_card[22].description , "Quadric parallel to X-,Y-, or Z-axis    ", 40);
	strncpy(surface_card[23].description , "Quadric not parallel to X-,Y-, or Z-axis", 40);
	strncpy(surface_card[24].description , "Torus parallel to X-axis                ", 40);
	strncpy(surface_card[25].description , "Torus parallel to Y-axis                ", 40);
	strncpy(surface_card[26].description , "Torus parallel to Z-axis                ", 40);
	strncpy(surface_card[27].description , "Surface defined by points sym. about X  ", 40);
	strncpy(surface_card[28].description , "Surface defined by points sym. about Y  ", 40);
	strncpy(surface_card[29].description , "Surface defined by points sym. about Z  ", 40);
	strncpy(surface_card[30].description , "Arbitrarily oriented orthogonal box     ", 40);
	strncpy(surface_card[31].description , "Rectangular Parallelepiped              ", 40);
	strncpy(surface_card[32].description , "Sphere                                  ", 40);
	strncpy(surface_card[33].description , "Right Circular Cylinder                 ", 40);
	strncpy(surface_card[34].description , "Right Elliptical Cylinder               ", 40);
	strncpy(surface_card[35].description , "Ellipsoid                               ", 40);
	strncpy(surface_card[36].description , "Truncated Right-angle Cone              ", 40);
	strncpy(surface_card[37].description , "Wedge                                   ", 40);
	strncpy(surface_card[38].description , "Arbitrary polydron                      ", 40);
	strncpy(surface_card[39].description , "Right Hexagonal Prism                   ", 40);
	strncpy(surface_card[40].description , "Right Hexagonal Prism - Same as RHP     ", 40);
}

SurfaceSource::SurfaceSource(const std::string& fileName){
	Init();
	OpenWssaFile(fileName.c_str());
}
SurfaceSource::SurfaceSource(char*         fileName){
	Init();
	OpenWssaFile(fileName);
}

SurfaceSource::OpenWssaFile(const char* fileName){

	if(input_file.is_open()){

	}
	else{
		input_file.open(fileName, ios::binary)
	}
}

const int RECORD_DELIMITER_LENGTH = 4;
bool ReadRecord(void** destination, size_t* size, size_t NumberOfEntries)
{

	if (input_file.good())
	{
		input_file.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		for(int i=0;i<NumberOfEntries;i++){
			input_file.read((char*) destination[i], size[i]);
		}
		input_file.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		return true;
	}
	else
	{
		return false;
	}

}
bool ReadSurfaceRecord0(int* numbers, surface* paramters)
{
	// internal variables
	int ks, n;

	// read record
	if (input_file.good())
	{
		input_file.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		input_file.read((char*) numbers,	sizeof(int));
		input_file.read((char*) &ks,		sizeof(int));
		input_file.read((char*) &n,			sizeof(int));
		input_file.read((char*) paramters,n*sizeof(double));
		input_file.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		return true;
	}
	else
	{
		return false;
	}

}
bool ReadSurfaceRecord1(int* numbers, int* facets, surface* paramters)
{

	// internal variables
	int ks, n;

	if (input_file.good())
	{
		input_file.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		input_file.read((char*) numbers,	sizeof(int));
		input_file.read((char*) facets,		sizeof(int));
		input_file.read((char*) &ks,		sizeof(int));
		input_file.read((char*) &n,			sizeof(int));
		input_file.read((char*) paramters,n*sizeof(double));
		input_file.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		return true;
	}
	else
	{
		return false;
	}

}



SurfaceSource::ReadHeader(){
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
	ReadRecord((void**) &id, sizeof(id), 1);

	// second record, first make array of pointers, then sizes
	pointers = 	new void**	[15];
	sizes = 	new size_t	[15];
	size_t size = 7;
	pointers[0]	= (void**) &kods;
	pointers[1]	= (void**) &vers;
	pointers[2]	= (void**) &lods;
	pointers[3]	= (void**) &idtms;
	pointers[4]	= (void**) &probs;
	pointers[5]	= (void**) &aids;
	pointers[6]	= (void**) &knods;
	sizes[0]	= sizeof(kods);
	sizes[1]	= sizeof(vers);
	sizes[2]	= sizeof(lods);
	sizes[3]	= sizeof(idtms);
	sizes[4]	= sizeof(probs);
	sizes[5]	= sizeof(aids);
	sizes[6]	= sizeof(knods);
	ReadRecord(pointers, sizes, size);
	delete pointers;
	delete sizes;

	// third record, first make array of pointers, then sizes
	size = 5;
	pointers[0]	= (void**) &np1;
	pointers[1]	= (void**) &nrss;
	pointers[2]	= (void**) &nrcd;
	pointers[3]	= (void**) &njsw;
	pointers[4]	= (void**) &niss;
	sizes[0]	= sizeof(np1);
	sizes[1]	= sizeof(nrss);
	sizes[2]	= sizeof(nrcd);
	sizes[3]	= sizeof(njsw);
	sizes[4]	= sizeof(niss);
	ReadRecord(pointers, sizes, size);

	// third record, first make array of pointers, then sizes
	size = 3;
	pointers[0]	= (void**) &niwr;
	pointers[1]	= (void**) &mipts;
	pointers[2]	= (void**) &kjaq;
	sizes[0]	= sizeof(niwr);
	sizes[1]	= sizeof(mipts);
	sizes[2]	= sizeof(kjaq);
	ReadRecord(pointers, sizes, size);

	// init arays for surface information
	surface_count		= njsw+niwr;
	surface_parameters	= new surface 	[surface_count];
	surface_numbers		= new int 		[surface_count];
	surface_facets		= new int 		[surface_count];
	for(int i = 0 ; i < surface_count ; i++){
		surface_parameters	[i].A	= 0;
		surface_parameters	[i].B	= 0;
		surface_parameters	[i].C	= 0;
		surface_parameters	[i].D	= 0;
		surface_parameters	[i].E	= 0;
		surface_parameters	[i].F	= 0;
		surface_parameters	[i].G	= 0;
		surface_parameters	[i].H	= 0;
		surface_parameters	[i].I	= 0;
		surface_parameters	[i].K	= 0;
		surface_numbers		[i] 	= -1;
		surface_facets		[i] 	= -1;
	}

	// go on, copying surface/cell information from the next records until particle data starts
	for(int i = 0 ; i < surface_count ; i++){
		if( kjaq==0 | i>njsw-1 ) {
			ReadSurfaceRecord0(&surface_numbers[i],&surface_parameters) 
		}
		if( kjaq==1 & i<=njsw-1 ) {
			ReadSurfaceRecord1(&surface_numbers[i],&surface_facets[i],&surface_parameters)
		}
	}

	// last record is the summary table

}

SurfaceSource::PrintHeader(){


};



