#ifndef COMMON_H_
#define COMMON_H_

#define NOEXCEPTIONS throw()

#define EXCEPTIONS_BED_ALLOC throw(std::bad_alloc)

namespace experis {

class Uncopyable {
public:
    Uncopyable(){}
private:
    Uncopyable(const Uncopyable& a_uncopyableToCopy);
    const Uncopyable operator=(const Uncopyable& a_uncopyableToCopy);
};

} //experis
#endif // COMMON_H_