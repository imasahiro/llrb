#include <stdexcept>
#include "ruby.h"
#include "llruby/iseq.h"

namespace llruby {

static int object_set_hash_i(VALUE key, VALUE value, VALUE obj_value) {
  Object *obj_ptr = (Object *)obj_value;
  VALUE key_str = rb_sym_to_s(key);
  obj_ptr->hash[RSTRING_PTR(key_str)] = Object(value);
  return 0;
}

void Object::SetTypeSpecificField() {
  switch (TYPE(raw)) {
    case T_ARRAY:
      for (int i = 0; i < RARRAY_LEN(raw); i++) {
        Object object(RARRAY_AREF(raw, i));
        array.push_back(object);
      }
      break;
    case T_SYMBOL:
      {
        VALUE str = rb_sym_to_s(raw);
        symbol = RSTRING_PTR(str);
      }
      break;
    case T_STRING:
      string = RSTRING_PTR(raw);
      break;
    case T_FIXNUM:
      integer = FIX2INT(raw);
      break;
    case T_HASH:
      rb_hash_foreach(raw, (int (*)(...))object_set_hash_i, (VALUE)this);
      break;
    case T_NIL:
      break; // do nothing
    case T_TRUE:
    case T_FALSE:
      boolean = RTEST(raw);
      break;
    default:
      throw std::runtime_error(std::string("unexpected type is given!: ") + rb_obj_classname(raw));
  }
}

}; // namespace llruby