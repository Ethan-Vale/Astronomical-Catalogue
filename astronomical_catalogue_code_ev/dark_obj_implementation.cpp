#include "obj_interface.h"
#include<iomanip>

//DARK OBJECT BASE IMPLEMENTATION
//CONSTRUCTORS

//parametrised constructor
dark_object::dark_object(std::string nam, double prd, double m) : 
	name{ nam }, 
	period{ prd }, 
	mass{ m } {}

//FUNCTIONS
void dark_object::data_out(std::ostream& os) const
{
	using namespace std;
	os << left << std::setw(11) << name << delim
	   << left << std::setw(6) << mass << delim
	   << left << std::setw(7) << period << delim;
}

void dark_object::data_in(std::istream& is)
{
	std::string buff;
	getline(is, name, ',');
	getline(is, buff, ',');
	checker(R"(\d+.?d*\s*)", buff, "mass error. ");
	mass = stod(buff);
	getline(is, buff, ',');
	checker(R"(\d+.?d*\s*)", buff, "period error. ");
	period = stod(buff);
}

std::pair<double, double> dark_object::to_sort()
{
	return std::make_pair(mass, period);
}



//PLANET DERIVED IMPLEMENTATION
//CONSTRUCTORS

//default constructor
planet::planet() { object_type = "planet"; }

//parametrised constructor
planet::planet(std::string h_nam, 
			   std::string nam, 
			   double prd, 
			   double m, 
			   double smaxis, 
			   double ecc) :
	host_star{ h_nam }, 
	dark_object(nam, prd, m), 
	semi_major_axis{ smaxis }, 
	eccentricity{ ecc } 
{
	object_type = "planet";
}

//FUNCTIONS
void planet::data_out(std::ostream& os) const
{
	using namespace std;
	os << left << std::setw(11) << host_star << delim;
	dark_object::data_out(os);
	os << left << std::setw(12) << semi_major_axis << delim
	   << left << std::setw(4) << eccentricity << "\n";
}

void planet::data_in(std::istream& is)
{
	getline(is, host_star, ',');
	dark_object::data_in(is);
	std::string buff3;
	getline(is, buff3, ',');
	checker(R"(\d+.?d*\s*)", buff3, "axis error. ");
	semi_major_axis = stod(buff3);
	getline(is, buff3);
	if (buff3.find(',') != std::string::npos) {
		buff3.erase(std::remove(buff3.begin(), buff3.end(), ','), buff3.end());
	}
	checker(R"(\d+.?d*\s*)", buff3, "eccentricity error. ");
	eccentricity = stod(buff3);
}

void planet::print_header() { std::cout << header_p; }



//MOON DERIVED IMPLEMENTATION
//CONSTRUCTORS

//default constructor
moon::moon() { object_type = "moon"; }

//parametrised constructor
moon::moon(std::string h_nam, std::string nam, double prd, double m, double d) :
	host_planet{ h_nam }, 
	dark_object(nam, prd, m), 
	diameter{ d }
{
	object_type = "moon";
}

//FUNCTIONS
void moon::data_out(std::ostream& os) const
{
	using namespace std;
	os << left << std::setw(11) << host_planet << delim;
	dark_object::data_out(os);
	os << left << std::setw(13) << diameter << "\n";
}

void moon::data_in(std::istream& is)
{
	getline(is, host_planet, ',');
	dark_object::data_in(is);
	std::string buff4;
	getline(is, buff4);
	if (buff4.find(',') != std::string::npos) {
		buff4.erase(std::remove(buff4.begin(), buff4.end(), ','), buff4.end());
	}
	checker(R"(\d+.?d*\s*)", buff4, "diameter error. ");
	diameter = stod(buff4);
}

void moon::print_header() { std::cout << header_m; }
