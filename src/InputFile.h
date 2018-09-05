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
	std::valarray<double> principle_vector;

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
	double mag(std::valarray<double>);

};
