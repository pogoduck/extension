#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_vector.h"
#include "../objects/php_map.h"
#include "../objects/php_pair.h"
#include "../objects/php_set.h"

#include "../iterators/php_map_iterator.h"
#include "../handlers/php_map_handlers.h"

#include "php_collection_ce.h"
#include "php_map_ce.h"

#define METHOD(name) PHP_METHOD(Map, name)

zend_class_entry *php_ds_map_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            ds_map_allocate(THIS_DS_MAP(), Z_LVAL_P(values));
        } else {
            ds_map_put_all(THIS_DS_MAP(), values);
        }
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_map_allocate(THIS_DS_MAP(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_map_capacity(THIS_DS_MAP()));
}

ARGINFO_ZVAL(putAll, values)
METHOD(putAll)
{
    PARSE_ZVAL(values);
    ds_map_put_all(THIS_DS_MAP(), values);
}

ARGINFO_ZVAL_ZVAL(put, key, value)
METHOD(put)
{
    PARSE_ZVAL_ZVAL(key, value);
    ds_map_put(THIS_DS_MAP(), key, value);
}

ARGINFO_ZVAL_OPTIONAL_ZVAL(get, key, default)
METHOD(get)
{
    PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    RETURN_ZVAL_COPY(ds_map_get(THIS_DS_MAP(), key, def));
}

ARGINFO_DS_RETURN_DS(intersect, map, Map, Map)
METHOD(intersect)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_intersect(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

ARGINFO_ZVAL_OPTIONAL_ZVAL(remove, key, default)
METHOD(remove)
{
    PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    ds_map_remove(THIS_DS_MAP(), key, def, return_value);
}

ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(hasKey, keys)
METHOD(hasKey)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_map_has_keys(THIS_DS_MAP(), argc, argv));
}

ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(hasValue, values)
METHOD(hasValue)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_map_has_values(THIS_DS_MAP(), argc, argv));
}

ARGINFO_DS_RETURN_DS(diff, map, Map, Map)
METHOD(diff)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_diff(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    ds_map_clear(THIS_DS_MAP());
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(sort, comparator, Map)
METHOD(sort)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        RETURN_DS_MAP(ds_map_sorted_by_value_callback(THIS_DS_MAP()));
    } else {
        RETURN_DS_MAP(ds_map_sorted_by_value(THIS_DS_MAP()));
    }
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(ksort, comparator, Map)
METHOD(ksort)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        RETURN_DS_MAP(ds_map_sorted_by_key_callback(THIS_DS_MAP()));
    } else {
        RETURN_DS_MAP(ds_map_sorted_by_key(THIS_DS_MAP()));
    }
}

ARGINFO_NONE_RETURN_DS(keys, Set)
METHOD(keys)
{
    PARSE_NONE;
    RETURN_DS_SET(ds_set_ex(ds_htable_clone(THIS_DS_MAP()->table)));
}

ARGINFO_NONE_RETURN_DS(last, Pair)
METHOD(last)
{
    PARSE_NONE;
    RETURN_DS_PAIR(ds_map_last(THIS_DS_MAP()));
}

ARGINFO_ZVAL_RETURN_DS(merge, values, Map)
METHOD(merge)
{
    PARSE_ZVAL(values);
    RETURN_DS_MAP(ds_map_merge(THIS_DS_MAP(), values));
}

ARGINFO_NONE_RETURN_DS(pairs, Sequence)
METHOD(pairs)
{
    PARSE_NONE;
    RETURN_DS_VECTOR(ds_map_pairs_to_vector(THIS_DS_MAP()));
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    ds_map_to_array(THIS_DS_MAP(), return_value);
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_MAP_SIZE(THIS_DS_MAP()));
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_MAP_IS_EMPTY(THIS_DS_MAP()));
}

ARGINFO_NONE_RETURN_DS(copy, Map)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_map_create_clone(THIS_DS_MAP()));
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_map_to_array(THIS_DS_MAP(), return_value);
}

ARGINFO_CALLABLE_RETURN_DS(filter, callback, Map)
METHOD(filter)
{
    PARSE_CALLABLE();
    RETURN_DS_MAP(ds_map_filter_callback(THIS_DS_MAP(), FCI_ARGS));
}

ARGINFO_NONE_RETURN_DS(first, Pair)
METHOD(first)
{
    PARSE_NONE;
    RETURN_DS_PAIR(ds_map_first(THIS_DS_MAP()));
}

ARGINFO_CALLABLE_OPTIONAL_ZVAL(reduce, callback, initial)
METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_map_reduce(THIS_DS_MAP(), FCI_ARGS, initial, return_value);
}

ARGINFO_NONE_RETURN_DS(reverse, Map)
METHOD(reverse)
{
    PARSE_NONE;
    RETURN_DS_MAP(ds_map_reversed(THIS_DS_MAP()));
}

ARGINFO_LONG_RETURN_DS(skip, position, Pair)
METHOD(skip)
{
    PARSE_LONG(position);
    RETURN_DS_PAIR(ds_map_skip(THIS_DS_MAP(), position));
}

ARGINFO_CALLABLE_RETURN_DS(map, callback, Map)
METHOD(map)
{
    PARSE_CALLABLE();
    RETURN_DS_MAP(ds_map_map(THIS_DS_MAP(), FCI_ARGS));
}

ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(slice, index, length, Map)
METHOD(slice)
{
    ds_map_t *map = THIS_DS_MAP();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        RETURN_DS_MAP(ds_map_slice(map, index, length));
    } else {
        PARSE_LONG(index);
        RETURN_DS_MAP(ds_map_slice(map, index, DS_MAP_SIZE(map)));
    }
}

ARGINFO_NONE_RETURN_DS(values, Sequence)
METHOD(values)
{
    PARSE_NONE;
    RETURN_DS_VECTOR(ds_map_values_to_vector(THIS_DS_MAP()));
}

ARGINFO_DS_RETURN_DS(xor, map, Map, Map)
METHOD(xor)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_xor(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

void php_ds_register_map()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(Map, __construct)
        COLLECTION_ME(Map, allocate)
        COLLECTION_ME(Map, capacity)
        COLLECTION_ME(Map, hasKey)
        COLLECTION_ME(Map, hasValue)
        COLLECTION_ME(Map, diff)
        COLLECTION_ME(Map, filter)
        COLLECTION_ME(Map, first)
        COLLECTION_ME(Map, get)
        COLLECTION_ME(Map, intersect)
        COLLECTION_ME(Map, keys)
        COLLECTION_ME(Map, ksort)
        COLLECTION_ME(Map, last)
        COLLECTION_ME(Map, map)
        COLLECTION_ME(Map, merge)
        COLLECTION_ME(Map, pairs)
        COLLECTION_ME(Map, put)
        COLLECTION_ME(Map, putAll)
        COLLECTION_ME(Map, reduce)
        COLLECTION_ME(Map, remove)
        COLLECTION_ME(Map, reverse)
        COLLECTION_ME(Map, skip)
        COLLECTION_ME(Map, slice)
        COLLECTION_ME(Map, sort)
        COLLECTION_ME(Map, values)
        COLLECTION_ME(Map, xor)

        COLLECTION_ME_LIST(Map)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Map), methods);

    php_ds_map_ce = zend_register_internal_class(&ce);
    php_ds_map_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_map_ce->create_object  = php_ds_map_create_object;
    php_ds_map_ce->get_iterator   = php_ds_map_get_iterator;
    php_ds_map_ce->serialize      = php_ds_map_serialize;
    php_ds_map_ce->unserialize    = php_ds_map_unserialize;

    zend_declare_class_constant_long(
        php_ds_map_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_HTABLE_MIN_CAPACITY
    );

    zend_class_implements(php_ds_map_ce, 1, collection_ce);
    php_ds_register_map_handlers();
}