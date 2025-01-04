"""
/* Copyright (c) 2023 Amazon
   Written by Jan Buethe */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
"""

import torch

from .base_sparsifier import BaseSparsifier
from .common import sparsify_matrix


class LinearSparsifier(BaseSparsifier):
    def __init__(self, task_list, start, stop, interval, exponent=3):
        """ Sparsifier for torch.nn.GRUs

            Parameters:
            -----------
            task_list : list
                task_list contains a list of tuples (linear, params), where linear is an instance
                of torch.nn.Linear and params is a tuple (density, [m, n]),
                where density is the target density in [0, 1], [m, n] is the shape sub-blocks to which
                sparsification is applied.

            start : int
                training step after which sparsification will be started.

            stop : int
                training step after which sparsification will be completed.

            interval : int
                sparsification interval for steps between start and stop. After stop sparsification will be
                carried out after every call to GRUSparsifier.step()

            exponent : float
                Interpolation exponent for sparsification interval. In step i sparsification will be carried out
                with density (alpha + target_density * (1 * alpha)), where
                alpha = ((stop - i) / (start - stop)) ** exponent

            Example:
            --------
            >>> import torch
            >>> linear = torch.nn.Linear(8, 16)
            >>> params = (0.2, [8, 4])
            >>> sparsifier = LinearSparsifier([(linear, params)], 0, 100, 50)
            >>> for i in range(100):
            ...         sparsifier.step()
        """

        super().__init__(task_list, start, stop, interval, exponent=3)

        self.last_mask = None

    def sparsify(self, alpha, verbose=False):
        """ carries out sparsification step

            Call this function after optimizer.step in your
            training loop.

            Parameters:
            ----------
            alpha : float
                density interpolation parameter (1: dense, 0: target density)
            verbose : bool
                if true, densities are printed out

            Returns:
            --------
            None

        """

        with torch.no_grad():
            for linear, params in self.task_list:
                if hasattr(linear, 'weight_v'):
                    weight = linear.weight_v
                else:
                    weight = linear.weight
                target_density, block_size = params
                density = alpha + (1 - alpha) * target_density
                weight[:], new_mask = sparsify_matrix(weight, density, block_size, return_mask=True)

                if self.last_mask is not None:
                    if not torch.all(self.last_mask * new_mask == new_mask) and debug:
                        print("weight resurrection in conv.weight")

                self.last_mask = new_mask

                if verbose:
                    print(f"linear_sparsifier[{self.step_counter}]: {density=}")


if __name__ == "__main__":
    print("Testing sparsifier")

    import torch
    linear = torch.nn.Linear(8, 16)
    params = (0.2, [4, 2])

    sparsifier = LinearSparsifier([(linear, params)], 0, 100, 5)

    for i in range(100):
            sparsifier.step(verbose=True)

    print(linear.weight)
