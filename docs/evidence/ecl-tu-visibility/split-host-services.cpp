#include "EclVm.hpp"
#include <string.h>

int EclVmHost::Run(float timeDelta)
{
    int first = 1;
    EclVmThreadNode *node = &threadList;
    while (node != NULL)
    {
        EclVmThreadNode *next = node->next;
        activeContext = node->context;
        if (first)
        {
            if (activeContext->Run(timeDelta) != 0)
                return -1;
            first = false;
        }
        else if (activeContext->Run(timeDelta) != 0)
        {
            delete activeContext;
            if (node->next != NULL)
                node->next->previous = node->previous;
            if (node->previous != NULL)
                node->previous->next = node->next;
            node->next = NULL;
            node->previous = NULL;
            delete node;
        }
        node = next;
    }
    activeContext = &embeddedContext;
    return 0;
}


