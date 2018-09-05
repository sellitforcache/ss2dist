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
	if (argc!=3) { printf("A wssa filename and a write file name must be given.\n"); return 1;}

	// init some data
	track this_track;
	SurfaceSource ss(argv[1],argv[2]);

	// load up WSSA file
	ss.ReadHeader();
	ss.PrintHeader();

	// write the header to the output file
	ss.WriteHeader();

	// set loop length
	long N = ss.nrss;

	// stuff for status printing
	long Ns =  int(N/10);
	printf("\nREADING TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	// loop over tracks
	for(long i=0;i<N;i++){

		// print a new status
		if ( i%Ns == 0){printf("||||");fflush(stdout);}

		// get track
		ss.GetTrack(&this_track);
		ss.PutTrack(&this_track);

	}

	printf("<X>   DONE.\n\n");

}
