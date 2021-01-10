/* Itanium C++ ABI support */
#pragma once

namespace __cxxabiv1 {

/* Pure virtual function default handler not defined. Usually defined in
 * default libs, provide own version so linker is happy */
extern "C" void __cxa_pure_virtual() { }

}

/* > The header file will also declare a namespace alias abi for __cxxabiv1 */
namespace abi = __cxxabiv1;
