struct track
{
	double nps, bitarray, wgt, erg, tme, x, y, z, xhat, yhat, cs, zhat;		

};

struct surface
{
	double value[10];
};

struct surface_card_data
{
	char	symbol[4];
	int 	n_coefficients;
	char 	description[41]; 
};


class SurfaceSource
{	

	//
	//
	// DATA
	//
	//

	//
	//
	// METHODS
	//
	//

	bool ReadRecord(void** , size_t*, size_t);
	bool ReadSurfaceRecord0(int* , int* , int* , surface*);
	bool ReadSurfaceRecord1(int* , int* , int* , surface*, int*);
	bool ReadSummaryRecord(int**);


public:

	//
	//
	// DATA
	//
	//

	// parameters
	char				id[9], kods[9], vers[6], lods[9], idtms[20], probs[20], aids[81];
	int					knods, nrcd, njsw, niwr, mipts, kjaq;	
	// these are always declared as 8byte?!
	long long 			np1, nrss, niss;

	// arrays
	int*				surface_numbers;
	int*				surface_types;
	int*				surface_facets;
	int*				surface_parameters_lengths;
	int** 				surface_summaries;
	surface*			surface_parameters;
	int					surface_count;
	int 				surface_summary_length;

	// surface description lookup table
	surface_card_data 	surface_card[41];

	// file object
	std::string			input_file_name;
	std::ifstream 		input_file;

	//
	//
	// METHODS
	//
	//

	~SurfaceSource();
	SurfaceSource();
	SurfaceSource( const std::string& );
	SurfaceSource( const char*   );
	void Init();
	void OpenWssaFile( const char* );
	void ReadHeader();
	void PrintHeader();
	void PrintSizes();
	void GetTrack(track*);


};

class InputFile {

	std::vector<std::string>  SplitString(const std::string&, char );

public:

	//
	//
	// DATA
	//
	//
	
	//  surface vectors
	std::valarray<double> surface_plane;
	std::valarray<double> surface_center;

	// bin vectors
	std::vector<double> E_bins;
	std::vector<double> theta_bins;
	std::vector<double> phi_bins;

	// particle naming
	std::valarray<char> particle_symbols;
	long this_particle;

	// regular xy binning paramters
	double 	x_min, x_max, x_res, y_min, y_max, y_res;
	long 	x_len, y_len, this_sc;

	// spectral binning paramters
	double 	spec_E_min, spec_E_max, spec_x_min, spec_x_max, spec_y_min, spec_y_max;
	long 	spec_E_bins;
	std::vector<double> spec_theta_edges;

	// file object
	std::string			input_file_name;
	std::ifstream 		input_file;

	//
	//
	// METHODS
	//
	//

	~InputFile();
	InputFile();
	InputFile( const std::string& );
	InputFile( const char*   );
	void Init();
	void OpenInputFile( const char* );
	bool GetSurface( SurfaceSource* );
	void Parse();
	void PrintSummary();

};

class histogram_log {

public:

	//
	//
	// DATA
	//
	//

	std::vector<double> edges;
	long                n_bins;
	std::vector<double> values;
	std::vector<double> sqvals;
	std::vector<long>   counts;
	std::vector<double> err;
	double              E_min, E_max;

	//
	//
	// METHODS
	//
	//

	~histogram_log();
	histogram_log();
	histogram_log(double,double,long);
	void add(double,double);
	void update();

};