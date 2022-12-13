// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lite/operators/unique_op.h"
#include "lite/core/op_registry.h"
namespace paddle {
namespace lite {
namespace operators {

bool UniqueOp::CheckShape() const {
  CHECK_OR_FALSE(param_.X);
  CHECK_OR_FALSE(param_.Out);
  if (param_.return_index) {
    CHECK_OR_FALSE(param_.Indices);
  }
  if (param_.return_inverse) {
    CHECK_OR_FALSE(param_.Index);
  }
  if (param_.return_counts) {
    CHECK_OR_FALSE(param_.Counts)
  }
  return true;
}

bool UniqueOp::InferShapeImpl() const {
  DDim in_dims = param_.X->dims();
  if (param_.Out) param_.Out->Resize(in_dims);
  if (param_.Index) param_.Index->Resize(in_dims);
  if (param_.Indices) param_.Indices->Resize(in_dims);
  if (param_.Counts) param_.Counts->Resize(in_dims);
  return true;
}

bool UniqueOp::AttachImpl(const cpp::OpDesc &opdesc,
                                    lite::Scope *scope) {
  param_.X = scope->FindTensor(opdesc.Input("X").front());
  CHECK(param_.X) << "Input(X) of UniqueOp should not be null.";
  param_.Out = scope->FindMutableTensor(opdesc.Output("Out").front());
  CHECK(param_.Out) << "Output(Out) of UniqueOp should not be null.";
  if (opdesc.HasOutput("Index")) {
    param_.Index = scope->FindMutableTensor(opdesc.Output("Index").front());
    CHECK(param_.Out) << "Output(Index) of UniqueOp should not be null.";
  }
  if (opdesc.HasOutput("Indices")) {
    param_.Indices = scope->FindMutableTensor(opdesc.Output("Indices").front());
    CHECK(param_.Out) << "Output(Indices) of UniqueOp should not be null.";
  }
  if (opdesc.HasOutput("Counts")) {
    param_.Counts = scope->FindMutableTensor(opdesc.Output("Counts").front());
    CHECK(param_.Counts) << "Output(Counts) of UniqueOp should not be null.";
  }
  
  if (opdesc.HasAttr("dtype")) {
    param_.dtype = opdesc.GetAttr<int>("dtype");
  }
  if (opdesc.HasAttr("return_index")) {
    param_.return_index = opdesc.GetAttr<bool>("return_index");
  }
  if (opdesc.HasAttr("return_reverse")) {
    param_.return_inverse = opdesc.GetAttr<bool>("return_inverse");
  }
  if (opdesc.HasAttr("return_counts")) {
    param_.return_counts = opdesc.GetAttr<bool>("return_counts");
  }
  param_.axis = opdesc.GetAttr<std::vector<int>>("axis");
  if (opdesc.HasAttr("is_sorted")) {
    param_.is_sorted = opdesc.GetAttr<bool>("is_sorted");
  } 

  return true;
}

}  // namespace operators
}  // namespace lite
}  // namespace paddle

REGISTER_LITE_OP(unique, paddle::lite::operators::UniqueOp);
