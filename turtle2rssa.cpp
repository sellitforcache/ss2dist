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
	if (argc!=3) { printf("A turtle output and a wssa/rssa filename must be given.\n"); return 1;}

	// init some data
	track this_track;
	SurfaceSource ss(argv[2],WSSA_WRITE);

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

	// turtle vectors
	double this_val;
	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> xhat;
	std::vector<double> yhat;
	std::vector<double> mom;
	std::vector<double> wgt;

	std::string line;
	while (std::getline(turtle_file, line))
	{
    	std::istringstream iss(line);
			iss>>this_val;
			x.push_back(this_val);
			iss>>this_val;
			y.push_back(this_val);
			iss>>this_val;
			xhat.push_back(this_val);
			iss>>this_val;
			yhat.push_back(this_val);
			iss>>this_val;
			mom.push_back(this_val);
			iss>>this_val;
			wgt.push_back(this_val);
			iss>>this_val;
	}
	long N  = x.size();
	printf("DONE READING TURTLE FILE\n");

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
	ss.np1=N;
	ss.nrss=N;
	ss.nrcd=-11;
	ss.njsw=1;
	ss.niss=N;
	ss.niwr=0;
	ss.mipts=9;
	ss.kjaq=0;
	// init surface arrays
	ss.surface_count=1;
	ss.surface_summary_length			= 2+4*37+1;
	ss.surface_count							= 1;
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
	ss.surface_numbers[0]=1324; 	// set later
	ss.surface_types[0]=1;  			// general plane
	ss.surface_parameters_lengths[0] =5;
	ss.surface_parameters[0].value[0]=1.0;  // A
	ss.surface_parameters[0].value[1]=0.0;  // B
	ss.surface_parameters[0].value[2]=0.0;  // C
	ss.surface_parameters[0].value[3]=0.0;  // D

	// write header
	ss.WriteHeader();

	// stuff for status printing
	long Ns =  int(N/10);
	printf("\nWRITING TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	// loop over tracks
	for(long i=0;i<N;i++){

		// print a new status
		if ( i%Ns == 0){printf("||||");fflush(stdout);}

		// get track
		ss.PutTrack(&this_track);

	}

	printf("<X>   DONE.\n\n");

}