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
#include "histogram.h"
#include "helpers.h"
 

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

	// init variables
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
	//
	std::array<double,3> 				array3;
	std::vector<std::array<double,3>>	spatial_data;
	std::vector<std::array<double,3>>	angle_data;
	std::vector<double>					energy_data;
	std::vector<double>					weight_data;


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
	long N = 5000;//ss.nrss;

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

		// push data into array
		array3[0]=this_track.x;
		array3[1]=this_track.y;
		array3[2]=this_track.z;
		spatial_data.push_back(array3);
		array3[0]=this_track.xhat;
		array3[1]=this_track.yhat;
		array3[2]=this_track.zhat;
		angle_data.push_back(array3);
		energy_data.push_back(this_track.erg);
		energy_data.push_back(this_track.wgt);

		/*
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
		
		}
		*/
	}

	printf("<X>   DONE.\n\n");

	// give data pointer to flann matrix
	flann::Matrix<double> points_mat = flann::Matrix<double>(&spatial_data[0][0], spatial_data.size(), 3);

	printf("rows %d bytes %d\n", points_mat.rows, sizeof(double)*spatial_data.size());

	int nn = 3;
	//flann::Matrix<float> dataset;
	//flann::Matrix<float> query;
	//flann::load_from_file(dataset, "dataset.hdf5","dataset");
	//flann::load_from_file(query, "dataset.hdf5","query");
	flann::Matrix<int>    indices( new int[   points_mat.rows*nn], points_mat.rows, nn);
	flann::Matrix<double> dists(   new double[points_mat.rows*nn], points_mat.rows, nn);
	// construct an randomized kd-tree index using 4 kd-trees
	printf("\nCONSTRUCTING KD TREE ...\n");
	flann::Index<flann::L2<double> > index(points_mat, flann::KDTreeIndexParams(4));
	index.buildIndex();
	// do a knn search, using 128 checks
	printf("\nPERFORMING KNN with n=%d ...\n",nn);
	index.knnSearch(points_mat, indices, dists, nn, flann::SearchParams(0));


	
	/*
	printf("press enter to proceed with printing of results ...\n");
	std::cin.get();

	int dex;
	for (int i=0;i<points_mat.rows;i++){
		printf(" point %d: % 3.4E % 3.4E % 3.4E -- \n",i,points_mat[i][0],points_mat[i][1],points_mat[i][2]);
		for (int j=0;j<nn;j++){
			dex = indices[i][j];
			printf("       %d : % 3.4E % 3.4E % 3.4E\n",dex,points_mat[dex][0],points_mat[dex][1],points_mat[dex][2]);
		}
		printf("\n");
	}
	*/

	// get last track nps
	// long 	surface_nps 	= fabs(this_track.nps);
	// get surface source nps
	long 	surface_nps 	= abs(ss.np1);

	// sum distributions to get total weight present
	for(long i=0;i<dist_len;i++){
		total_weight += dist[i];
	}

	
	

}
