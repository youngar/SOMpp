#pragma once

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

#include "VMPrimitive.h"

class VMEvaluationPrimitive: public VMPrimitive {
public:
    typedef GCEvaluationPrimitive Stored;
    
    VMEvaluationPrimitive(long argc);
    virtual void WalkObjects(walk_heap_fn);
    virtual VMEvaluationPrimitive* Clone() const;
    
    virtual StdString AsDebugString() const;
    
    int64_t GetNumberOfArguments() { return INT_VAL(load_ptr(numberOfArguments)); };
    
private:
    static const int VMEvaluationPrimitiveNumberOfFields;
    static VMSymbol* computeSignatureString(long argc);
    void evaluationRoutine(Interpreter*, VMFrame*);
    gc_oop_t numberOfArguments;

};

class EvaluationRoutine : public PrimitiveRoutine {
private:
    GCEvaluationPrimitive* evalPrim;
public:
    EvaluationRoutine(VMEvaluationPrimitive* prim)
        : PrimitiveRoutine(), evalPrim(_store_ptr(prim)) {};
    void WalkObjects(walk_heap_fn);
    virtual bool isClassSide() { return false; }
    virtual void Invoke(Interpreter* interp, VMFrame* frame);
};
