#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <valarray>
#include <climits>
#include <sstream>
#include <sys/stat.h>
#include <bitset>
#include "SurfaceSource.h"
#include "helpers.h"

#include <vector>
#include <numeric>
#include <iterator>
#include <Python.h>
#include <numpy/numpyconfig.h>
#include <numpy/arrayobject.h>
//#include <Python/Python.h>

extern "C" {
    void initss2py(void);
}


static PyObject* read_wssa(PyObject *self, PyObject* args){

	// get arguments
	const char *input_string;
	if (!PyArg_ParseTuple(args, "s", &input_string))
		return NULL;

	// init some data
	track this_track;
	SurfaceSource ss(input_string);//argv[1]);

	// load up WSSA file
	ss.ReadHeader();

	//
	unsigned b			= 0;
	unsigned j			= 0;
	unsigned ipt		= 0;
	unsigned nsf		= 0;
	unsigned i_positron = 0;
	unsigned jgp		= 0;
	//
	//double 	total_weight	= 0.0;
	//double 	total_tracks	= 0;
	//
	bool printflag = false;
	bool errorflag = false;

	// set loop length
	long N = ss.nrss;

	// data array
	long n_per_element = 9;
	long Ndataset = N*n_per_element;
	npy_intp dims[2];
	dims[0] = N;
	dims[1] = n_per_element;
	//PyArrayObject* data_object   = PyArray_SimpleNew(Ndataset, dims, NPY_DOUBLE);
	double* data_matrix	= (double*) malloc(Ndataset*sizeof(double));//new double [Ndataset];

	// loop over tracks
	for(long i=0;i<N;i++){

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
		data_matrix[i*n_per_element+0] 	= ipt;
		data_matrix[i*n_per_element+1]	=	this_track.xhat;
		data_matrix[i*n_per_element+2]	=	this_track.yhat;
		data_matrix[i*n_per_element+3]	=	this_track.zhat;
		data_matrix[i*n_per_element+4]	=	this_track.x;
		data_matrix[i*n_per_element+5]	=	this_track.y;
		data_matrix[i*n_per_element+6]	=	this_track.z;
		data_matrix[i*n_per_element+7] 	= this_track.erg;
		data_matrix[i*n_per_element+8] 	= this_track.wgt;

	}


	//std::cout << "DONE." << std::endl;

	return PyArray_SimpleNewFromData(2, dims, NPY_FLOAT64, data_matrix);

}


static PyMethodDef ss2py_methods[] = {
	{"read_wssa", read_wssa,	METH_VARARGS,
	 "Read wssa file and return the phase space as a contiguous array."},
	{NULL,		NULL}		/* sentinel */
};

extern void initss2py(void)
{
	PyImport_AddModule("ss2py");
	Py_InitModule("ss2py", ss2py_methods);
	import_array();
}

int main(int argc, char **argv)
{
	Py_SetProgramName(argv[0]);

	Py_Initialize();

	initss2py();

	Py_Exit(0);
}
