
#include <iostream>

class BaseClass{
public:
        virtual ~BaseClass() = default;

	virtual void Hello()
	{
		std::cout << "hello" << std::endl;
	}

	virtual void Name()
	{
		std::cout << "Base class" << std::endl;
	}

	virtual void Order()
	{
		std::cout << "0" << std::endl;
	}
};

class DerivedClass : public BaseClass{
public:
	virtual ~DerivedClass() = default;

	virtual void Name() override
	{
		std::cout << "Derived class" << std::endl;
	}

	virtual void Order() override
	{
		std::cout << "1" << std::endl;
	}
};