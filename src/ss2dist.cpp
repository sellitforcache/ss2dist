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
#include "histogram.h"
#include "helpers.h"


/*
MAIN FUNCTION
*/

int main(int argc, char* argv[]){

	//
	if (argc!=3) { printf("A wssa filename and a surface description file must be given.\n"); return 1;}

  // print the name
  std::string title_file = "======> " + std::string(argv[1]) + " <======";
  std::cout << "\n" << std::string(title_file.length(), '=') << std::endl;
  std::cout << title_file <<std::endl;
  std::cout << std::string(title_file.length(), '=') << "\n" << std::endl;

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
	bool res = input.GetSurface(&ss);
	if(!res){exit(1);}
	input.PrintSummary();

	// constants
	//int 	this_sc = 0;
	bool 	sphere = false;

	// calculate the other vectors from plane vectors or input principle vector
	std::valarray<double> surface_normal 	(3);
	std::valarray<double> surface_vec1	 	(3);
	std::valarray<double> surface_vec2   	(3);
	std::valarray<double> surface_vec3   	(3);
	std::valarray<double> surface_vec_avg	(3);
	std::valarray<double> principle_vector1	(3);
	std::valarray<double> principle_vector2	(3);
	std::valarray<double> principle_vector3	(3);
	surface_normal[0]	=  input.surface_plane[0];
	surface_normal[1]	=  input.surface_plane[1];
	surface_normal[2]	=  input.surface_plane[2];
	//
	//
	// FOR ANGULAR DISTRIBUTION
	double principle_vector_mag = sqrtf( input.principle_vector[0]*input.principle_vector[0] + input.principle_vector[1]*input.principle_vector[1] + input.principle_vector[2]*input.principle_vector[2] );
	if (principle_vector_mag == 0.0){
		principle_vector1[0]	=  input.surface_plane[0];
		principle_vector1[1]	=  input.surface_plane[1];
		principle_vector1[2]	=  input.surface_plane[2];
	}
	else{
		principle_vector1[0]	=  input.principle_vector[0]/principle_vector_mag;
		principle_vector1[1]	=  input.principle_vector[1]/principle_vector_mag;
		principle_vector1[2]	=  input.principle_vector[2]/principle_vector_mag;
	}
	// second vector is rotation of y axis that is orthogonal
	double xy_rot_angle = atanf(principle_vector1[1]/principle_vector1[0]);
	principle_vector2[0]	= cosf(xy_rot_angle+pi/2.);
	principle_vector2[1]	= sinf(xy_rot_angle+pi/2.);
	principle_vector2[2]	=  0.0;
	// compute third vector from cross product
	principle_vector3[0]= ( principle_vector1[1]*principle_vector2[2] - principle_vector1[2]*principle_vector2[1] );
	principle_vector3[1]= ( principle_vector1[2]*principle_vector2[0] - principle_vector1[0]*principle_vector2[2] );
	principle_vector3[2]= ( principle_vector1[0]*principle_vector2[1] - principle_vector1[1]*principle_vector2[0] );
	//
	//
	// FOR SPATIAL DISTRIBUTION
	// main vector is surface normal
	surface_vec1[0]	=  input.surface_plane[0];
	surface_vec1[1]	=  input.surface_plane[1];
	surface_vec1[2]	=  input.surface_plane[2];
	// second vector is rotation of y axis that is orthogonal
	xy_rot_angle = atanf(surface_vec1[1]/surface_vec1[0]);
	surface_vec2[0]	= cosf(xy_rot_angle+pi/2.);
	surface_vec2[1]	= sinf(xy_rot_angle+pi/2.);
	surface_vec2[2]	=  0.0;
	// compute third vector from cross product
	surface_vec3[0]= ( surface_vec1[1]*surface_vec2[2] - surface_vec1[2]*surface_vec2[1] );
	surface_vec3[1]= ( surface_vec1[2]*surface_vec2[0] - surface_vec1[0]*surface_vec2[2] );
	surface_vec3[2]= ( surface_vec1[0]*surface_vec2[1] - surface_vec1[1]*surface_vec2[0] );

	// init average vector
	surface_vec_avg[0]= 0.0;
	surface_vec_avg[1]= 0.0;
	surface_vec_avg[2]= 0.0;

	printf(" ======================== BASIS VECTORS ========================= \n");
	printf("V1: % 10.8E % 10.8E % 10.8E \n"  , surface_vec2[0], surface_vec2[1], surface_vec2[2]);
	printf("V2: % 10.8E % 10.8E % 10.8E \n"  , surface_vec3[0], surface_vec3[1], surface_vec3[2]);
	printf("V3: % 10.8E % 10.8E % 10.8E \n\n", surface_vec1[0], surface_vec1[1], surface_vec1[2]);

	printf(" ====================== PRINCIPLE VECTOR ======================== \n");
	printf("V1: % 10.8E % 10.8E % 10.8E \n"  , principle_vector1[0], principle_vector1[1], principle_vector1[2]);

	// init dist vector
	long E_len		= input.E_bins.end()     - input.E_bins.begin()     - 1;
	long theta_len	= input.theta_bins.end() - input.theta_bins.begin() - 1;
	long phi_len	= input.phi_bins.end()   - input.phi_bins.begin()   - 1;
	long dist_len	= E_len*theta_len*phi_len*input.x_len*input.y_len;
	printf("\n DISTRIBUTION ARRAY LENGTH = %ld elements =  %ld B, %5.1f MB\n\n",dist_len,dist_len*sizeof(double),double(dist_len)*double(sizeof(double))/1048576.0);
	std::vector<double> dist ( dist_len );

	// claculate strides for indexing
	long 	E_stride		=  theta_len*phi_len*input.y_len*input.x_len;
	long 	theta_stride	=  phi_len*input.y_len*input.x_len;
	long 	phi_stride		=  input.y_len*input.x_len;
	long 	y_stride		=  input.x_len;
	long 	x_stride		=  1;

	// init binning variables
	std::valarray<double> vec			(3);
	std::valarray<double> pos			(3);
	std::valarray<double> xfm_pos		(3);
	std::valarray<double> this_vec		(3);
	std::valarray<double> this_pos		(2);
	double this_E 		= 0.0;
	double this_wgt		= 0.0;
	double this_theta	= 0.0;
	double this_phi		= 0.0;
	double sense 		= 0.0;
	double max_wgt 		= 1e99;
	double this_theta_deg = 0.0;
	long E_dex			= 0;
	long x_dex			= 0;
	long y_dex			= 0;
	long theta_dex		= 0;
	long spec_theta_dex	= 0;
	long phi_dex		= 0;
	long array_dex		= 0;
	//
	unsigned b			= 0;
	unsigned j			= 0;
	unsigned ipt		= 0;
	unsigned nsf		= 0;
	unsigned i_positron = 0;
	unsigned jgp		= 0;
	//
	double 	total_weight	= 0.0;
	double 	total_tracks	= 0;
	//
	bool printflag = false;
	bool errorflag = false;
	//
	std::vector<double>::iterator E_dex2;
	std::vector<double>::iterator theta_dex2;
	std::vector<double>::iterator spec_theta_dex2;
	std::vector<double>::iterator phi_dex2;

	// histogram vector stuff
	// NEED TO FIX THIS - specifying no spec_theta means this becomes an infinite loop...
	std::vector<histogram> spectra;
	histogram this_histogram;
	if (input.spec_theta_edges.size()>0){
		for (long i=0; i<(input.spec_theta_edges.size()-1);i++){
			this_histogram = histogram();
			this_histogram.set_grid_log(input.spec_E_min,input.spec_E_max,input.spec_E_bins);
			spectra.push_back(this_histogram);
		}
	}

	// angle histogram for whole angle range
	histogram angle_spectrum = histogram();
	angle_spectrum.set_grid_lin(input.spec_theta_edges.front(),input.spec_theta_edges.back(),10000);

	// set loop length
	long N = ss.nrss;//std::min(ss.nrss,10000000000);

	// stuff for status printing
	long Ns =  int(N/10);
	printf("\nBINNING TRACKS\n");
	printf("00==10==20==30==40==50==60==70==80==90==||100%%\n");
	printf("||");

	// loop over tracks
	for(long i=0;i<N;i++){

		// print a new status
		if ( i%Ns == 0){printf("||||");fflush(stdout);}

		// get track
		ss.GetTrack(&this_track);

		// decode bitarray, this decode is only for mcnpx
		//b   = fabs(this_track.bitarray); // sign is the sign of zhat
		//j   = int(b / 2e8);              // collided?  history?
		//ipt = int(b / 1e6 - j*2e2);      // particle type (1=n,2=p,3=e,4=mu-,9=proton,20=pi_+)
		//nsf = int(b - ipt*1e6 - j*2e8);  // surface
		//printf("b %4.2f j %d ipt %d nsf %d\n",b,j,ipt,nsf);

		b   = abs(int(this_track.bitarray));
		decode_bitarray(b, &ipt, &i_positron, &jgp);
		nsf=this_track.cs;

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

		if  ((ipt==input.this_particle) & (fabs(sense)<=1e-5)){

			// transform vector to normal system
			this_vec[0] = (surface_vec2*vec).sum();
			this_vec[1] = (surface_vec3*vec).sum();
			this_vec[2] = (surface_vec1*vec).sum();

			// transform position to surface coordinates using basis vectors specified
			this_pos[0] = (surface_vec2*xfm_pos).sum();
			this_pos[1] = (surface_vec3*xfm_pos).sum();

			// calc angular values from the principle vector
			this_theta  = acos((principle_vector1*vec).sum());
			this_phi 	= atan2((principle_vector3*vec).sum(),(principle_vector2*vec).sum());
			this_theta_deg = this_theta*180.0/pi;

			if (this_phi < 0.0){
				this_phi = 2.0*pi + this_phi;
			}


			// find the bin indices
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
			if (this_theta_deg > *input.theta_bins.begin() & this_theta_deg < *(input.theta_bins.end()-1)){
				theta_dex2 	= std::lower_bound (input.theta_bins.begin(), input.theta_bins.end(), this_theta_deg);
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

			// increment spatial array
			if (E_dex < INT_MAX & theta_dex < INT_MAX & phi_dex < INT_MAX & y_dex < INT_MAX & x_dex < INT_MAX & this_wgt <= max_wgt) {
				array_dex = E_dex*E_stride + theta_dex*theta_stride + phi_dex*phi_stride + y_dex*y_stride + x_dex*x_stride;
				dist[ array_dex ] = dist[ array_dex] + this_wgt;
				total_tracks++;
			}

			// increment spectra
			if ( (this_E      >= input.spec_E_min) & (     this_E <= input.spec_E_max) ){
			if ( (this_pos[0] >= input.spec_x_min) & (this_pos[0] <= input.spec_x_max) ){
			if ( (this_pos[1] >= input.spec_y_min) & (this_pos[1] <= input.spec_y_max) ){
			if ( (this_theta_deg  >=  *input.spec_theta_edges.begin()) & (this_theta_deg <= *(input.spec_theta_edges.end()-1)) ){
				// keep track of average vector
				surface_vec_avg[0] += this_wgt*vec[0];
				surface_vec_avg[1] += this_wgt*vec[1];
				surface_vec_avg[2] += this_wgt*vec[2];
				// increment total angle spec
				angle_spectrum.add(this_theta_deg,this_wgt);
				// increment energy spectra
				spec_theta_dex2 = std::lower_bound (input.spec_theta_edges.begin(), input.spec_theta_edges.end(), this_theta_deg);
				spec_theta_dex	= spec_theta_dex2-input.spec_theta_edges.begin()-1;
				if (spec_theta_dex<0){spec_theta_dex+=1;}
				spectra[spec_theta_dex].add(this_E,this_wgt);
			}
			}
			}
			}
			else{
			if ((E_dex >= E_len )& printflag & errorflag){
				printf( "E = %6.4E index %ld is outside bin boundaries\n",this_E,E_dex);}
			if((theta_dex >= theta_len) & printflag & errorflag){
				printf(  "theta = %6.4E index %ld is outside bin boundaries\n",this_theta,theta_dex);}
			if((phi_dex >= phi_len) & printflag & errorflag){
				printf(  "phi = %6.4E index %ld is outside bin boundaries\n",this_phi,phi_dex);}
			if((y_dex >= input.y_len) & printflag & errorflag){
				printf(  "y = %6.4E index %ld is outside bin boundaries\n" ,this_pos[1],y_dex);}
			if((x_dex >= input.x_len) & printflag & errorflag){
				printf(  "x = %6.4E index %ld is outside bin boundaries\n",this_pos[0],x_dex);}
			if((this_wgt > max_wgt) & printflag & errorflag){
				printf(  "wgt = %6.4E is greater than maximum specified weight %6.4E\n",this_wgt,max_wgt);}
			}
		}
	}
	printf("<X>   DONE.\n\n");

	// get last track nps
	// long 	surface_nps 	= fabs(this_track.nps);
	// get surface source nps
	long 	surface_nps 	= abs(ss.np1);

	// sum distributions to get total weight present
	for(long i=0;i<dist_len;i++){
		total_weight += dist[i];
	}

	// normalize dist to nps read
	for(long i=0;i<dist_len;i++){
		dist[i] = dist[i] / surface_nps;
	}

	// finalize spec, normalize spec to nps read
	for(long i=0;i<spectra.size();i++){
		spectra[i].update(surface_nps);
	}
	angle_spectrum.update(surface_nps);

	// renormalize average vector and print
	surface_vec_avg[0] = surface_vec_avg[0]/N;
	surface_vec_avg[1] = surface_vec_avg[1]/N;
	surface_vec_avg[2] = surface_vec_avg[2]/N;
	double avg_vec_mag = sqrtf(surface_vec_avg[0]*surface_vec_avg[0]+surface_vec_avg[1]*surface_vec_avg[1]+surface_vec_avg[2]*surface_vec_avg[2]);
	surface_vec_avg[0] = surface_vec_avg[0] / avg_vec_mag;
	surface_vec_avg[1] = surface_vec_avg[1] / avg_vec_mag;
	surface_vec_avg[2] = surface_vec_avg[2] / avg_vec_mag;

	printf("AVERAGE VECTOR OF SURFACE: % 10.8E % 10.8E % 10.8E \n\n",surface_vec_avg[0],surface_vec_avg[1],surface_vec_avg[2]);

	//
	// write output
	//

	// open dist output file
	char* ofileName = new char [ int(floor(log10(input.this_sc)))+9 ];
	if(input.this_particle==20){
		sprintf(ofileName,"%ld_%s_dist.bin",input.this_sc,"pi0");
	}
	else{
		sprintf(ofileName,"%ld_%1c_dist.bin",input.this_sc,input.particle_symbols[input.this_particle]);
	}
	printf("writing output to %s \n\n",ofileName);
	std::ofstream output_file;
	output_file.open(ofileName, std::ios::binary);

	// cast integers as doubles to make the reading more regular, adjust length values to be the number of edges, not bins, which is the length of the bins vectors
	double fE_len 		 = (double) (      E_len+1);
	double ftheta_len 	 = (double) (  theta_len+1);
	double fphi_len 	 = (double) (    phi_len+1);
	double fy_len 		 = (double) (input.y_len+1);
	double fy_min 		 = (double)  input.y_min;
	double fy_max 		 = (double)  input.y_max;
	double fy_res 		 = (double)  input.y_res;
	double fx_len 		 = (double) (input.x_len+1);
	double fx_min 		 = (double)  input.x_min;
	double fx_max 		 = (double)  input.x_max;
	double fx_res 		 = (double)  input.x_res;
	double surf_a		 = (double)  input.surface_plane[0];
	double surf_b		 = (double)  input.surface_plane[1];
	double surf_c		 = (double)  input.surface_plane[2];
	double surf_d		 = (double)  input.surface_plane[3];
	double this_sc		 = (double)  input.this_sc;
	double surf_cx		 = (double)  input.surface_center[0];
	double surf_cy		 = (double)  input.surface_center[1];
	double surf_cz		 = (double)  input.surface_center[2];
	double this_particle = (double)  input.this_particle;

	// write the single values so all lengths can be read  before vectors
	output_file.write((char*) &fE_len,			sizeof(double));
	output_file.write((char*) &ftheta_len,		sizeof(double));
	output_file.write((char*) &fphi_len,		sizeof(double));
	output_file.write((char*) &fy_len,			sizeof(double));
	output_file.write((char*) &fy_min,			sizeof(double));
	output_file.write((char*) &fy_max,			sizeof(double));
	output_file.write((char*) &fy_res,			sizeof(double));
	output_file.write((char*) &fx_len,			sizeof(double));
	output_file.write((char*) &fx_min,			sizeof(double));
	output_file.write((char*) &fx_max,			sizeof(double));
	output_file.write((char*) &fx_res,			sizeof(double));
	output_file.write((char*) &surf_a,			sizeof(double));
	output_file.write((char*) &surf_b,			sizeof(double));
	output_file.write((char*) &surf_c,			sizeof(double));
	output_file.write((char*) &surf_d,			sizeof(double));
	output_file.write((char*) &this_sc,			sizeof(double));
	output_file.write((char*) &surf_cx,			sizeof(double));
	output_file.write((char*) &surf_cy,			sizeof(double));
	output_file.write((char*) &surf_cz,			sizeof(double));
	output_file.write((char*) &this_particle,	sizeof(double));
	output_file.write((char*) &surface_vec1[0],	sizeof(double));
	output_file.write((char*) &surface_vec1[1],	sizeof(double));
	output_file.write((char*) &surface_vec1[2],	sizeof(double));
	output_file.write((char*) &surface_vec2[0],	sizeof(double));
	output_file.write((char*) &surface_vec2[1],	sizeof(double));
	output_file.write((char*) &surface_vec2[2],	sizeof(double));
	output_file.write((char*) &surface_vec3[0],	sizeof(double));
	output_file.write((char*) &surface_vec3[1],	sizeof(double));
	output_file.write((char*) &surface_vec3[2],	sizeof(double));
	output_file.write((char*) &principle_vector1[0],	sizeof(double));
	output_file.write((char*) &principle_vector1[1],	sizeof(double));
	output_file.write((char*) &principle_vector1[2],	sizeof(double));

	// write vectors
	output_file.write((char*) input.E_bins.data(),			(E_len+1)*     sizeof(double));
	output_file.write((char*) input.theta_bins.data(),		(theta_len+1)* sizeof(double));
	output_file.write((char*) input.phi_bins.data(),		(phi_len+1)*   sizeof(double));

	// write dist
	output_file.write((char*) &dist[0], dist_len*sizeof(double));

	// close file
	output_file.close();

	// open spec output file
	if(input.this_particle==20){
		sprintf(ofileName,"%ld_%s_spec.bin",input.this_sc,"pi0");
	}
	else{
		sprintf(ofileName,"%ld_%1c_spec.bin",input.this_sc,input.particle_symbols[input.this_particle]);
	}
	printf("writing output to %s \n\n",ofileName);
	//std::ofstream output_file;
	output_file.open(ofileName, std::ios::binary);

	// cast integers as doubles to make the reading more regular, adjust length values to be the number of edges, not bins, which is the length of the bins vectors
	double fspec_E_bins		= (double)   input.spec_E_bins;
	double fspec_theta_bins	= (double)   input.spec_theta_edges.size()-1;

	// write the single values so all lengths can be read  before vectors
	output_file.write((char*) &input.spec_E_min,			sizeof(double));
	output_file.write((char*) &input.spec_E_max,			sizeof(double));
	output_file.write((char*) &fspec_E_bins,				sizeof(double));
	output_file.write((char*) &fspec_theta_bins,			sizeof(double));
	output_file.write((char*) &input.spec_x_min,			sizeof(double));
	output_file.write((char*) &input.spec_x_max,			sizeof(double));
	output_file.write((char*) &input.spec_y_min,			sizeof(double));
	output_file.write((char*) &input.spec_y_max,			sizeof(double));
	output_file.write((char*) &this_sc,						sizeof(double));
	output_file.write((char*) &this_particle,				sizeof(double));
	double this_val = angle_spectrum.values.size();
	output_file.write((char*) &this_val,					sizeof(double));

	// write theta vector
	output_file.write((char*) &input.spec_theta_edges[0], input.spec_theta_edges.size()*sizeof(double));

	// write specs
	for(long i=0;i<spectra.size();i++){
		output_file.write((char*) &spectra[i].values[0], input.spec_E_bins*sizeof(double));
	}

	// write total angle spectrum grid and values
	output_file.write((char*) &angle_spectrum.edges[0],  angle_spectrum.edges.size()*sizeof(double));
	output_file.write((char*) &angle_spectrum.values[0], angle_spectrum.values.size()*sizeof(double));

	// close file
	output_file.close();


}
