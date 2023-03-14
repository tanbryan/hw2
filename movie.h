#ifndef movie_h
#define movie_h

#include "product.h"
#include <string>
#include <iostream>


class Movie : public Product{
	public:
		std::string ngenre;
		std::string nrating;

		Movie(const std::string category, const std::string name, double price, int qty, std::string genre, std::string rating);
		~Movie();
		std::set<std::string> keywords() const;
		std::string displayString() const;
		void dump(std::ostream& os) const;
		
		

};

#endif
