# NArg

Named argument utility. Sort of works with C++14 or later.

# How To Use

### Delcaring a class with a `NArg` named argument constructor (header only)
```c++
struct MyClass
{
  NARG_DECL_CONSTRUCTOR(MyClass,
                        arg_pack,
                        NARG_SPEC("argument_1", int),
                        NARG_SPEC("argument_2", std::string)) :
    argument1_var{NARG_GET(arg_pack, "argument_1")},
    argument2_var{NARG_GET(arg_pack, "argument_2")}
  {}

  int argument1_var;
  std::string argument2_var;
};
```

### Separate declaration and implementation:
```c++
struct MyClass
{
  NARG_DECL_CONSTRUCTOR(MyClass,
                        arg_pack,
                        NARG_SPEC("argument_1", int),
                        NARG_SPEC("argument_2", std::string));

  int argument1_var;
  std::string argument2_var;
};
```

```c++
MyClass::NARG_IMPL_CONSTRUCTOR(MyClass, arg_pack) :
  argument1_var{NARG_GET(arg_pack, "argument_1")},
  argument2_var{NARG_GET(arg_pack, "argument_2")}
{}
```

### Using `MyClass`

#### You can mix positional and named arguments:
```c++
MyClass obj{narg::use_nargs,
            2,  // position 0
            NARG_FILL("argument_2", "the_string")};
```

#### You can declare with named arguments in any order:
```c++
MyClass obj{narg::use_nargs,
            NARG_FILL("argument_2", "the_string")
            NARG_FILL("argument_1", 0)};
```
