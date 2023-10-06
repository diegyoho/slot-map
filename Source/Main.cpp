#include "SlotMap.h"

#include <iostream>
#include <memory>

int main()
{
	std::cout << "Simple Slot Map implementation." << std::endl;
	
	SlotMap<std::shared_ptr<int>> sm;
	
	auto id = sm.Insert(std::make_shared<int>());
	id = sm.Insert(std::make_shared<int>());
	id = sm.Insert(std::make_shared<int>());
	id = sm.Insert(std::make_shared<int>());
	id = sm.Insert(std::make_shared<int>());
	
	std::cout << "Last ID inserted into the Slot Map: "<< id << std::endl;

	return 0;
}
