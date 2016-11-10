struct track
{
	double x, y, z, xhat, yhat, zhat, erg, surface;
};

struct surface
{
	double A, B, C, D, E, F, G, H, I, K;
};

struct surface_card_data
{
	char	symbol[3];
	int 	n_coefficients;
	char 	description[40]; 
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
	bool ReadSurfaceRecord0(int* ,       surface*);
	bool ReadSurfaceRecord1(int* , int*, surface*);


public:

	//
	//
	// DATA
	//
	//

	// parameters
	char*				id, kods, vers, lods, idtms, probs, aids;
	int 				knods, np1, nrss, nrcd, njsw, niss, niwr, mipts, kjaq;	

	// arrays
	int*				surface_numbers;
	surface*			surface_parameters;
	int 				surface_count;

	// surface description lookup table
	surface_card_data 	surface_card[41];

	// file object
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

};