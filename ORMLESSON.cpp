#include <Wt/Dbo/backend/Postgres.h>
#include <wt/dbo/dbo.h>

#include <iostream>
#include <memory>
using namespace std;
class book;
class publisher;
class stock;
class shop;
class sale;

class publisher {
 public:
  string name;
  Wt::Dbo::collection<Wt::Dbo::ptr<book>> books;
  template <typename Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");
    Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "publisher");
  }
};

class book {
 public:
  string title;
  Wt::Dbo::ptr<publisher> publisher;
  Wt::Dbo::collection<Wt::Dbo::ptr<stock>> stock1;
  template <typename Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, title, "title");
    Wt::Dbo::belongsTo(a, publisher, "publisher");
    Wt::Dbo::hasMany(a, stock1, Wt::Dbo::ManyToOne, "book");
  }
};
class stock {
 public:
  int count;
  Wt::Dbo::ptr<book> books;
  Wt::Dbo::ptr<shop> shops;
  Wt::Dbo::collection<Wt::Dbo::ptr<sale>> sales;
  template <typename Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, count, "count");
    Wt::Dbo::belongsTo(a, shops, "shop");
    Wt::Dbo::belongsTo(a, books, "book");
    Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "stock");
  }
};
class shop {
 public:
  string name;
  template <typename Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, name, "name");
  }
};

class sale {
 public:
  string price;
  string data_sale;
  int count;
  Wt::Dbo::ptr<stock> stock;
  template <typename Action>
  void persist(Action& a) {
    Wt::Dbo::field(a, price, "price");
    Wt::Dbo::field(a, data_sale, "data_sale");
    Wt::Dbo::belongsTo(a, stock, "stock");
    Wt::Dbo::field(a, count, "count");
  }
};

int main() {
  try {
    string ConnectionString =
        "host=localhost "
        "port=5432 "
        "dbname=postgres "
        "user=postgres "
        "password=123";

    unique_ptr<Wt::Dbo::backend::Postgres> connection =
        make_unique<Wt::Dbo::backend::Postgres>(ConnectionString);

    Wt::Dbo::Session session;

    session.setConnection(move(connection));

    session.mapClass<publisher>("publisher");
    session.mapClass<book>("book");
    session.mapClass<stock>("stock");
    session.mapClass<shop>("shop");
    session.mapClass<sale>("sale");
    Wt::Dbo::Transaction tran{session};
   session.createTables();
   unique_ptr<publisher> p_user1{new publisher{"Nikita"}};
   unique_ptr<publisher> p_user2{new publisher{"Dima"}};
   auto user1 =  session.add(move(p_user1));
   auto user2 =  session.add(move(p_user2));
   unique_ptr<book> p_book1{new book{"News"}};
   unique_ptr<book> p_book2{new book{"Last days"}};
   unique_ptr<book> p_book3{new book{"Our_days"}};
   unique_ptr<book> p_book4{new book{"Future"}};
   auto book1 = session.add(move(p_book1));
   auto book2 = session.add(move(p_book2));
   auto book3 = session.add(move(p_book3));
   auto book4 = session.add(move(p_book4));
   book1.modify()->publisher = user1;
   book2.modify()->publisher = user1;
   book3.modify()->publisher = user2;
   book4.modify()->publisher = user2;

   unique_ptr<shop> p_shop1{new shop{"shop_one"}};
   unique_ptr<shop> p_shop2{new shop{"shop_two"}};
   unique_ptr<shop> p_shop3{new shop{"shop_three"}};
   unique_ptr<shop> p_shop4{new shop{"shop_four"}};
   auto shop1 = session.add(move(p_shop1));
   auto shop2 = session.add(move(p_shop2));
   auto shop3 = session.add(move(p_shop3));
   auto shop4 = session.add(move(p_shop4));

   unique_ptr<stock> p_stock1{new stock{30}};
   unique_ptr<stock> p_stock2{new stock{300}};
   unique_ptr<stock> p_stock3{new stock{3000}};
   unique_ptr<stock> p_stock4{new stock{3089}};
   auto stock1 = session.add(move(p_stock1));
   auto stock2 = session.add(move(p_stock2));
   auto stock3 = session.add(move(p_stock3));
   auto stock4 = session.add(move(p_stock4));
   stock1.modify()->books = book1;
   stock1.modify()->shops = shop1;
   stock2.modify()->books = book2;
   stock2.modify()->shops = shop2;
   stock3.modify()->books = book3;
   stock3.modify()->shops = shop3;
   stock4.modify()->books = book4;
   stock4.modify()->shops = shop4;
   
   unique_ptr<sale> p_sale1{new sale{"4500", "11-10-2024",30}};
   unique_ptr<sale> p_sale2{new sale{"45000", "10-10-2024", 300}};
   unique_ptr<sale> p_sale3{new sale{"450000", "09-10-2024", 3000}};
   unique_ptr<sale> p_sale4{new sale{"463350", "08-10-2024", 3089}};

   auto sale1 = session.add(move(p_sale1));
   auto sale2 = session.add(move(p_sale2));
   auto sale3 = session.add(move(p_sale3));
   auto sale4 = session.add(move(p_sale4));
   sale1.modify()->stock = stock1;
   sale2.modify()->stock = stock2;
   sale3.modify()->stock = stock3;
   sale4.modify()->stock = stock4;



   tran.commit();
  } catch (exception& e) {
    cout << e.what() << endl;
  }
}
