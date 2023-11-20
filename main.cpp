#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

class Publisher;
class Book;
class Stock;
class Shop;
class Sale;

class Publisher
{
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Book>> book;

	template<class Action>
	void persist(Action& a) 
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, book, Wt::Dbo::ManyToOne, "publisher");
	}
};

class Book
{
public:
	std::string title = "";
	Wt::Dbo::ptr<Publisher> publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stock;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, publisher, "publisher");
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "book");
	}
};

class Stock
{
public:
	int count = 0;
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sale;

	template <class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, shop, "shop");
		Wt::Dbo::belongsTo(a, book, "book");
		Wt::Dbo::hasMany(a, sale, Wt::Dbo::ManyToOne, "stock");
	}
};

class Shop
{
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stock;

	template <class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "shop");
	}
};

class Sale
{
public:
	double price = 0;
	std::string data_sale = "";
	int count = 0;
	Wt::Dbo::ptr<Stock> stock;

	template <class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, data_sale, "data_sale");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, stock, "stock");
	}

};
	

int main()
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
	try {
		std::string connectionString =
			"host=localhost "
			"port=5432 "
			"dbname=cppORM "
			"user=postgres "
			"password=7845";

		auto postgresConnect = std::make_unique<Wt::Dbo::backend::Postgres>(connectionString);

		Wt::Dbo::Session session;
		session.setConnection(std::move(postgresConnect));

		session.mapClass<Publisher>("publisher");
		session.mapClass<Book>("book");
		session.mapClass<Stock>("stock");
		session.mapClass<Shop>("shop");
		session.mapClass<Sale>("sale");

		try
		{
			session.createTables();
		}
		catch (const std::exception& e)
		{
			std::cout << "Tables alredy exists: " << e.what() << std::endl;
		}

		Wt::Dbo::Transaction tr{ session };

		std::unique_ptr<Publisher> publisher = std::make_unique<Publisher>();
		publisher->name = "Eksmo";
		session.add(std::move(publisher));

		std::unique_ptr<Book> book = std::make_unique<Book>();
		book->title = "Rise of the Horde";
		session.add(std::move(book));

		std::unique_ptr<Stock> stock = std::make_unique<Stock>();
		stock->count = 10;
		session.add(std::move(stock));

		std::unique_ptr<Shop> shop = std::make_unique<Shop>();
		shop->name = "OZON";
		session.add(std::move(shop));

		std::unique_ptr<Sale> sale = std::make_unique<Sale>();
		sale->price = 476.50;
		sale->count = 130;
		sale->data_sale = "2023-11-16";
		session.add(std::move(sale));

		
		std::string result;
		std::cin >> result;

	
			Wt::Dbo::collection<Wt::Dbo::ptr<Shop>> shops = session.find<Shop>();
			for (const Wt::Dbo::ptr<Shop> shop : shops)
			{
				for (const Wt::Dbo::ptr<Stock> stock : shop->stock)
				{
					if (stock->book->publisher->name == "Eksmo");

				}
				std::cout << shop->name << std::endl;
			}
		std::cout << "OK!" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Connection error!" << e.what() << std::endl;
	}

}
