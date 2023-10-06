#include "SlotMap.h"

#include <exception>
#include <iostream>
#include <memory>
#include <string>

int main()
{
	try
	{
		std::cout << "Simple Slot Map implementation." << std::endl;

		SM::SlotMap<std::shared_ptr<int>> sm;
		
		SM::ObjectId id{};
		
		for (size_t i = 0; i < 5; ++i)
		{
			id = sm.Insert(std::make_shared<int>(i));
		}

		std::cout << "Last ID inserted into the Slot Map: " << id << std::endl;
		
		sm.DestroyObject(1);
		sm.DestroyObject(3);
		sm.Insert(std::make_shared<int>(2342));
		sm.Insert(std::make_shared<int>(456));

		for (size_t i = 0; i <= id + 1; ++i)
		{
			if (auto valid = sm.GetObject(i))
			{
				std::shared_ptr<int> value = valid->value;
				std::cout << "Value at ID(" << i << "): " << *value << std::endl;
			}
			else
			{
				throw std::runtime_error("ID(" + std::to_string(i) + ") isn't valid!");
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
