class histogram {

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

	~histogram();
	histogram();
	void set_grid_log();
	void set_grid_log(double,double,long);
	void set_grid_lin();
	void set_grid_lin(double,double,long);
	void set_grid_cos();
	void set_grid_cos(double,double,long);
	void add(double,double);
	void update(double);

};