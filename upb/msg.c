
#include "upb/msg.h"

#include "upb/table.int.h"

#include "upb/port_def.inc"

/** upb_msg *******************************************************************/

static const size_t overhead = sizeof(upb_msg_internaldata);

static const upb_msg_internal *upb_msg_getinternal_const(const upb_msg *msg) {
  ptrdiff_t size = sizeof(upb_msg_internal);
  return (upb_msg_internal*)((char*)msg - size);
}

upb_msg *_upb_msg_new(const upb_msglayout *l, upb_arena *a) {
  return _upb_msg_new_inl(l, a);
}

void _upb_msg_clear(upb_msg *msg, const upb_msglayout *l) {
  void *mem = UPB_PTR_AT(msg, -sizeof(upb_msg_internal), char);
  memset(mem, 0, upb_msg_sizeof(l));
}

static bool realloc_internal(upb_msg *msg, size_t need, upb_arena *arena) {
  upb_msg_internal *in = upb_msg_getinternal(msg);
  if (!in->internal) {
    /* No internal data, allocate from scratch. */
    size_t size = UPB_MAX(128, _upb_lg2ceilsize(need + overhead));
    upb_msg_internaldata *internal = upb_arena_malloc(arena, size);
    if (!internal) return false;
    internal->size = size;
    internal->unknown_end = overhead;
    internal->ext_begin = size;
    in->internal = internal;
  } else if (in->internal->ext_begin - in->internal->unknown_end < need) {
    /* Internal data is too small, reallocate. */
    size_t new_size = _upb_lg2ceilsize(in->internal->size + need);
    size_t ext_bytes = in->internal->size - in->internal->ext_begin;
    size_t new_ext_begin = new_size - ext_bytes;
    upb_msg_internaldata *internal =
        upb_arena_realloc(arena, in->internal, in->internal->size, new_size);
    if (!internal) return false;
    if (ext_bytes) {
      /* Need to move extension data to the end. */
      char *ptr = (char*)internal;
      memmove(ptr + new_ext_begin, ptr + internal->ext_begin, ext_bytes);
    }
    internal->ext_begin = new_ext_begin;
    internal->size = new_size;
    in->internal = internal;
  }
  UPB_ASSERT(in->internal->ext_begin - in->internal->unknown_end >= need);
  return true;
}

bool _upb_msg_addunknown(upb_msg *msg, const char *data, size_t len,
                         upb_arena *arena) {
  realloc_internal(msg, len, arena);
  upb_msg_internal *in = upb_msg_getinternal(msg);
  memcpy(UPB_PTR_AT(in->internal, in->internal->unknown_end, char), data, len);
  in->internal->unknown_end += len;
  return true;
}

void _upb_msg_discardunknown_shallow(upb_msg *msg) {
  upb_msg_internal *in = upb_msg_getinternal(msg);
  if (in->internal) {
    in->internal->unknown_end = overhead;
  }
}

const char *upb_msg_getunknown(const upb_msg *msg, size_t *len) {
  const upb_msg_internal *in = upb_msg_getinternal_const(msg);
  if (in->internal) {
    *len = in->internal->unknown_end - overhead;
    return (char*)(in->internal + 1);
  } else {
    *len = 0;
    return NULL;
  }
}

const upb_msg_ext *_upb_msg_getexts(const upb_msg *msg, size_t *count) {
  const upb_msg_internal *in = upb_msg_getinternal_const(msg);
  if (in->internal) {
    *count =
        (in->internal->size - in->internal->ext_begin) / sizeof(upb_msg_ext);
    return UPB_PTR_AT(in->internal, in->internal->ext_begin, void);
  } else {
    *count = 0;
    return NULL;
  }
}

const upb_msg_ext *_upb_msg_getext(const upb_msg *msg, uint32_t fieldnum) {
  const upb_msg_internal *in = upb_msg_getinternal_const(msg);
  if (in->internal) {
    size_t n;
    const upb_msg_ext *ext = _upb_msg_getexts(msg, &n);
    for (size_t i = 0; i < n; i++) {
      if (ext[i].fieldnum == fieldnum) {
        return &ext[i];
      }
    }
  }
  return NULL;
}

upb_msg_ext *_upb_msg_getorcreateext(upb_msg *msg, uint32_t fieldnum,
                                     upb_arena *arena) {
  upb_msg_ext *ext = (upb_msg_ext*)_upb_msg_getext(msg, fieldnum);
  if (ext) return ext;
  realloc_internal(msg, sizeof(upb_msg_ext), arena);
  upb_msg_internal *in = upb_msg_getinternal(msg);
  in->internal->ext_begin -= sizeof(upb_msg_ext);
  ext = UPB_PTR_AT(in->internal, in->internal->ext_begin, void);
  memset(ext, 0, sizeof(upb_msg_ext));
  return ext;
}

/** upb_array *****************************************************************/

bool _upb_array_realloc(upb_array *arr, size_t min_size, upb_arena *arena) {
  size_t new_size = UPB_MAX(arr->size, 4);
  int elem_size_lg2 = arr->data & 7;
  size_t old_bytes = arr->size << elem_size_lg2;
  size_t new_bytes;
  void* ptr = _upb_array_ptr(arr);

  /* Log2 ceiling of size. */
  while (new_size < min_size) new_size *= 2;

  new_bytes = new_size << elem_size_lg2;
  ptr = upb_arena_realloc(arena, ptr, old_bytes, new_bytes);

  if (!ptr) {
    return false;
  }

  arr->data = _upb_tag_arrptr(ptr, elem_size_lg2);
  arr->size = new_size;
  return true;
}

static upb_array *getorcreate_array(upb_array **arr_ptr, int elem_size_lg2,
                                    upb_arena *arena) {
  upb_array *arr = *arr_ptr;
  if (!arr) {
    arr = _upb_array_new(arena, 4, elem_size_lg2);
    if (!arr) return NULL;
    *arr_ptr = arr;
  }
  return arr;
}

void *_upb_array_resize_fallback(upb_array **arr_ptr, size_t size,
                                 int elem_size_lg2, upb_arena *arena) {
  upb_array *arr = getorcreate_array(arr_ptr, elem_size_lg2, arena);
  return arr && _upb_array_resize(arr, size, arena) ? _upb_array_ptr(arr)
                                                    : NULL;
}

bool _upb_array_append_fallback(upb_array **arr_ptr, const void *value,
                                int elem_size_lg2, upb_arena *arena) {
  upb_array *arr = getorcreate_array(arr_ptr, elem_size_lg2, arena);
  if (!arr) return false;

  size_t elems = arr->len;

  if (!_upb_array_resize(arr, elems + 1, arena)) {
    return false;
  }

  char *data = _upb_array_ptr(arr);
  memcpy(data + (elems << elem_size_lg2), value, 1 << elem_size_lg2);
  return true;
}

/** upb_map *******************************************************************/

upb_map *_upb_map_new(upb_arena *a, size_t key_size, size_t value_size) {
  upb_map *map = upb_arena_malloc(a, sizeof(upb_map));

  if (!map) {
    return NULL;
  }

  upb_strtable_init(&map->table, 4, a);
  map->key_size = key_size;
  map->val_size = value_size;

  return map;
}

static void _upb_mapsorter_getkeys(const void *_a, const void *_b, void *a_key,
                                   void *b_key, size_t size) {
  const upb_tabent *const*a = _a;
  const upb_tabent *const*b = _b;
  upb_strview a_tabkey = upb_tabstrview((*a)->key);
  upb_strview b_tabkey = upb_tabstrview((*b)->key);
  _upb_map_fromkey(a_tabkey, a_key, size);
  _upb_map_fromkey(b_tabkey, b_key, size);
}

static int _upb_mapsorter_cmpi64(const void *_a, const void *_b) {
  int64_t a, b;
  _upb_mapsorter_getkeys(_a, _b, &a, &b, 8);
  return a - b;
}

static int _upb_mapsorter_cmpu64(const void *_a, const void *_b) {
  uint64_t a, b;
  _upb_mapsorter_getkeys(_a, _b, &a, &b, 8);
  return a - b;
}

static int _upb_mapsorter_cmpi32(const void *_a, const void *_b) {
  int32_t a, b;
  _upb_mapsorter_getkeys(_a, _b, &a, &b, 4);
  return a - b;
}

static int _upb_mapsorter_cmpu32(const void *_a, const void *_b) {
  uint32_t a, b;
  _upb_mapsorter_getkeys(_a, _b, &a, &b, 4);
  return a - b;
}

static int _upb_mapsorter_cmpbool(const void *_a, const void *_b) {
  bool a, b;
  _upb_mapsorter_getkeys(_a, _b, &a, &b, 1);
  return a - b;
}

static int _upb_mapsorter_cmpstr(const void *_a, const void *_b) {
  upb_strview a, b;
  _upb_mapsorter_getkeys(_a, _b, &a, &b, UPB_MAPTYPE_STRING);
  size_t common_size = UPB_MIN(a.size, b.size);
  int cmp = memcmp(a.data, b.data, common_size);
  if (cmp) return cmp;
  return a.size - b.size;
}

bool _upb_mapsorter_pushmap(_upb_mapsorter *s, upb_descriptortype_t key_type,
                            const upb_map *map, _upb_sortedmap *sorted) {
  int map_size = _upb_map_size(map);
  sorted->start = s->size;
  sorted->pos = sorted->start;
  sorted->end = sorted->start + map_size;

  /* Grow s->entries if necessary. */
  if (sorted->end > s->cap) {
    s->cap = _upb_lg2ceilsize(sorted->end);
    s->entries = realloc(s->entries, s->cap * sizeof(*s->entries));
    if (!s->entries) return false;
  }

  s->size = sorted->end;

  /* Copy non-empty entries from the table to s->entries. */
  upb_tabent const**dst = &s->entries[sorted->start];
  const upb_tabent *src = map->table.t.entries;
  const upb_tabent *end = src + upb_table_size(&map->table.t);
  for (; src < end; src++) {
    if (!upb_tabent_isempty(src)) {
      *dst = src;
      dst++;
    }
  }
  UPB_ASSERT(dst == &s->entries[sorted->end]);

  /* Sort entries according to the key type. */

  int (*compar)(const void *, const void *);

  switch (key_type) {
    case UPB_DESCRIPTOR_TYPE_INT64:
    case UPB_DESCRIPTOR_TYPE_SFIXED64:
    case UPB_DESCRIPTOR_TYPE_SINT64:
      compar = _upb_mapsorter_cmpi64;
      break;
    case UPB_DESCRIPTOR_TYPE_UINT64:
    case UPB_DESCRIPTOR_TYPE_FIXED64:
      compar = _upb_mapsorter_cmpu64;
      break;
    case UPB_DESCRIPTOR_TYPE_INT32:
    case UPB_DESCRIPTOR_TYPE_SINT32:
    case UPB_DESCRIPTOR_TYPE_SFIXED32:
    case UPB_DESCRIPTOR_TYPE_ENUM:
      compar = _upb_mapsorter_cmpi32;
      break;
    case UPB_DESCRIPTOR_TYPE_UINT32:
    case UPB_DESCRIPTOR_TYPE_FIXED32:
      compar = _upb_mapsorter_cmpu32;
      break;
    case UPB_DESCRIPTOR_TYPE_BOOL:
      compar = _upb_mapsorter_cmpbool;
      break;
    case UPB_DESCRIPTOR_TYPE_STRING:
      compar = _upb_mapsorter_cmpstr;
      break;
    default:
      UPB_UNREACHABLE();
  }

  qsort(&s->entries[sorted->start], map_size, sizeof(*s->entries), compar);
  return true;
}
