/**
 * PEIGEN: a Platform for Evaluation, Implementation, and Generation of S-boxes
 *
 *  Copyright 2019 by
 *    Zhenzhen Bao <baozhenzhen10[at]gmail.com>
 *    Jian Guo <guojian[at]ntu.edu.sg>
 *    San Ling <lingsan[at]ntu.edu.sg>
 *    Yu Sasaki <yu[dot]sasaki[dot]sk@hco.ntt.co.jp>
 * 
 * This platform is developed based on the open source application
 * <http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz>
 * Optimizing Implementations of Lightweight Building Blocks
 *
 *  Copyright 2017 by
 *    Jade Tourteaux <Jade[dot]Tourteaux[at]gmail.com>
 *    Jérémy Jean <Jean[dot]Jeremy[at]gmail.com>
 * 
 * We follow the same copyright policy.
 * 
 * This file is part of some open source application.
 * 
 * Some open source application is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * 
 * Some open source application is distributed in the hope that it will 
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#ifndef CONSTANTS_H__
#define CONSTANTS_H__

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <inttypes.h>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <set>
#include <tuple>
#include <stack>
#include <cmath>
#include "omp.h"
#include <limits>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <bitset>
#include <immintrin.h>
#include <tmmintrin.h>
#include <utility>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "static_sort.h"
#pragma GCC diagnostic pop
#include "HadamardMatrix.hpp"

using namespace std;

namespace Peigen
{

namespace depth
{
template<int N> class faster;
template<int N> struct bool_function_t;
template<int N> struct function_t;
struct bool_op_t;
}

template<int N> struct function_t;
template<int N> struct evaluator;

namespace weight
{
template<int N> class lighter;
struct bool_op_t;
}

}


#define NDEBUG
#ifdef NDEBUG
#define PRINT(x) ((void)0)
#else
#define PRINT(x) x
#endif

//#define NCHECKR
#ifdef NCHECKR
#define CHECKR(x) ((void)0)
#else
#define CHECKR(x) x
#endif
//#define NCHECKAB
#ifdef NCHECKAB
#define CHECK(x) ((void)0)
#else
#define CHECK(x) x
#endif

#define ALIGNED_(x) __attribute__((aligned(x)))
#define ALIGNED_TYPE_(t,x) t ALIGNED_(x)

template<int N>
using UINT_ =
  typename std::conditional<N <= 3, std::uint8_t,
    typename std::conditional<N == 4, std::uint16_t,
      typename std::conditional<N == 5, std::uint32_t,
        typename std::conditional<N == 6, std::uint64_t, std::uint64_t
        >::type
      >::type
    >::type
  >::type;

#define UINT_MAX (numeric_limits<UINT_<N> >::max())

#define FACT_(n) \
  (n == 0 ? 1 : \
    (n == 1 ? 1 : \
      (n == 2 ? 2 : \
        (n == 3 ? 6 : \
          (n == 4 ? 24 : \
            (n == 5 ? 120 : \
              (n == 6 ? 720 : \
                (n == 7 ? 5040 : \
                  (n == 8 ? 40320 : \
                    (n == 9 ? 362880 : \
                      (n == 10 ? 3628800 : \
                        (n == 11 ? 39916800 : \
                          (n == 12 ? 479001600 :  \
                            (numeric_limits<int>::max()) \
                          ) \
                        ) \
                      ) \
                    ) \
                  ) \
                ) \
              ) \
            ) \
          ) \
        ) \
      ) \
    ) \
  )

#define C(n, k) (FACT_(n) /(FACT_(k) * FACT_(n - (k))))

#define digits(x)         \
{                         \
  ((x) < 10 ? 1 :         \
  ((x) < 100 ? 2 :        \
  ((x) < 1000 ? 3 :       \
  ((x) < 10000 ? 4 :      \
  ((x) < 100000 ? 5 :     \
  ((x) < 1000000 ? 6 :    \
  ((x) < 10000000 ? 7 :   \
  ((x) < 100000000 ? 8 :  \
  ((x) < 1000000000 ? 9 : \
  10)))))))))             \
}


#define UNIT_BIT_N (8*sizeof(UINT_<N>))
#define UNIT_NIBBLE_N (2*sizeof(UINT_<N>))
#define BIT_SLICE_BITS_N (1 << N)
#define BIT_SLICE_NIBBLES_N ((1 << N) >> 2 )
#define BIT_SLICE_BYTES_N ((1 << N) >> 3 )
#define BIT_SLICE_ULLS_N ((1 << N) >> 6 )

#define UNIT_N (BIT_SLICE_BYTES_N/sizeof(UINT_<N>))


#define LUT_UNIT_N (1 << N)
#define LUT_UNIT_NIBBLE_N ((N+3)/4)
#define LUT_NIBBLE_N (LUT_UNIT_NIBBLE_N * LUT_UNIT_N)
#define LUT_UNIT_BYTE_N ((N+7)/8)
#define LUT_BYTE_N (LUT_UNIT_BYTE_N * LUT_UNIT_N)

#define LUT_ULL_N ((LUT_BYTE_N + 7) / 8)
#define LUT_XMM_N ((LUT_UNIT_N + 15) / 16)

template<int N>
using bit_slice_l_t = std::array<UINT_<N>, UNIT_N>;

template<int N>
using bit_slice_t = std::array<bit_slice_l_t<N>, N>;

template<int N>
using bit_slice_l_PE_t = std::pair<bit_slice_l_t<N>, int>;

template<int N>
using bit_slice_PE_t = std::array<bit_slice_l_PE_t<N>, N>;

template<int N>
inline bool operator < (const bit_slice_l_PE_t<N> & lhs, const bit_slice_l_PE_t<N> & rhs)
{
  return (lhs.first < rhs.first);
}

template<int N>
inline bool operator == (const bit_slice_l_PE_t<N> & lhs, const bit_slice_l_PE_t<N> & rhs)
{
  return (lhs.first == rhs.first);
}

template<typename T>
inline T operator ^ (const T& lhs, const T& rhs)
{
  T tmp;
  for(int i = 0; i < lhs.size(); i++)
  {
    tmp[i] = lhs[i] ^ rhs[i];
  }
  return tmp;
}

template<typename T>
inline T operator | (const T& lhs, const T& rhs)
{
  T tmp;
  for(int i = 0; i < lhs.size(); i++)
  {
    tmp[i] = lhs[i] | rhs[i];
  }
  return tmp;
}

template<typename T>
inline T operator & (const T& lhs, const T& rhs)
{
  T tmp;
  for(int i = 0; i < lhs.size(); i++)
  {
    tmp[i] = lhs[i] & rhs[i];
  }
  return tmp;
}

template<typename T>
inline T operator ~ (const T& rhs)
{
  T tmp;
  for(int i = 0; i < rhs.size(); i++)
  {
    tmp[i] = ~rhs[i];
  }
  return tmp;
}

const __m128i zero_128 = _mm_setzero_si128();
const __m256i zero_256 = _mm256_setzero_si256();
const __m128i one_128 = _mm_set_epi64x(0xffffffffffffffffULL, 0xffffffffffffffffULL);

const  __m128i  S4_I = _mm_set_epi8(0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0);

const __m128i x  = _mm_set_epi8(0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0);
const __m128i x0 = _mm_set_epi8(0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
const __m128i x1 = _mm_set_epi8(0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1);
const __m128i x2 = _mm_set_epi8(0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2);
const __m128i x3 = _mm_set_epi8(0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3);
const __m128i x4 = _mm_set_epi8(0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4);
const __m128i x5 = _mm_set_epi8(0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5);
const __m128i x6 = _mm_set_epi8(0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6);
const __m128i x7 = _mm_set_epi8(0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7);
const __m128i x8 = _mm_set_epi8(0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8);
const __m128i x9 = _mm_set_epi8(0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9);
const __m128i xa = _mm_set_epi8(0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa);
const __m128i xb = _mm_set_epi8(0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb);
const __m128i xc = _mm_set_epi8(0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc);
const __m128i xd = _mm_set_epi8(0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd);
const __m128i xe = _mm_set_epi8(0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe);
const __m128i xf = _mm_set_epi8(0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf);

const __m128i S3_valv_mask8 = _mm_set_epi64x(0x0ULL, 0xffffffULL);
const __m128i S3_valv_mask4 = _mm_set_epi64x(0x0ULL, 0xf0f0f0ULL);
const __m128i S3_valv_mask2 = _mm_set_epi64x(0x0ULL, 0xccccccULL);
const __m128i S3_valv_mask1 = _mm_set_epi64x(0x0ULL, 0xaaaaaaULL);

const __m128i S3_comps_ind_mask  = _mm_set_epi64x(0x0ULL, 0x8080808080808080ULL);
const __m128i S3_comps_deg_mask3 = _mm_set_epi64x(0x0ULL, 0x8080808080808080ULL); // 1000 0000
const __m128i S3_comps_deg_mask2 = _mm_set_epi64x(0x0ULL, 0x6868686868686868ULL); // 0110 1000
const __m128i S3_comps_deg_mask1 = _mm_set_epi64x(0x0ULL, 0x1616161616161616ULL); // 0001 0110

const __m128i S3_coors_ind_mask  = _mm_set_epi64x(0x0ULL, 0x808080ULL);
const __m128i S3_coors_deg_mask3 = _mm_set_epi64x(0x0ULL, 0x808080ULL); // 1000 0000
const __m128i S3_coors_deg_mask2 = _mm_set_epi64x(0x0ULL, 0x686868ULL); // 0110 1000
const __m128i S3_coors_deg_mask1 = _mm_set_epi64x(0x0ULL, 0x161616ULL); // 0001 0110

const __m128i S4_valv_maskg = _mm_set_epi64x(0x0ULL, 0xffffffffffffffffULL);
const __m128i S4_valv_mask8 = _mm_set_epi64x(0x0ULL, 0xff00ff00ff00ff00ULL);
const __m128i S4_valv_mask4 = _mm_set_epi64x(0x0ULL, 0xf0f0f0f0f0f0f0f0ULL);
const __m128i S4_valv_mask2 = _mm_set_epi64x(0x0ULL, 0xccccccccccccccccULL);
const __m128i S4_valv_mask1 = _mm_set_epi64x(0x0ULL, 0xaaaaaaaaaaaaaaaaULL);

const __m256i S4_comps_ind_mask  = _mm256_set_epi64x(0x8000800080008000ULL, 0x8000800080008000ULL, 0x8000800080008000ULL, 0x8000800080008000ULL);                                                                                                                                           // fedc ba98 7654 3210
const __m256i S4_comps_deg_mask4 = _mm256_set_epi64x(0x8000800080008000ULL, 0x8000800080008000ULL, 0x8000800080008000ULL, 0x8000800080008000ULL); // 1000 0000 0000 0000
const __m256i S4_comps_deg_mask3 = _mm256_set_epi64x(0x6880688068806880ULL, 0x6880688068806880ULL, 0x6880688068806880ULL, 0x6880688068806880ULL); // 0110 1000 1000 0000
const __m256i S4_comps_deg_mask2 = _mm256_set_epi64x(0x1668166816681668ULL, 0x1668166816681668ULL, 0x1668166816681668ULL, 0x1668166816681668ULL); // 0001 0110 0110 1000
const __m256i S4_comps_deg_mask1 = _mm256_set_epi64x(0x0116011601160116ULL, 0x0116011601160116ULL, 0x0116011601160116ULL, 0x0116011601160116ULL); // 0000 0001 0001 0110

const __m128i S4_coors_ind_mask  = _mm_set_epi64x(0x0ULL, 0x8000800080008000ULL);                                                                                                                                                      // fedc ba98 7654 3210
const __m128i S4_coors_deg_mask4 = _mm_set_epi64x(0x0ULL, 0x8000800080008000ULL); // 1000 0000 0000 0000
const __m128i S4_coors_deg_mask3 = _mm_set_epi64x(0x0ULL, 0x6880688068806880ULL); // 0110 1000 1000 0000
const __m128i S4_coors_deg_mask2 = _mm_set_epi64x(0x0ULL, 0x1668166816681668ULL); // 0001 0110 0110 1000
const __m128i S4_coors_deg_mask1 = _mm_set_epi64x(0x0ULL, 0x0116011601160116ULL); // 0000 0001 0001 0110

template<int N>
const array<int, N+1> HWorder_off = [&]
{
  array<int, N+1> aHWorder_off;
  aHWorder_off[0] = 0;
  for (int i = 1; i <= N; i++)
  {
    aHWorder_off[i] = aHWorder_off[i - 1] + C(N, i-1);
  }
  return aHWorder_off;
}();

template<int N>
const array<int, 1<<N> HWorder = [&]
{
  array<int, 1<<N> aHWorder;
  int offset[N + 1] = { 0 };
  offset[0] = 0;
  for (int i = 1; i <= N; i++)
  {
    offset[i] = offset[i - 1] + C(N, i-1);
  }
  for (int i = 0; i < (1<<N); i++)
  {
    int hw = _mm_popcnt_u32(i);
    aHWorder[offset[hw]] = i;
    offset[hw]++;
  }
  return aHWorder;
}();


template<int N>
struct OPs_t
{
  bit_slice_l_t<N> sll (const bit_slice_l_t<N> & lhs, const int offsite) const
  {
    bit_slice_l_t<N> res = {{}};
  
    UINT_<N> inter_offsite = (UINT_<N>)offsite / (UINT_<N>)UNIT_BIT_N;
  
    UINT_<N> * src_inter_right = (UINT_<N> *)lhs.data();
    UINT_<N> * dst_inter_right = (UINT_<N> *)res.data();
    UINT_<N> * dst_inter_left = dst_inter_right + inter_offsite;
  
    memcpy(dst_inter_left, src_inter_right, (UNIT_N - inter_offsite)*sizeof(UINT_<N>));
  
    UINT_<N> intra_offsite = (UINT_<N>)offsite % (UINT_<N>)UNIT_BIT_N;

    if (intra_offsite != 0)
    {
      bit_slice_l_t<N> left = {{}};
      bit_slice_l_t<N> right = {{}};

      UINT_<N> * src_intra = (UINT_<N> *)res.data();
      UINT_<N> * dst_intra_right = (UINT_<N> *)right.data();
      UINT_<N> * dst_intra_left = (UINT_<N> *)left.data();
      for (int i = 0; i < UNIT_N - 1; i++)
      {
        dst_intra_right[i+1] = src_intra[i] >> ((UINT_<N>)UNIT_BIT_N - intra_offsite);
      }
      dst_intra_right[0] = 0;
      for (int i = 0; i < UNIT_N; i++)
      {
        dst_intra_left[i] = src_intra[i] << intra_offsite;
        src_intra[i] = dst_intra_left[i] | dst_intra_right[i];
      }
    }
    return res;
  }

  bit_slice_l_t<N> srl (const bit_slice_l_t<N> & lhs, const int offsite) const
  {
    bit_slice_l_t<N> res = {{}};
  
    UINT_<N> inter_offsite = (UINT_<N>)offsite / (UINT_<N>)UNIT_BIT_N;
  
    UINT_<N> * src_inter_right = (UINT_<N> *)lhs.data();
    UINT_<N> * src_inter_left  = src_inter_right + inter_offsite;
    UINT_<N> * dst_inter_right = (UINT_<N> *)res.data();
  
    memcpy(dst_inter_right, src_inter_left, (UNIT_N - inter_offsite)*sizeof(UINT_<N>));
  
    UINT_<N> intra_offsite = (UINT_<N>)offsite % (UINT_<N>)UNIT_BIT_N;

    if (intra_offsite != 0)
    {
      bit_slice_l_t<N> left = {{}};
      bit_slice_l_t<N> right = {{}};

      UINT_<N> * src_intra = (UINT_<N> *)res.data();
      UINT_<N> * dst_intra_right = (UINT_<N> *)right.data();
      UINT_<N> * dst_intra_left = (UINT_<N> *)left.data();
      for (int i = 0; i < UNIT_N - 1; i++)
      {
        dst_intra_left[i] = src_intra[i+1] << ((UINT_<N>)UNIT_BIT_N - intra_offsite);
      }
      dst_intra_left[UNIT_N - 1] = 0;
      for (int i = 0; i < UNIT_N; i++)
      {
        dst_intra_right[i] = src_intra[i] >> intra_offsite;
        src_intra[i] = dst_intra_left[i] | dst_intra_right[i];
      }
    }
    return res;
  }

  bit_slice_l_t<N> rotl (const bit_slice_l_t<N> & lhs, const int offsite) const
  {
    bit_slice_l_t<N> res = {{}};
  
    UINT_<N> inter_offsite = (UINT_<N>) offsite / (UINT_<N>) UNIT_BIT_N;
  
    UINT_<N> * src_inter_right = (UINT_<N> *)lhs.data();
    UINT_<N> * src_inter_left = src_inter_right + ((UINT_<N>)UNIT_N - inter_offsite);
    UINT_<N> * dst_inter_right = (UINT_<N> *)res.data();
    UINT_<N> * dst_inter_left = dst_inter_right + inter_offsite;
  
    if (inter_offsite != 0)
    {
      memcpy(dst_inter_right, src_inter_left, inter_offsite * sizeof(UINT_<N>));
      memcpy(dst_inter_left, src_inter_right, ((UINT_<N>) UNIT_N - inter_offsite)*sizeof(UINT_<N>));
    }
    else
    {
      memcpy(dst_inter_left, src_inter_right, (UINT_<N>)UNIT_N * sizeof(UINT_<N>));
    }
  
    UINT_<N> intra_offsite = offsite % UNIT_BIT_N;

    if (intra_offsite != 0)
    {
      bit_slice_l_t<N> left = {{}};
      bit_slice_l_t<N> right = {{}};

      UINT_<N> * src_intra = (UINT_<N> *)res.data();
      UINT_<N> * dst_intra_right = (UINT_<N> *)right.data();
      UINT_<N> * dst_intra_left = (UINT_<N> *)left.data();
      for (int i = 0; i < UNIT_N; i++)
      {
        dst_intra_right[(i+1) % UNIT_N] = src_intra[i] >> ((UINT_<N>) UNIT_BIT_N - intra_offsite);
      }
      for (int i = 0; i < UNIT_N; i++)
      {
        dst_intra_left[i] = src_intra[i] << intra_offsite;
        src_intra[i] = dst_intra_left[i] | dst_intra_right[i];
      }
    }
    return res;
  }
  
  int get_bit(const bit_slice_l_t<N> & lhs, const int offsite) const
  {
    int bit;
    UINT_<N> * src = (UINT_<N> *)lhs.data();
    bit = (src[(UINT_<N>) offsite / (UINT_<N>) UNIT_BIT_N] >> ((UINT_<N>) offsite % (UINT_<N>) UNIT_BIT_N)) & 1;
    return bit;
  }

  bit_slice_l_t<N> set_bit(const bit_slice_l_t<N> & lhs, const int offsite) const
  {
    bit_slice_l_t<N> res = lhs;
    UINT_<N> inter_offsite = (UINT_<N>)offsite / (UINT_<N>)UNIT_BIT_N;
    UINT_<N> intra_offsite = (UINT_<N>)offsite % (UINT_<N>)UNIT_BIT_N;
    res[inter_offsite] |= ((UINT_<N>)1 << intra_offsite);
    return res;
  }

  void set_bit_inplace(bit_slice_l_t<N> & lhs, const int offsite) const
  {
    UINT_<N> inter_offsite = (UINT_<N>)offsite / (UINT_<N>)UNIT_BIT_N;
    UINT_<N> intra_offsite = (UINT_<N>)offsite % (UINT_<N>)UNIT_BIT_N;
    lhs[inter_offsite] |= ((UINT_<N>)1 << intra_offsite);
  }

  bit_slice_l_t<N> unset_bit(const bit_slice_l_t<N> & lhs, const int offsite) const
  {
    bit_slice_l_t<N> res = lhs;
    UINT_<N> inter_offsite = (UINT_<N>)offsite / (UINT_<N>)UNIT_BIT_N;
    UINT_<N> intra_offsite = (UINT_<N>)offsite % (UINT_<N>)UNIT_BIT_N;
    res[inter_offsite] &= ~((UINT_<N>)1 << intra_offsite);
    return res;
  }

  void unset_bit_inplace(bit_slice_l_t<N> & lhs, const int offsite) const
  {
    UINT_<N> inter_offsite = (UINT_<N>)offsite / (UINT_<N>)UNIT_BIT_N;
    UINT_<N> intra_offsite = (UINT_<N>)offsite % (UINT_<N>)UNIT_BIT_N;
    lhs[inter_offsite] &= ~((UINT_<N>)1 << intra_offsite);
  }

  bit_slice_l_t<N> set_zero() const
  {
    bit_slice_l_t<N> res = { {0} };
    return res;
  }

  void set_zero_inplace(bit_slice_l_t<N> & lhs) const
  {
    lhs = lhs ^ lhs;
  }

  bit_slice_l_t<N> set_one() const
  {
    bit_slice_l_t<N> res = { {0} };

    UINT_<N> inter_offsite = (UINT_<N>)BIT_SLICE_BITS_N / (UINT_<N>)UNIT_BIT_N;
    for (int i = 0; i < inter_offsite; i++)
    {
      res[i] = numeric_limits<UINT_<N> >::max();
    }    
    return res;
  }  

  void set_one_inplace(bit_slice_l_t<N> & lhs) const
  {
    UINT_<N> inter_offsite = (UINT_<N>)BIT_SLICE_BITS_N / (UINT_<N>)UNIT_BIT_N;
    for (int i = 0; i < inter_offsite; i++)
    {
      lhs[i] = numeric_limits<UINT_<N> >::max();
    }
  }

  int lsb_idx(bit_slice_l_t<N> & lhs) const
  {
    if (lhs == set_zero()) return -1;

    int base = 0;

    for (int i = 0; i < UNIT_N; i++)
    {
      if (lhs[i] != 0) return base + __builtin_ffsll(lhs[i]) - 1;
      base += UNIT_BIT_N;
    }
    cout << "Should not run into here:" << __LINE__ << endl;
    return -1;
  }

  int unset_lsb_idx_inplace(bit_slice_l_t<N> & lhs) const
  {
    if (lhs == set_zero()) return -1;
    int base = 0;
    for (int i = 0; i < UNIT_N; i++)
    {
      if (lhs[i] != 0)
      {
        int offsite = __builtin_ffsll(lhs[i]) - 1;
        lhs[i] &= ~((UINT_<N>)1 << (UINT_<N>)offsite);
        return base + offsite;
      } 
      base += UNIT_BIT_N;
    }
    cout << "Should not run into here:" << __LINE__ << endl;
    return -1;
  }

  int next_lsb_idx(const bit_slice_l_t<N> & lhs, int idx) const
  {
    if (idx == (LUT_UNIT_N-1)) return -1;
    idx = idx + 1;
    bit_slice_l_t<N> tmp = lhs;
    UINT_<N> inter_offsite = (UINT_<N>)idx / (UINT_<N>)UNIT_BIT_N;
    UINT_<N> intra_offsite = (UINT_<N>)idx % (UINT_<N>)UNIT_BIT_N;
    for (int i = 0; i < inter_offsite; i++)
    {
      tmp[i] = 0;
    }
    if (intra_offsite != 0)
    {
      tmp[inter_offsite] = tmp[inter_offsite] & (~(((UINT_<N>)1 << intra_offsite) - (UINT_<N>)1));
    }

    return lsb_idx(tmp);
  }

  int cnt_1(const bit_slice_l_t<N> & lhs) const
  {
    int sum = 0;
    if (UNIT_BIT_N <= 64)
    {
      sum = _mm_popcnt_u64((uint64_t)lhs[0]);
      return sum;
    }
    for (int i = 0; i < UNIT_N; i++)
    {
      sum += _mm_popcnt_u64((uint64_t)lhs[i]);
    }
    return sum;
  }

  int rank(set<uint8_t, std::greater<uint8_t> > X)
  {
    set<uint8_t, std::greater<uint8_t> > X1 = X;
    set<uint8_t, std::greater<uint8_t> > X2;
    while (X1.size() != 0)
    {
      int maxx = *(X1.begin());
      X1.erase(maxx);            
      X2.insert(maxx);
      if (X1.size() == 0) break;
      int b1 = __builtin_clz(maxx);
      set<uint8_t, std::greater<uint8_t> > X1t;
      for (auto xr : X1)
      {
        int b2 = __builtin_clz(xr);
        uint8_t xn = (b1 == b2) ? (xr ^ maxx) : xr;
        X1t.insert(xn);
      }
      X1 = X1t;
      X1t.clear();
    }
    // cout << "X2 = {" << hex;
    // for (auto nx : X2) cout << nx + '\0' << "," << endl;
    // cout << "}" << dec << endl;
    return X2.size();
  }

  void gen_subspaces(vector<vector<vector<uint8_t> > > & subspaces)
  {
    #define N0 N
    #define Ci(x) (C0[x+1]-1)

    int A0[N0 + 1], T0[N0 + 1], F0[N0 + 1], H0[N0 + 1],  C0[N0 + 1], X0, Y0, I0, L0, Z0;
    int M0;

    array<array<int, N>, N> m;
    vector<pair<int, int> > free_positions;

    vector<vector<uint8_t> > newdim_subspaces;
    vector<uint8_t> anew_subspace;

    for (int dim = 1; dim < N; dim++)
    {
      newdim_subspaces.clear();

      M0 = dim;

			for (int i=0; i<=(N0-M0); i++) A0[i] = 0;	for (int i=N0-M0+1; i<=N0; i++) A0[i] = 1;
			for (int i = 1; i<=M0; i++) { C0[i] = N0 - M0 + i; H0[N0-M0+i] = i; }
			T0[N0-M0] = -1; T0[1] = 0; F0[N0] = N0 - M0 + 1; I0 = N0 - M0; L0 = N0;
      
      do {
        for (int i = 0; i < dim; i++) m[i].fill(0);
        free_positions.clear();
        for (int i = 0; i < dim; i++)
        {
          m[i][Ci(i)] = 1;
          for (int j = Ci(i) + 1; j < N; j++)
          {
            if (A0[j+1] != 1) 
            {
              free_positions.push_back(pair<int, int>(i, j));
            }
          }
        }

        int num_free_pos = free_positions.size();
        for (uint64_t v = 0; v < (1<<num_free_pos); v++)
        {
          for (int fi = 0; fi < num_free_pos; fi++)
          {
            auto freepos = free_positions[fi];
            int rowi = freepos.first;
            int coli = freepos.second;
            m[rowi][coli] = (int)((v >> (uint64_t)fi) & 1ULL);
          }
          anew_subspace.clear();
          for (int ri = 0; ri < dim; ri++)
          {
            uint8_t vec = 0;
            for (int ci = 0; ci < N; ci++) vec |= (m[ri][ci] << ci);
            anew_subspace.push_back(vec);
          }
          anew_subspace.shrink_to_fit();
          newdim_subspaces.push_back(anew_subspace);
        }

				if (I0 == 0)
				{
					break;
				} 
				else
				{
					if (T0[I0] < 0) { if ((-T0[I0]) != (I0-1)){ T0[I0-1] = T0[I0]; } T0[I0] = I0-1; }
					if ( A0[I0]==0 )
					{
						X0 = I0; Y0 = F0[L0]; if (A0[I0-1] == 1){ F0[I0] = F0[I0 - 1]; } else { F0[I0] = I0; }
						if (F0[L0] == L0) { L0 = I0; I0 = T0[I0]; goto CHANGE; }
						if (L0 == N0) { T0[F0[N0]] = -I0 - 1; T0[I0 + 1] = T0[I0]; I0 = F0[N0]; F0[N0] = F0[N0] + 1; goto CHANGE; }
						T0[L0] = -I0-1; T0[I0+1] = T0[I0]; F0[L0] = F0[L0] + 1; I0 = L0; goto CHANGE;
					}
					Y0 = I0;
					if (I0 != L0)
					{
						F0[L0] = X0 = F0[L0] - 1; F0[I0 - 1] = F0[I0];
						if (L0 == N0)
						{
							if (I0 == (F0[N0] - 1)) { I0 = T0[I0]; goto CHANGE; }
							T0[F0[N0]-1] = -I0-1; T0[I0+1] = T0[I0]; I0 = F0[N0] - 1; goto CHANGE;
						}
						T0[L0] = -I0 -1; T0[I0 + 1] = T0[I0]; I0 = L0; goto CHANGE;
					}
					X0 = N0; F0[L0 - 1] = F0[L0]; F0[N0] = N0; L0 = N0;
					if (I0 == N0 - 1) { I0 = T0[N0 - 1]; goto CHANGE; }
					T0[N0 - 1] = -I0 - 1; T0[I0 + 1] = T0[I0]; I0 = N0 - 1;
CHANGE:
					A0[X0] = 1; A0[Y0] = 0; H0[X0] = Z0 = H0[Y0]; C0[Z0] = X0;
				}
			} while (true);

      newdim_subspaces.shrink_to_fit();
      subspaces.push_back(newdim_subspaces);
    }
    #undef Ci
    #undef N0
  }

  void show_subspaces(ofstream &fout)
  {
    fout << hex << setfill('0');
    vector<vector<vector<uint8_t> > > subspaces;
    gen_subspaces(subspaces);
    for (auto newdim_subspaces: subspaces)
    {
      fout << "{" << endl;
      int i = 0;
      for (auto new_subspaces: newdim_subspaces)
      {
        fout << "{";
        for (auto new_base: new_subspaces)
        {
          fout << "0x" << setw(2) << new_base + '\0' << ",";
        }
        fout << "}, ";
        if ((i + 1) % 10 == 0) fout << endl;
      }
      fout << endl << "}," << endl;
    }
  }

  Peigen::function_t<N> composite(const uint8_t sb2[], const uint8_t sb1[])
  {
    Peigen::function_t<N> func;

    __m128i xmmsb1[LUT_XMM_N];
    __m128i xmmsb2[LUT_XMM_N];
    __m128i xmmsb3[LUT_XMM_N];

    if (N == 3)
    {
      xmmsb2[0] =  _mm_load_si128((__m128i *)(sb2));
      xmmsb1[0] =  _mm_load_si128((__m128i *)(sb1));

      __m128i mask = _mm_set_epi64x(0x0, 0xffffffffffffffffULL);
      xmmsb2[0] = _mm_and_si128(xmmsb2[0], mask);
      xmmsb1[0] = _mm_and_si128(xmmsb1[0], mask);
      xmmsb3[0] = _mm_shuffle_epi8(xmmsb2[0], xmmsb1[0]);
    }
    else if (N == 4)
    {
      xmmsb2[0] =  _mm_load_si128((__m128i *)(sb2));
      xmmsb1[0] =  _mm_load_si128((__m128i *)(sb1));

      xmmsb3[0] = _mm_shuffle_epi8(xmmsb2[0], xmmsb1[0]);
    }
    else
    {
      uint8_t sb3[LUT_UNIT_N];
      for (int i = 0; i < LUT_UNIT_N; i++)
      {
        sb3[i] = sb2[sb1[i]];
      }
      for (int i = 0; i < LUT_XMM_N; i++)
      {
        xmmsb3[i] = _mm_loadu_si128((__m128i *)(sb3 + i * 16));
      }
    }

    func.bit_slice = bit_slice(xmmsb3);

    return func;
  }

  void composite(__m128i dst_xmm[LUT_XMM_N], const __m128i src_xmm2[LUT_XMM_N], const __m128i src_xmm1[LUT_XMM_N])
  {
    if ((N == 3) || (N == 4))
    {
      dst_xmm[0] = _mm_shuffle_epi8(src_xmm2[0], src_xmm1[0]);
    }
    else if (N <= 8)
    {
      uint8_t * sb3 = (uint8_t *) dst_xmm;
      uint8_t * sb2 = (uint8_t *) src_xmm2;
      uint8_t * sb1 = (uint8_t *) src_xmm1;
      for (int i = 0; i < LUT_UNIT_N; i++)
      {
        sb3[i] = sb2[sb1[i]];
      }
    }
  }

  bit_slice_t<N> composite(const __m128i xmmsb2[LUT_XMM_N], const __m128i xmmsb1[LUT_XMM_N])
  {
    bit_slice_t<N> s = {{}};
    __m128i xmmsb3[LUT_XMM_N];

    composite(xmmsb3, xmmsb2, xmmsb1);

    for (int i = 0; i < LUT_XMM_N; i++)
    {
        xmmsb3[i] = _mm_slli_epi16(xmmsb3[i], 8 - N);
    }

    for (int i = N - 1; i >= 0; i--)
    {
        for (int j = LUT_XMM_N - 1; j >= 0; j--)
        {
          s[i][(j*16)/UNIT_BIT_N] <<= 16;
          s[i][(j*16)/UNIT_BIT_N] |= (unsigned short)_mm_movemask_epi8(xmmsb3[j]);
          xmmsb3[j] = _mm_slli_epi16(xmmsb3[j], 1);
        } 
    }

    return s;
  }

  
  // From https://www.agner.org/optimize/#vectorclass
  // C++ vector class library
  // Horizontal add: Calculates the sum of all vector elements.
  // Overflow will wrap around
  int32_t horizontal_add (__m128i const & a) {
  #ifdef __XOP__       // AMD XOP instruction set
      __m128i sum1  = _mm_haddq_epi32(a);
      __m128i sum2  = _mm_shuffle_epi32(sum1,0x0E);          // high element
      __m128i sum3  = _mm_add_epi32(sum1,sum2);              // sum
      return          _mm_cvtsi128_si32(sum3);               // truncate to 32 bits
  #elif  INSTRSET >= 4  // SSSE3
      __m128i sum1  = _mm_hadd_epi32(a,a);                   // horizontally add 4 elements in 2 steps
      __m128i sum2  = _mm_hadd_epi32(sum1,sum1);
      return          _mm_cvtsi128_si32(sum2);               // 32 bit sum
  #else                 // SSE2
      __m128i sum1  = _mm_shuffle_epi32(a,0x0E);             // 2 high elements
      __m128i sum2  = _mm_add_epi32(a,sum1);                 // 2 sums
      __m128i sum3  = _mm_shuffle_epi32(sum2,0x01);          // 1 high element
      __m128i sum4  = _mm_add_epi32(sum2,sum3);              // 2 sums
      return          _mm_cvtsi128_si32(sum4);               // 32 bit sum
  #endif
  }

  void mulHM (int D[LUT_UNIT_N][LUT_UNIT_N], const int S1[LUT_UNIT_N][LUT_UNIT_N])
  {
    #if 0
    __m128i HC[(LUT_UNIT_N >> 2)];
    for (int ci = 0; ci < LUT_UNIT_N; ci++)
    {
      switch (N)
      {
        case 3: for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++) HC[wi] = _mm_load_si128(((__m128i *)(H3[ci])) + wi); break;
        case 4: for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++) HC[wi] = _mm_load_si128(((__m128i *)(H4[ci])) + wi); break;
        case 5: for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++) HC[wi] = _mm_load_si128(((__m128i *)(H5[ci])) + wi); break;
        case 6: for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++) HC[wi] = _mm_load_si128(((__m128i *)(H6[ci])) + wi); break;
        case 7: for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++) HC[wi] = _mm_load_si128(((__m128i *)(H7[ci])) + wi); break;
        case 8: for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++) HC[wi] = _mm_load_si128(((__m128i *)(H8[ci])) + wi); break;
        default: break;
      } 

      for (int ri = 0; ri < LUT_UNIT_N; ri++)
      {
        int sum = 0;
        __m128i S1r;
        for (int wi = 0; wi < (LUT_UNIT_N >> 2); wi++)
        {
          S1r = _mm_loadu_si128(((__m128i *)(S1[ri])) + wi);
          sum += horizontal_add(_mm_mullo_epi32(S1r, HC[wi]));
        }
        D[ri][ci] = sum;
      }
    }
    #endif

    #if 1
    int HC[LUT_UNIT_N];
    for (int ci = 0; ci < LUT_UNIT_N; ci++)
    {
      switch (N)
      {
        case 3: for (int ri = 0; ri < LUT_UNIT_N; ri++) HC[ri] = H3[ri][ci]; break;
        case 4: for (int ri = 0; ri < LUT_UNIT_N; ri++) HC[ri] = H4[ri][ci]; break;
        case 5: for (int ri = 0; ri < LUT_UNIT_N; ri++) HC[ri] = H5[ri][ci]; break;
        case 6: for (int ri = 0; ri < LUT_UNIT_N; ri++) HC[ri] = H6[ri][ci]; break;
        case 7: for (int ri = 0; ri < LUT_UNIT_N; ri++) HC[ri] = H7[ri][ci]; break;
        case 8: for (int ri = 0; ri < LUT_UNIT_N; ri++) HC[ri] = H8[ri][ci]; break;
        default: break;
      }
      
      for (int ri = 0; ri < LUT_UNIT_N; ri++)
      {
        int sum = 0;
        for (int wi = 0; wi < LUT_UNIT_N; wi++)
        {
          sum += S1[ri][wi] * HC[wi];
        }
        D[ri][ci] = sum;
      }
    }
    #endif
  }

  template<typename T>
  string show_matrix(T * matrix, int rn, int cn)
  {
    stringstream ss;
    ss << "{" << endl;
    for (int i = 0; i < rn; i++)
    {
      ss << "{";
      for (int j = 0; j < cn; j++)
      {
        ss << setw((int)log10(1<<N)+2) << (*(matrix + i * cn + j)) << ", ";
      }
      ss <<"}," << endl;
    }
    ss << "};" << endl;
    return ss.str();
  }

  template<typename T>
  string show_matrix_HWorder(T matrix[LUT_UNIT_N][LUT_UNIT_N])
  {
    stringstream ss;
    ss << "{" << endl;
    ss << setw((N+3)/4 + 3) << " " << hex;
    for (int j = 0; j < LUT_UNIT_N; j++)
    {
      ss << setw((int)log10(1<<N)+2) << HWorder<N>[j] << "| ";
    }
    ss << endl << setfill(' ') << dec;
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      ss << "{";
      ss << setfill('0') << hex << setw((N+3)/4) << HWorder<N>[i] << ": ";
      ss << setfill(' ') << dec;
      for (int j = 0; j < LUT_UNIT_N; j++)
      {
        ss << setw((int)log10(1<<N)+2) << matrix[HWorder<N>[i]][HWorder<N>[j]] << ", ";
      }
      ss <<"}," << endl;
    }
    ss << "};" << endl;
    return ss.str();
  }

  template<typename T>
  string show_indicate_matrix_HWorder(T matrix[LUT_UNIT_N][LUT_UNIT_N])
  {
    stringstream ss;
    ss << "{" << endl;
    ss << setw((N+3)/4 + 3) << " " << hex;
    for (int j = 0; j < LUT_UNIT_N; j++)
    {
      ss << setw((N+3)/4 + 1) << HWorder<N>[j] << "| ";
    }
    ss << endl << setfill(' ') << dec;
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      ss << "{";
      ss << setfill('0') << hex << setw((N+3)/4) << HWorder<N>[i] << ": ";
      ss << setfill(' ') << dec;
      for (int j = 0; j < LUT_UNIT_N; j++)
      {
        if (matrix[HWorder<N>[i]][HWorder<N>[j]] == 0)
        {
          ss << setw((N+3)/4 + 1) << " " << ", ";
        }
        else
        {
          ss << setw((N+3)/4 + 1) << "x" << ", ";
        }
      }
      ss <<"}," << endl;
    }
    ss << "};" << endl;
    return ss.str();
  }


};

template<int N>
OPs_t<N> OPs;

template<int N>
struct Perm_t
{
  Peigen::function_t<N> * func = NULL;
  int idx[FACT_(N)][N];

  Perm_t()
  {
    Peigen::function_t<N> I = Peigen::function_t<N>::INPUT_DEFAULT();
    func = new Peigen::function_t<N>[FACT_(N)];
    __m128i a;
    __m128i x;
    uint8_t * akp = (uint8_t *)&a;
	  uint8_t * xkp = (uint8_t *)&x;
    int k;
    int ct = 0;

    __m128i shuf[16];
	    shuf[0xf] = _mm_set_epi8(0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0xe] = _mm_set_epi8(15, 0, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0xd] = _mm_set_epi8(15, 14, 0, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0xc] = _mm_set_epi8(15, 14, 13, 0, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0xb] = _mm_set_epi8(15, 14, 13, 12, 0, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0xa] = _mm_set_epi8(15, 14, 13, 12, 11, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0x9] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 0, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0x8] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 0, 8, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0x7] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 0, 7, 6, 5, 4, 3, 2, 1);
	    shuf[0x6] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 0, 6, 5, 4, 3, 2, 1);
	    shuf[0x5] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 0, 5, 4, 3, 2, 1);
	    shuf[0x4] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 0, 4, 3, 2, 1);
	    shuf[0x3] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 0, 3, 2, 1);
	    shuf[0x2] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 0, 2, 1);
	    shuf[0x1] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0, 1);
	    shuf[0x0] = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

      x = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
	  a = x;

    C2:
      for (int i = 0; i < N; i++)
      {
        func[ct].bit_slice[i] = I.bit_slice[akp[i]];
        idx[ct][i] = akp[i];
      }
      func[ct].bit_slice_to_LUT();
      
      ct++;
    	k = N - 1;

    C3:
    	a = _mm_shuffle_epi8(a, shuf[k]);
    	if (akp[k] != xkp[k]) goto C2;

    	k = k - 1;
    	if (k > 0) goto C3;
  }

  ~Perm_t()
  {
    if (func != NULL)
    {
      delete [] func;
    }
  }
};


template<int N>
const Perm_t<N> PE;

template<int N>
const array<Peigen::function_t<N>, (1<<N)> XE = [&]
{
  Peigen::function_t<N> I = Peigen::function_t<N>::INPUT_DEFAULT();
  array<Peigen::function_t<N>, (1<<N)> aXE;
  for (int ix = 0; ix < (1<<N); ix++)
  {
    for (int i = 0; i < N; i++)
    {
      if ((ix >> i) & 1) aXE[ix].bit_slice[i] = ~I.bit_slice[i];
      else aXE[ix].bit_slice[i] = I.bit_slice[i];
    }
    aXE[ix].bit_slice_to_LUT();
  }
  return aXE;
}();

template<int N>
const bit_slice_t<N> masks = [&]
{
  bit_slice_t<N> amask;
  bit_slice_l_t<N> mask_init;
  memset(mask_init.data(), 0xff, sizeof(bit_slice_l_t<N>));
  amask[N - 1] = OPs<N>.sll(mask_init, 1ULL << (N - 1ULL));
  for (int i = N - 2; i >= 0; i--)
  {
    mask_init = OPs<N>.rotl(amask[i+1], 1ULL << i);
    mask_init = mask_init ^ amask[i+1];
    mask_init = ~mask_init;
    amask[i] = mask_init;
  }
  return amask;
}();

#include "OptimalSboxes.hpp"
#include "AffineMatrixColPEBox.hpp"
#include "AffineMatrixRowPEBox.hpp"

template<int N>
struct LExinfoGx_t
{
  int Gx;
  array<uint8_t, LUT_UNIT_N> L1;
  array<uint8_t, LUT_UNIT_N> L2;
  uint8_t c;
};

template<int N>
const map<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> > G_Rs = [&]
{
  map<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> > aG_Rs;
  if (N == 4)
  {
    Peigen::function_t<N> GaF;
    Peigen::function_t<N> GF;
    LExinfoGx_t<N>  LExinfo;
    array<uint8_t, LUT_UNIT_N> R;
    uint8_t * Rp = R.data();
    for (int i = 0; i < OPTIMAL_G4_N; i++)
    {
      LExinfo.Gx = i;
      GF.LUT[0] = _mm_load_si128((__m128i *)G4[i]);
      for (int ai = 0; ai < (1 << N); ai++)
      {
        LExinfo.c = ai;
        OPs<N>.composite(GaF.LUT, GF.LUT, XE<N>[ai].LUT);
        GaF.LE_representative(Rp, LExinfo.L1.data(), LExinfo.L2.data());
        aG_Rs.insert(pair<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> >(R, LExinfo));
      }
    }
  }
  return aG_Rs;
}();

template<int N>
void optimalAEclass_split(array<set<Peigen::function_t<N> >,  OPTIMAL_G4_N> & optimalPXEreps)
{
  if (N == 4)
  {
    string fn;
    ofstream fout;
    Peigen::function_t<N> GF;
    for (int i = 0; i < OPTIMAL_G4_N; i++)
    {
      fn = "optimalPXEreps_G" + to_string(i);
      fn += ".txt";
      fout.open(fn.c_str(), ios::app);
      GF.LUT[0] = _mm_load_si128((__m128i *)G4[i]);
      GF.get_AE_PXEreps(optimalPXEreps[i]);
      int idx = 0;
      for (auto per : optimalPXEreps[i])
      {
        fout << idx << "," << per.LUT_to_string() << endl;
        idx++;
      }
      fout.close();
    }
  }
  else
  {
    cout << "Not support for N != 4" << endl;
  }
}


#include "subspaces.hpp"
#endif //#define CONSTANTS_H__

