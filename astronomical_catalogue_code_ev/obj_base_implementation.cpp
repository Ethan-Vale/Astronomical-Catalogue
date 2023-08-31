#include "obj_interface.h"
#include<regex>
#include<stdexcept>

//OPERATORS
std::ostream& operator<<(std::ostream& os, const object& obj)
{
	obj.data_out(os);
	return os;
}

std::istream& operator>>(std::istream& is, object& obj)
{

	int attempts{};
	while (true) {
		try {
			obj.data_in(is);
			return is;
		} catch (const std::invalid_argument& msg) {
			//rethrowing for file reading so exception handled differently
			if (obj.from_file == true) { throw; }
			else {
				std::cerr << msg.what() << "\n";
				is.clear();
				is.ignore(256, '\n');
				attempts++;
				if (attempts > 4) {
					std::cout << "attempts exceeded limit, exiting"; 
					exit(EXIT_FAILURE);
				}
			}
			continue;
		}
	}
}

//FUNCTIONS
void object::checker(std::string regx, std::string input, std::string info)
{
	//function able to pinpoint source of input error at cost 
	//of repetitive code
	std::regex r{ regx };
	std::smatch valid;
	if (!std::regex_match(input, valid, r)) {
		throw std::invalid_argument(
			info + "[" + input + "] is an invalid input");
	}
}
