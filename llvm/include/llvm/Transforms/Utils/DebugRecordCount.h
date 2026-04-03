//===- DebugRecordCount.h ---------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Custom function pass for counting Debug Records
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_DEBUGRECORDCOUNT_H
#define LLVM_TRANSFORMS_UTILS_DEBUGRECORDCOUNT_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class DebugRecordCountPass : public PassInfoMixin<DebugRecordCountPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_DEBUGRECORDCOUNT_H
