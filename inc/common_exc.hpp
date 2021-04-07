
#ifndef DEFINE_EXCEPTION_H
#define DEFINE_EXCEPTION_H

#include <exception>
#include <stdexcept>

// ExceptionHelper augments the standard exception class by
// allowing a const char * parameter in the constructor.
class ExceptionHelper : public std::runtime_error
{
public:
  ExceptionHelper(const char *msg)
    : std::runtime_error(msg)
    { }
  virtual const char * what() const throw() { return msg_; }
private:
  const char *msg_;
};

#define DEFINE_EXCEPTION(ClassName,Message) \
    class ClassName : public ExceptionHelper \
    { \
    public: \
        ClassName(const char *msg = Message) \
            : ExceptionHelper(msg) \
        { } \
    };

#endif //DEFINE_EXCEPTION_H