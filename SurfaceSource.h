enum sequence1 {
	RSSA_WRITE = 0,
	RSSA_READ
};

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

// FORTRAN record delimiter length... usually 4 bytes.  Can be 8!  Should set as a preprocessor option
const int RECORD_DELIMITER_LENGTH = 4;

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
	bool WriteRecord(void** , size_t*, size_t);
	bool WriteSurfaceRecord0(int* , int* , int* , surface*);
	bool WriteSurfaceRecord1(int* , int* , int* , surface*, int*);
	bool WriteSummaryRecord(int**);


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
	int 				mipt = 37;
	int 				surface_summary_length = 2+4*mipt;

	// surface description lookup table
	surface_card_data 	surface_card[41];

	// file objects
	std::string			input_file_name;
	std::ifstream 		input_file;
	std::string			output_file_name;
	std::ofstream 		output_file;

	//
	//
	// METHODS
	//
	//

	~SurfaceSource();
	SurfaceSource();
	SurfaceSource( const std::string& );
	SurfaceSource( const char*   );
	SurfaceSource( const std::string& ,const std::string& );
	SurfaceSource( const char*   ,const char*   );
	SurfaceSource( const std::string& , const int );
	SurfaceSource( const char*   , const int );
	void Init();
	void OpenWssaFile_Read(  const char* );
	void OpenWssaFile_Write( const char* );
	void ReadHeader();
	void WriteHeader();
	void PrintHeader();
	void PrintSizes();
	void GetTrack(track*);
	void PutTrack(track*);
	void PutTrack(double,double,double,double,double,double,double,double,double,double,double,double);


};
