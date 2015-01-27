/*
 * AbstractVMObject.cpp
 *
 *  Created on: 10.03.2011
 *      Author: christian
 */

#include "AbstractObject.h"

#include <interpreter/Interpreter.inline.h>
#include <vm/Universe.h>

#include "VMFrame.h"
#include "VMClass.h"
#include "VMInvokable.h"

intptr_t AbstractVMObject::GetHash() {
    return reinterpret_cast<intptr_t>(this);
}

void AbstractVMObject::Send(Interpreter* interp, StdString selectorString, vm_oop_t* arguments, long argc) {
    VMFrame* frame = interp->GetFrame();
    VMSymbol* selector = GetUniverse()->SymbolFor(selectorString, interp->GetPage());
    frame->Push(this);

    for (long i = 0; i < argc; ++i) {
        frame->Push(arguments[i]);
    }

    VMClass* cl = GetClass();
    VMInvokable* invokable = cl->LookupInvokable(selector);
    invokable->Invoke(interp, frame);
}

long AbstractVMObject::GetFieldIndex(VMSymbol* fieldName) const {
    return GetClass()->LookupFieldIndex(fieldName);
}
