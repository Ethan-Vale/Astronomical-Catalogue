#ifndef CAT_INTERFACE_H
#define CAT_INTERFACE_H
#include "obj_interface.h"
#include<map>
#include <fstream>
#include<regex>

//ASTRONOMICAL CATALOGUE CLASS
class astronomical_catalogue
{
	friend std::ostream& operator<<(std::ostream&, 
									const astronomical_catalogue&);
	friend std::istream& operator>>(std::istream&, astronomical_catalogue&);

protected:

	//typedef used to shorten some long arguments
	typedef std::unique_ptr<object> object_ptr;
	typedef std::vector<object_ptr> object_ptr_vec;

	//multimap allows for storage of objects linked to
	//string keys. user friendly and allows for nice coding later on
	std::multimap<std::string, object_ptr> catalogue;

	//object types
	const std::array<std::string, 4> types{ "galaxy","star","planet","moon" };
public:
	//used for different exception handling
	bool writing{ false };
	bool reading{ false };

	//no parametrised constructor needed as wont be initialising catalogue
	astronomical_catalogue() = default;
	~astronomical_catalogue() {}

	//function to check if catalogue is empty
	int empty(std::string);

	//go between for input and output operators
	void sub_catalogue_out(std::ostream&, std::string) const;
	template<typename T>
	void cat_object_in(std::istream&);

	//specific file read function for special exception handling
	void read_from_file(std::ifstream&);

	//function to check if file was opened/created correctly
	bool file_good(std::string);

	//generic pompt input checker functions
	std::string user_input_check(std::string, std::string, std::string);

	//sort data function
	void sort_data(std::string, std::string, std::string);
};

template<typename T>
void astronomical_catalogue::cat_object_in(std::istream& is)
{
	T obj{};
	if (reading == true) obj.from_file = true;
	is >> obj;
	catalogue.insert(std::make_pair(obj.object_type, std::make_unique<T>(obj)));
};


#endif