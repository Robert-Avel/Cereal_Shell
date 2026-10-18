#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "ArgList.h"
#include "ArgToken.h"

enum Status {
  SUCESS = 0,
  FAIL,
  INVALID_INPUT,
  ALLOCATION_FAIL,
  OUT_OF_RANGE,
  TURN_AROUND
};

struct ArgList* ArgList_new() {
    struct ArgList* ab = (struct ArgList*) malloc(sizeof(struct ArgList));
    if(ab == NULL) {return NULL;}

    ab->head = NULL;
    ab->size = 0;
    return ab;
}

int ArgList_end(struct ArgList* ab) {
    if(ab == NULL) {return INVALID_INPUT;}

    free(ab->head);
    free(ab);
    return SUCESS;
}

struct _ArgListNode* _ArgListNode_new(const struct ArgToken* at) {
    struct _ArgListNode* alb = malloc(sizeof(struct _ArgListNode));
    if(alb == NULL) {return NULL;}
    alb->next_node = NULL;
    alb->at = ArgToken_new();
    switch (at->type) {
        case POSITIONAL:
            ArgToken_setPosArg(alb->at, at->arg.pa.pos, at->arg.pa.value);
            break;
        case KEY:
            ArgToken_setKeyArg(alb->at, at->arg.ka.key, at->arg.ka.value);
        case NONE:
            break;
    }
    return alb;
}


struct _ArgListNode* _ArgListNode_end(struct _ArgListNode* aln) {
    if(aln == NULL) {return NULL;}
    struct _ArgListNode* ret = aln->next_node;
    ArgToken_end(aln->at);
    free(aln);
    return ret;
}


struct _ArgListNode* _ArgList_getNode(struct _ArgListNode* ald, size_t index) {
    if(ald == NULL) {return NULL;}

    if(index <= 0) {
        return ald;
    } else {
        return _ArgList_getNode(ald->next_node, index -1);
    }
}


const struct ArgToken* ArgList_getArg(struct ArgList* al, size_t index) {
    if(al == NULL) {return NULL;}
    if(index > al->size) {return NULL;}

    const struct _ArgListNode* node = _ArgList_getNode(al->head, index);
    if(node == NULL) {return NULL;}

    return node->at;
}


const struct PosArg* ArgList_getPosArg(struct ArgList* al, size_t index) {
    const struct ArgToken* at = ArgList_getArg(al, index);
    return ArgToken_getPosArg(at);
}


const struct KeyArg* ArgList_getKeyArg(struct ArgList* al, size_t index) {
    const struct ArgToken* at = ArgList_getArg(al, index);
    return ArgToken_getKeyArg(at);
}


int ArgList_append(struct ArgList* ab, const struct ArgToken* at) {
    if(ab == NULL) {return INVALID_INPUT;}

    struct _ArgListNode* n_node = _ArgListNode_new(at);
    if(n_node == NULL) {return ALLOCATION_FAIL;}

    if(ab->head == NULL) {
        ab->head = n_node;
    } else {
        struct _ArgListNode* last_node = _ArgList_getNode(ab->head, ab->size - 1);
        if(last_node == NULL) {
            free(n_node);
            return FAIL;
        }
        last_node->next_node = n_node;
    }
    ab->size += 1;

    return SUCESS;
}

int ArgList_appendPosArg(struct ArgList* ab, const struct PosArg* pa) {
    struct ArgToken at = {
        .arg.pa = *pa,
        .type = POSITIONAL
    };
    return ArgList_append(ab, &at);
}

int ArgList_appendManualPosArg(struct ArgList* ab, const char* arg, size_t pos) {
    struct PosArg pa = {
        .pos = pos,
        .value = arg
    };
    return ArgList_appendPosArg(ab, &pa);
}


int ArgList_appendKeyArg(struct ArgList* ab, const struct KeyArg* ka) {
    struct ArgToken at = {
        .arg.ka = *ka,
        .type = KEY
    };
    return ArgList_append(ab, &at);
}

int ArgList_appendManualKeyArg(struct ArgList* ab, const char* arg, const char* key) {
    struct KeyArg ka = {
        .key = key,
        .value = arg
    };
    return ArgList_appendKeyArg(ab, &ka);
}


int ArgList_remove(struct ArgList* ab, size_t index) {
    if(ab == NULL) {return INVALID_INPUT;}
    if(index < 0 || index >= ab->size) {return OUT_OF_RANGE;}

    struct _ArgListNode* prvw_node = _ArgList_getNode(ab->head, index  -1);
    if(prvw_node == NULL) {return FAIL;}
    struct _ArgListNode* next_node = _ArgListNode_end(prvw_node->next_node);
    prvw_node = next_node;
    return SUCESS;
}

void ArgListIterator_init(struct ArgListIterator* ali, const struct ArgList* al) {
    ali->_head = al->head;
    ali->iterator = al->head;
}


int ArgListIterator_jump(struct ArgListIterator* ali) {
    if(ali == NULL) {return INVALID_INPUT;}
    if(ali->iterator->next_node == NULL) {
        ali->iterator = ali->_head;
        return TURN_AROUND;
    }
    else {
        ali->iterator = ali->iterator->next_node;
        return SUCESS;
    }
}
