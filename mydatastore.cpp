#include "mydatastore.h"
#include <set>
#include <vector>
#include <string>

using namespace std;

MyDataStore::MyDataStore() {
}

MyDataStore:: ~MyDataStore(){
  // deletes each pointer to a product
  std::set<Product*>::iterator it1;
	for (it1 = productList.begin(); it1 != productList.end(); it1++) {
		delete *it1; 
	}
  // deletes each pointer to user
	 std::map<std::string, User*>::iterator it2;
  for (it2 = userMap.begin(); it2 != userMap.end(); it2++){
    delete it2->second;
  }

}

void MyDataStore::addProduct(Product* product) {

  // add product to the set
	productList.insert(product);
	// find the keywords for the product
	std::set<std::string> productKeywords = product->keywords();
	// iterate through the set of keywords
	for (std::set<std::string>::iterator it = productKeywords.begin(); it != productKeywords.end(); ++it) {
		// convert the keyword to lower case
		std::string keyword = convToLower(*it);
		// check if the keyword already exists in the product map
		if (productMap.find(keyword) == productMap.end()) {
			// if the keyword does not exist, create a new set of products and add the current product to it
			std::set<Product*> new_productSet;
			new_productSet.insert(product);
			productMap.insert(std::make_pair(keyword, new_productSet));
		}
		else {
			// if the keyword already exists, add the current product to the existing set of products
			productMap.find(keyword)->second.insert(product);
		}
	}

}

void MyDataStore::addUser(User* user){
	userList.insert(user);
  // get the name of user 
	std::string name = user->getName();
  // insert the user
	userMap.insert({name, user});
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
	
    std::set<Product*> temp_results;
    std::vector<Product*> results;

    // Check if it is empty
    if (terms.empty()) {
        return results;
    }

    if (type == 1) { // union 
        for (unsigned int i = 0; i < terms.size(); i++) {
            std::set<Product*> products = productMap[terms[i]];
            temp_results = setUnion(temp_results, products);
        }
    } 
    else if (type == 0) { // intersection
        temp_results = productMap[terms[0]]; // set the first set 
        for (unsigned int i = 1; i < terms.size(); i++) {
            std::set<Product*> products = productMap[terms[i]];
            temp_results = setIntersection(temp_results, products);
        }
    }
    //push the final result in the set to the vector 
    for (std::set<Product*>::iterator it = temp_results.begin(); it != temp_results.end(); ++it) {
        results.push_back(*it);
    }
    return results;
}



void MyDataStore::dump(std::ostream& ofile){
	ofile << "<products>" << endl;
	for(std:: set<Product*> :: iterator it1 = productList.begin(); it1!= productList.end(); ++it1){
		(*it1)->dump(ofile);
	}
	ofile << "</products>" << endl;
	ofile << "<users>" << endl;
	std:: set<User*> :: iterator it1;
	for(	std:: set<User*> :: iterator it2 = userList.begin(); it2!= userList.end(); ++it2){
		(*it2)->dump(ofile);
	}
	ofile << "</users>" << endl;

}


std::map<std::string, User*> MyDataStore::findUser(){
	return userMap;
}

