//===- DebugRecordCount.cpp -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the DebugRecordCountPass class.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/DebugRecordCount.h"

#include "llvm/IR/DebugProgramInstruction.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

PreservedAnalyses DebugRecordCountPass::run(Function &F, FunctionAnalysisManager &) {
  unsigned DbgValue = 0;
  unsigned DbgDeclare = 0;
  unsigned DbgAssign = 0;

  for (auto &BB : F) {
    for (auto &I : BB) {
        for(auto &D : I.getDbgRecordRange()) {
            if(auto *DB = dyn_cast<DbgVariableRecord>(&D)) {
                if(DB->isDbgValue()) DbgValue++;
                else if(DB->isDbgDeclare()) DbgDeclare++;
                else if(DB->isDbgAssign()) DbgAssign++;
            }
        }
    }
  }

  outs() << "Function: " << F.getName() << "\n";
  outs() << "\t#dbg_values : " << DbgValue << "\n";
  outs() << "\t#dbg_declare: " << DbgDeclare << "\n";
  outs() << "\t#dbg_assign : " << DbgAssign << "\n";

  return PreservedAnalyses::all();
}
