//===- DeleteDebugRecord.h --------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Custom function pass for deleting Debug records.
//
//===----------------------------------------------------------------------===//


#ifndef LLVM_TRANSFORMS_UTILS_DELETEDEBUGRECORD_H
#define LLVM_TRANSFORMS_UTILS_DELETEDEBUGRECORD_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class DeleteDebugRecordPass : public PassInfoMixin<DeleteDebugRecordPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_DELETEDEBUGRECORD_H
