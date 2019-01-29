/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2019 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Simd/SimdMemory.h"
#include "Simd/SimdStore.h"
#include "Simd/SimdWinograd.h"
#include "Simd/SimdBase.h"

namespace Simd
{
#ifdef SIMD_SSE_ENABLE    
    namespace Sse
    {
        SIMD_INLINE void Load4(const float * src, size_t step, __m128 * dst)
        {
            __m128 a0 = _mm_loadu_ps(src + 0 * step);
            __m128 a1 = _mm_loadu_ps(src + 1 * step);
            __m128 a2 = _mm_loadu_ps(src + 2 * step);
            __m128 a3 = _mm_loadu_ps(src + 3 * step);
            __m128 b0 = _mm_unpacklo_ps(a0, a2);
            __m128 b1 = _mm_unpackhi_ps(a0, a2);
            __m128 b2 = _mm_unpacklo_ps(a1, a3);
            __m128 b3 = _mm_unpackhi_ps(a1, a3);
            dst[0] = _mm_unpacklo_ps(b0, b2);
            dst[1] = _mm_unpackhi_ps(b0, b2);
            dst[2] = _mm_unpacklo_ps(b1, b3);
            dst[3] = _mm_unpackhi_ps(b1, b3);
        }

        SIMD_INLINE void Winograd2x3SetFilter4n(const float * src, float * dst, size_t stride)
        {
            const __m128 r2 = _mm_set1_ps(1.0f / 2.0f);
            const __m128 r4 = _mm_set1_ps(1.0f / 4.0f);

            __m128 s[9];
            Load4(src + 0, 9, s + 0);
            Load4(src + 4, 9, s + 4);
            s[8] = _mm_setr_ps(src[8], src[17], src[26], src[35]);

            _mm_storeu_ps(dst + 0 * stride, s[0]);
            __m128 _0a2 = _mm_add_ps(s[0], s[2]);
            _mm_storeu_ps(dst + 1 * stride, _mm_mul_ps(_mm_add_ps(_0a2, s[1]), r2));
            _mm_storeu_ps(dst + 2 * stride, _mm_mul_ps(_mm_sub_ps(_0a2, s[1]), r2));
            _mm_storeu_ps(dst + 3 * stride, s[2]);

            __m128 _0a6a3 = _mm_add_ps(_mm_add_ps(s[0], s[6]), s[3]);
            _mm_storeu_ps(dst + 4 * stride, _mm_mul_ps(_0a6a3, r2));
            __m128 _2a8a5 = _mm_add_ps(_mm_add_ps(s[2], s[8]), s[5]);
            __m128 _1a7a4 = _mm_add_ps(_mm_add_ps(s[1], s[7]), s[4]);
            _mm_storeu_ps(dst + 5 * stride, _mm_mul_ps(_mm_add_ps(_mm_add_ps(_0a6a3, _2a8a5), _1a7a4), r4));
            _mm_storeu_ps(dst + 6 * stride, _mm_mul_ps(_mm_sub_ps(_mm_add_ps(_0a6a3, _2a8a5), _1a7a4), r4));
            _mm_storeu_ps(dst + 7 * stride, _mm_mul_ps(_2a8a5, r2));

            __m128 _0a6s3 = _mm_sub_ps(_mm_add_ps(s[0], s[6]), s[3]);
            _mm_storeu_ps(dst + 8 * stride, _mm_mul_ps(_0a6s3, r2));
            __m128 _2a8s5 = _mm_sub_ps(_mm_add_ps(s[2], s[8]), s[5]);
            __m128 _1a7s4 = _mm_sub_ps(_mm_add_ps(s[1], s[7]), s[4]);
            _mm_storeu_ps(dst + 9 * stride, _mm_mul_ps(_mm_add_ps(_mm_add_ps(_0a6s3, _2a8s5), _1a7s4), r4));
            _mm_storeu_ps(dst + 10 * stride, _mm_mul_ps(_mm_sub_ps(_mm_add_ps(_0a6s3, _2a8s5), _1a7s4), r4));
            _mm_storeu_ps(dst + 11 * stride, _mm_mul_ps(_2a8s5, r2));

            _mm_storeu_ps(dst + 12 * stride, s[6]);
            __m128 _6a8 = _mm_add_ps(s[6], s[8]);
            _mm_storeu_ps(dst + 13 * stride, _mm_mul_ps(_mm_add_ps(_6a8, s[7]), r2));
            _mm_storeu_ps(dst + 14 * stride, _mm_mul_ps(_mm_sub_ps(_6a8, s[7]), r2));
            _mm_storeu_ps(dst + 15 * stride, s[8]);
        }

        SIMD_INLINE void Winograd2x3SetFilter4t(const float * src, float * dst, size_t stride)
        {
            const __m128 r2 = _mm_set1_ps(1.0f / 2.0f);
            const __m128 r4 = _mm_set1_ps(1.0f / 4.0f);

            __m128 s[9];
            s[0] = _mm_loadu_ps(src + 0 * stride);
            s[1] = _mm_loadu_ps(src + 1 * stride);
            s[2] = _mm_loadu_ps(src + 2 * stride);
            s[3] = _mm_loadu_ps(src + 3 * stride);
            s[4] = _mm_loadu_ps(src + 4 * stride);
            s[5] = _mm_loadu_ps(src + 5 * stride);
            s[6] = _mm_loadu_ps(src + 6 * stride);
            s[7] = _mm_loadu_ps(src + 7 * stride);
            s[8] = _mm_loadu_ps(src + 8 * stride);

            _mm_storeu_ps(dst + 0 * stride, s[0]);
            __m128 _0a2 = _mm_add_ps(s[0], s[2]);
            _mm_storeu_ps(dst + 1 * stride, _mm_mul_ps(_mm_add_ps(_0a2, s[1]), r2));
            _mm_storeu_ps(dst + 2 * stride, _mm_mul_ps(_mm_sub_ps(_0a2, s[1]), r2));
            _mm_storeu_ps(dst + 3 * stride, s[2]);

            __m128 _0a6a3 = _mm_add_ps(_mm_add_ps(s[0], s[6]), s[3]);
            _mm_storeu_ps(dst + 4 * stride, _mm_mul_ps(_0a6a3, r2));
            __m128 _2a8a5 = _mm_add_ps(_mm_add_ps(s[2], s[8]), s[5]);
            __m128 _1a7a4 = _mm_add_ps(_mm_add_ps(s[1], s[7]), s[4]);
            _mm_storeu_ps(dst + 5 * stride, _mm_mul_ps(_mm_add_ps(_mm_add_ps(_0a6a3, _2a8a5), _1a7a4), r4));
            _mm_storeu_ps(dst + 6 * stride, _mm_mul_ps(_mm_sub_ps(_mm_add_ps(_0a6a3, _2a8a5), _1a7a4), r4));
            _mm_storeu_ps(dst + 7 * stride, _mm_mul_ps(_2a8a5, r2));

            __m128 _0a6s3 = _mm_sub_ps(_mm_add_ps(s[0], s[6]), s[3]);
            _mm_storeu_ps(dst + 8 * stride, _mm_mul_ps(_0a6s3, r2));
            __m128 _2a8s5 = _mm_sub_ps(_mm_add_ps(s[2], s[8]), s[5]);
            __m128 _1a7s4 = _mm_sub_ps(_mm_add_ps(s[1], s[7]), s[4]);
            _mm_storeu_ps(dst + 9 * stride, _mm_mul_ps(_mm_add_ps(_mm_add_ps(_0a6s3, _2a8s5), _1a7s4), r4));
            _mm_storeu_ps(dst + 10 * stride, _mm_mul_ps(_mm_sub_ps(_mm_add_ps(_0a6s3, _2a8s5), _1a7s4), r4));
            _mm_storeu_ps(dst + 11 * stride, _mm_mul_ps(_2a8s5, r2));

            _mm_storeu_ps(dst + 12 * stride, s[6]);
            __m128 _6a8 = _mm_add_ps(s[6], s[8]);
            _mm_storeu_ps(dst + 13 * stride, _mm_mul_ps(_mm_add_ps(_6a8, s[7]), r2));
            _mm_storeu_ps(dst + 14 * stride, _mm_mul_ps(_mm_sub_ps(_6a8, s[7]), r2));
            _mm_storeu_ps(dst + 15 * stride, s[8]);
        }

        void Winograd2x3SetFilter(const float * src, size_t size, float * dst, SimdBool trans)
        {
            size_t size4 = AlignLo(size, 4), i = 0;
            if (trans)
            {
                for (; i < size4; i += 4)
                    Winograd2x3SetFilter4t(src + i, dst + i, size);
                for (; i < size; i += 1)
                    Base::Winograd2x3SetFilter1t(src + i, dst + i, size);
            }
            else
            {
                for (; i < size4; i += 4, src += 36, dst += 4)
                    Winograd2x3SetFilter4n(src, dst, size);
                for (; i < size; i += 1, src += 9, dst += 1)
                    Base::Winograd2x3SetFilter1n(src, dst, size);
            }
        }

        SIMD_INLINE void Winograd2x3SetInputLoad4n(const float * src, __m128 * dst)
        {
            __m128 a0 = _mm_loadu_ps(src + 0);
            __m128 a1 = _mm_loadu_ps(src + 2);
            __m128 a2 = _mm_loadu_ps(src + 4);
            __m128 a3 = _mm_loadu_ps(src + 6);
            dst[0] = _mm_shuffle_ps(a0, a2, 0x88);
            dst[1] = _mm_shuffle_ps(a0, a2, 0xDD);
            dst[2] = _mm_shuffle_ps(a1, a3, 0x88);
            dst[3] = _mm_shuffle_ps(a1, a3, 0xDD);
        }

        SIMD_INLINE void Winograd2x3SetInputLoad4n(const float * src, __m128 * dst, PadType pad)
        {
            __m128 a0 = (pad == PadNose1 ? LoadPadZeroNose1(src + 0) : _mm_loadu_ps(src + 0));
            __m128 a1 = _mm_loadu_ps(src + 2);
            __m128 a2 = _mm_loadu_ps(src + 4);
            __m128 a3 = (pad == PadTail2 ? LoadPadZeroTail2(src + 6) : (pad == PadTail1 ? LoadPadZeroTail1(src + 6) : _mm_loadu_ps(src + 6)));
            dst[0] = _mm_shuffle_ps(a0, a2, 0x88);
            dst[1] = _mm_shuffle_ps(a0, a2, 0xDD);
            dst[2] = _mm_shuffle_ps(a1, a3, 0x88);
            dst[3] = _mm_shuffle_ps(a1, a3, 0xDD);
        }

        SIMD_INLINE void Winograd2x3SetInputLoad4z(__m128 * dst)
        {
            dst[0] = _mm_setzero_ps();
            dst[1] = _mm_setzero_ps();
            dst[2] = _mm_setzero_ps();
            dst[3] = _mm_setzero_ps();
        }

        SIMD_INLINE void Winograd2x3SetInput4Store(const __m128 * src, float * dst, size_t stride)
        {
            _mm_storeu_ps(dst + 0 * stride, _mm_sub_ps(_mm_sub_ps(src[0], src[8]), _mm_sub_ps(src[2], src[10])));
            _mm_storeu_ps(dst + 1 * stride, _mm_add_ps(_mm_sub_ps(src[1], src[9]), _mm_sub_ps(src[2], src[10])));
            _mm_storeu_ps(dst + 2 * stride, _mm_sub_ps(_mm_sub_ps(src[2], src[10]), _mm_sub_ps(src[1], src[9])));
            _mm_storeu_ps(dst + 3 * stride, _mm_sub_ps(_mm_sub_ps(src[1], src[9]), _mm_sub_ps(src[3], src[11])));
            _mm_storeu_ps(dst + 4 * stride, _mm_sub_ps(_mm_add_ps(src[4], src[8]), _mm_add_ps(src[6], src[10])));
            _mm_storeu_ps(dst + 5 * stride, _mm_add_ps(_mm_add_ps(src[5], src[9]), _mm_add_ps(src[6], src[10])));
            _mm_storeu_ps(dst + 6 * stride, _mm_sub_ps(_mm_add_ps(src[6], src[10]), _mm_add_ps(src[5], src[9])));
            _mm_storeu_ps(dst + 7 * stride, _mm_sub_ps(_mm_add_ps(src[5], src[9]), _mm_add_ps(src[7], src[11])));
            _mm_storeu_ps(dst + 8 * stride, _mm_sub_ps(_mm_sub_ps(src[8], src[4]), _mm_sub_ps(src[10], src[6])));
            _mm_storeu_ps(dst + 9 * stride, _mm_add_ps(_mm_sub_ps(src[9], src[5]), _mm_sub_ps(src[10], src[6])));
            _mm_storeu_ps(dst + 10 * stride, _mm_sub_ps(_mm_sub_ps(src[10], src[6]), _mm_sub_ps(src[9], src[5])));
            _mm_storeu_ps(dst + 11 * stride, _mm_sub_ps(_mm_sub_ps(src[9], src[5]), _mm_sub_ps(src[11], src[7])));
            _mm_storeu_ps(dst + 12 * stride, _mm_sub_ps(_mm_sub_ps(src[4], src[12]), _mm_sub_ps(src[6], src[14])));
            _mm_storeu_ps(dst + 13 * stride, _mm_add_ps(_mm_sub_ps(src[5], src[13]), _mm_sub_ps(src[6], src[14])));
            _mm_storeu_ps(dst + 14 * stride, _mm_sub_ps(_mm_sub_ps(src[6], src[14]), _mm_sub_ps(src[5], src[13])));
            _mm_storeu_ps(dst + 15 * stride, _mm_sub_ps(_mm_sub_ps(src[5], src[13]), _mm_sub_ps(src[7], src[15])));
        }

        SIMD_INLINE void Winograd2x3SetInput4n(const float * src, size_t srcStride, float * dst, size_t dstStride)
        {
            __m128 t[16];
            Winograd2x3SetInputLoad4n(src + 0 * srcStride, t + 0);
            Winograd2x3SetInputLoad4n(src + 1 * srcStride, t + 4);
            Winograd2x3SetInputLoad4n(src + 2 * srcStride, t + 8);
            Winograd2x3SetInputLoad4n(src + 3 * srcStride, t + 12);
            Winograd2x3SetInput4Store(t, dst, dstStride);
        }

        SIMD_INLINE void Winograd2x3SetInput4n(const float * src, size_t srcStride, PadType rowPad, PadType colPad, float * dst, size_t dstStride)
        {
            __m128 t[16];
            if (rowPad == PadNose1)
                Winograd2x3SetInputLoad4z(t + 0);
            else
                Winograd2x3SetInputLoad4n(src + 0 * srcStride, t + 0, colPad);
            Winograd2x3SetInputLoad4n(src + 1 * srcStride, t + 4, colPad);
            if (rowPad == PadTail2)
                Winograd2x3SetInputLoad4z(t + 8);
            else
                Winograd2x3SetInputLoad4n(src + 2 * srcStride, t + 8, colPad);
            if (rowPad >= PadTail1)
                Winograd2x3SetInputLoad4z(t + 12);
            else
                Winograd2x3SetInputLoad4n(src + 3 * srcStride, t + 12, colPad);
            Winograd2x3SetInput4Store(t, dst, dstStride);
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcS, size_t srcC, __m128 dst[16])
        {
            dst[0] = _mm_loadu_ps(src + 0 * srcS + 0 * srcC);
            dst[1] = _mm_loadu_ps(src + 0 * srcS + 1 * srcC);
            dst[2] = _mm_loadu_ps(src + 0 * srcS + 2 * srcC);
            dst[3] = _mm_loadu_ps(src + 0 * srcS + 3 * srcC);
            dst[4] = _mm_loadu_ps(src + 1 * srcS + 0 * srcC);
            dst[5] = _mm_loadu_ps(src + 1 * srcS + 1 * srcC);
            dst[6] = _mm_loadu_ps(src + 1 * srcS + 2 * srcC);
            dst[7] = _mm_loadu_ps(src + 1 * srcS + 3 * srcC);
            dst[8] = _mm_loadu_ps(src + 2 * srcS + 0 * srcC);
            dst[9] = _mm_loadu_ps(src + 2 * srcS + 1 * srcC);
            dst[10] = _mm_loadu_ps(src + 2 * srcS + 2 * srcC);
            dst[11] = _mm_loadu_ps(src + 2 * srcS + 3 * srcC);
            dst[12] = _mm_loadu_ps(src + 3 * srcS + 0 * srcC);
            dst[13] = _mm_loadu_ps(src + 3 * srcS + 1 * srcC);
            dst[14] = _mm_loadu_ps(src + 3 * srcS + 2 * srcC);
            dst[15] = _mm_loadu_ps(src + 3 * srcS + 3 * srcC);
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcW, size_t srcC, float * dst, size_t dstStride)
        {
            size_t srcS = srcW * srcC;
            size_t srcCF = AlignLo(srcC, F);
            for (size_t c = 0; c < srcCF; c += F)
            {
                __m128 tmp[16];
                Winograd2x3SetInput4t(src + c, srcS, srcC, tmp);
                Winograd2x3SetInput4Store(tmp, dst + c, dstStride);
            }
            if(srcCF < srcC)
            {
                __m128 tmp[16];
                Winograd2x3SetInput4t(src + srcC - F, srcS, srcC, tmp);
                Winograd2x3SetInput4Store(tmp, dst + srcC - F, dstStride);
            }
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcS, size_t srcC, size_t rowB, size_t rowE, size_t colB, size_t colE, __m128 dst[16])
        {
            for (size_t i = 0; i < 16; ++i)
                dst[i] = _mm_setzero_ps();
            for (size_t row = rowB; row < rowE; ++row)
                for (size_t col = colB; col < colE; ++col)
                    dst[row * 4 + col] = _mm_loadu_ps(src + row * srcS + col * srcC);
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcW, size_t srcC, size_t rowB, size_t rowE, size_t colB, size_t colE, float * dst, size_t dstStride)
        {
            size_t srcS = srcW * srcC;
            size_t srcCF = AlignLo(srcC, F);
            for (size_t c = 0; c < srcCF; c += F)
            {
                __m128 tmp[16];
                Winograd2x3SetInput4t(src + c, srcS, srcC, rowB, rowE, colB, colE, tmp);
                Winograd2x3SetInput4Store(tmp, dst + c, dstStride);
            }
            if (srcCF < srcC)
            {
                __m128 tmp[16];
                Winograd2x3SetInput4t(src + srcC - F, srcS, srcC, rowB, rowE, colB, colE, tmp);
                Winograd2x3SetInput4Store(tmp, dst + srcC - F, dstStride);
            }
        }

        void Winograd2x3SetInput(const float * src, size_t srcChannels, size_t srcHeight, size_t srcWidth, float * dst, SimdBool pad, SimdBool trans)
        {
            if (trans ? (srcChannels < 4) : (srcHeight < 4 || srcWidth < 10))
            {
                Base::Winograd2x3SetInput(src, srcChannels, srcHeight, srcWidth, dst, pad, trans);
                return;
            }
            size_t dstH = pad ? srcHeight : srcHeight - 2;
            size_t dstW = pad ? srcWidth : srcWidth - 2;
            size_t tileH = (dstH + 1) / 2;
            size_t tileW = (dstW + 1) / 2;
            size_t dstStride = srcChannels*tileH*tileW;
            size_t dstH2 = AlignLo(dstH, 2);
            size_t dstW2 = AlignLo(dstW, 2);
            if (trans)
            {
                size_t noseW = Simd::Min<size_t>(4, dstW + 1);
                size_t noseH = Simd::Min<size_t>(4, dstH + 1);
                size_t start = pad ? 2 : 0;
                if (pad)
                {
                    if (dstH == dstH2)
                        dstH2 -= 2;
                    if (dstW == dstW2)
                        dstW2 -= 2;
                    src -= (srcWidth + 1)*srcChannels;
                }            
                size_t tailW = dstW - dstW2 + (pad ? 1 : 2);
                size_t tailH = dstH - dstH2 + (pad ? 1 : 2);
                size_t row = 0, col = 0;
                if (pad)
                {
                    if (pad)
                        Winograd2x3SetInput4t(src, srcWidth, srcChannels, 1, noseH, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW2; col += 2)
                        Winograd2x3SetInput4t(src + col * srcChannels, srcWidth, srcChannels, 1, noseH, 0, 4, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd2x3SetInput4t(src + col * srcChannels, srcWidth, srcChannels, 1, noseH, 0, tailW, dst, dstStride), dst += srcChannels;
                }
                for (row = start; row < dstH2; row += 2)
                {
                    if (pad)
                        Winograd2x3SetInput4t(src + row * srcWidth * srcChannels, srcWidth, srcChannels, 0, 4, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW2; col += 2)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, 4, 0, tailW, dst, dstStride), dst += srcChannels;
                }
                if (row < dstH)
                {
                    if (pad)
                        Winograd2x3SetInput4t(src + row * srcWidth* srcChannels, srcWidth, srcChannels, 0, tailH, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW2; col += 2)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, tailH, 0, 4, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, tailH, 0, tailW, dst, dstStride), dst += srcChannels;
                }
            }
            else
            {
                size_t dstW8 = AlignLo(dstW, 8);
                if (pad && dstW8 == dstW)
                    dstW8 -= 8;
                PadType rowPad = dstH2 < dstH ? PadTail1 : PadNone;
                PadType colPad = dstW2 < dstW ? PadTail1 : PadNone;
                size_t tailCol = dstW2 < dstW ? dstW - 7 : dstW - 8;
                size_t tailRow = dstH2 < dstH ? dstH - 1 : dstH - 2;
                bool specialColTail = dstW8 < dstW || pad;
                bool specialRowTail = dstH2 < dstH || pad;
                if (pad)
                {
                    src -= srcWidth + 1;
                    rowPad = dstH2 < dstH ? PadTail2 : PadTail1;
                    colPad = dstW2 < dstW ? PadTail2 : PadTail1;
                    if (dstH2 == dstH)
                        dstH2 -= 2;
                }
                for (size_t c = 0; c < srcChannels; ++c)
                {
                    size_t row = 0, tileY = 0;
                    if (pad)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + row * srcWidth;
                        float * d = dst + tileY * tileW;
                        if (pad)
                            Winograd2x3SetInput4n(s + col, srcWidth, PadNose1, PadNose1, d + tileX, dstStride), col += 8, tileX += 4;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetInput4n(s + col, srcWidth, PadNose1, PadNone, d + tileX, dstStride);
                        if (specialColTail)
                            Winograd2x3SetInput4n(s + tailCol, srcWidth, PadNose1, colPad, d + tileW - 4, dstStride);
                        row += 2, tileY += 1;
                    }
                    for (; row < dstH2; row += 2, tileY += 1)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + row * srcWidth;
                        float * d = dst + tileY * tileW;
                        if (pad)
                            Winograd2x3SetInput4n(s + col, srcWidth, PadNone, PadNose1, d + tileX, dstStride), col += 8, tileX += 4;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetInput4n(s + col, srcWidth, d + tileX, dstStride);
                        if (specialColTail)
                            Winograd2x3SetInput4n(s + tailCol, srcWidth, PadNone, colPad, d + tileW - 4, dstStride);
                    }
                    if (specialRowTail)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + tailRow * srcWidth;
                        float * d = dst + (tileH - 1) * tileW;
                        if (pad)
                            Winograd2x3SetInput4n(s + col, srcWidth, rowPad, PadNose1, d + tileX, dstStride), col += 8, tileX += 4;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetInput4n(s + col, srcWidth, rowPad, PadNone, d + tileX, dstStride);
                        if (specialColTail)
                            Winograd2x3SetInput4n(s + tailCol, srcWidth, rowPad, colPad, d + tileW - 4, dstStride);
                    }
                    src += srcWidth * srcHeight;
                    dst += tileW * tileH;
                }
            }
        }

        SIMD_INLINE void Winograd2x3SetOutputLoad4(const float * src, size_t stride, __m128 * dst)
        {
            __m128 s0 = _mm_loadu_ps(src + 0 * stride);
            __m128 s1 = _mm_loadu_ps(src + 1 * stride);
            __m128 s2 = _mm_loadu_ps(src + 2 * stride);
            __m128 s3 = _mm_loadu_ps(src + 3 * stride);
            dst[0] = _mm_add_ps(_mm_add_ps(s0, s1), s2);
            dst[1] = _mm_sub_ps(_mm_sub_ps(s1, s2), s3);
        }

        SIMD_INLINE void Winograd2x3SetOutputLoad16(const float * src, size_t stride, __m128 * dst)
        {
            __m128 tmp[8];
            Winograd2x3SetOutputLoad4(src + 0 * stride, stride, tmp + 0);
            Winograd2x3SetOutputLoad4(src + 4 * stride, stride, tmp + 2);
            Winograd2x3SetOutputLoad4(src + 8 * stride, stride, tmp + 4);
            Winograd2x3SetOutputLoad4(src + 12 * stride, stride, tmp + 6);
            dst[0] = _mm_add_ps(_mm_add_ps(tmp[0], tmp[2]), tmp[4]);
            dst[1] = _mm_add_ps(_mm_add_ps(tmp[1], tmp[3]), tmp[5]);
            dst[2] = _mm_sub_ps(_mm_sub_ps(tmp[2], tmp[4]), tmp[6]);
            dst[3] = _mm_sub_ps(_mm_sub_ps(tmp[3], tmp[5]), tmp[7]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4n(const float * src, size_t srcStride, float * dst, size_t dstStride)
        {
            __m128 tmp[4];
            Winograd2x3SetOutputLoad16(src, srcStride, tmp);
            _mm_storeu_ps(dst + 0 * dstStride + 0, _mm_unpacklo_ps(tmp[0], tmp[1]));
            _mm_storeu_ps(dst + 0 * dstStride + 4, _mm_unpackhi_ps(tmp[0], tmp[1]));
            _mm_storeu_ps(dst + 1 * dstStride + 0, _mm_unpacklo_ps(tmp[2], tmp[3]));
            _mm_storeu_ps(dst + 1 * dstStride + 4, _mm_unpackhi_ps(tmp[2], tmp[3]));
        }

        SIMD_INLINE void Winograd2x3SetOutput4n(const float * src, size_t srcStride, float * dst, size_t dstStride, bool lastRow, bool lastCol, const __m128 & mask)
        {
            __m128 tmp[4];
            Winograd2x3SetOutputLoad16(src, srcStride, tmp);
            _mm_storeu_ps(dst + 0, _mm_unpacklo_ps(tmp[0], tmp[1]));
            if(lastCol)
                _mm_storeu_ps(dst + 4, _mm_unpackhi_ps(tmp[0], tmp[1]));
            else
                StoreMasked<false>(dst + 4, _mm_unpackhi_ps(tmp[0], tmp[1]), mask);
            if (lastRow)
            {
                dst += dstStride;
                _mm_storeu_ps(dst + 0, _mm_unpacklo_ps(tmp[2], tmp[3]));
                if (lastCol)
                    _mm_storeu_ps(dst + 4, _mm_unpackhi_ps(tmp[2], tmp[3]));
                else
                    StoreMasked<false>(dst + 4, _mm_unpackhi_ps(tmp[2], tmp[3]), mask);
            }
        }

        SIMD_INLINE void Winograd2x3SetOutputStore4(const __m128 src[4], float * dst, size_t dstS, size_t dstC)
        {
            _mm_storeu_ps(dst + 0 * dstS + 0 * dstC, src[0]);
            _mm_storeu_ps(dst + 0 * dstS + 1 * dstC, src[1]);
            _mm_storeu_ps(dst + 1 * dstS + 0 * dstC, src[2]);
            _mm_storeu_ps(dst + 1 * dstS + 1 * dstC, src[3]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4t(const float * src, size_t srcStride, float * dst, size_t dstW, size_t dstC)
        {
            size_t dstS = dstW * dstC, dstCF = AlignLo(dstC, F);
            for (size_t d = 0; d < dstCF; d += F)
            {
                __m128 tmp[4];
                Winograd2x3SetOutputLoad16(src + d, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + d, dstS, dstC);
            }
            if (dstCF < dstC)
            {
                __m128 tmp[4];
                Winograd2x3SetOutputLoad16(src + dstC - F, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + dstC - F, dstS, dstC);
            }
        }

        SIMD_INLINE void Winograd2x3SetOutputStore4(const __m128 src[4], float * dst, size_t dstS, size_t dstC, size_t rowE, size_t colE)
        {
            for (size_t row = 0; row < rowE; ++row)
                for (size_t col = 0; col < colE; ++col)
                    _mm_storeu_ps(dst + row * dstS + col * dstC, src[row * 2 + col]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4t(const float * src, size_t srcStride, float * dst, size_t dstW, size_t dstC, size_t rowE, size_t colE)
        {
            size_t dstS = dstW * dstC, dstCF = AlignLo(dstC, F);
            for (size_t d = 0; d < dstCF; d += F)
            {
                __m128 tmp[4];
                Winograd2x3SetOutputLoad16(src + d, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + d, dstS, dstC, rowE, colE);
            }
            if (dstCF < dstC)
            {
                __m128 tmp[4];
                Winograd2x3SetOutputLoad16(src + dstC - F, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + dstC - F, dstS, dstC, rowE, colE);
            }
        }

        void Winograd2x3SetOutput(const float * src, float * dst, size_t dstChannels, size_t dstHeight, size_t dstWidth, SimdBool trans)
        {
            if (trans ? (dstChannels < 4) : (dstHeight < 2 || dstWidth < 8))
            {
                Base::Winograd2x3SetOutput(src, dst, dstChannels, dstHeight, dstWidth, trans);
                return;
            }
            size_t tileH = (dstHeight + 1) / 2;
            size_t tileW = (dstWidth + 1) / 2;
            size_t srcStride = dstChannels*tileH*tileW;
            size_t dstH2 = AlignLo(dstHeight, 2);
            size_t dstW2 = AlignLo(dstWidth, 2);
            if (trans)
            {
                size_t row, col;
                for (row = 0; row < dstH2; row += 2)
                {
                    for (col = 0; col < dstW2; col += 2)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels), src += dstChannels;
                    if (col < dstWidth)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, 2, dstWidth - col), src += dstChannels;
                }
                if (row < dstHeight)
                {
                    for (col = 0; col < dstW2; col += 2)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, dstHeight - row, 2), src += dstChannels;
                    if (col < dstWidth)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, dstHeight - row, dstWidth - col), src += dstChannels;
                }
            }
            else
            {
                size_t dstW8 = AlignLo(dstWidth, 8);
                __m128 tailMask = LeftNotZero(4 + dstW2 - dstWidth);
                size_t tailCol = dstW2 < dstWidth ? dstWidth - 7 : dstWidth - 8;
                size_t tailRow = dstH2 < dstHeight ? dstHeight - 1 : dstHeight - 2;
                for (size_t c = 0; c < dstChannels; ++c)
                {
                    size_t row = 0, tileY = 0;
                    for (; row < dstH2; row += 2, tileY += 1)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + tileY * tileW;
                        float * d = dst + row * dstWidth;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetOutput4n(s + tileX, srcStride, d + col, dstWidth);
                        if (col < dstWidth)
                            Winograd2x3SetOutput4n(s + tileW - 4, srcStride, d + tailCol, dstWidth, true, false, tailMask);
                    }
                    if (row < dstHeight)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + (tileH - 1) * tileW;
                        float * d = dst + (dstHeight - 1) * dstWidth;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetOutput4n(s + tileX, srcStride, d + col, dstWidth, false, true, tailMask);
                        if (col < dstWidth)
                            Winograd2x3SetOutput4n(s + tileW - 4, srcStride, d + tailCol, dstWidth, false, false, tailMask);
                    }
                    src += tileW * tileH;
                    dst += dstHeight * dstWidth;
                }
            }
        }

        SIMD_INLINE void Winograd4x3SetFilter4Row(const __m128 * t, float * dst, size_t stride)
        {
            const __m128 r4 = _mm_set1_ps(1.0f / 4.0f);
            const __m128 r6 = _mm_set1_ps(1.0f / 6.0f);
            const __m128 mr6 = _mm_set1_ps(-1.0f / 6.0f);
            const __m128 r12 = _mm_set1_ps(1.0f / 12.0f);
            const __m128 r24 = _mm_set1_ps(1.0f / 24.0f);
            _mm_storeu_ps(dst + 0 * stride, _mm_mul_ps(r4, t[0]));
            __m128 t0 = _mm_add_ps(t[0], t[2]);
            _mm_storeu_ps(dst + 1 * stride, _mm_mul_ps(mr6, _mm_add_ps(t0, t[1])));
            _mm_storeu_ps(dst + 2 * stride, _mm_mul_ps(mr6, _mm_sub_ps(t0, t[1])));
            __m128 t1 = _mm_add_ps(_mm_mul_ps(r24, t[0]), _mm_mul_ps(r6, t[2]));
            __m128 t2 = _mm_mul_ps(r12, t[1]);
            _mm_storeu_ps(dst + 3 * stride, _mm_add_ps(t1, t2));
            _mm_storeu_ps(dst + 4 * stride, _mm_sub_ps(t1, t2));
            _mm_storeu_ps(dst + 5 * stride, t[2]);
        }

        SIMD_INLINE void Winograd4x3SetFilter4All(const __m128 * s, float * dst, size_t stride)
        {
            const __m128 r4 = _mm_set1_ps(1.0f / 4.0f);
            const __m128 r6 = _mm_set1_ps(1.0f / 6.0f);
            const __m128 mr6 = _mm_set1_ps(-1.0f / 6.0f);
            const __m128 r12 = _mm_set1_ps(1.0f / 12.0f);
            const __m128 r24 = _mm_set1_ps(1.0f / 24.0f);

            __m128 t[3];
            t[0] = _mm_mul_ps(r4, s[0]);
            t[1] = _mm_mul_ps(r4, s[1]);
            t[2] = _mm_mul_ps(r4, s[2]);
            Winograd4x3SetFilter4Row(t, dst + 0 * stride, stride);

            t[0] = _mm_mul_ps(mr6, _mm_add_ps(_mm_add_ps(s[0], s[3]), s[6]));
            t[1] = _mm_mul_ps(mr6, _mm_add_ps(_mm_add_ps(s[1], s[4]), s[7]));
            t[2] = _mm_mul_ps(mr6, _mm_add_ps(_mm_add_ps(s[2], s[5]), s[8]));
            Winograd4x3SetFilter4Row(t, dst + 6 * stride, stride);

            t[0] = _mm_mul_ps(mr6, _mm_add_ps(_mm_sub_ps(s[0], s[3]), s[6]));
            t[1] = _mm_mul_ps(mr6, _mm_add_ps(_mm_sub_ps(s[1], s[4]), s[7]));
            t[2] = _mm_mul_ps(mr6, _mm_add_ps(_mm_sub_ps(s[2], s[5]), s[8]));
            Winograd4x3SetFilter4Row(t, dst + 12 * stride, stride);

            t[0] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(r24, s[0]), _mm_mul_ps(r12, s[3])), _mm_mul_ps(r6, s[6]));
            t[1] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(r24, s[1]), _mm_mul_ps(r12, s[4])), _mm_mul_ps(r6, s[7]));
            t[2] = _mm_add_ps(_mm_add_ps(_mm_mul_ps(r24, s[2]), _mm_mul_ps(r12, s[5])), _mm_mul_ps(r6, s[8]));
            Winograd4x3SetFilter4Row(t, dst + 18 * stride, stride);

            t[0] = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(r24, s[0]), _mm_mul_ps(r12, s[3])), _mm_mul_ps(r6, s[6]));
            t[1] = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(r24, s[1]), _mm_mul_ps(r12, s[4])), _mm_mul_ps(r6, s[7]));
            t[2] = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(r24, s[2]), _mm_mul_ps(r12, s[5])), _mm_mul_ps(r6, s[8]));
            Winograd4x3SetFilter4Row(t, dst + 24 * stride, stride);

            Winograd4x3SetFilter4Row(s + 6, dst + 30 * stride, stride);
        }


        SIMD_INLINE void Winograd4x3SetFilter4n(const float * src, float * dst, size_t stride)
        {
            __m128 s[9];
            Load4(src + 0, 9, s + 0);
            Load4(src + 4, 9, s + 4);
            s[8] = _mm_setr_ps(src[8], src[17], src[26], src[35]); 
            Winograd4x3SetFilter4All(s, dst + 0 * stride, stride);
        }

        SIMD_INLINE void Winograd4x3SetFilter4t(const float * src, float * dst, size_t stride)
        {
            __m128 s[9];
            s[0] = _mm_loadu_ps(src + 0 * stride);
            s[1] = _mm_loadu_ps(src + 1 * stride);
            s[2] = _mm_loadu_ps(src + 2 * stride);
            s[3] = _mm_loadu_ps(src + 3 * stride);
            s[4] = _mm_loadu_ps(src + 4 * stride);
            s[5] = _mm_loadu_ps(src + 5 * stride);
            s[6] = _mm_loadu_ps(src + 6 * stride);
            s[7] = _mm_loadu_ps(src + 7 * stride);
            s[8] = _mm_loadu_ps(src + 8 * stride);
            Winograd4x3SetFilter4All(s, dst + 0 * stride, stride);
        }

        void Winograd4x3SetFilter(const float * src, size_t size, float * dst, SimdBool trans)
        {
            size_t size4 = AlignLo(size, 4), i = 0;
            if (trans)
            {
                for (; i < size4; i += 4)
                    Winograd4x3SetFilter4t(src + i, dst + i, size);
                for (; i < size; i += 1)
                    Base::Winograd4x3SetFilter1t(src + i, dst + i, size);
            }
            else
            {
                for (; i < size4; i += 4, src += 36, dst += 4)
                    Winograd4x3SetFilter4n(src, dst, size);
                for (; i < size; i += 1, src += 9, dst += 1)
                    Base::Winograd4x3SetFilter1n(src, dst, size);
            }
        }
    }
#endif// SIMD_SSE_ENABLE
}