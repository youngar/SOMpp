#pragma once
#ifndef VMINTEGER_H_
#define VMINTEGER_H_

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

#include "AbstractObject.h"

#include "../misc/defs.h"

class VMInteger: public AbstractVMObject {
public:
    VMInteger();
    VMInteger(long);

    inline long GetEmbeddedInteger() const;
    virtual pVMInteger Clone() const;
    virtual pVMClass GetClass() /*const*/;
    virtual inline size_t GetObjectSize() const;
    
    virtual void MarkObjectAsInvalid() {}

private:
    long embeddedInteger;
};

long VMInteger::GetEmbeddedInteger() const {
    return this->embeddedInteger;
}

size_t VMInteger::GetObjectSize() const {
    //no need to pad -> sizeof returns padded size anyway
    return sizeof(VMInteger);
}

#endif
