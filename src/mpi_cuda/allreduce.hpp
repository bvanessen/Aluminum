////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Lawrence Livermore National Security, LLC.  Produced at the
// Lawrence Livermore National Laboratory in collaboration with University of
// Illinois Urbana-Champaign.
//
// Written by the LBANN Research Team (N. Dryden, N. Maruyama, et al.) listed in
// the CONTRIBUTORS file. <lbann-dev@llnl.gov>
//
// LLNL-CODE-756777.
// All rights reserved.
//
// This file is part of Aluminum GPU-aware Communication Library. For details, see
// http://software.llnl.gov/Aluminum or https://github.com/LLNL/Aluminum.
//
// Licensed under the Apache License, Version 2.0 (the "Licensee"); you
// may not use this file except in compliance with the License.  You may
// obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the license.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cuda.hpp"
#include "mpi_cuda/communicator.hpp"
#include "mpi_cuda/allreduce_ring.hpp"
#include "mpi_cuda/util.hpp"
#include <cassert>

namespace Al {
namespace internal {
namespace mpi_cuda {

template <typename T> inline
void ring_allreduce(const T* sendbuf, T* recvbuf, size_t count,
                    ReductionOperator op, MPICUDACommunicator& comm,
                    cudaStream_t stream) {
  if (sendbuf != recvbuf) {
    COLL_CHECK_CUDA(cudaMemcpyAsync(recvbuf, sendbuf, sizeof(T) * count,
                                    cudaMemcpyDefault, stream));
  }
  comm.get_ring().allreduce<T>(recvbuf, count, op, stream, false);
}

template <typename T> inline
void bi_ring_allreduce(const T* sendbuf, T* recvbuf, size_t count,
                       ReductionOperator op, MPICUDACommunicator& comm,
                       cudaStream_t stream) {
  if (sendbuf != recvbuf) {
    COLL_CHECK_CUDA(cudaMemcpyAsync(recvbuf, sendbuf, sizeof(T) * count,
                                    cudaMemcpyDefault, stream));
  }
  comm.get_ring().allreduce<T>(recvbuf, count, op, stream, true);
}

} // namespace mpi_cuda
} // namespace internal
} // namespace Al
