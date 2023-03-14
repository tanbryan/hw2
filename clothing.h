#ifndef clothing_h
#define clothing_h

#include "product.h"
#include <string>
#include <iostream>


class Clothing : public Product{
	public:
    std::string nsize;
    std::string nbrand;
		
		Clothing(const std::string category, const std::string name, double price, int qty, std::string size, std::string brand);
    ~Clothing();
		std::set<std::string> keywords() const;
		std::string displayString() const;
		void dump(std::ostream& os) const;
		
};

#endif