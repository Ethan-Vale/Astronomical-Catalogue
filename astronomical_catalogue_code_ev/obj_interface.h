#ifndef OBJ_INTERFACE_H
#define OBJ_INTERFACE_H
#include<iostream>
#include<string>
#include<memory>
#include<array>
#include<vector>

//OBJECT ABSTRACT BASE CLASS
class object
{
	friend class astronomical_catalogue;
	friend std::ostream& operator<<(std::ostream&, const object&);
	friend std::istream& operator>>(std::istream&, object&);

protected:
	//delimeter depending on printing to screen or file 
	std::string delim{ "|" };

	//used for rethrowing exception in input if file is being read
	bool from_file{ false };
public:
	std::string object_type{};
	virtual ~object() {}

	//go-between functions for psuedo overriding in-out operators
	virtual void data_out(std::ostream&) const = 0;
	virtual void data_in(std::istream&) = 0;

	//virtual function for printing collumn headers in catalogue
	virtual void print_header() = 0;

	//generic function to check validity of string using regex
	static void checker(std::string, std::string, std::string);

	//function to retrieve comparable object data
	virtual std::pair<double, double> to_sort() = 0;
};

//BRIGHT OBJECT DERIVED ABSTRACT BASE CLASS
class bright_object : public object
{
private:
	//regex strings
	std::string ras_format{ R"([0-2]?[0-4] [0-6]?[0-9] [0-6]?[0-9](\.[0-9]*)?)" },
				dec_format{ R"([+-]?[0-9]{1,2} [0-6]?[0-9] [0-6]?[0-9](\.[0-9]*)?)" };
protected:
	typedef std::array<double, 3> three_arr;

	std::unique_ptr<double[]> right_ascension{ nullptr },
							  declination{ nullptr };
	std::string name{};
	double distance{}, magnitude{};
public:
	bright_object() = default;
	bright_object(std::string, three_arr, three_arr, double, double);

	//copy constructor and assignment needed due to unique pointer
	bright_object(const bright_object&);

	//rule of 5 need to explicitly default move constructor and assignment
	bright_object(bright_object&&) noexcept = default;
	virtual ~bright_object() = default;
	bright_object& operator= (const bright_object&);
	bright_object& operator= (bright_object&&) noexcept = default;
	virtual void print_header() = 0;
	virtual void data_out(std::ostream&) const;
	virtual void data_in(std::istream&);
	std::pair<double, double> to_sort();
};

//DARK OBJECT DERIVED ABSTRACT BASE CLASS
class dark_object : public object
{
protected:
	std::string name{};
	double mass{}, period{};
public:
	dark_object() = default;
	dark_object(std::string, double, double);
	dark_object(const dark_object&) = default;
	dark_object(dark_object&&) noexcept = default;
	virtual ~dark_object() = default;
	dark_object& operator= (const dark_object&) = default;
	dark_object& operator= (dark_object&&) noexcept = default;
	virtual void data_out(std::ostream&) const;
	virtual void data_in(std::istream&);
	std::pair<double, double> to_sort();
	virtual void print_header() = 0;
};

//GALAXY DERIVED CLASS
class galaxy : public bright_object
{
private:
	std::string hub_format{ R"((E[0-7]|SB?0|SB?[abc]|Irr)\s*)" },
				header_g{ "HUB|NAME       |RA / J2000|DEC/J2000|DIST / Mly|MAG   |RSHFT|HELIO RADV / km/s\n" };
protected:
	std::string hubble_type{};
	double redshift, helio_radial_velocity;
public:
	galaxy();
	galaxy(std::string,
		std::string,
		three_arr,
		three_arr,
		double,
		double,
		double,
		double);

	//all class members copyable and moveable so no need for new definitions 
	galaxy(const galaxy&) = default;
	galaxy(galaxy&&) = default;
	~galaxy() = default;

	//rule of 5 again
	galaxy& operator=(const galaxy&) = default;
	galaxy& operator=(galaxy&&) = default;
	void data_out(std::ostream&) const;
	void data_in(std::istream&);
	void print_header();
};

//STAR DERIVED CLASS
class star : public bright_object
{
protected:
	std::string host_galaxy;
	std::string evolutionary_stage;
	double mass, temperature;
	const std::string ev_format{ R"((Bd|Wd|Rd||Rg|Rsg)\s*)" },
					  header_s{ "HOST GALAXY|NAME       |RA / J2000|DEC/J2000|DIST / Mly|MAG   |EV |MASS / Ms|TEMP / K\n"};
public:
	star();
	star(std::string, 
		 std::string, 
		 three_arr, 
		 three_arr, 
		 double, 
		 double, 
		 std::string, 
		 double,
		 double);

	star(const star&) = default;
	star(star&&) = default;
	~star() {}
	star& operator=(const star&) = default;
	star& operator=(star&&) = default;
	void data_out(std::ostream&) const;
	void data_in(std::istream&);
	void print_header();
};

//PLANET DERIVED CLASS
class planet : public dark_object
{
private:
	std::string header_p{ "ORBITING   |NAME       |M / Mj|PRD / d|ORB RAD / AU|ECC \n" };
protected:
	std::string host_star;
	double semi_major_axis, eccentricity;
public:
	planet();
	planet(std::string, std::string, double, double, double, double);
	planet(const planet&) = default;
	planet(planet&&) = default;
	~planet() {}
	planet& operator=(const planet&) = default;
	planet& operator=(planet&&) = default;
	void data_out(std::ostream&) const;
	void data_in(std::istream&);
	void print_header();
};

//MOON DERIVED CLASS
class moon : public dark_object
{
private:
	std::string header_m{ "ORBITING   |NAME       |M / kg|PRD / d|DIAMETER / km\n" };
protected:
	std::string host_planet;
	double diameter;
public:
	moon();
	moon(std::string, std::string, double, double, double);
	moon(const moon&) = default;
	moon(moon&&) = default;
	~moon() {}
	moon& operator=(const moon&) = default;
	moon& operator=(moon&&) = default;
	void data_out(std::ostream&) const;
	void data_in(std::istream&);
	void print_header();
};
#endif
