#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include "map.h"

//int main()
//{
//	//std::ifstream ifs;
//	char ch[] = "./Resource/map/tiles2.txt";
//	char* p = &ch[0];
//	std::string finame = "./Resource/map/tiles2.txt";
//	//ifs.open(p, std::ios::in);
//	//if (!ifs.is_open())
//	//	return 1;
//	//std::string str;
//	//std::getline(ifs, str);
//	//
//	//std::cout << str.substr(str.find(' ') + 1) << typeid(str.substr(str.find(' ') + 1)).name()<< std::endl;
//	//std::getline(ifs, str);
//	//std::cout << str << std::endl;
//	//std::getline(ifs, str);
//	//std::cout << str << std::endl;
//	//std::getline(ifs, str);
//	//std::cout << str << std::endl;
//	//std::getline(ifs, str);
//	//std::cout << str << std::endl;
//	//std::getline(ifs, str);
//	//std::cout << str << std::endl;
//	//std::getline(ifs, str);
//	//std::stringstream ss(str);
//	//unsigned int ti;
//	//ss >> ti;
//	//std::cout << ti << std::endl;
//
//	Map mp;
//	mp.load_map("./Resource/map/tiles2.txt");
//	mp.allocate();
//	
//	return 0;
//}