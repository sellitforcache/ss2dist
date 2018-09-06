class Histogram {

	//
	// RNG objects
	//
	std::mt19937_64 generator;
	std::uniform_real_distribution<double> distribution{0.0,1.0};

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
	std::vector<double> cumsum_normed;
	std::vector<double> err;
	double              E_min, E_max;


	//
	//
	// METHODS
	//
	//

	~Histogram();
	Histogram();
	void set_grid_log();
	void set_grid_log(double,double,long);
	void set_grid_lin();
	void set_grid_lin(double,double,long);
	void set_grid_cos();
	void set_grid_cos(double,double,long);
	void add(double,double);
	void update(double);
	double get_rn();
	double sample();

};
