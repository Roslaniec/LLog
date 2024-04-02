
#if 0
# include <boost/intrusive_ptr.hpp>
using namespace boost;
#else
#  include <llog/intrusive_ptr.hpp>
using namespace linko;
#endif

struct A { unsigned ref = 0; };
struct AA : public A { };
struct B { unsigned ref = 0; };

typedef intrusive_ptr<A> PtrA;
typedef intrusive_ptr<AA> PtrAA;
typedef intrusive_ptr<B> PtrB;

void intrusive_ptr_add_ref(A *ptr) { ++ptr->ref; }
void intrusive_ptr_release(A *ptr) { if (!--ptr->ref) delete ptr; }

void intrusive_ptr_add_ref(B *ptr) { ++ptr->ref; }
void intrusive_ptr_release(B *ptr) { if (!--ptr->ref) delete ptr; }

void intrusive_ptr_add_ref(AA *ptr) { ++ptr->ref; }
void intrusive_ptr_release(AA *ptr) { if (!--ptr->ref) delete ptr; }


int
main()
{
    PtrA a(new A);
    PtrB b(new B);
    PtrAA aa(new AA);
    PtrA a2(aa);
    PtrA a3(std::move(aa));
    // PtrA a4(std::move(b));
    // PtrA a3(b);
    // aa.swap(b);
}
