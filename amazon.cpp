#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include <map>
#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    std::map<string, vector<Product*>> cartMap;

    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            else if (cmd == "ADD") {
                std::map<string, User*> userMap = ds.findUser();
                std::string user;
                unsigned int index;
                ss >> user >> index;
                index--;

                if (index < 0 || index >= hits.size() || userMap.find(user) == userMap.end()) {
                    // if out of bounds or the username is not found
                    cout << "Invalid request" << endl;
                  }
                else {
                    // get the product at the given index
                    Product* hitProduct = hits.at(index);
                    if (cartMap.find(user) == cartMap.end()) {
                        // add a new entry with the product
                        std::vector<Product*> new_cart_items = {hitProduct};
                        cartMap.insert({user, new_cart_items});
                    } else {
                        // add the product to the existing entry
                        cartMap[user].push_back(hitProduct);
                    }
                }
            }


						else if (cmd == "VIEWCART") {
                std::map<string, User*> userMap = ds.findUser();
                string user;
                ss >> user;
                if(cartMap.find(user) == cartMap.end()){
								  cout << "Invalid username" << endl;
							  }
                else{
                  // Find the cart for the user
                  std::vector<Product*> cart;
                  if (cartMap.find(user) != cartMap.end()) {
                    cart = cartMap[user];
                  }

                // Display the contents of the cart
                  int i = 1;
                  for (Product* nproduct : cart) {
                    std::cout << "Item " << i << endl;
                    std::cout << nproduct->displayString() << endl << endl;
                    i++;
                  }
                }
            }
            
						else if(cmd == "BUYCART"){  
                std::map<string, User*> userMap = ds.findUser();
                std::string user;
                ss >> user;
                if(cartMap.find(user) == cartMap.end()){
                    cout << "Invalid username" << endl;
                }
                else{
                    std::vector<Product*> nproduct = cartMap[user];
                    User* nuser = userMap[user];
                    // create a new cart to hold products that can't be purchased
                    std::vector<Product*> ncart; 
                    for(std::vector<Product*>::iterator it = nproduct.begin(); it != nproduct.end();){
                        Product* product = *it;
                        double prodPrice = product->getPrice();
                        if(product->getQty() > 0 && prodPrice <= nuser->getBalance()){
                            it = nproduct.erase(it);
                            product->subtractQty(1);
                            nuser->deductAmount(prodPrice);
                        } 
                        else if (nuser->getBalance() < prodPrice) { 
                          // check if user has insufficient balance
                            std::cout << "Insufficient balance to purchase " << product->getName() << std::endl;
                            // add product to new cart
                            ncart.push_back(product); 
                            // remove product from original cart
                            it = nproduct.erase(it); 
                        } 
                        else {
                            it += 1;
                        }
                    }
                    // update original cart with products that were purchased
                    cartMap[user] = nproduct; 
                    // if there are products in new cart
                    if (!ncart.empty()) { 
                      // add them to original cart
                        cartMap[user].insert(cartMap[user].end(), ncart.begin(), ncart.end()); 
                    }
                }
            }

            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
