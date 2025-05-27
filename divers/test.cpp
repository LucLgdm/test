#include <iostream>

using namespace std;

class base {
  public:
    base();
    virtual ~base();  
};

base::base(){
	cout << "Constructing base\n";
}

base::~base(){
	cout << "Destructing base\n";
}

class derived: virtual public base {
  public:
    derived();
    ~derived();
};

class derived2: virtual public base {
  public:
    derived2()
		{cout << "Constructing 2\n";}
    ~derived2()
		{cout << "Destructing 2\n";}
};

class bloub: public derived2 , public derived {
  public:
    bloub()
		{cout << "Constructing 3\n";}
    ~bloub()
		{cout << "Destructing 3\n";}
};


derived::derived()
{
	cout << "Constructing derived\n";
}

derived::~derived()
{
	cout << "Destructing derived\n";
}

int main()
{
  derived *d = new bloub();
  delete d;

  return 0;
}
