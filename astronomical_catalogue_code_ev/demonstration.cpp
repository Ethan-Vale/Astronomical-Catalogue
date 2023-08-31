#include "obj_interface.h"
#include "cat_interface.h"

int main()
{
   astronomical_catalogue g;
    std::cout << "this is a program to catalogue data from various"
              << " astronomical objects\nplease pick an action\n";
    std::string user_command;

    while (true) 
    {
        std::cout << "input|print|sort|exit : ";
        getline(std::cin,user_command);

        //USER INPUT
        if (user_command == "input") 
        {
            std::string a{};
            a = g.user_input_check("manual|read|back : ",
                                   R"(manual|read|back)",
                                   "input error. ");

            if (a == "back") { continue; }

            //FROM KEYBOARD
            else if (a == "manual") {
                while (true){
                    std::string p{};
                    std::cout << "input data in csv form (guide in txt file)\n";
                    std::cin >> g;
                    std::cout << "add another object?\n";
                    p = g.user_input_check("y|n",R"(y|n)","input error");
                    if (p == "y") {continue;}
                    else { break; }
                }

            } 
            
            //FROM FILE
            else {
                std::string fn;
                g.reading = true;

                fn = g.user_input_check("filename.csv|back : ",
                                        R"((\w+(\.csv))|back)",
                                        "check type or select a valid action");

                if (fn == "back" || !g.file_good(fn)) { 
                    g.reading = false; 
                    continue; 
                }

                else {
                    std::cout << "reading from file\n";
                    std::ifstream ifs{ fn };
                    g.read_from_file(ifs);
                    g.reading = false;
                }
            }
        }

        //PRINT CATALOGUE
        else if (user_command == "print")
        {
            if (g.empty("all")) { continue; }

            std::string e{};
            std::string f{};
            e = g.user_input_check("galaxy|star|planet|moon|all|back : ",
                R"(galaxy|star|planet|moon|all|back)",
                "input type error. ");

            if (e == "back" || g.empty(e)) { continue; }

            f = g.user_input_check("screen|file|back : ",
                R"(screen|file|back)",
                "input error. ");

            if (f == "back") { continue; }

            //TO SCREEN
            else if (f == "screen") {
                std::cout << "\n";
                if (e == "all") { std::cout << g; }
                else { g.sub_catalogue_out(std::cout, e); }
                std::cout << "\n";
            }

            //TO FILE
            else
            {
                std::string fn;
                g.writing = true;

                fn = g.user_input_check("filename.csv|back : ",
                    R"((\w+(\.csv))|back)",
                    "check type or select a valid action");

                if (fn == "back" || !g.file_good(fn)) {
                    g.writing = false;
                    continue;
                }

                else {
                    std::cout << "writing to file\n";
                    std::ofstream ofs{ fn };
                    if (e == "all") ofs << g;
                    else { g.sub_catalogue_out(ofs, e); }
                    ofs.close();
                    g.writing = false;
                }
            }
        }

        //SORT SUB CATALOGUE OBJECTS
        else if (user_command == "sort") 
        {
            if (g.empty("all")) { continue; }

            std::string b{};
            std::string c{};
            std::string d{};
            b = g.user_input_check("galaxy|star|planet|moon|back : ",
                                   R"(galaxy|star|planet|moon|back)",
                                   "input error. plesase select an option. ");

            if (b == "back" || g.empty(b)) { continue; }

            c = (b == "galaxy" || b == "star") ?
                g.user_input_check("dist|mag|back : ",
                                   R"(dist|mag|back)",
                                   "parameter error! ") :
                g.user_input_check("prd|mass|back : ",
                                   R"(prd|mass|back)",
                                   "parameter error! ");

            if (c == "back") { continue; }

            d = g.user_input_check("inc|dec|back : ",
                                   R"(inc|dec|back)",
                                   "order error!");

            if (d == "back") { continue; }

            g.sort_data(b, c, d);
        }

        //EXIT CODE
        else if (user_command == "exit") 
        {
            exit(EXIT_SUCCESS);
        }

        else 
        {
            std::cout << "[" << user_command << "] invalid input, try again\n";
        }

        continue;
        }

    return 0;
}