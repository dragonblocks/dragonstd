# Dragontype
Dragontype is a small C library providing the types that [dragonblocks_alpha](https://github.com/dragonblocks/dragonblocks_alpha) uses.
It serves a similar purpose as the C++ standard library would, but C++ is just bloated af.
This library is only capable of what dragonblocks needs, and it does not intend to be general purpose.

Included are Array, Bintree, List and Queue as well as numeric types and (de-)serialisation for them.
To use this in your project, just compile the implementation.c file into your project's binary.

## endian.h
By default, dragontype will use the Linux <endian.h> header. However, since other systems may not have that header,
you can use [this library](https://github.com/mikepb/endian.h) to replaced. Make sure to define DRAGONTYPE_ENDIAN_HEADER
to the include path of this portable endian.h file.
