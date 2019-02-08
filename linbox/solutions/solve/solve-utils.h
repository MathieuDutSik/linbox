/*
 * Copyright(C) LinBox
 *
 * ========LICENCE========
 * This file is part of the library LinBox.
 *
 * LinBox is free software: you can redistribute it and/or modify
 * it under the terms of the  GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 */

#pragma once

namespace LinBox {
    /**
     * Check that all input data are coherent for the solve.
     */
    template <class ResultVector, class Matrix, class Vector>
    inline void solve_precheck(const ResultVector& x, const Matrix& A, const Vector& b)
    {
        if ((A.coldim() != x.size()) || (A.rowdim() != b.size())) {
            throw LinboxError("Incompatible dimensions of data in 'solve'.");
        }
    }
}