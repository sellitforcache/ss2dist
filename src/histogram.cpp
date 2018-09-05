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
#include "histogram.h"
#include "helpers.h"

histogram::histogram(){
}
histogram::~histogram(){
}

void histogram::set_grid_log(){
	// DEFAULT TO SOMETHING REASONABLE
	set_grid_log(1e-10,1e3,1024);
}

void histogram::set_grid_log( double E_min_in, double E_max_in, long n_bins_in ){
	// make linearly-spaced vector in log space
	double E_min_log = log10(E_min_in);
	double E_max_log = log10(E_max_in);
	double stride    = (E_max_log - E_min_log)/n_bins_in;
	for(long i=0;i<n_bins_in+1;i++){
		edges.push_back( i*stride + E_min_log );
	}
	// convert back to energy  
	for(long i=0;i<n_bins_in+1;i++){
		edges[i] = pow(10,edges[i]);
	}
	// init rest to zeros
	n_bins	=	n_bins_in;
	E_min	=	E_min_in;
	E_max	=	E_max_in;
	values	=	std::vector<double> (n_bins, 0);
	sqvals	=	std::vector<double> (n_bins, 0);
	counts	=	std::vector<long>   (n_bins, 0);
	err		=	std::vector<double> (n_bins, 0);
}

void histogram::set_grid_lin(){
	// DEFAULT TO SOMETHING REASONABLE
	set_grid_lin(0,90,100);
}

void histogram::set_grid_lin( double E_min_in, double E_max_in, long n_bins_in ){
	// make linearly-spaced vector in lin space
	double stride    = (E_max_in - E_min_in)/n_bins_in;
	for(long i=0;i<n_bins_in+1;i++){
		edges.push_back( i*stride + E_min_in );
	}
	// init rest to zeros
	n_bins	=	n_bins_in;
	E_min	=	E_min_in;
	E_max	=	E_max_in;
	values	=	std::vector<double> (n_bins, 0);
	sqvals	=	std::vector<double> (n_bins, 0);
	counts	=	std::vector<long>   (n_bins, 0);
	err		=	std::vector<double> (n_bins, 0);
}

void histogram::set_grid_cos(){
	// DEFAULT TO SOMETHING REASONABLE
	set_grid_cos(0,90,100);
}

void histogram::set_grid_cos( double E_min_in, double E_max_in, long n_bins_in ){
	// make linearly-spaced vector in cos space
	double E_min_cos = cosf(E_min_in/180.0*pi);
	double E_max_cos = cosf(E_max_in/180.0*pi);
	double stride    = (E_max_cos - E_min_cos)/n_bins_in;
	for(long i=0;i<n_bins_in+1;i++){
		edges.push_back( i*stride + E_min_cos );
	}
	// convert back to degree  
	for(long i=0;i<n_bins_in+1;i++){
		edges[i] = acos(edges[i])/pi*180.0;
	}
	// init rest to zeros
	n_bins	=	n_bins_in;
	E_min	=	E_min_in;
	E_max	=	E_max_in;
	values	=	std::vector<double> (n_bins, 0);
	sqvals	=	std::vector<double> (n_bins, 0);
	counts	=	std::vector<long>   (n_bins, 0);
	err		=	std::vector<double> (n_bins, 0);
}



void histogram::add( double bin_val, double weight ){

	// check if in bounds
	bool valid = true;
	if (bin_val < E_min | bin_val > E_max){
		valid = false;
	}

	// add weight to bin if between bin edges
	long dex;
	std::vector<double>::iterator it;
	std::vector<double>::iterator beg = edges.begin(); 
	std::vector<double>::iterator end = edges.end();
	if (valid){
		if (bin_val == E_min){
			dex = 0;
		}
		else{
			it = std::lower_bound (beg, end, bin_val);
			dex = it-beg-1; //dex = std::distance(beg,it)-1;
		}
		values[dex] = values[dex] + weight;
		sqvals[dex] = sqvals[dex] + weight*weight;
		counts[dex] = counts[dex] + 1;
	}

}
void histogram::update(double norm_val){

	// calculate error
	double tally_err_sq, sum_xi, sum_xi2;
	long N;
	for (long dex=0;dex<n_bins;dex++){
		N		= counts[dex];
		sum_xi	= values[dex];
		sum_xi2	= sqvals[dex];
		if (N==1){
			err[dex] = 1.0;
		}
		else if(N > 1){
			tally_err_sq =   1.0/(N-1) * ( N*sum_xi2/(sum_xi*sum_xi) - 1.0);
			if (tally_err_sq > 0){
				err[dex] = sqrt(tally_err_sq);
			}
			else{
				err[dex] = 0.0;
			}
		}
		else{
			err[dex] = 0.0;
		}
		// apply norm
		values[dex] = values[dex] / norm_val;
		sqvals[dex] = sqvals[dex] / norm_val;

	}

	// calculate normalized cumulative probability distribution
	double total_value = std::accumulate(values.begin(), values.end(), 0.0);
	cumsum_normed = values;
	std::partial_sum(cumsum_normed.begin(), cumsum_normed.end(), cumsum_normed.begin());   // compute cumulative sum in-place
	std::transform  (cumsum_normed.begin(), cumsum_normed.end(), cumsum_normed.begin(), std::bind2nd (std::divides <double> () , total_value)) ;  // binds the second argument to the total value

}
double  histogram::get_rn(){
	// simply return the value from the lib 
	return distribution(generator);
}
double histogram::sample(){

	double rn = get_rn();

}