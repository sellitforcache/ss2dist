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
#include "flann/flann.hpp"
#include "SurfaceSource.h"
#include "InputFile.h"
#include "histogram.h"
#include "helpers.h"


/*
MAIN FUNCTION
*/
int main(int argc, char* argv[]){

	//
	if (argc!=4) { printf("A turtle output and a wssa/rssa filename must be given.\n"); return 1;}

	// init some data
	track this_track;
	SurfaceSource ss(argv[2],argv[3]);

	ss.ReadHeader();
	ss.PrintHeader();

	// turtle file
	std::ifstream 		turtle_file;
	struct stat buffer;
 	if( (stat (argv[1], &buffer) == 0)){
		turtle_file.open(argv[1], std::ios::in );
	}
	else{
		printf("problem opening %s for reading.  Aborting\n",argv[1]);
		return 1;
	}

	// goto end to get the total number of TRACKS
	std::string line;
	turtle_file.seekg (0, turtle_file.end);
  int length = turtle_file.tellg();
  turtle_file.seekg (0, turtle_file.beg);
	std::getline(turtle_file, line);
	turtle_file.seekg (0, turtle_file.beg);
	int N = length/(line.length()+1);

	// turtle vectors
	double this_val;
	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> xhat;
	std::vector<double> yhat;
	std::vector<double> mom;
	std::vector<double> wgt;

	// printing
	long Ns =  int(N/10);
	int i=0;
	printf("\nREADING TURTLE TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	while (std::getline(turtle_file, line))
	{
			// print a new status
			if ( i%Ns == 0){printf("||||");fflush(stdout);}

			std::istringstream iss(line);
			iss>>this_val;
			x.push_back(this_val);
			iss>>this_val;
			xhat.push_back(this_val);
			iss>>this_val;
			y.push_back(this_val);
			iss>>this_val;
			yhat.push_back(this_val);
			iss>>this_val;
			mom.push_back(this_val);
			iss>>this_val;
			wgt.push_back(this_val);
			iss>>this_val;  // last value is number, unecessary information

			i++;

	}

	printf("<X>   DONE READING TURTLE FILE\n\n");

	if (N != x.size()){
		printf("line numbers differ ->  calc: %d actual: %d\n   ->  Setting to actual...\n", N ,x.size());
		N  = x.size();
	}

	// set header INFORMATION
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
	strncpy(ss.id, 			"SF_00001                                                                        ",    8);
	strncpy(ss.kods, 		"turtle                                                                          ",    8);
	strncpy(ss.vers, 		"1.0                                                                             ",    5);
	strncpy(ss.lods, 		"01/01/18                                                                        ",    8);
	strncpy(ss.idtms, 	"start time                                                                      ",   19);
	strncpy(ss.probs, 	"end time                                                                        ",   19);
	strncpy(ss.aids, 		"CONVERSION FROM TURTLE                                                          ",   80);
	ss.knods=1;
	ss.np1=-N;
	ss.nrss=N;
	ss.nrcd=-11;
	ss.njsw=2;
	ss.niss=N;
	ss.niwr=0;
	ss.mipts=37;
	ss.kjaq=0;

	// init surface arrays
	ss.surface_count							= ss.njsw;
	ss.surface_parameters					= new surface 	[ss.surface_count];
	ss.surface_parameters_lengths	= new int 			[ss.surface_count];
	ss.surface_numbers						= new int 			[ss.surface_count];
	ss.surface_types							= new int 			[ss.surface_count];
	ss.surface_facets							= new int 			[ss.surface_count];
	ss.surface_summaries					= new int* 			[ss.surface_count];
	for(int i = 0 ; i < ss.surface_count ; i++){
		for(int j = 0 ; j < 10 ; j++){
			ss.surface_parameters	[i].value[j]	= 0;
		}
		ss.surface_summaries[i] = new int [ss.surface_summary_length];
		for(int k=0;k<ss.surface_summary_length;k++){
			ss.surface_summaries[i][k]=0;
		}
		ss.surface_numbers			[i] 			= -1;
		ss.surface_facets			[i] 			= -1;
	}

	// set surface
	ss.surface_numbers[0]=1234; 	// set later
	ss.surface_types[0]=1;  			// general plane
	ss.surface_parameters_lengths[0] =4;
	ss.surface_parameters[0].value[0]=1.0;  // A
	ss.surface_parameters[0].value[1]=0.0;  // B
	ss.surface_parameters[0].value[2]=0.0;  // C
	ss.surface_parameters[0].value[3]=0.0;  // D

	// set surface
	ss.surface_numbers[1]=9999; 	// set later
	ss.surface_types[1]=1;  			// general plane
	ss.surface_parameters_lengths[1] =4;
	ss.surface_parameters[1].value[0]=1.0;  // A
	ss.surface_parameters[1].value[1]=0.0;  // B
	ss.surface_parameters[1].value[2]=0.0;  // C
	ss.surface_parameters[1].value[3]=0.0;  // D

	//
//	ss.surface_summaries[0][0] = 0 ;
//	ss.surface_summaries[0][1] = 0 ;
//	ss.surface_summaries[0][2] = 3178952 ;
//	ss.surface_summaries[0][3] = 583252 ;
//	ss.surface_summaries[0][4] = 2128732 ;
//	ss.surface_summaries[0][5] = 8165;
//	ss.surface_summaries[0][6] = 319;
//	ss.surface_summaries[0][7] = 3;
//	ss.surface_summaries[0][8] = 1043881;
//	ss.surface_summaries[0][9] = 574717;
//	ss.surface_summaries[0][10] = 596309;
//	ss.surface_summaries[0][11] = 574059;
//	ss.surface_summaries[0][12] = 6244;
//	ss.surface_summaries[0][13] = 357;
//	ss.surface_summaries[0][14] = 0;
//	ss.surface_summaries[0][15] = 0;
//	ss.surface_summaries[0][16] = 0;
//	ss.surface_summaries[0][17] = 0;
//	ss.surface_summaries[0][18] = 0;
//	ss.surface_summaries[0][19] = 0;
//	ss.surface_summaries[0][20] = 0;
//	ss.surface_summaries[0][21] = 0;
//	ss.surface_summaries[0][22] = 0;
//	ss.surface_summaries[0][23] = 0;
//	ss.surface_summaries[0][24] = 0;
//	ss.surface_summaries[0][25] = 0;
//	ss.surface_summaries[0][26] = 0;
//	ss.surface_summaries[0][27] = 0;
//	ss.surface_summaries[0][28] = 0;
//	ss.surface_summaries[0][29] = 0;
//	ss.surface_summaries[0][30] = 0;
//	ss.surface_summaries[0][31] = 0;
//	ss.surface_summaries[0][32] = 0;
//	ss.surface_summaries[0][33] = 0;
//	ss.surface_summaries[0][34] = 0;
//	ss.surface_summaries[0][35] = 0;
//	ss.surface_summaries[0][36] = 0;
//	ss.surface_summaries[0][37] = 0;
//	ss.surface_summaries[0][38] = 0;
//	ss.surface_summaries[0][39] = 0;
//	ss.surface_summaries[0][40] = 0;
//	ss.surface_summaries[0][41] = 0;
//	ss.surface_summaries[0][42] = 0;
//	ss.surface_summaries[0][43] = 0;
//	ss.surface_summaries[0][44] = 0;
//	ss.surface_summaries[0][45] = 0;
//	ss.surface_summaries[0][46] = 0;
//	ss.surface_summaries[0][47] = 0;
//	ss.surface_summaries[0][48] = 0;
//	ss.surface_summaries[0][49] = 0;
//	ss.surface_summaries[0][50] = 0;
//	ss.surface_summaries[0][51] = 0;
//	ss.surface_summaries[0][52] = 0;
//	ss.surface_summaries[0][53] = 0;
//	ss.surface_summaries[0][54] = 0;
//	ss.surface_summaries[0][55] = 0;
//	ss.surface_summaries[0][56] = 0;
//	ss.surface_summaries[0][57] = 0;
//	ss.surface_summaries[0][58] = 0;
//	ss.surface_summaries[0][59] = 0;
//	ss.surface_summaries[0][60] = 0;
//	ss.surface_summaries[0][61] = 0;
//	ss.surface_summaries[0][62] = 0;
//	ss.surface_summaries[0][63] = 0;
//	ss.surface_summaries[0][64] = 0;
//	ss.surface_summaries[0][65] = 0;
//	ss.surface_summaries[0][66] = 0;
//	ss.surface_summaries[0][67] = 0;
//	ss.surface_summaries[0][68] = 0;
//	ss.surface_summaries[0][69] = 0;
//	ss.surface_summaries[0][70] = 0;
//	ss.surface_summaries[0][71] = 0;
//	ss.surface_summaries[0][72] = 0;
//	ss.surface_summaries[0][73] = 0;
//	ss.surface_summaries[0][74] = 0;
//	ss.surface_summaries[0][75] = 0;
//	ss.surface_summaries[0][76] = 0;
//	ss.surface_summaries[0][77] = 0;
//	ss.surface_summaries[0][78] = 0;
//	ss.surface_summaries[0][79] = 0;
//	ss.surface_summaries[0][80] = 41;
//	ss.surface_summaries[0][81] = 4;
//	ss.surface_summaries[0][82] = 0;
//	ss.surface_summaries[0][83] = 0;
//	ss.surface_summaries[0][84] = 0;
//	ss.surface_summaries[0][85] = 0;
//	ss.surface_summaries[0][86] = 0;
//	ss.surface_summaries[0][87] = 0;
//	ss.surface_summaries[0][88] = 0;
//	ss.surface_summaries[0][89] = 0;
//	ss.surface_summaries[0][90] = 0;
//	ss.surface_summaries[0][91] = 0;
//	ss.surface_summaries[0][92] = 0;
//	ss.surface_summaries[0][93] = 0;
//	ss.surface_summaries[0][94] = 0;
//	ss.surface_summaries[0][95] = 0;
//	ss.surface_summaries[0][96] = 0;
//	ss.surface_summaries[0][97] = 0;
//	ss.surface_summaries[0][98] = 0;
//	ss.surface_summaries[0][99] = 0;
//	ss.surface_summaries[0][100] = 0;
//	ss.surface_summaries[0][101] = 0;
//	ss.surface_summaries[0][102] = 0;
//	ss.surface_summaries[0][103] = 0;
//	ss.surface_summaries[0][104] = 0;
//	ss.surface_summaries[0][105] = 0;
//	ss.surface_summaries[0][106] = 0;
//	ss.surface_summaries[0][107] = 0;
//	ss.surface_summaries[0][108] = 0;
//	ss.surface_summaries[0][109] = 0;
//	ss.surface_summaries[0][110] = 0;
//	ss.surface_summaries[0][111] = 0;
//	ss.surface_summaries[0][112] = 0;
//	ss.surface_summaries[0][113] = 0;
//	ss.surface_summaries[0][114] = 0;
//	ss.surface_summaries[0][115] = 0;
//	ss.surface_summaries[0][116] = 0;
//	ss.surface_summaries[0][117] = 0;
//	ss.surface_summaries[0][118] = 0;
//	ss.surface_summaries[0][119] = 0;
//	ss.surface_summaries[0][120] = 0;
//	ss.surface_summaries[0][121] = 0;
//	ss.surface_summaries[0][122] = 0;
//	ss.surface_summaries[0][123] = 0;
//	ss.surface_summaries[0][124] = 0;
//	ss.surface_summaries[0][125] = 0;
//	ss.surface_summaries[0][126] = 0;
//	ss.surface_summaries[0][127] = 0;
//	ss.surface_summaries[0][128] = 0;
//	ss.surface_summaries[0][129] = 0;
//	ss.surface_summaries[0][130] = 0;
//	ss.surface_summaries[0][131] = 0;
//	ss.surface_summaries[0][132] = 0;
//	ss.surface_summaries[0][133] = 0;
//	ss.surface_summaries[0][134] = 0;
//	ss.surface_summaries[0][135] = 0;
//	ss.surface_summaries[0][136] = 0;
//	ss.surface_summaries[0][137] = 0;
//	ss.surface_summaries[0][138] = 0;
//	ss.surface_summaries[0][139] = 0;
//	ss.surface_summaries[0][140] = 54;
//	ss.surface_summaries[0][141] = 9;
//	ss.surface_summaries[0][142] = 0;
//	ss.surface_summaries[0][143] = 0;
//	ss.surface_summaries[0][144] = 0;
//	ss.surface_summaries[0][145] = 0;
//	ss.surface_summaries[0][146] = 0;
//	ss.surface_summaries[0][147] = 0;
//	ss.surface_summaries[0][148] = 0;
//	ss.surface_summaries[0][149] = 0;
//	ss.surface_summaries[0][150] = 0;

	// do header stuff
	ss.PrintSizes();
	ss.PrintHeader();
	ss.WriteHeader();

	double  electron_mass = 0.511008;
	double  neutron_mass  = 939.58;
	double  proton_mass   = 938.271998;
	double 	a, b, c;

	// stuff for status printing
	printf("\nWRITING RSSA TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	// loop over tracks
	for( i=0;i<N;i++){

		// print a new status
		if ( i%Ns == 0){printf("||||");fflush(stdout);}

		// compute direction cosines from the xy angles
		a = cos(xhat[i]*1e-3);
	  b = cos(yhat[i]*1e-3);
		c = sqrt(1.0 - a*a - b*b);

		// fill in track struct
		this_track.x				= x[i]*0.1;  // given in mm!
		this_track.y				= y[i]*0.1;  // given in mm!
		this_track.z				= 0.0;
		this_track.xhat			= a;
		this_track.yhat			= b;
		this_track.zhat			= c;
		this_track.erg			= sqrt(mom[i]*mom[i] + proton_mass*proton_mass) - proton_mass; //erg: particle energy in MeV
		this_track.wgt			= wgt[i];
		this_track.tme			= 0.;
		this_track.bitarray = encode_bitarray(9,0,0); // proton, not second+ generation, not antiparticle

		// put track
		ss.PutTrack(&this_track);

	}

	printf("<X>   DONE.\n\n");

}
