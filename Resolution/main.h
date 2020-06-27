#pragma once

/*
* Resolution here means mapping a symbol's name to its address.
* A good implementation requires more than a simple hashmap because
* of partial scoping and several ways in which ambiguity appears.
*
* Partial scoping means ExampleFunction should resolve to
* "Client.dll:ExampleClass::ExampleFunction" if there are not other
* symbols with the final name ExampleFunction.
*
* Ambiguities arise when, for example, there are two methods named
* ExampleFunction. It seems ok to have it default to the first if
* "ExampleFunction" is resolved here, but what about more practical examples,
* such as there being overloaded constructors? A::A() and A::A(A&) are
* currently ambiguous, and so only one can be accessed.
*
* Matching rules:
* - Final name MUST match
* - Extra path components, if present, MUST be valid. For example, Cow::Meow cannot resolve to Cat::Meow.
* - File name should be treated specially. For "A.dll::B::C", rearrange it to "dll:A::B::C." This is so
* "A::B::C" or "A.dll::B::C" can both resolve, but if there's ambiguity (such as there being A.dll and A.exe with the symbol)
* then the user can specify enough detail to resolve correctly.
*
* Parameter matching rules:
* - Parameter matching is to take care of the case that multiple symbols are both valid, and have the same "bad score."
* - However, INVALID parameter information is still not ok. For example, A::B() does not match A::B(int).
* - If no parameter information is specified, i.e. "A::B", then it is not used to match.
* - Parameter names should be completely ignored, to faciliate usage by copy and pasting from users' code.
* (meaning A::B(int x) would match A::B(int))
* -
*
* From the list of symbols still valid so far, generate a "bad score" to be minimized over the set.
* Let "bad score" be the number of extra path components missing. For example, "C::D" matching to "A::B::C::D" would have
* a "bad score" of 2, because A and B are missing. This generation is generalized, so "bad score" refers to this exact procedure.
*/