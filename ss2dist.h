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