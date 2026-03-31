//===- DeleteDebugRecord.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the DeleteDebugRecordPass class.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/DeleteDebugRecord.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Analysis.h"
#include "llvm/IR/DebugProgramInstruction.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"

using namespace llvm;

PreservedAnalyses DeleteDebugRecordPass::run(Function &F, FunctionAnalysisManager &) {
    
    bool Changed = false;

    for (auto &BB : F) {
        for (auto &I : BB) {
            SmallVector<DbgRecord *, 4> ToDelete;

            for (auto &D : I.getDbgRecordRange())
                if(isa<DbgVariableRecord>(&D))
                    ToDelete.push_back(&D);
            
            for (auto *I : ToDelete) {
                I->eraseFromParent();
                Changed = true;
            }
        }
    }

    return !Changed? PreservedAnalyses::all(): PreservedAnalyses::none();
}
