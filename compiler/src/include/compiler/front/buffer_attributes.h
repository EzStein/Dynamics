#ifndef BUFFER_ATTRIBUTES_GUARD
#define BUFFER_ATTRIBUTES_GUARD
struct buffer_attributes {
public:
  buffer_attributes() : size(0), available(true) { }
  buffer_attributes(size_t _size, bool _available) : size(_size), available(_available) { }
  size_t size;

  /*True if this buffer may be overrwritten with a new function*/
  bool available;
};

#endif
