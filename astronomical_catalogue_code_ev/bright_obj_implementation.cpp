#include "obj_interface.h"
#include<iomanip>
#include<sstream>
//BRIGHT OBJECT BASE IMPLEMENTATION
//CONSTRUCTORS
//parametrised constructor
bright_object::bright_object(std::string nam, 
							 three_arr ras, 
							 three_arr dec, 
							 double dist, 
							 double mag) :
	name{ nam }, 
	right_ascension{ std::make_unique<double[]>(3) },
	declination{ std::make_unique<double[]>(3) },
	distance{ dist }, 
	magnitude{ mag }
{
	for (int i{}; i < 3; i++) {
		right_ascension[i] = ras[i];
		declination[i] = dec[i];
	}
}
//copy constructor
bright_object::bright_object(const bright_object & obj_to_cpy) :
	name{ obj_to_cpy.name }, 
	right_ascension{ std::make_unique<double[]>(3) },
	declination{ std::make_unique<double[]>(3) }, 
	distance{ obj_to_cpy.distance },
	magnitude{ obj_to_cpy.magnitude }
{
	for (int i{}; i < 3; i++) {
		right_ascension[i] = obj_to_cpy.right_ascension[i];
		declination[i] = obj_to_cpy.declination[i];
	}
}


//OPERATORS
//copy assignment
bright_object& bright_object::operator= (const bright_object& obj_rhs)
{
	//check for self referencing
	if (&obj_rhs != this) {
		right_ascension.reset(); declination.reset();
		name = obj_rhs.name;
		right_ascension = std::make_unique<double[]>(3);
		declination = std::make_unique<double[]>(3);
		for (int i{}; i < 3; i++) {
			right_ascension[i] = obj_rhs.right_ascension[i];
			declination[i] = obj_rhs.declination[i];
		}
		distance = obj_rhs.distance;
		magnitude = obj_rhs.magnitude;
	}
	return *this;
}

//FUNCTIONS
void bright_object::data_out(std::ostream& os) const
{
	using namespace std;
	os << left << setw(11) << name << delim;

	//ensure moved from object is in valid but unspecified state
	if (right_ascension != nullptr && declination != nullptr) {
		os << setw(2) << setfill('0') << right_ascension[0] << " "
			<< setw(2) << setfill('0') << right_ascension[1] << " "
			<< setw(4) << setfill('0') << right_ascension[2] << delim
			<< internal << setw(3) << setfill('0') << showpos << declination[0] << " "
			<< setw(2) << setfill('0') << noshowpos << declination[1] << " "
			<< setw(2) << setfill('0') << declination[2] << delim;
	 } else {
		os << setw(2) << setfill('0') << 0 << " "
		   << setw(2) << setfill('0') << 0 << " "
		   << setw(4) << setfill('0') << 0 << delim
		   << internal << setw(3) << setfill('0') << 0 << " "
		   << setw(2) << setfill('0') << 0 << " "
		   << setw(2) << setfill('0') << 0 << delim;
	}

	os << left << setfill(' ') << setw(10) << distance << delim
	   << left << setw(6) << magnitude << delim;
};

void bright_object::data_in(std::istream& is)
{
	std::string buff;
	getline(is, name, ',');
	getline(is, buff, ',');
	checker(ras_format, buff, "right ascension error. ");
	std::istringstream ra_str(buff);
	right_ascension = std::make_unique<double[]>(3);

	for (int i{}; i < 3; i++) {
		getline(ra_str, buff, ' ');
		right_ascension[i] = stod(buff);
	}

	getline(is, buff, ',');
	checker(dec_format, buff, "declination error. ");
	std::istringstream dec_str(buff);
	declination = std::make_unique<double[]>(3);

	for (int i{}; i < 3; i++) {
		getline(dec_str, buff, ' ');
		declination[i] = stod(buff);
	}

	getline(is, buff, ',');
	checker(R"(\d+.?d*\s*)", buff, "distance error. ");
	distance = stod(buff);
	getline(is, buff, ',');
	checker(R"(-?\d+.?\d*\s*)", buff, "magnitude error. ");
	magnitude = stod(buff);
}

std::pair<double, double> bright_object::to_sort()
{
	return std::make_pair(distance, magnitude);
}



//GALAXY DERIVED IMPLEMENTATION
//CONSTRUCTORS

//default constructor
galaxy::galaxy() { object_type = "galaxy"; }

//parametrised constructor
galaxy::galaxy(std::string h_type, 
			   std::string name,
			   three_arr ras, 
			   three_arr dec, 
			   double dist, 
			   double mag, 
			   double red, 
			   double hrv) : 
	hubble_type {h_type}, 
	bright_object{ name, ras, dec, dist, mag }, 
	redshift{ red },
	helio_radial_velocity{ hrv } 
{
	object_type = "galaxy";
}

//FUNCTIONS
void galaxy::data_out(std::ostream& os) const
{
	using namespace std;
	os << left << std::setw(3) << hubble_type << delim;
	bright_object::data_out(os);
	os << left << std::setw(5) << redshift << delim
	   << left << std::setw(17) << helio_radial_velocity << "\n";
}

void galaxy::data_in(std::istream& is)
{
	std::string buff2;
	getline(is, buff2, ',');
	checker(hub_format, buff2, "hubble type error. ");
	hubble_type = buff2;
	bright_object::data_in(is);
	getline(is, buff2, ',');
	checker(R"(\d+.?d*\s*)", buff2, "redshift error. ");
	redshift = stod(buff2);
	getline(is, buff2);

	//csv files of varying column number assume widest column width by default
	//work around this by reading in those extra empty cells and removing them
	//from buffer string
	if (buff2.find(',') != std::string::npos) {
		buff2.erase(std::remove(buff2.begin(), buff2.end(), ','), buff2.end());
	}
	checker(R"(\d+.?d*\s*)", buff2, "radial velocity error. ");
	helio_radial_velocity = stod(buff2);
}

void galaxy::print_header() { std::cout << header_g; }



//STAR DERIVED IMPLEMENTATION
//CONSTRUCTORS

//default constructor
star::star() { object_type = "star";}

//parametrised constructor
star::star(std::string h_nam, 
		   std::string name, 
		   three_arr ras, 
		   three_arr dec, 
		   double dist, 
		   double mag,
		   std::string ev,
		   double m, 
		   double t) : 
	host_galaxy{ h_nam },
	bright_object{ name, ras, dec, dist, mag }, 
	evolutionary_stage{ ev },
	mass{ m },
	temperature{ t } 
{	
	object_type = "star";
}

//FUNCTIONS
void star::data_out(std::ostream& os) const
{
	using namespace std;
	os << left << std::setw(11) << host_galaxy << delim;
	bright_object::data_out(os);
	os << left << std::setw(3) << evolutionary_stage << delim 
	   << left << std::setw(9) << mass << delim 
	   << left << std::setw(8) << temperature << "\n";
}

void star::data_in(std::istream& is)
{
	std::string buff4;
	getline(is, host_galaxy, ',');
	bright_object::data_in(is);
	getline(is, buff4, ',');
	checker(ev_format, buff4, "evolutionary stage error. ");
	evolutionary_stage = buff4;
	getline(is, buff4, ',');
	checker(R"(\d+.?d*\s*)", buff4, "mass error. ");
	mass = stod(buff4);
	getline(is, buff4);
	checker(R"(\d+.?d*\s*)", buff4, "temperature error. ");
	temperature = stod(buff4);
}

void star::print_header() { std::cout << header_s; }