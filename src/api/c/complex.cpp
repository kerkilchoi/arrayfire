/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/array.h>
#include <af/defines.h>
#include <af/arith.h>
#include <af/data.h>
#include <ArrayInfo.hpp>
#include <optypes.hpp>
#include <implicit.hpp>
#include <err_common.hpp>
#include <handle.hpp>
#include <backend.hpp>

#include <complex.hpp>

using namespace detail;

template<typename To, typename Ti>
static inline af_array cplx(const af_array lhs, const af_array rhs, bool destroy=true)
{
    af_array res = getHandle(*cplx<To, Ti>(getArray<Ti>(lhs), getArray<Ti>(rhs)));
    if (destroy) {
        // All inputs to this function are temporary references
        // Delete the temporary references
        destroyHandle<Ti>(lhs);
        destroyHandle<Ti>(rhs);
    }
    return res;
}

af_err af_cplx2(af_array *out, const af_array lhs, const af_array rhs)
{
    try {

        af_dtype type = implicit(lhs, rhs);

        if (type == c32 || type == c64) {
            AF_ERROR("Inputs to cplx2 can not be of complex type", AF_ERR_ARG);
        }

        if (type != f64) type = f32;

        const af_array left  = cast(lhs, type);
        const af_array right = cast(rhs, type);

        af_array res;
        switch (type) {
        case f32: res = cplx<cfloat , float>(left, right); break;
        case f64: res = cplx<cdouble, double>(left, right); break;
        default: TYPE_ERROR(0, type);
        }

        std::swap(*out, res);
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_cplx(af_array *out, const af_array in)
{
    try {

        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();

        if (type == c32 || type == c64) {
            AF_ERROR("Inputs to cplx2 can not be of complex type", AF_ERR_ARG);
        }

        af_array tmp;
        AF_CHECK(af_constant(&tmp,
                             0, info.ndims(),
                             info.dims().get(),
                             type));

        af_array res;
        switch (type) {

        case f32: res = cplx<cfloat , float >(in, tmp, false); break;
        case f64: res = cplx<cdouble, double>(in, tmp, false); break;

        default: TYPE_ERROR(0, type);
        }

        AF_CHECK(af_destroy_array(tmp));

        std::swap(*out, res);
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_real(af_array *out, const af_array in)
{
    try {

        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();

        if (type != c32 && type != c64) {
            AF_ERROR("Inputs to real must be of complex type", AF_ERR_ARG);
        }

        af_array res;
        switch (type) {

        case c32: res = getHandle(*real<float , cfloat >(getArray<cfloat >(in))); break;
        case c64: res = getHandle(*real<double, cdouble>(getArray<cdouble>(in))); break;

        default: TYPE_ERROR(0, type);
        }

        std::swap(*out, res);
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_imag(af_array *out, const af_array in)
{
    try {

        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();

        if (type != c32 && type != c64) {
            AF_ERROR("Inputs to imag must be of complex type", AF_ERR_ARG);
        }

        af_array res;
        switch (type) {

        case c32: res = getHandle(*imag<float , cfloat >(getArray<cfloat >(in))); break;
        case c64: res = getHandle(*imag<double, cdouble>(getArray<cdouble>(in))); break;

        default: TYPE_ERROR(0, type);
        }

        std::swap(*out, res);
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_conjg(af_array *out, const af_array in)
{
    try {

        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();

        if (type != c32 && type != c64) {
            AF_ERROR("Inputs to imag must be of complex type", AF_ERR_ARG);
        }

        af_array res;
        switch (type) {

        case c32: res = getHandle(*conj<cfloat >(getArray<cfloat >(in))); break;
        case c64: res = getHandle(*conj<cdouble>(getArray<cdouble>(in))); break;

        default: TYPE_ERROR(0, type);
        }

        std::swap(*out, res);
    }
    CATCHALL;
    return AF_SUCCESS;
}
