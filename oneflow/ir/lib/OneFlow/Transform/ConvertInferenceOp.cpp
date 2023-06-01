/*
Copyright 2020 The OneFlow Authors. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <iostream>
#include <string>
#include "OneFlow/Passes.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"
#include "OneFlow/OneFlowPatternUtils.h"

namespace mlir {

namespace oneflow {

namespace {

class PreConvertInferenceOpPass : public PreConvertInferenceOpPassBase<PreConvertInferenceOpPass> {
  void runOnOperation() override {
    Operation* op = getOperation();
    RewritePatternSet patterns(op->getContext());
    oneflow::populatePreConvertInferenceOp(patterns);
    (void)applyPatternsAndFoldGreedily(op, std::move(patterns));
  }
};

class ConvertInferenceOpPass : public ConvertInferenceOpPassBase<ConvertInferenceOpPass> {
  void runOnOperation() override {
    Operation* op = getOperation();
    RewritePatternSet patterns(op->getContext());
    oneflow::populateConvertInferenceOp(patterns);
    oneflow::rewrites::populateRewrites(patterns);
    (void)applyPatternsAndFoldGreedily(op, std::move(patterns));
  }
};

class PostConvertInferenceOpPass
    : public PostConvertInferenceOpPassBase<PostConvertInferenceOpPass> {
  void runOnOperation() override {
    Operation* op = getOperation();
    RewritePatternSet patterns(op->getContext());
    oneflow::populatePostConvertInferenceOp(patterns);
    (void)applyPatternsAndFoldGreedily(op, std::move(patterns));
  }
};

}  // namespace

std::unique_ptr<Pass> createPreConvertInferenceOpPass() {
  return std::make_unique<PreConvertInferenceOpPass>();
}

std::unique_ptr<Pass> createConvertInferenceOpPass() {
  return std::make_unique<ConvertInferenceOpPass>();
}

std::unique_ptr<Pass> createPostConvertInferenceOpPass() {
  return std::make_unique<PostConvertInferenceOpPass>();
}

}  // namespace oneflow

}  // namespace mlir