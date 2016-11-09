
#include <fstream>
#include <vector>
#include <string>
#include "ss2dist.h"
 
using namespace std;

/*
Adapated from https://wiki.calculquebec.ca/w/C%2B%2B_:_fichier_Fortran_binaire/en
*/
 
 const int RECORD_DELIMITER_LENGTH = 4;
 
bool ReadPoints(const string& fileName, vector& points)
{
	int nbPoints;
	
	// clear the points
	points.clear();
	
	// open file in binary mode
	ifstream input(fileName.c_str(), ios::binary);
	
	if (input.good())
	{
	   // read number of points
		input.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		input.read((char*) &nbPoints, sizeof(nbPoints));
		input.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
	
		// set vector size
		points.resize(nbPoints);
	
		// read each point
		for (int i = 0; i < nbPoints; ++i)
		{
			input.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
	
			input.read((char*) &points[i].x, sizeof(points[i].x));
			input.read((char*) &points[i].y, sizeof(points[i].y));
			input.read((char*) &points[i].z, sizeof(points[i].z));
	
			input.seekg(RECORD_DELIMITER_LENGTH, ios::cur);
		}
	
		return true;
	}
	else
	{
		return false;
	}
}



SurfaceSource::SurfaceSource(){

	// init surface lookup table
	surface_card[ 0].symbol = 'XXX';
	surface_card[ 1].symbol = 'p  ';
	surface_card[ 2].symbol = 'px ';
	surface_card[ 3].symbol = 'py ';
	surface_card[ 4].symbol = 'pz ';
	surface_card[ 5].symbol = 'so ';
	surface_card[ 6].symbol = 's  ';
	surface_card[ 7].symbol = 'sx ';
	surface_card[ 8].symbol = 'sy ';
	surface_card[ 9].symbol = 'sz ';
	surface_card[10].symbol = 'c/x';
	surface_card[11].symbol = 'c/y';
	surface_card[12].symbol = 'c/z';
	surface_card[13].symbol = 'cx ';
	surface_card[14].symbol = 'cy ';
	surface_card[15].symbol = 'cz ';
	surface_card[16].symbol = 'k/x';
	surface_card[17].symbol = 'k/y';
	surface_card[18].symbol = 'k/z';
	surface_card[19].symbol = 'kx ';
	surface_card[20].symbol = 'ky ';
	surface_card[21].symbol = 'kz ';
	surface_card[22].symbol = 'sq ';
	surface_card[23].symbol = 'gq ';
	surface_card[24].symbol = 'tx ';
	surface_card[25].symbol = 'ty ';
	surface_card[26].symbol = 'tz ';
	surface_card[27].symbol = 'x  ';
	surface_card[28].symbol = 'y  ';
	surface_card[29].symbol = 'z  ';
	surface_card[30].symbol = 'box';
	surface_card[31].symbol = 'rpp';
	surface_card[32].symbol = 'sph';
	surface_card[33].symbol = 'rcc';
	surface_card[34].symbol = 'rec';
	surface_card[35].symbol = 'ell';
	surface_card[36].symbol = 'trc';
	surface_card[37].symbol = 'wed';
	surface_card[38].symbol = 'arb';
	surface_card[39].symbol = 'rhp';
	surface_card[40].symbol = 'hex';
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
	surface_card[ 0].description = 'INDEXING ERROR                          ';
	surface_card[ 1].description = 'General plane                           ';
	surface_card[ 2].description = 'Plane normal to X-axis                  ';
	surface_card[ 3].description = 'Plane normal to Y-axis                  ';
	surface_card[ 4].description = 'Plane normal to Z-axis                  ';
	surface_card[ 5].description = 'Sphere centered at the origin           ';
	surface_card[ 6].description = 'General sphere                          ';
	surface_card[ 7].description = 'Sphere centered on X-axis               ';
	surface_card[ 8].description = 'Sphere centered on Y-axis               ';
	surface_card[ 9].description = 'Sphere centered on Z-axis               ';
	surface_card[10].description = 'Cylinder parallel to X-axis             ';
	surface_card[11].description = 'Cylinder parallel to Y-axis             ';
	surface_card[12].description = 'Cylinder parallel to Z-axis             ';
	surface_card[13].description = 'Cylinder on X-axis                      ';
	surface_card[14].description = 'Cylinder on Y-axis                      ';
	surface_card[15].description = 'Cylinder on Z-axis                      ';
	surface_card[16].description = 'Cone parallel to X-axis                 ';
	surface_card[17].description = 'Cone parallel to Y-axis                 ';
	surface_card[18].description = 'Cone parallel to Z-axis                 ';
	surface_card[19].description = 'Cone on X-axis                          ';
	surface_card[20].description = 'Cone on Y-axis                          ';
	surface_card[21].description = 'Cone on Z-axis                          ';
	surface_card[22].description = 'Quadric parallel to X-,Y-, or Z-axis    ';
	surface_card[23].description = 'Quadric not parallel to X-,Y-, or Z-axis';
	surface_card[24].description = 'Torus parallel to X-axis                ';
	surface_card[25].description = 'Torus parallel to Y-axis                ';
	surface_card[26].description = 'Torus parallel to Z-axis                ';
	surface_card[27].description = 'Surface defined by points sym. about X  ';
	surface_card[28].description = 'Surface defined by points sym. about Y  ';
	surface_card[29].description = 'Surface defined by points sym. about Z  ';
	surface_card[30].description = 'Arbitrarily oriented orthogonal box     ';
	surface_card[31].description = 'Rectangular Parallelepiped              ';
	surface_card[32].description = 'Sphere                                  ';
	surface_card[33].description = 'Right Circular Cylinder                 ';
	surface_card[34].description = 'Right Elliptical Cylinder               ';
	surface_card[35].description = 'Ellipsoid                               ';
	surface_card[36].description = 'Truncated Right-angle Cone              ';
	surface_card[37].description = 'Wedge                                   ';
	surface_card[38].description = 'Arbitrary polydron                      ';
	surface_card[39].description = 'Right Hexagonal Prism                   ';
	surface_card[40].description = 'Right Hexagonal Prism - Same as RHP     ';

	//
	//  START
	//
	//  READING
	//
	//  DATA
	//

	// open file in binary mode
	ifstream input( fileName.c_str(), ios::binary );
	
	if (input.good()){

	}
	else{

	}

	read_header();

	void read_header(){
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

	}



};



