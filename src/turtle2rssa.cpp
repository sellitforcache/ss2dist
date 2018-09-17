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
#include <random>
#include "ss2lib.hpp"


/*
MAIN FUNCTION
*/
int main(int argc, char* argv[]){

	//
	if (argc!=4) { printf("An input file, turtle output, and a wssa/rssa filename must be given.\n"); return 1;}

	// init some data
	track this_track;
	SurfaceSource ss(argv[3],RSSA_WRITE);

	// turtle file
	std::ifstream 		turtle_file;
	struct stat buffer;
 	if( (stat (argv[1], &buffer) == 0)){
		turtle_file.open(argv[2], std::ios::in );
	}
	else{
		printf("problem opening %s for reading.  Aborting\n",argv[1]);
		return 1;
	}

	// load input file
	InputFile input(argv[1]);
	input.Parse();

	// calculate the other vectors from plane vectors or input principle vector
	std::valarray<double> surface_normal  	(3);
	std::valarray<double> surface_vec1	  	(3);
	std::valarray<double> surface_vec2    	(3);
	std::valarray<double> surface_vec3    	(3);
	std::valarray<double> surface_vec_avg 	(3);
	std::valarray<double> principle_vector1	(3);
	std::valarray<double> principle_vector2	(3);
	std::valarray<double> principle_vector3	(3);
	std::valarray<double> vec								(3);
	std::valarray<double> pos								(3);
	std::valarray<double> xfm_pos						(3);
	std::valarray<double> this_vec					(3);
	std::valarray<double> this_pos					(2);
	surface_normal[0]	=  input.surface_plane[0];
	surface_normal[1]	=  input.surface_plane[1];
	surface_normal[2]	=  input.surface_plane[2];
	//
	// main vector is surface normal
	surface_vec3[0]	= input.surface_plane[0];
	surface_vec3[1]	= input.surface_plane[1];
	surface_vec3[2]	= input.surface_plane[2];
	// second vector is rotation of y axis that is orthogonal
	surface_vec1[0]	= input.principle_vector[0];
	surface_vec1[1]	= input.principle_vector[1];
	surface_vec1[2]	= input.principle_vector[2];
	// check for orthogonality
	double mag;
	double dotp = surface_vec1[0]*surface_vec3[0] + surface_vec1[1]*surface_vec3[1] + surface_vec1[2]*surface_vec3[2];
	if (dotp>0.0){
		printf("vec not in plane!  projecting into plane.\n");
		surface_vec1 = surface_vec1 - dotp*surface_vec3;
		mag = sqrt(surface_vec1[0]*surface_vec1[0]+surface_vec1[1]*surface_vec1[1]+surface_vec1[2]*surface_vec1[2]);
		surface_vec1 = surface_vec1 / mag;
	}
	// compute third vector from cross product
	surface_vec2[0]= ( surface_vec3[1]*surface_vec1[2] - surface_vec3[2]*surface_vec1[1] );
	surface_vec2[1]= ( surface_vec3[2]*surface_vec1[0] - surface_vec3[0]*surface_vec1[2] );
	surface_vec2[2]= ( surface_vec3[0]*surface_vec1[1] - surface_vec3[1]*surface_vec1[0] );

	// print select summary...
	printf(" ======================= INPUT FILE SUMMARY ======================= \n");
	printf("SURFACE: %4d\n",input.this_sc);
	printf("ORIGIN:  % 8.6E   % 8.6E   % 8.6E\n",input.surface_center[0],input.surface_center[1],input.surface_center[2]);
	printf("VEC1:    % 8.6E   % 8.6E   % 8.6E\n",surface_vec1[0],surface_vec1[1],surface_vec1[2]);
	printf("VEC2:    % 8.6E   % 8.6E   % 8.6E\n",surface_vec2[0],surface_vec2[1],surface_vec2[2]);
	printf("VEC3:    % 8.6E   % 8.6E   % 8.6E\n",surface_vec3[0],surface_vec3[1],surface_vec3[2]);
	printf("\n");

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
			iss>>this_val;	x.push_back(this_val);
			iss>>this_val;	xhat.push_back(this_val);
			iss>>this_val;	y.push_back(this_val);
			iss>>this_val;	yhat.push_back(this_val);
			iss>>this_val;	mom.push_back(this_val);
			iss>>this_val;	wgt.push_back(this_val);
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
	ss.njsw=1;
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
	int particle_number = 9; //proton
	ss.surface_numbers[0]=input.this_sc; 	// set later
	ss.surface_types[0]=1;  			// general plane
	ss.surface_parameters_lengths[0] =4;
	ss.surface_parameters[0].value[0]=input.surface_plane[0];  // A
	ss.surface_parameters[0].value[1]=input.surface_plane[1];  // B
	ss.surface_parameters[0].value[2]=input.surface_plane[2];  // C
	ss.surface_parameters[0].value[3]=input.surface_plane[3];  // D
	ss.surface_summaries[ 0][0]=N; //  total tracks
	ss.surface_summaries[ 0][1]=N; //  independent histories
	ss.surface_summaries[ 0][2+4*(particle_number-1)+0]=N;  // first two ints are totals.. then goes by: tracks, indp. tracks, uncollided, indp. uncol.
	ss.surface_summaries[ 0][2+4*(particle_number-1)+1]=N;
	ss.surface_summaries[ 0][2+4*(particle_number-1)+2]=0;
	ss.surface_summaries[ 0][2+4*(particle_number-1)+3]=0;

	// do header stuff
	ss.PrintSizes();
	ss.PrintHeader();
	ss.WriteHeader();

	double  electron_mass = 0.511008;
	double  neutron_mass  = 939.58;
	double  proton_mass   = 938.271998;
	double 	a, b, c, this_E, this_wgt, this_theta, this_phi, this_theta_deg;

	// stuff for status printing
	printf("\nWRITING RSSA TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	// loop over tracks
	for( i=0;i<N;i++){

		// print a new status
		if ( i%Ns == 0){printf("||||");fflush(stdout);}

		// compute direction cosines from the xy angles
		vec[0] = sin(xhat[i]*1e-3);
		vec[1] = sin(yhat[i]*1e-3);
		vec[2] = sqrt(1.0 - a*a - b*b);

		// get position, energy, weight data
		pos[0] 		=	x[i]*0.1;  // given in mm!
		pos[1] 		=	y[i]*0.1;  // given in mm!
		pos[2] 		=	0.0;
		this_E 	  = sqrt(mom[i]*mom[i] + proton_mass*proton_mass) - proton_mass; //erg: particle energy in MeV
		this_wgt  = wgt[i];

		// calc angular values from the principle vector
		this_theta  = acos((principle_vector1*vec).sum());
		this_phi 	= atan2((principle_vector3*vec).sum(),(principle_vector2*vec).sum());
		this_theta_deg = this_theta*180.0/pi;

		if (this_phi < 0.0){
			this_phi = 2.0*pi + this_phi;
		}

		// fill in track struct
		// nps, bitarray, wgt, erg, tme, x, y, z, xhat, yhat, cs, zhat
		this_track.nps 			= -(i+1);
		this_track.bitarray = encode_bitarray(9,0,0); // proton, not second+ generation, not antiparticle
		this_track.wgt			= this_wgt;
		this_track.erg			= this_E;
		this_track.tme			= 0.;
		this_track.x				= input.surface_center[0] + (surface_vec1[0]*pos[0] + surface_vec2[0]*pos[1]	+ surface_vec3[0]*pos[2]);
		this_track.y				= input.surface_center[1] + (surface_vec1[1]*pos[0] + surface_vec2[1]*pos[1]	+ surface_vec3[1]*pos[2]);
		this_track.z				= input.surface_center[2] + (surface_vec1[2]*pos[0] + surface_vec2[2]*pos[1]	+ surface_vec3[2]*pos[2]);
		this_track.xhat			=                          	 surface_vec1[0]*vec[0]	+ surface_vec2[0]*vec[1]	+ surface_vec3[0]*vec[2];
		this_track.yhat			=                          	 surface_vec1[1]*vec[0]	+ surface_vec2[1]*vec[1]	+ surface_vec3[1]*vec[2];
		this_track.zhat			=                          	 surface_vec1[2]*vec[0]	+ surface_vec2[2]*vec[1]	+ surface_vec3[2]*vec[2];
		this_track.cs 			= input.this_sc;

		// put track
		ss.PutTrack(&this_track);

	}

	printf("<X>   DONE.\n\n");

	printf("\n BE SURE TO SET NPS TO * %d * ... OR OMIT THE NPS CARD !!!!!!\n",N);

}
