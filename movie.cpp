#include "movie.h"
#include "util.h"


Movie::Movie(const std::string category, const std::string name, double price, int qty, std::string genre, std::string rating):
  Product(category, name, price, qty){
	ngenre = genre;
	nrating = rating;
}


std::set<std::string> Movie::keywords() const{
	std::set<std::string> key;
	std::set<std::string> nameSet = parseStringToWords(name_);
  std::set<std::string> genreSet = parseStringToWords(ngenre);

	
	for(std::set<std::string>::iterator it = nameSet.begin(); it != nameSet.end(); it++){
		key.insert(convToLower(*it));
	}

  for(std::set<std::string>::iterator it = genreSet.begin(); it != genreSet.end(); it++){
		key.insert(convToLower(*it));
	}
	
	key.insert(convToLower(nrating));

	return key;

}


std::string Movie::displayString() const{
	std::string result = name_ + "\n" + "Genre: " + ngenre + " Rating: " + nrating + "\n" + std::to_string(price_)+ " " + std::to_string(qty_) + " left." + "\n";
  return result;
}


void Movie::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n"<< price_ << "\n"<< qty_ << "\n"<< ngenre << "\n" << nrating << "\n";
}

Movie::~Movie(){

}