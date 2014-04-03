/*
 *
 *
 Copyright (c) 2007 Michael Haupt, Tobias Pape, Arne Bergmann
 Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
 http://www.hpi.uni-potsdam.de/swa/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <iostream>
#include <string.h>

#include "Heap.h"
#include "../vmobjects/VMObject.h"
#include "../vm/Universe.h"
#include "../natives/VMThread.h"

HEAP_CLS* Heap::theHeap = NULL;

void Heap::InitializeHeap(long objectSpaceSize) {
    if (theHeap) {
        cout << "Warning, reinitializing already initialized Heap, "
                << "all data will be lost!" << endl;
        delete theHeap;
    }
    theHeap = new HEAP_CLS(objectSpaceSize);
}

void Heap::DestroyHeap() {
    if (theHeap)
        delete theHeap;
}

Heap::Heap(long objectSpaceSize) {
    gcTriggered = false;
    threadCount = 0;
    readyForGCThreads = 0;
    pthread_mutex_init(&doCollect, NULL);
    pthread_mutex_init(&threadCountMutex, NULL);
    pthread_mutex_init(&allocationLock, NULL);
    pthread_cond_init(&stopTheWorldCondition, NULL);
    pthread_cond_init(&mayProceed, NULL);
}

Heap::~Heap() {
    delete gc;
}

void Heap::FullGC() {
    // one thread is going to do the GC
    if (pthread_mutex_trylock(&doCollect) == 0) {
        // all threads must have reached a safe point for the GC to take place
        pthread_mutex_lock(&threadCountMutex);
        while (threadCount != readyForGCThreads) {
            pthread_cond_wait(&stopTheWorldCondition, &threadCountMutex);
        }
        pthread_mutex_unlock(&threadCountMutex);
        // all threads have reached a safe point
        //cout << "GC : gc collection starting, threadID: " << _UNIVERSE->GetInterpreter()->GetThread()->GetThreadId() << endl;
        gc->Collect();
        //cout << "GC : gc collection finished, threadId: " << _UNIVERSE->GetInterpreter()->GetThread()->GetThreadId() << endl;
        // signal all the threads that the GC is completed
        pthread_cond_broadcast(&mayProceed);
        //cout << "GC : broadcast after completing GC, threadId: " << _UNIVERSE->GetInterpreter()->GetThread()->GetThreadId() << endl;
        pthread_mutex_unlock(&doCollect);
        //cout << "GC : unlock doCollect after completing GC, threadId: " << _UNIVERSE->GetInterpreter()->GetThread()->GetThreadId() << endl;
    // other threads signal the barrier that for them the GC may take place
    } else {
        pthread_mutex_lock(&threadCountMutex);
        readyForGCThreads++;
        //cout << "GC : thread waiting for GC to happen, threadId: " << _UNIVERSE->GetInterpreter()->GetThread()->GetThreadId() << endl;
        pthread_cond_signal(&stopTheWorldCondition);
        while (gcTriggered) {
            pthread_cond_wait(&mayProceed, &threadCountMutex);
        }
        readyForGCThreads--;
        //cout << "GC : thread resuming after GC has happend, threadId: " << _UNIVERSE->GetInterpreter()->GetThread()->GetThreadId() << endl;
        pthread_mutex_unlock(&threadCountMutex);
    }
}

void Heap::IncrementThreadCount() {
    pthread_mutex_lock(&threadCountMutex);
    threadCount++;
    pthread_mutex_unlock(&threadCountMutex);
}

void Heap::DecrementThreadCount() {
    pthread_mutex_lock(&threadCountMutex);
    threadCount--;
    pthread_mutex_unlock(&threadCountMutex);
}

void Heap::IncrementWaitingForGCThreads() {
    pthread_mutex_lock(&threadCountMutex);
    readyForGCThreads++;
    pthread_cond_signal(&stopTheWorldCondition);
    pthread_mutex_unlock(&threadCountMutex);
}

void Heap::DecrementWaitingForGCThreads() {
    pthread_mutex_lock(&threadCountMutex);
    readyForGCThreads--;
    pthread_cond_signal(&stopTheWorldCondition);
    pthread_mutex_unlock(&threadCountMutex);
}
