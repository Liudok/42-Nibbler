#pragma once

#include <stdexcept>
#include <dlfcn.h>
#include <string>

template <typename T>
class Handle
{
    public:

        Handle(const char* libName);
        T* get();

        Handle() = delete;
        Handle(Handle const&) = delete;
        Handle& operator=(Handle const&) = delete;
        ~Handle();

    private:

        T* o_ = nullptr;
        void* lib_ = nullptr;

};

template <typename T>
Handle<T>::Handle(const char* libName)
{
    lib_ = dlopen(libName, RTLD_LAZY | RTLD_LOCAL);
    std::string errorMessage("The dynamic library '");
    errorMessage += libName;
    errorMessage += "' is not found";
    if (!lib_) throw std::runtime_error(errorMessage.c_str());
    o_ = reinterpret_cast<T* (*)()>(dlsym(lib_, "create"))();
    if (!o_) std::runtime_error("Failed to create the window");
    if (dlerror()) throw std::runtime_error(dlerror());
}

template <typename T>
Handle<T>::~Handle()
{
    delete o_;
    dlclose(lib_);
}

template <typename T>
T* Handle<T>::get()
{
    return o_;
}
