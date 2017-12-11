#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "../list/list.h"
#include "../string/sstring.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _spdIoTDictionary {
    spdIoTListHead list;

    spdIoTString *key;
    spdIoTString *value;
}spdIoTDictionaryElement, spdIoTDictionary;

spdIoTDictionaryElement *spdIoT_dictionary_element_new(void);
void spdIoT_dictionary_element_delete(spdIoTDictionaryElement *dirElem);
#define spdIoT_dictionary_element_setkey(dirElem, name) spdIoT_string_setvalue(dirElem->key, name);
#define spdIoT_dictionary_element_getkey(dirElem) spdIoT_string_getvalue(dirElem->key);
#define spdIoT_dictionary_element_setvalue(dirElem, val) spdIoT_string_setvalue(dirElem->value, val);
#define spdIoT_dictionary_element_getvalue(dirElem) spdIoT_string_getvalue(dirElem->value);

spdIoTDictionary *spdIoT_dictionary_new();
void spdIoT_dictionary_delete(spdIoTDictionary *dir);
void spdIoT_dictionary_clear(spdIoTDictionary *dir);
int spdIoT_dictionary_size(spdIoTDictionary *dir);
void spdIoT_dictionary_add(spdIoTDictionary *dir, spdIoTDictionaryElement *dirElem);
void spdIoT_dictionary_remove(spdIoTDictionaryElement *dirElem);
spdIoTDictionaryElement *spdIoT_dictionary_get(spdIoTDictionary *dir, const char *key);
void spdIoT_dictionary_setvalue(spdIoTDictionary *dir, const char *key, const char *value);
const char *spdIoT_dictionary_getvalue(spdIoTDictionary *dir, const char *key);
#ifdef __cplusplus
}
#endif

#endif // DICTIONARY_H
