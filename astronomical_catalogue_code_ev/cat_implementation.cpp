#include "cat_interface.h"
#include <algorithm>

//OPERATORS
std::ostream& operator<<(std::ostream& os, const astronomical_catalogue& cat)
{

	for_each(cat.types.begin(), cat.types.end(), [&](const std::string& type) {
		cat.sub_catalogue_out(os, type);
		if (cat.writing != true) os << "\n";
		});

	return os;
}

std::istream& operator>>(std::istream& is, astronomical_catalogue& cat)
{
	while (true) {
		std::string type{};
		try {
			getline(is, type, ',');
			object::checker(R"(\s*(galaxy|star|planet|moon))",
				type,
				"object type error. ");

			if (type == "galaxy") { cat.cat_object_in<galaxy>(is); }
			else if (type == "star") { cat.cat_object_in<star>(is); }
			else if (type == "planet") { cat.cat_object_in<planet>(is); }

			//no need for else-if as checker function already run
			else { cat.cat_object_in<moon>(is); }
			return is;
		}
		catch (std::invalid_argument& msg) {

			//throw again for specific error handling
			if (cat.reading == true) throw;
			std::cerr << msg.what() << "\n";
			is.clear();
			is.ignore(256, '\n');
			continue;
		}
	}

}


//FUNCTIONS
void astronomical_catalogue::sub_catalogue_out(std::ostream& os, 
											   std::string type) const
{
	//precaution to avoid infinite loop if code itself is wrong
	try {
		size_t row{};
		object::checker(R"(galaxy|star|planet|moon)",
						type, 
						"object type error. ");

		auto i = catalogue.equal_range(type);
		for (auto& itr = i.first; itr != i.second; itr++, row++) {
			if (writing != true) {
				if (row == 0) {
					//convert to uppercase
					std::string t = type;
					std::transform(t.begin(), t.end(), t.begin(), ::toupper);
					os << t + " CATALOGUE (" 
					   << catalogue.count(type) 
					   << " objects)\n";

					//printing type specific header
					(*itr->second).print_header();
				}
			} else {
				(*itr->second).delim = ",";
				os << type << ",";
			}
			os << *itr->second;
			(*itr->second).delim = "|";
		}
	}
	catch (std::invalid_argument& msg) {
		std::cerr << msg.what();
		exit(EXIT_FAILURE);
	}
}

std::string astronomical_catalogue::user_input_check(std::string prompt,
													 std::string r,
													 std::string e)
{
	std::string input;
	while (true) {
		try {
			std::cout << prompt;
			getline(std::cin, input, '\n');
			object::checker(r, input, e);
			break;
		}
		catch (std::invalid_argument& msg) {
			std::cerr << msg.what() << "\n";
			std::cin.clear();
			continue;
		}
	}
	return input;
}

bool astronomical_catalogue::file_good(std::string fn)
{
	std::fstream fs;
	try {

		fs.open(fn);

		//if not reading file, create new one
		if (!reading) { std::cout << fn + " created\n"; return true; }

		(!fs.is_open()) ? throw std::runtime_error("failed to open " + fn) :
			std::cout << fn + " opened \n";
		return true;
	}
	catch (const std::runtime_error& msg) {
		std::cerr << msg.what()
			<< ": check spelling or file access privileges\n";
		fs.clear();
		return false;
	}
}

void astronomical_catalogue::read_from_file(std::ifstream& ifs)
{
	int row_num{ 1 };
	while (ifs.peek() != EOF) {
		try {
			ifs >> *this;
			row_num++;
		}
		catch (std::invalid_argument& msg) {

			//output location and source of error
			std::cerr << "[row " + std::to_string(row_num) + " ignored] ";
			std::cerr << std::string(msg.what()) + "\n";
			ifs.clear();
			ifs.ignore(256, '\n');
			row_num++;
			continue;
		}
	}
	ifs.close();
}

void astronomical_catalogue::sort_data(std::string type,
									   std::string by, 
									   std::string order)
{
	object_ptr_vec objects;
	auto i = catalogue.equal_range(type);

	//objects moved from map into vector
	for (auto& itr = i.first; itr != i.second; itr++)
		objects.push_back(std::move(itr->second));

	//sub_map deleted
	catalogue.erase(type);

	std::sort(objects.begin(), objects.end(), [&](const object_ptr& a, 
												  const object_ptr& b) {
		if (by == "dist" || by == "prd") {
			return (order == "inc") ? a->to_sort().first < b->to_sort().first :
									  a->to_sort().first > b->to_sort().first;
		} else if (by == "mag" || by == "mass") {
			return (order == "inc") ? a->to_sort().second < b->to_sort().second :
									  a->to_sort().second > b->to_sort().second;
		} else { std::cerr << "unforseen error has occurred. exiting"; 
				 exit(EXIT_FAILURE); }
		});

	//objects moved back into map
	for_each(objects.begin(), objects.end(), [&](object_ptr& a) {
		catalogue.insert(std::make_pair(type, std::move(a)));
		});

	sub_catalogue_out(std::cout, type);
}

int astronomical_catalogue::empty(std::string s)
{
	if (s == "all") {
		size_t i{};
		for_each(types.begin(), types.end(), [&](const std::string& t) {
			i += catalogue.count(t);
			});

		if (i == 0) {
			std::cout << "catalogue is currently empty\n";
			return 1;
		}
	}
	else {
		if (catalogue.count(s) == 0) {
			std::cout << "no " + s + "'s in catalogue\n";
			return 1;
		}
	}
	return 0;
}