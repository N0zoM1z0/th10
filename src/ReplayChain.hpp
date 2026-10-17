#ifndef TH10_REPLAY_CHAIN_HPP
#define TH10_REPLAY_CHAIN_HPP

#include "ReplayManager.hpp"
#include <stddef.h>

// Existing ReplayManager.cpp declaration and assertions shared unchanged.
typedef int (*ReplayChainCallback)(ReplayManager *manager);

struct ReplayChainElement
{
    int priority;
    unsigned int isHeapAllocated : 1;
    unsigned int unknownFlag1 : 1;
    unsigned int unknownFlags : 30;
    ReplayChainCallback callback;
    ReplayChainCallback addedCallback;
    ReplayChainCallback deletedCallback;
    ReplayListNode link;
    ReplayManager *argument;

    ReplayChainElement()
    {
        priority = 0;
        isHeapAllocated = 0;
        callback = NULL;
        addedCallback = NULL;
        deletedCallback = NULL;
        link.value = this;
        link.next = NULL;
        link.previous = NULL;
    }
};
typedef char ReplayChainElementSizeIs24[
    (sizeof(ReplayChainElement) == 0x24) ? 1 : -1];
typedef char ReplayChainElementCallbackAt08[
    (offsetof(ReplayChainElement, callback) == 0x08) ? 1 : -1];
typedef char ReplayChainElementLinkAt14[
    (offsetof(ReplayChainElement, link) == 0x14) ? 1 : -1];
typedef char ReplayChainElementArgumentAt20[
    (offsetof(ReplayChainElement, argument) == 0x20) ? 1 : -1];

#endif
