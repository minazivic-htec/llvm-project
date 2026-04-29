//===- ToyCombine.cpp - Toy High Level Optimizer --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a set of simple combiners for optimizing operations in
// the Toy dialect.
//
//===----------------------------------------------------------------------===//

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/IR/Value.h"
#include "toy/Dialect.h"
using namespace mlir;
using namespace toy;

namespace {
/// Include the patterns defined in the Declarative Rewrite framework.
#include "ToyCombine.inc"
} // namespace

/// This is an example of a c++ rewrite pattern for the TransposeOp. It
/// optimizes the following scenario: transpose(transpose(x)) -> x

//definicija patterna, pravilo ove transformacije je ovde napisano
struct SimplifyRedundantTranspose : public mlir::OpRewritePattern<TransposeOp> {
  /// We register this pattern to match every toy.transpose in the IR.
  /// The "benefit" is used by the framework to order the patterns and process
  /// them in order of profitability.

  //nad ovim mlircontext-om se okristi pass, tj konstruktor se sa njim pravi
  SimplifyRedundantTranspose(mlir::MLIRContext *context)
      : OpRewritePattern<TransposeOp>(context, /*benefit=*/1) {}

  /// This method attempts to match a pattern and rewrite it. The rewriter
  /// argument is the orchestrator of the sequence of rewrites. The pattern is
  /// expected to interact with it to perform any changes to the IR from here.

  //ovo je logika transformacije, sta se matchuje, koji transpose op i njegov operand ako je dosao isto iz drugog transpose op
  //i rewriter kome se posle proledi replaceop koa op operacija se zameni cime tj transposeInputOp.getOperand()
  llvm::LogicalResult
  matchAndRewrite(TransposeOp op,
                  mlir::PatternRewriter &rewriter) const override {
    // Look through the input of the current transpose.
    //zgrabi operand, znamo da je operacija sigurno transpose
    mlir::Value transposeInput = op.getOperand();
    //gleda je l toperand isto potekao iz neke transpose operacije, tj je l odgovarajuca operacija proizvela ovu mlir::Value
    //jer u tom slucaju imamo transpose od transposa koji zelimo da svedemo samo na operand tj da se oni cancel outuju
    
    //ovo je zapravo matchovanje
    TransposeOp transposeInputOp = transposeInput.getDefiningOp<TransposeOp>();

    // Input defined by another transpose? If not, no match.
    if (!transposeInputOp)
      return failure();

    // Otherwise, we have a redundant transpose. Use the rewriter.

    //ovo je zapravo rewritovanje
    rewriter.replaceOp(op, {transposeInputOp.getOperand()});
    return success();
  }
};

/// Register our patterns as "canonicalization" patterns on the TransposeOp so
/// that they can be picked up by the Canonicalization framework.

//registracija ove transformacije nad mlircontext-om
void TransposeOp::getCanonicalizationPatterns(RewritePatternSet &results,
                                              MLIRContext *context) {
                                                // oov kaze da kada god canonicalizer pita TransposeOp za optimizacije, ubaci ovaj pattern u listu
                                                //svaka operacija ima svoje optimizacije, tako i transposeop
                                                //i onda canonicalizer trai sve optimizacije svih operacija
                                                //canonicpass poziva automatski transposeop::getcanonpatterns koji ce vratiti 
                                                //listu njih, u koju ovde upravo dodajemo ovaj nas pattern
  
//kaze mliru da doda ovaj pattern u canonizaciju, tj ovde se registruje, kaze mliru d aga koristi kad optimizuje transposeop  
results.add<SimplifyRedundantTranspose>(context);
}

/// Register our patterns as "canonicalization" patterns on the ReshapeOp so
/// that they can be picked up by the Canonicalization framework.
void ReshapeOp::getCanonicalizationPatterns(RewritePatternSet &results,
                                            MLIRContext *context) {
  //reshape op ima 3 kanonizacije
  //ReshapeReshapeOptPattern : reshape(reshape(x)) → reshape(x)
  //RedundantReshapeOptPattern : reshape(x) → x   (ako nema promene oblika)
  //FoldConstantReshapeOptPattern : reshape(constant) → constant (folding)
  results.add<ReshapeReshapeOptPattern, RedundantReshapeOptPattern,
              FoldConstantReshapeOptPattern>(context);
}
