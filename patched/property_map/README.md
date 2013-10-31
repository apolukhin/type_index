This patch requires additional care. 

It works and tested through the Boost.Graph tests, but in some places `typeid(variable)` were replaced with `typeid(Type)`.
