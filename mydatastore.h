#ifndef mydatastore_h
#define mydatastore_h

#include <set>
#include <map>
#include <vector>
#include <string>
#include "product.h"
#include "datastore.h"
#include "user.h"
#include "util.h"
#include "book.h"
#include "movie.h"
#include "clothing.h"


class MyDataStore : public DataStore{
public:
    MyDataStore();
    ~MyDataStore();

    std::set<User*> userList;
		std::set<Product*> productList;
    std::map<std::string, std::set<Product*> > productMap;
    std::map<std::string, User*> userMap;
    std::map<std::string, std::vector<Product*> > cartMap;

    void addProduct(Product* product); 
    void addUser(User* user);
    std::vector<Product*> search(std::vector<std::string>& terms, int type);
    void dump(std::ostream& ofile);


		std::map<std::string, User*> findUser();


};

#endif
