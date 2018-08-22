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
#include "SurfaceSource.h"
#include "InputFile.h"

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
	principle_vector.resize(3,0);
	spec_E_min=0;
	spec_E_max=0;
	spec_x_min=0;
	spec_x_max=0;
	spec_y_min=0;
	spec_y_max=0;
	spec_E_bins=0;
	// particle naming
	particle_symbols.resize(38,' ');
	particle_symbols[ 0] = ' ';
	particle_symbols[ 1] = 'n';
	particle_symbols[ 5] = 'q';
	particle_symbols[ 2] = 'p';
	particle_symbols[ 3] = 'e';
	particle_symbols[ 8] = 'f';
	particle_symbols[ 4] = '|';
	particle_symbols[16] = '!';
	particle_symbols[ 6] = 'u';
	particle_symbols[17] = '<';
	particle_symbols[ 7] = 'v';
	particle_symbols[18] = '>';
	particle_symbols[ 9] = 'h';
	particle_symbols[19] = 'g';
	particle_symbols[10] = 'l';
	particle_symbols[25] = 'b';
	particle_symbols[11] = '+';
	particle_symbols[26] = '_';
	particle_symbols[12] = '-';
	particle_symbols[27] = '~';
	particle_symbols[13] = 'x';
	particle_symbols[28] = 'c';
	particle_symbols[14] = 'y';
	particle_symbols[29] = 'w';
	particle_symbols[15] = 'o';
	particle_symbols[30] = '@';
	particle_symbols[20] = '/';
	particle_symbols[35] = '*';
	particle_symbols[21] = 'z';
	particle_symbols[22] = 'k';
	particle_symbols[36] = '?';
	particle_symbols[23] = '%';
	particle_symbols[24] = '^';
	particle_symbols[31] = 'd';
	particle_symbols[32] = 't';
	particle_symbols[33] = 's';
	particle_symbols[34] = 'a';
	particle_symbols[37] = '#';
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
		return;
	}
}
std::vector<std::string> InputFile::SplitString(const std::string &s, char delim) {
    std::istringstream buf(s);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);
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
				//printf("this_sc %ld\n",this_sc);
			}
			else if(!strcmp(tokens[0].c_str(),"center")){
				surface_center[0] = atof(tokens[1].c_str());
				surface_center[1] = atof(tokens[2].c_str());
				surface_center[2] = atof(tokens[3].c_str());
			}
			else if(!strcmp(tokens[0].c_str(),"principle_vector")){
				principle_vector[0] = atof(tokens[1].c_str());
				principle_vector[1] = atof(tokens[2].c_str());
				principle_vector[2] = atof(tokens[3].c_str());
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
				x_len	=  ceil((x_max-x_min)/x_res);
			}
			else if(!strcmp(tokens[0].c_str(),"y_params")){
				y_min	= atof(tokens[1].c_str());
				y_max	= atof(tokens[2].c_str());
				y_res	= atof(tokens[3].c_str());
				y_len	=  ceil((y_max-y_min)/y_res);
			}
			else if(!strcmp(tokens[0].c_str(),"spec_E")){
				spec_E_min	= atof(tokens[1].c_str());
				spec_E_max	= atof(tokens[2].c_str());
				spec_E_bins	= atoi(tokens[3].c_str());
			}
			else if(!strcmp(tokens[0].c_str(),"spec_x")){
				spec_x_min	= atof(tokens[1].c_str());
				spec_x_max	= atof(tokens[2].c_str());
			}
			else if(!strcmp(tokens[0].c_str(),"spec_y")){
				spec_y_min	= atof(tokens[1].c_str());
				spec_y_max	= atof(tokens[2].c_str());
			}
			else if(!strcmp(tokens[0].c_str(),"particle")){
				for (long i=0;i<particle_symbols.size();i++){
					if(*tokens[1].c_str() == particle_symbols[i]){
						this_particle = i;
						break;
					}
				};
			}
			else if(!strcmp(tokens[0].c_str(),"spec_theta")){
				//spec_theta_edges.push_back(0.0);  // don't imply anything
				for(long i=1;i<tokens.size();i++){
					spec_theta_edges.push_back(atof(tokens[i].c_str()));
				}
			}
		}
	}

}
void InputFile::PrintSummary(){

	long E_len		= E_bins.end()     - E_bins.begin()     - 1;
	long theta_len	= theta_bins.end() - theta_bins.begin() - 1;
	long phi_len	= phi_bins.end()   - phi_bins.begin()   - 1;
	long i = 0;

	printf(" ======================= INPUT FILE SUMMARY ======================= \n");
	printf("Particle  %3ld = %1c\n",this_particle,particle_symbols[this_particle]);
	printf("Surface   %5ld\n",this_sc);
	printf("E_len,	  %5ld\n",	E_len		);
	printf("theta_len %5ld\n",	theta_len	);
	printf("phi_len	  %5ld\n",	phi_len		);
	printf("y_len	  %5ld\n",	y_len		);
	printf("y_min	  % 6.4E\n",	y_min		);
	printf("y_max	  % 6.4E\n",	y_max		);
	printf("y_res	  % 6.4E\n",	y_res		);
	printf("x_len	  %5ld\n",	x_len		);
	printf("x_min	  % 6.4E\n",	x_min		);
	printf("x_max	  % 6.4E\n",	x_max		);
	printf("x_res	  % 6.4E\n",	x_res		);
	printf("\n");
	printf(" ========================= E_bins VECTOR ========================= \n");
	for(i=0;i<E_len+1;i++){
		printf(" % 6.4E",E_bins[i]);
	}
	printf("\n\n");
	printf(" ======================= theta_bins VECTOR ======================= \n");
	for(i=0;i<theta_len+1;i++){
		printf(" % 6.4E",theta_bins[i]);
	}
	printf("\n\n");
	printf(" ======================== phi_bins VECTOR ======================== \n");
	for(i=0;i<phi_len+1;i++){
		printf(" % 6.4E",phi_bins[i]);
	}
	printf("\n");
	if(spec_E_bins>0){
		printf("\n");
		printf(" ======================= Spectral Binning ======================== \n");
		printf(" E_min %6.4E E_max %6.4E bins %ld\n",spec_E_min,spec_E_max,spec_E_bins);
		printf(" theta");
		for(i=1;i<spec_theta_edges.size()+1;i++){
			printf(" %6.4E",spec_theta_edges[i-1]);
			if (i%4==0){printf("\n      ");}
		}
		if ((i-1)%4!=0){printf("\n");}
		printf(" x_min %6.4E x_max %6.4E \n",spec_x_min,spec_x_max);
		printf(" y_min %6.4E y_max %6.4E \n",spec_y_min,spec_y_max);
		printf("\n");
	}

}
bool InputFile::GetSurface( SurfaceSource* ss ){

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
