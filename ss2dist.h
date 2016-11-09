struct track
{
	double x, y, z, xhat, yhat, zhat, erg, surface;
};

struct surface
{
	double A,B,C,D,E,F,G,H,I,K;
};

struct surface_card_data
{
	char	symbol[3];
	int 	n_coefficients;
	char 	description[40]; 
};


class SurfaceSource
{	
public:

	//
	//
	// DATA
	//
	//

	// parameters
	string		id, kods, vers, lods, idtms, probs, aids;
	int 		knods, np1, nrss, nrcd, njsw, niss, niwr, mipts, kjaq;	

	// arrays
	int*		surface_numbers;
	surface*	surface_parameters;

	// surface description lookup table
	surface_card_data 	surface_card[41];

	//
	//
	// METHODS
	//
	//
	~SurfaceSource();
	SurfaceSource();
	SurfaceSource(const string& fileName );
	SurfaceSource(char*         fileName );


}