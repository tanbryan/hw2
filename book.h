#ifndef book_h
#define book_h

#include "product.h"
#include <string>
#include <iostream>


class Book : public Product{
	public:
    std::string nauthor;
    std::string nISBN;
		
		Book(const std::string category, const std::string name, double price, int qty, std::string author, std::string ISBN);
    ~Book();
		std::set<std::string> keywords() const;
		std::string displayString() const;
		void dump(std::ostream& os) const;
		
};


#endif 