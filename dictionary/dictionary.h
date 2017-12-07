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


#ifdef __cplusplus
}
#endif

#endif // DICTIONARY_H
