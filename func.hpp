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

#ifndef FUNC_H__
#define FUNC_H__

#include "constants.hpp"
using namespace Peigen;
using namespace std;

#define NDEBUG
#ifdef NDEBUG
#define PRINT(x) ((void)0)
#else
#define PRINT(x) x
#endif

#ifdef NCHECKR
#define CHECKR(x) ((void)0)
#else
#define CHECKR(x) x
#endif
#ifdef NCHECKAB
#define CHECK(x) ((void)0)
#else
#define CHECK(x) x
#endif

template<int N>
struct Peigen::function_t
{
  /*
    the bit-sliced representation of the function
  */
  bit_slice_t<N> bit_slice{{}};
  /*
    the line which was in the previous function
  */
  bit_slice_l_t<N> prev = {{ 0 }};

  /*
    index of changed line
  */
  uint8_t info_line = 0;
  /*
    identifier for last boolean operation
  */
  uint8_t info_op = 0;

  __m128i LUT[LUT_XMM_N];

  function_t() {  };

  function_t(const bit_slice_t<N> a)
  {
    for (int i = 0; i < N; i++)
    {
      bit_slice[i] = a[i];
    }
    bit_slice_to_LUT();
  }

  function_t(const uint8_t aLUT[LUT_UNIT_N])
  {
    const __m128i * aLUTp = (const __m128i *)aLUT;
    for (int i = 0; i < LUT_XMM_N; i++)
    {
      LUT[i] = _mm_loadu_si128(aLUTp + i);
    }
    LUT_to_bit_slice();
  }

  function_t(const string str)
  {
    parse_function(str);
  }

  static function_t INPUT_DEFAULT()
  {    
    function_t I;
    
    uint8_t * x = (uint8_t *)I.LUT;
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      x[i] = i & 0xff;
    }
    I.LUT_to_bit_slice();

    return I;
  }

  void parse_function(const string str)
  {
    if(str.size() == (BIT_SLICE_NIBBLES_N * N) + (N - 1))
    {
      string_to_bit_slice(str);
      bit_slice_to_LUT();
    }
    else if(str.size() == (BIT_SLICE_BITS_N<<1))
    {
      string_to_LUT(str);
      LUT_to_bit_slice();
    }
    else
    {
      cout << "Error: Length of the string representing the function should be either "
         << (BIT_SLICE_NIBBLES_N * N) + (N - 1) << " or "
         << (BIT_SLICE_BITS_N<<1) << endl;
    }
  }

  bool operator < (const function_t& f) const
  {
    return bit_slice < f.bit_slice;
  }

  bool operator == (const function_t& f) const
  {
    return bit_slice == f.bit_slice;
  }

  string to_string() const
  {
    stringstream ss;
    ss << hex << setfill('0');
    
    for (int i = N - 1; i >= 0; i--)
    {
      for (int j = UNIT_N - 1; j >= 0; j--)
      {
          ss << setw(UNIT_NIBBLE_N) << bit_slice[i][j] + '\0';
      }
      ss << "_";            
    }
    string res = ss.str();
    res.pop_back();
    return res;
  }
 
  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  string LUT_to_string() const
  {
    stringstream ss;
    ss << hex << setfill('0');

    uint8_t * S = (uint8_t *) LUT;
    
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      ss << setw(2) << S[i] + '\0';
    }

    return ss.str();
  }

  string show_LUT() const
  {
    stringstream ss;
    ss << "LUT = {" << endl;
    ss << hex << setfill('0');

    uint8_t * S = (uint8_t *) LUT;    
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      ss << "0x" << setw(2) << S[i] + '\0' << ",";
    }
    ss << endl;
    ss << "};" << endl;
    return ss.str();
  }

  void show_LUT(ofstream & fout) const
  {
    fout << show_LUT();
  }

  void string_to_bit_slice(const string& str)
  {
    if(str.size() == (BIT_SLICE_NIBBLES_N * N) + (N - 1))
    {
      stringstream ss(str);
      string number;
      int i = N - 1;
      while(getline(ss, number, '_') && i >= 0)
      {
        if(number.length() != BIT_SLICE_NIBBLES_N)
        {
          cout << "Length of each substring error.";
          exit(1);
        }
        for (int j = UNIT_N - 1; j >= 0; j--)
        {
          bit_slice[i][j] = stoul(number.substr((UNIT_N - 1 - j)*UNIT_NIBBLE_N, UNIT_NIBBLE_N), nullptr, 16);
        }
        i--; 
      }
    }
    else
    {
      cout << "Error: Length of the string representing the function in bitsliced form should be "
           << (BIT_SLICE_NIBBLES_N * N) + (N - 1) << endl;
      exit(1);
    }
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void string_to_LUT(const string& str)
  {
    if(str.size() == (BIT_SLICE_BITS_N<<1))
    {
      string tmp;
      uint8_t * x = (uint8_t *)LUT;

      for (int i = 0; i < LUT_BYTE_N; i++)
      {
        tmp = str.substr(i * 2, 2);
        x[i] = (uint8_t) stoul(tmp, nullptr, 16);
      }
    }
    else
    {
      cout << "Error: Length of the string representing the function in LUT form should be "
           << (BIT_SLICE_BITS_N<<1) << endl;
      exit(1);
    }
  }

  string show() const
  {
    stringstream ss;
    ss << hex << setfill('0');

    uint8_t * S = (uint8_t *) LUT;
    ss << "{";
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      ss << "0x" << setw(LUT_UNIT_NIBBLE_N) << S[i] + '\0' << ",";
    }
    string res = ss.str();
    res.pop_back();
    res = res + "}";
    return res;
  }

  string show_concise() const
  {
    stringstream ss;
    ss << hex << setfill('0');

    uint8_t * S = (uint8_t *) LUT;
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      ss << setw(LUT_UNIT_NIBBLE_N) << S[i] + '\0';
    }
    string res = ss.str();
    return res;
  }

  void sort()
  {
    bit_slice_l_t<N> mod = bit_slice[info_line];
    StaticSort<N> staticSort;
    staticSort(bit_slice);
    for(int i = 0; i < N; i++)
    {
      if(bit_slice[i] == mod) info_line = i;
    }
  }

  void sort(int iPE[N])
  {
    bit_slice_PE_t<N> b_c_tmp;

    for (int i = 0; i < N; i++)
		{
      b_c_tmp[i].first = bit_slice[i];
      b_c_tmp[i].second = i;
		}

    StaticSort<N> staticSort;
    staticSort(b_c_tmp);

    for (int i = 0; i < N; i++)
    {
      bit_slice[i] = b_c_tmp[i].first;
      iPE[b_c_tmp[i].second] = i;
    }
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void LUT_to_bit_slice()
  {
    if (N == 3)
    {
      const __m128i mask = _mm_set_epi64x(0x0, 0xffffffffffffffffULL);
      LUT[0] = _mm_and_si128(LUT[0], mask);
	    bit_slice[2][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 5));
	    bit_slice[1][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 6));
	    bit_slice[0][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 7));

    }
    else if (N == 4)
    {
	    bit_slice[3][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 4));
	    bit_slice[2][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 5));
	    bit_slice[1][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 6));
	    bit_slice[0][0] = _mm_movemask_epi8(_mm_slli_epi16(LUT[0], 7));
    }
    else if (N <= 8)
    {
      __m128i xmm[LUT_XMM_N];

      for (int i = 0; i < LUT_XMM_N; i++)
      {
        xmm[i] = _mm_slli_epi16(LUT[i], 8 - N);
      }
  
      for (int i = N - 1; i >= 0; i--)
      {
        for (int j = LUT_XMM_N - 1; j >= 0; j--)
        {
          bit_slice[i][(j*16)/UNIT_BIT_N] <<= 16;
          bit_slice[i][(j*16)/UNIT_BIT_N] |= (unsigned short)_mm_movemask_epi8(xmm[j]);
          xmm[j] = _mm_slli_epi16(xmm[j], 1);
        } 
      }
    }
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void LUT_to_bit_slice(uint8_t x[])
  {
    for (int i = 0; i < LUT_XMM_N; i++)
    {
      LUT[i] = _mm_loadu_si128((__m128i *)(x + i * 16));
    }
    LUT_to_bit_slice();
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void bit_slice_to_LUT()
  {
    uint8_t * x = (uint8_t *) LUT;

    if (N == 3)
    {
        const __m128i mask = _mm_set_epi64x(0x0, 0xffffffULL); 
        __m128i xmm[1];
        xmm[0] = _mm_loadu_si128((__m128i *)((*bit_slice.data()).data()));
        xmm[0] = _mm_and_si128(xmm[0], mask);
        for (int i = 7; i >= 0; i--)
        {
            int tmp = _mm_movemask_epi8(xmm[0]);
            x[i] = tmp & 0x7;
            xmm[0] = _mm_slli_epi16(xmm[0], 1);
        }
    }
    else if (N == 4)
    {
        const __m128i smm = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x7, 0x5, 0x3, 0x1, 0x80, 0x80, 0x80, 0x80, 0x6, 0x4, 0x2, 0x0);
        __m128i xmm[1];
        xmm[0] = _mm_loadu_si128((__m128i *)((*bit_slice.data()).data()));
        xmm[0] = _mm_shuffle_epi8(xmm[0], smm);
        for (int i = 7; i >= 0; i--)
        {
            int tmp = _mm_movemask_epi8(xmm[0]);
            x[i + 8] = (tmp >> 8) & 0xf;  x[i] = tmp & 0xf;
            xmm[0] = _mm_slli_epi16(xmm[0], 1);
        }
    }
    else if (N == 5)
    {
        const __m128i mmmask = _mm_set_epi32(0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff);
        const __m128i smm = _mm_set_epi8(0xf, 0xb, 0x7, 0x3, 0xe, 0xa, 0x6, 0x2, 0xd, 0x9, 0x5, 0x1, 0xc, 0x8, 0x4, 0x0);
        __m128i xmm[2];
        xmm[0] = _mm_loadu_si128((__m128i *)bit_slice.data());       // 3_3  3_2  3_1  3_0  2_3  2_2  2_1  2_0  1_3  1_2  1_1  1_0  0_3  0_2  0_1  0_0
        xmm[1] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 1); // x_3  x_2  x_1  x_0  x_3  x_2  x_1  x_0  x_3  x_2  x_1  x_0  4_3  4_2  4_1  4_0
        xmm[0] = _mm_shuffle_epi8(xmm[0], smm); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
        xmm[1] = _mm_shuffle_epi8(xmm[1], smm);
        xmm[1] = _mm_and_si128(xmm[1], mmmask); // x_3  x_3  x_3  4_3  x_2  x_2  x_2  4_2  x_1  x_1  x_1  4_1  x_0  x_0  x_0  4_0

        for (int i = 7; i >= 0; i--)
        {
            int tmp0 = _mm_movemask_epi8(xmm[0]);
            int tmp1 = _mm_movemask_epi8(xmm[1]);
            x[i + 8 * 3] = ((tmp0 >> (4 * 3)) & 0xf) | ((tmp1 >> (4 * 2)) & 0x10);
            x[i + 8 * 2] = ((tmp0 >> (4 * 2)) & 0xf) | ((tmp1 >> (4 * 1)) & 0x10);
            x[i + 8 * 1] = ((tmp0 >> (4 * 1)) & 0xf) | ((tmp1 >> (4 * 0)) & 0x10);
            x[i + 8 * 0] = ((tmp0 >> (4 * 0)) & 0xf) | ((tmp1 << (4 * 1)) & 0x10);
            xmm[0] = _mm_slli_epi16(xmm[0], 1);
            xmm[1] = _mm_slli_epi16(xmm[1], 1);
        }
    }
    else if (N == 6)
    {
        const __m128i smm = _mm_set_epi8(0xf, 0x7, 0xe, 0x6, 0xd, 0x5, 0xc, 0x4, 0xb, 0x3, 0xa, 0x2, 0x9, 0x1, 0x8, 0x0);
        __m128i xmm[4];
        __m128i tmm[4];
        xmm[0] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 0); // 1_7  1_6  1_5  1_4  1_3  1_2  1_1  1_0  0_7  0_6  0_5  0_4  0_3  0_2  0_1  0_0  
        xmm[1] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 1); // 3_7  3_6  3_5  3_4  3_3  3_2  3_1  3_0  2_7  2_6  2_5  2_4  2_3  2_2  2_1  2_0 
        xmm[2] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 2); // 5_7  5_6  5_5  5_4  5_3  5_2  5_1  5_0  4_7  4_6  4_5  4_4  4_3  4_2  4_1  4_0

        xmm[0] = _mm_shuffle_epi8(xmm[0], smm);      // 1_7  0_7  1_6  0_6  1_5  0_5  1_4  0_4  1_3  0_3  1_2  0_2  1_1  0_1  1_0  0_0 
        xmm[1] = _mm_shuffle_epi8(xmm[1], smm);      // 3_7  2_7  3_6  2_6  3_5  2_5  3_4  2_4  3_3  2_3  3_2  2_2  3_1  2_1  3_0  2_0 
        xmm[2] = _mm_shuffle_epi8(xmm[2], smm);      // 5_7  4_7  5_6  4_6  5_5  4_5  5_4  4_4  5_3  4_3  5_2  4_2  5_1  4_1  5_0  4_0 
        xmm[3] = _mm_setzero_si128();                // x_7  x_7  x_6  x_6  x_5  x_5  x_4  x_4  x_3  x_3  x_2  x_2  x_1  x_1  x_0  x_0  

        tmm[0] = _mm_unpacklo_epi16(xmm[0], xmm[1]); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
        tmm[1] = _mm_unpackhi_epi16(xmm[0], xmm[1]); // 3_7  2_7  1_7  0_7  3_6  2_6  1_6  0_6  3_5  2_5  1_5  0_5  3_4  2_4  1_4  0_4
        tmm[2] = _mm_unpacklo_epi16(xmm[2], xmm[3]); // x_3  x_3  5_3  4_3  x_2  x_2  5_2  4_2  x_1  x_1  5_1  4_1  x_0  x_0  5_0  4_0
        tmm[3] = _mm_unpackhi_epi16(xmm[2], xmm[3]); // x_7  x_7  5_7  4_7  x_6  x_6  5_6  4_6  x_5  x_5  5_5  4_5  x_4  x_4  5_4  4_4

        xmm[0] = _mm_unpacklo_epi32(tmm[0], tmm[2]); // x_1  x_1  5_1  4_1  3_1  2_1  1_1  0_1  x_0  x_0  5_0  4_0  3_0  2_0  1_0  0_0
        xmm[1] = _mm_unpackhi_epi32(tmm[0], tmm[2]); // x_3  x_3  5_3  4_3  3_3  2_3  1_3  0_3  x_2  x_2  5_2  4_2  3_2  2_2  1_2  0_2  
        xmm[2] = _mm_unpacklo_epi32(tmm[1], tmm[3]); // x_5  x_5  5_5  4_5  3_5  2_5  1_5  0_5  x_4  x_4  5_4  4_4  3_4  2_4  1_4  0_4
        xmm[3] = _mm_unpackhi_epi32(tmm[1], tmm[3]); // x_7  x_7  5_7  4_7  3_7  2_7  1_7  0_7  x_6  x_6  5_6  4_6  3_6  2_6  1_6  0_6

        for (int i = 7; i >= 0; i--)
        {
            int tmp0 = _mm_movemask_epi8(xmm[0]);
            int tmp1 = _mm_movemask_epi8(xmm[1]);
            int tmp2 = _mm_movemask_epi8(xmm[2]);
            int tmp3 = _mm_movemask_epi8(xmm[3]);

            x[i + 8 * 7] = ((tmp3 >> (8 * 1)) & 0x3f);  x[i + 8 * 6] = ((tmp3 >> (8 * 0)) & 0x3f);
            x[i + 8 * 5] = ((tmp2 >> (8 * 1)) & 0x3f);  x[i + 8 * 4] = ((tmp2 >> (8 * 0)) & 0x3f);
            x[i + 8 * 3] = ((tmp1 >> (8 * 1)) & 0x3f);  x[i + 8 * 2] = ((tmp1 >> (8 * 0)) & 0x3f);
            x[i + 8 * 1] = ((tmp0 >> (8 * 1)) & 0x3f);  x[i + 8 * 0] = ((tmp0 >> (8 * 0)) & 0x3f);

            xmm[0] = _mm_slli_epi16(xmm[0], 1);
            xmm[1] = _mm_slli_epi16(xmm[1], 1);   
            xmm[2] = _mm_slli_epi16(xmm[2], 1);
            xmm[3] = _mm_slli_epi16(xmm[3], 1); 
        }
    }
    else if (N == 7)
    {
        __m128i xmm[8];
        __m128i tmm[8];
        for (int i = 0; i < 7; i++)
        {
            xmm[i] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + i);
        }
        xmm[7] = _mm_setzero_si128();
        // 0_f  0_e  0_d  0_c  0_b  0_a  0_9  0_8  0_7  0_6  0_5  0_4  0_3  0_2  0_1  0_0
        // 1_f  1_e  1_d  1_c  1_b  1_a  1_9  1_8  1_7  1_6  1_5  1_4  1_3  1_2  1_1  1_0
        // 2_f  2_e  2_d  2_c  2_b  2_a  2_9  2_8  2_7  2_6  2_5  2_4  2_3  2_2  2_1  2_0
        // 3_f  3_e  3_d  3_c  3_b  3_a  3_9  3_8  3_7  3_6  3_5  3_4  3_3  3_2  3_1  3_0
        // 4_f  4_e  4_d  4_c  4_b  4_a  4_9  4_8  4_7  4_6  4_5  4_4  4_3  4_2  4_1  4_0
        // 5_f  5_e  5_d  5_c  5_b  5_a  5_9  5_8  5_7  5_6  5_5  5_4  5_3  5_2  5_1  5_0
        // 6_f  6_e  6_d  6_c  6_b  6_a  6_9  6_8  6_7  6_6  6_5  6_4  6_3  6_2  6_1  6_0
        // x_f  x_e  x_d  x_c  x_b  x_a  x_9  x_8  x_7  x_6  x_5  x_4  x_3  x_2  x_1  x_0

        tmm[0] = _mm_unpacklo_epi8(xmm[0], xmm[1]); // 1_7  0_7  1_6  0_6  1_5  0_5  1_4  0_4  1_3  0_3  1_2  0_2  1_1  0_1  1_0  0_0
        tmm[1] = _mm_unpackhi_epi8(xmm[0], xmm[1]); // 1_f  0_f  1_e  0_e  1_d  0_d  1_c  0_c  1_b  0_b  1_a  0_a  1_9  0_9  1_8  0_8
        tmm[2] = _mm_unpacklo_epi8(xmm[2], xmm[3]); // 3_7  2_7  3_6  2_6  3_5  2_5  3_4  2_4  3_3  2_3  3_2  2_2  3_1  2_1  3_0  2_0
        tmm[3] = _mm_unpackhi_epi8(xmm[2], xmm[3]); // 3_f  2_f  3_e  2_e  3_d  2_d  3_c  2_c  3_b  2_b  3_a  2_a  3_9  2_9  3_8  2_8
        tmm[4] = _mm_unpacklo_epi8(xmm[4], xmm[5]); // 5_7  4_7  5_6  4_6  5_5  4_5  5_4  4_4  5_3  4_3  5_2  4_2  5_1  4_1  5_0  4_0
        tmm[5] = _mm_unpackhi_epi8(xmm[4], xmm[5]); // 5_f  4_f  5_e  4_e  5_d  4_d  5_c  4_c  5_b  4_b  5_a  4_a  5_9  4_9  5_8  4_8
        tmm[6] = _mm_unpacklo_epi8(xmm[6], xmm[7]); // x_7  6_7  x_6  6_6  x_5  6_5  x_4  6_4  x_3  6_3  x_2  6_2  x_1  6_1  x_0  6_0
        tmm[7] = _mm_unpackhi_epi8(xmm[6], xmm[7]); // x_f  6_f  x_e  6_e  x_d  6_d  x_c  6_c  x_b  6_b  x_a  6_a  x_9  6_9  x_8  6_8

        xmm[0] = _mm_unpacklo_epi16(tmm[0], tmm[2]); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
        xmm[1] = _mm_unpackhi_epi16(tmm[0], tmm[2]); // 3_7  2_7  1_7  0_7  3_6  2_6  1_6  0_6  3_5  2_5  1_5  0_5  3_4  2_4  1_4  0_4
        xmm[2] = _mm_unpacklo_epi16(tmm[1], tmm[3]); // 3_b  2_b  1_b  0_b  3_a  2_a  1_a  0_a  3_9  2_9  1_9  0_9  3_8  2_8  1_8  0_8
        xmm[3] = _mm_unpackhi_epi16(tmm[1], tmm[3]); // 3_f  2_f  1_f  0_f  3_e  2_e  1_e  0_e  3_d  2_d  1_d  0_d  3_c  2_c  1_c  0_c
        xmm[4] = _mm_unpacklo_epi16(tmm[4], tmm[6]); // x_3  6_3  5_3  4_3  x_2  6_2  5_2  4_2  x_1  6_1  5_1  4_1  x_0  6_0  5_0  4_0
        xmm[5] = _mm_unpackhi_epi16(tmm[4], tmm[6]); // x_7  6_7  5_7  4_7  x_6  6_6  5_6  4_6  x_5  6_5  5_5  4_5  x_4  6_4  5_4  4_4
        xmm[6] = _mm_unpacklo_epi16(tmm[5], tmm[7]); // x_b  6_b  5_b  4_b  x_a  6_a  5_a  4_a  x_9  6_9  5_9  4_9  x_8  6_8  5_8  4_8
        xmm[7] = _mm_unpackhi_epi16(tmm[5], tmm[7]); // x_f  6_f  5_f  4_f  x_e  6_e  5_e  4_e  x_d  6_d  5_d  4_d  x_c  6_c  5_c  4_c

        tmm[0] = _mm_unpacklo_epi32(xmm[0], xmm[4]); // x_1  6_1  5_1  4_1  3_1  2_1  1_1  0_1  x_0  6_0  5_0  4_0  3_0  2_0  1_0  0_0
        tmm[1] = _mm_unpackhi_epi32(xmm[0], xmm[4]); // x_3  6_3  5_3  4_3  3_3  2_3  1_3  0_3  x_2  6_2  5_2  4_2  3_2  2_2  1_2  0_2
        tmm[2] = _mm_unpacklo_epi32(xmm[1], xmm[5]); // x_5  6_5  5_5  4_5  3_5  2_5  1_5  0_5  x_4  6_4  5_4  4_4  3_4  2_4  1_4  0_4
        tmm[3] = _mm_unpackhi_epi32(xmm[1], xmm[5]); // x_7  6_7  5_7  4_7  3_7  2_7  1_7  0_7  x_6  6_6  5_6  4_6  3_6  2_6  1_6  0_6
        tmm[4] = _mm_unpacklo_epi32(xmm[2], xmm[6]); // x_9  6_9  5_9  4_9  3_9  2_9  1_9  0_9  x_8  6_8  5_8  4_8  3_8  2_8  1_8  0_8
        tmm[5] = _mm_unpackhi_epi32(xmm[2], xmm[6]); // x_b  6_b  5_b  4_b  3_b  2_b  1_b  0_b  x_a  6_a  5_a  4_a  3_a  2_a  1_a  0_a
        tmm[6] = _mm_unpacklo_epi32(xmm[3], xmm[7]); // x_d  6_d  5_d  4_d  3_d  2_d  1_d  0_d  x_c  6_c  5_c  4_c  3_c  2_c  1_c  0_c
        tmm[7] = _mm_unpackhi_epi32(xmm[3], xmm[7]); // x_f  6_f  5_f  4_f  3_f  2_f  1_f  0_f  x_e  6_e  5_e  4_e  3_e  2_e  1_e  0_e

        int tmp[8];
        for (int i = 7; i >= 0; i--)
        {
            tmp[0] = _mm_movemask_epi8(tmm[0]);
            tmp[1] = _mm_movemask_epi8(tmm[1]);
            tmp[2] = _mm_movemask_epi8(tmm[2]);
            tmp[3] = _mm_movemask_epi8(tmm[3]);
            tmp[4] = _mm_movemask_epi8(tmm[4]);
            tmp[5] = _mm_movemask_epi8(tmm[5]);
            tmp[6] = _mm_movemask_epi8(tmm[6]);
            tmp[7] = _mm_movemask_epi8(tmm[7]);

            x[i + 16 * 7 + 8] = ((tmp[7] >> (8 * 1)) & 0x7f);  x[i + 16 * 7] = ((tmp[7] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 6 + 8] = ((tmp[6] >> (8 * 1)) & 0x7f);  x[i + 16 * 6] = ((tmp[6] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 5 + 8] = ((tmp[5] >> (8 * 1)) & 0x7f);  x[i + 16 * 5] = ((tmp[5] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 4 + 8] = ((tmp[4] >> (8 * 1)) & 0x7f);  x[i + 16 * 4] = ((tmp[4] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 3 + 8] = ((tmp[3] >> (8 * 1)) & 0x7f);  x[i + 16 * 3] = ((tmp[3] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 2 + 8] = ((tmp[2] >> (8 * 1)) & 0x7f);  x[i + 16 * 2] = ((tmp[2] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 1 + 8] = ((tmp[1] >> (8 * 1)) & 0x7f);  x[i + 16 * 1] = ((tmp[1] >> (8 * 0)) & 0x7f);  
            x[i + 16 * 0 + 8] = ((tmp[0] >> (8 * 1)) & 0x7f);  x[i + 16 * 0] = ((tmp[0] >> (8 * 0)) & 0x7f);  

            tmm[0] = _mm_slli_epi16(tmm[0], 1);
            tmm[1] = _mm_slli_epi16(tmm[1], 1);
            tmm[2] = _mm_slli_epi16(tmm[2], 1);
            tmm[3] = _mm_slli_epi16(tmm[3], 1);
            tmm[4] = _mm_slli_epi16(tmm[4], 1);
            tmm[5] = _mm_slli_epi16(tmm[5], 1);
            tmm[6] = _mm_slli_epi16(tmm[6], 1);
            tmm[7] = _mm_slli_epi16(tmm[7], 1);
        }
    }
    else if (N == 8)
    {
        __m128i xmm[8];
        __m128i tmm[8];
        for (int k = 1; k >= 0; k--)
        {
            for (int i = 0; i < 8; i++)
            {
                xmm[i] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + i * 2 + k);
            }
            tmm[0] = _mm_unpacklo_epi8(xmm[0], xmm[1]); // 1_7  0_7  1_6  0_6  1_5  0_5  1_4  0_4  1_3  0_3  1_2  0_2  1_1  0_1  1_0  0_0
            tmm[1] = _mm_unpackhi_epi8(xmm[0], xmm[1]); // 1_f  0_f  1_e  0_e  1_d  0_d  1_c  0_c  1_b  0_b  1_a  0_a  1_9  0_9  1_8  0_8
            tmm[2] = _mm_unpacklo_epi8(xmm[2], xmm[3]); // 3_7  2_7  3_6  2_6  3_5  2_5  3_4  2_4  3_3  2_3  3_2  2_2  3_1  2_1  3_0  2_0
            tmm[3] = _mm_unpackhi_epi8(xmm[2], xmm[3]); // 3_f  2_f  3_e  2_e  3_d  2_d  3_c  2_c  3_b  2_b  3_a  2_a  3_9  2_9  3_8  2_8
            tmm[4] = _mm_unpacklo_epi8(xmm[4], xmm[5]); // 5_7  4_7  5_6  4_6  5_5  4_5  5_4  4_4  5_3  4_3  5_2  4_2  5_1  4_1  5_0  4_0
            tmm[5] = _mm_unpackhi_epi8(xmm[4], xmm[5]); // 5_f  4_f  5_e  4_e  5_d  4_d  5_c  4_c  5_b  4_b  5_a  4_a  5_9  4_9  5_8  4_8
            tmm[6] = _mm_unpacklo_epi8(xmm[6], xmm[7]); // 7_7  6_7  7_6  6_6  7_5  6_5  7_4  6_4  7_3  6_3  7_2  6_2  7_1  6_1  7_0  6_0
            tmm[7] = _mm_unpackhi_epi8(xmm[6], xmm[7]); // 7_f  6_f  7_e  6_e  7_d  6_d  7_c  6_c  7_b  6_b  7_a  6_a  7_9  6_9  7_8  6_8

            xmm[0] = _mm_unpacklo_epi16(tmm[0], tmm[2]); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
            xmm[1] = _mm_unpackhi_epi16(tmm[0], tmm[2]); // 3_7  2_7  1_7  0_7  3_6  2_6  1_6  0_6  3_5  2_5  1_5  0_5  3_4  2_4  1_4  0_4
            xmm[2] = _mm_unpacklo_epi16(tmm[1], tmm[3]); // 3_b  2_b  1_b  0_b  3_a  2_a  1_a  0_a  3_9  2_9  1_9  0_9  3_8  2_8  1_8  0_8
            xmm[3] = _mm_unpackhi_epi16(tmm[1], tmm[3]); // 3_f  2_f  1_f  0_f  3_e  2_e  1_e  0_e  3_d  2_d  1_d  0_d  3_c  2_c  1_c  0_c
            xmm[4] = _mm_unpacklo_epi16(tmm[4], tmm[6]); // 7_3  6_3  5_3  4_3  7_2  6_2  5_2  4_2  7_1  6_1  5_1  4_1  7_0  6_0  5_0  4_0
            xmm[5] = _mm_unpackhi_epi16(tmm[4], tmm[6]); // 7_7  6_7  5_7  4_7  7_6  6_6  5_6  4_6  7_5  6_5  5_5  4_5  7_4  6_4  5_4  4_4
            xmm[6] = _mm_unpacklo_epi16(tmm[5], tmm[7]); // 7_b  6_b  5_b  4_b  7_a  6_a  5_a  4_a  7_9  6_9  5_9  4_9  7_8  6_8  5_8  4_8
            xmm[7] = _mm_unpackhi_epi16(tmm[5], tmm[7]); // 7_f  6_f  5_f  4_f  7_e  6_e  5_e  4_e  7_d  6_d  5_d  4_d  7_c  6_c  5_c  4_c

            tmm[0] = _mm_unpacklo_epi32(xmm[0], xmm[4]); // 7_1  6_1  5_1  4_1  3_1  2_1  1_1  0_1  7_0  6_0  5_0  4_0  3_0  2_0  1_0  0_0
            tmm[1] = _mm_unpackhi_epi32(xmm[0], xmm[4]); // 7_3  6_3  5_3  4_3  3_3  2_3  1_3  0_3  7_2  6_2  5_2  4_2  3_2  2_2  1_2  0_2
            tmm[2] = _mm_unpacklo_epi32(xmm[1], xmm[5]); // 7_5  6_5  5_5  4_5  3_5  2_5  1_5  0_5  7_4  6_4  5_4  4_4  3_4  2_4  1_4  0_4
            tmm[3] = _mm_unpackhi_epi32(xmm[1], xmm[5]); // 7_7  6_7  5_7  4_7  3_7  2_7  1_7  0_7  7_6  6_6  5_6  4_6  3_6  2_6  1_6  0_6
            tmm[4] = _mm_unpacklo_epi32(xmm[2], xmm[6]); // 7_9  6_9  5_9  4_9  3_9  2_9  1_9  0_9  7_8  6_8  5_8  4_8  3_8  2_8  1_8  0_8
            tmm[5] = _mm_unpackhi_epi32(xmm[2], xmm[6]); // 7_b  6_b  5_b  4_b  3_b  2_b  1_b  0_b  7_a  6_a  5_a  4_a  3_a  2_a  1_a  0_a
            tmm[6] = _mm_unpacklo_epi32(xmm[3], xmm[7]); // 7_d  6_d  5_d  4_d  3_d  2_d  1_d  0_d  7_c  6_c  5_c  4_c  3_c  2_c  1_c  0_c
            tmm[7] = _mm_unpackhi_epi32(xmm[3], xmm[7]); // 7_f  6_f  5_f  4_f  3_f  2_f  1_f  0_f  7_e  6_e  5_e  4_e  3_e  2_e  1_e  0_e

            int tmp[8];
            for (int i = 7; i >= 0; i--)
            {
                tmp[0] = _mm_movemask_epi8(tmm[0]);
                tmp[1] = _mm_movemask_epi8(tmm[1]);
                tmp[2] = _mm_movemask_epi8(tmm[2]);
                tmp[3] = _mm_movemask_epi8(tmm[3]);
                tmp[4] = _mm_movemask_epi8(tmm[4]);
                tmp[5] = _mm_movemask_epi8(tmm[5]);
                tmp[6] = _mm_movemask_epi8(tmm[6]);
                tmp[7] = _mm_movemask_epi8(tmm[7]);

                x[128 * k + i + 16 * 7 + 8] = ((tmp[7] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 7] = ((tmp[7] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 6 + 8] = ((tmp[6] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 6] = ((tmp[6] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 5 + 8] = ((tmp[5] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 5] = ((tmp[5] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 4 + 8] = ((tmp[4] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 4] = ((tmp[4] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 3 + 8] = ((tmp[3] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 3] = ((tmp[3] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 2 + 8] = ((tmp[2] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 2] = ((tmp[2] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 1 + 8] = ((tmp[1] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 1] = ((tmp[1] >> (8 * 0)) & 0xff);  
                x[128 * k + i + 16 * 0 + 8] = ((tmp[0] >> (8 * 1)) & 0xff);  x[128 * k + i + 16 * 0] = ((tmp[0] >> (8 * 0)) & 0xff);  

                tmm[0] = _mm_slli_epi16(tmm[0], 1);
                tmm[1] = _mm_slli_epi16(tmm[1], 1);
                tmm[2] = _mm_slli_epi16(tmm[2], 1);
                tmm[3] = _mm_slli_epi16(tmm[3], 1);
                tmm[4] = _mm_slli_epi16(tmm[4], 1);
                tmm[5] = _mm_slli_epi16(tmm[5], 1);
                tmm[6] = _mm_slli_epi16(tmm[6], 1);
                tmm[7] = _mm_slli_epi16(tmm[7], 1);
            }
        }
    }
    else
    {
        cout << "bit_slice_to_LUT():: Not support for N > 8." << endl;
    }
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void bit_slice_to_LUT(uint8_t x[])
  {
    bit_slice_to_LUT();
    for (int i = 0; i < LUT_XMM_N; i++)
    {
      _mm_storeu_si128((__m128i *)(x + i * 16), LUT[i]);
    }
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void bit_slice_to_iLUT(uint8_t x[])
  {
    if (N == 3)
    {
        static const __m128i mask = _mm_set_epi64x(0x0, 0xffffffULL); 
        __m128i xmm[1];
        xmm[0] = _mm_loadu_si128((__m128i *)bit_slice.data());
        xmm[0] = _mm_and_si128(xmm[0], mask);
        for (int i = 7; i >= 0; i--)
        {
            int tmp = _mm_movemask_epi8(xmm[0]);
            x[tmp & 0x7] = i;
            xmm[0] = _mm_slli_epi16(xmm[0], 1);
        }
    }
    else if (N == 4)
    {
        static const __m128i smm = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x7, 0x5, 0x3, 0x1, 0x80, 0x80, 0x80, 0x80, 0x6, 0x4, 0x2, 0x0);
        __m128i xmm[1];
        xmm[0] = _mm_loadu_si128((__m128i *)bit_slice.data());
        xmm[0] = _mm_shuffle_epi8(xmm[0], smm);
        for (int i = 7; i >= 0; i--)
        {
            int tmp = _mm_movemask_epi8(xmm[0]);
            x[(tmp >> 8) & 0xf] = i + 8;  x[tmp & 0xf] = i;
            xmm[0] = _mm_slli_epi16(xmm[0], 1);
        }
    }
    else if (N == 5)
    {
        static const __m128i mmmask = _mm_set_epi32(0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff);
        static const __m128i smm = _mm_set_epi8(0xf, 0xb, 0x7, 0x3, 0xe, 0xa, 0x6, 0x2, 0xd, 0x9, 0x5, 0x1, 0xc, 0x8, 0x4, 0x0);
        __m128i xmm[2];
        xmm[0] = _mm_loadu_si128((__m128i *)bit_slice.data());       // 3_3  3_2  3_1  3_0  2_3  2_2  2_1  2_0  1_3  1_2  1_1  1_0  0_3  0_2  0_1  0_0
        xmm[1] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 1); // x_3  x_2  x_1  x_0  x_3  x_2  x_1  x_0  x_3  x_2  x_1  x_0  4_3  4_2  4_1  4_0
        xmm[0] = _mm_shuffle_epi8(xmm[0], smm); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
        xmm[1] = _mm_shuffle_epi8(xmm[1], smm);
        xmm[1] = _mm_and_si128(xmm[1], mmmask); // x_3  x_3  x_3  4_3  x_2  x_2  x_2  4_2  x_1  x_1  x_1  4_1  x_0  x_0  x_0  4_0

        for (int i = 7; i >= 0; i--)
        {
            int tmp0 = _mm_movemask_epi8(xmm[0]);
            int tmp1 = _mm_movemask_epi8(xmm[1]);
            x[((tmp0 >> (4 * 3)) & 0xf) | ((tmp1 >> (4 * 2)) & 0x10)] = i + 8 * 3;
            x[((tmp0 >> (4 * 2)) & 0xf) | ((tmp1 >> (4 * 1)) & 0x10)] = i + 8 * 2;
            x[((tmp0 >> (4 * 1)) & 0xf) | ((tmp1 >> (4 * 0)) & 0x10)] = i + 8 * 1;
            x[((tmp0 >> (4 * 0)) & 0xf) | ((tmp1 << (4 * 1)) & 0x10)] = i + 8 * 0;
            xmm[0] = _mm_slli_epi16(xmm[0], 1);
            xmm[1] = _mm_slli_epi16(xmm[1], 1);
        }
    }
    else if (N == 6)
    {
        static const __m128i smm = _mm_set_epi8(0xf, 0x7, 0xe, 0x6, 0xd, 0x5, 0xc, 0x4, 0xb, 0x3, 0xa, 0x2, 0x9, 0x1, 0x8, 0x0);
        __m128i xmm[4];
        __m128i tmm[4];
        xmm[0] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 0); // 1_7  1_6  1_5  1_4  1_3  1_2  1_1  1_0  0_7  0_6  0_5  0_4  0_3  0_2  0_1  0_0  
        xmm[1] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 1); // 3_7  3_6  3_5  3_4  3_3  3_2  3_1  3_0  2_7  2_6  2_5  2_4  2_3  2_2  2_1  2_0 
        xmm[2] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + 2); // 5_7  5_6  5_5  5_4  5_3  5_2  5_1  5_0  4_7  4_6  4_5  4_4  4_3  4_2  4_1  4_0

        xmm[0] = _mm_shuffle_epi8(xmm[0], smm);      // 1_7  0_7  1_6  0_6  1_5  0_5  1_4  0_4  1_3  0_3  1_2  0_2  1_1  0_1  1_0  0_0 
        xmm[1] = _mm_shuffle_epi8(xmm[1], smm);      // 3_7  2_7  3_6  2_6  3_5  2_5  3_4  2_4  3_3  2_3  3_2  2_2  3_1  2_1  3_0  2_0 
        xmm[2] = _mm_shuffle_epi8(xmm[2], smm);      // 5_7  4_7  5_6  4_6  5_5  4_5  5_4  4_4  5_3  4_3  5_2  4_2  5_1  4_1  5_0  4_0 
        xmm[3] = _mm_setzero_si128();               // x_7  x_7  x_6  x_6  x_5  x_5  x_4  x_4  x_3  x_3  x_2  x_2  x_1  x_1  x_0  x_0  

        tmm[0] = _mm_unpacklo_epi16(xmm[0], xmm[1]); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
        tmm[1] = _mm_unpackhi_epi16(xmm[0], xmm[1]); // 3_7  2_7  1_7  0_7  3_6  2_6  1_6  0_6  3_5  2_5  1_5  0_5  3_4  2_4  1_4  0_4
        tmm[2] = _mm_unpacklo_epi16(xmm[2], xmm[3]); // x_3  x_3  5_3  4_3  x_2  x_2  5_2  4_2  x_1  x_1  5_1  4_1  x_0  x_0  5_0  4_0
        tmm[3] = _mm_unpackhi_epi16(xmm[2], xmm[3]); // x_7  x_7  5_7  4_7  x_6  x_6  5_6  4_6  x_5  x_5  5_5  4_5  x_4  x_4  5_4  4_4

        xmm[0] = _mm_unpacklo_epi32(tmm[0], tmm[2]); // x_1  x_1  5_1  4_1  3_1  2_1  1_1  0_1  x_0  x_0  5_0  4_0  3_0  2_0  1_0  0_0
        xmm[1] = _mm_unpackhi_epi32(tmm[0], tmm[2]); // x_3  x_3  5_3  4_3  3_3  2_3  1_3  0_3  x_2  x_2  5_2  4_2  3_2  2_2  1_2  0_2  
        xmm[2] = _mm_unpacklo_epi32(tmm[1], tmm[3]); // x_5  x_5  5_5  4_5  3_5  2_5  1_5  0_5  x_4  x_4  5_4  4_4  3_4  2_4  1_4  0_4
        xmm[3] = _mm_unpackhi_epi32(tmm[1], tmm[3]); // x_7  x_7  5_7  4_7  3_7  2_7  1_7  0_7  x_6  x_6  5_6  4_6  3_6  2_6  1_6  0_6

        for (int i = 7; i >= 0; i--)
        {
            int tmp0 = _mm_movemask_epi8(xmm[0]);
            int tmp1 = _mm_movemask_epi8(xmm[1]);
            int tmp2 = _mm_movemask_epi8(xmm[2]);
            int tmp3 = _mm_movemask_epi8(xmm[3]);

            x[((tmp3 >> (8 * 1)) & 0x3f)] = i + 8 * 7;  x[((tmp3 >> (8 * 0)) & 0x3f)] = i + 8 * 6;
            x[((tmp2 >> (8 * 1)) & 0x3f)] = i + 8 * 5;  x[((tmp2 >> (8 * 0)) & 0x3f)] = i + 8 * 4;
            x[((tmp1 >> (8 * 1)) & 0x3f)] = i + 8 * 3;  x[((tmp1 >> (8 * 0)) & 0x3f)] = i + 8 * 2;
            x[((tmp0 >> (8 * 1)) & 0x3f)] = i + 8 * 1;  x[((tmp0 >> (8 * 0)) & 0x3f)] = i + 8 * 0;

            xmm[0] = _mm_slli_epi16(xmm[0], 1);
            xmm[1] = _mm_slli_epi16(xmm[1], 1);   
            xmm[2] = _mm_slli_epi16(xmm[2], 1);
            xmm[3] = _mm_slli_epi16(xmm[3], 1); 
        }
    }
    else if (N == 7)
    {
        __m128i xmm[8];
        __m128i tmm[8];
        for (int i = 0; i < 7; i++)
        {
            xmm[i] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + i);
        }
        xmm[7] = _mm_setzero_si128();
        // 0_f  0_e  0_d  0_c  0_b  0_a  0_9  0_8  0_7  0_6  0_5  0_4  0_3  0_2  0_1  0_0
        // 1_f  1_e  1_d  1_c  1_b  1_a  1_9  1_8  1_7  1_6  1_5  1_4  1_3  1_2  1_1  1_0
        // 2_f  2_e  2_d  2_c  2_b  2_a  2_9  2_8  2_7  2_6  2_5  2_4  2_3  2_2  2_1  2_0
        // 3_f  3_e  3_d  3_c  3_b  3_a  3_9  3_8  3_7  3_6  3_5  3_4  3_3  3_2  3_1  3_0
        // 4_f  4_e  4_d  4_c  4_b  4_a  4_9  4_8  4_7  4_6  4_5  4_4  4_3  4_2  4_1  4_0
        // 5_f  5_e  5_d  5_c  5_b  5_a  5_9  5_8  5_7  5_6  5_5  5_4  5_3  5_2  5_1  5_0
        // 6_f  6_e  6_d  6_c  6_b  6_a  6_9  6_8  6_7  6_6  6_5  6_4  6_3  6_2  6_1  6_0
        // x_f  x_e  x_d  x_c  x_b  x_a  x_9  x_8  x_7  x_6  x_5  x_4  x_3  x_2  x_1  x_0

        tmm[0] = _mm_unpacklo_epi8(xmm[0], xmm[1]); // 1_7  0_7  1_6  0_6  1_5  0_5  1_4  0_4  1_3  0_3  1_2  0_2  1_1  0_1  1_0  0_0
        tmm[1] = _mm_unpackhi_epi8(xmm[0], xmm[1]); // 1_f  0_f  1_e  0_e  1_d  0_d  1_c  0_c  1_b  0_b  1_a  0_a  1_9  0_9  1_8  0_8
        tmm[2] = _mm_unpacklo_epi8(xmm[2], xmm[3]); // 3_7  2_7  3_6  2_6  3_5  2_5  3_4  2_4  3_3  2_3  3_2  2_2  3_1  2_1  3_0  2_0
        tmm[3] = _mm_unpackhi_epi8(xmm[2], xmm[3]); // 3_f  2_f  3_e  2_e  3_d  2_d  3_c  2_c  3_b  2_b  3_a  2_a  3_9  2_9  3_8  2_8
        tmm[4] = _mm_unpacklo_epi8(xmm[4], xmm[5]); // 5_7  4_7  5_6  4_6  5_5  4_5  5_4  4_4  5_3  4_3  5_2  4_2  5_1  4_1  5_0  4_0
        tmm[5] = _mm_unpackhi_epi8(xmm[4], xmm[5]); // 5_f  4_f  5_e  4_e  5_d  4_d  5_c  4_c  5_b  4_b  5_a  4_a  5_9  4_9  5_8  4_8
        tmm[6] = _mm_unpacklo_epi8(xmm[6], xmm[7]); // x_7  6_7  x_6  6_6  x_5  6_5  x_4  6_4  x_3  6_3  x_2  6_2  x_1  6_1  x_0  6_0
        tmm[7] = _mm_unpackhi_epi8(xmm[6], xmm[7]); // x_f  6_f  x_e  6_e  x_d  6_d  x_c  6_c  x_b  6_b  x_a  6_a  x_9  6_9  x_8  6_8

        xmm[0] = _mm_unpacklo_epi16(tmm[0], tmm[2]); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
        xmm[1] = _mm_unpackhi_epi16(tmm[0], tmm[2]); // 3_7  2_7  1_7  0_7  3_6  2_6  1_6  0_6  3_5  2_5  1_5  0_5  3_4  2_4  1_4  0_4
        xmm[2] = _mm_unpacklo_epi16(tmm[1], tmm[3]); // 3_b  2_b  1_b  0_b  3_a  2_a  1_a  0_a  3_9  2_9  1_9  0_9  3_8  2_8  1_8  0_8
        xmm[3] = _mm_unpackhi_epi16(tmm[1], tmm[3]); // 3_f  2_f  1_f  0_f  3_e  2_e  1_e  0_e  3_d  2_d  1_d  0_d  3_c  2_c  1_c  0_c
        xmm[4] = _mm_unpacklo_epi16(tmm[4], tmm[6]); // x_3  6_3  5_3  4_3  x_2  6_2  5_2  4_2  x_1  6_1  5_1  4_1  x_0  6_0  5_0  4_0
        xmm[5] = _mm_unpackhi_epi16(tmm[4], tmm[6]); // x_7  6_7  5_7  4_7  x_6  6_6  5_6  4_6  x_5  6_5  5_5  4_5  x_4  6_4  5_4  4_4
        xmm[6] = _mm_unpacklo_epi16(tmm[5], tmm[7]); // x_b  6_b  5_b  4_b  x_a  6_a  5_a  4_a  x_9  6_9  5_9  4_9  x_8  6_8  5_8  4_8
        xmm[7] = _mm_unpackhi_epi16(tmm[5], tmm[7]); // x_f  6_f  5_f  4_f  x_e  6_e  5_e  4_e  x_d  6_d  5_d  4_d  x_c  6_c  5_c  4_c

        tmm[0] = _mm_unpacklo_epi32(xmm[0], xmm[4]); // x_1  6_1  5_1  4_1  3_1  2_1  1_1  0_1  x_0  6_0  5_0  4_0  3_0  2_0  1_0  0_0
        tmm[1] = _mm_unpackhi_epi32(xmm[0], xmm[4]); // x_3  6_3  5_3  4_3  3_3  2_3  1_3  0_3  x_2  6_2  5_2  4_2  3_2  2_2  1_2  0_2
        tmm[2] = _mm_unpacklo_epi32(xmm[1], xmm[5]); // x_5  6_5  5_5  4_5  3_5  2_5  1_5  0_5  x_4  6_4  5_4  4_4  3_4  2_4  1_4  0_4
        tmm[3] = _mm_unpackhi_epi32(xmm[1], xmm[5]); // x_7  6_7  5_7  4_7  3_7  2_7  1_7  0_7  x_6  6_6  5_6  4_6  3_6  2_6  1_6  0_6
        tmm[4] = _mm_unpacklo_epi32(xmm[2], xmm[6]); // x_9  6_9  5_9  4_9  3_9  2_9  1_9  0_9  x_8  6_8  5_8  4_8  3_8  2_8  1_8  0_8
        tmm[5] = _mm_unpackhi_epi32(xmm[2], xmm[6]); // x_b  6_b  5_b  4_b  3_b  2_b  1_b  0_b  x_a  6_a  5_a  4_a  3_a  2_a  1_a  0_a
        tmm[6] = _mm_unpacklo_epi32(xmm[3], xmm[7]); // x_d  6_d  5_d  4_d  3_d  2_d  1_d  0_d  x_c  6_c  5_c  4_c  3_c  2_c  1_c  0_c
        tmm[7] = _mm_unpackhi_epi32(xmm[3], xmm[7]); // x_f  6_f  5_f  4_f  3_f  2_f  1_f  0_f  x_e  6_e  5_e  4_e  3_e  2_e  1_e  0_e

        int tmp[8];
        for (int i = 7; i >= 0; i--)
        {
            tmp[0] = _mm_movemask_epi8(tmm[0]);
            tmp[1] = _mm_movemask_epi8(tmm[1]);
            tmp[2] = _mm_movemask_epi8(tmm[2]);
            tmp[3] = _mm_movemask_epi8(tmm[3]);
            tmp[4] = _mm_movemask_epi8(tmm[4]);
            tmp[5] = _mm_movemask_epi8(tmm[5]);
            tmp[6] = _mm_movemask_epi8(tmm[6]);
            tmp[7] = _mm_movemask_epi8(tmm[7]);

            x[((tmp[7] >> (8 * 1)) & 0x7f)] = i + 16 * 7 + 8;  x[((tmp[7] >> (8 * 0)) & 0x7f)] = i + 16 * 7;  
            x[((tmp[6] >> (8 * 1)) & 0x7f)] = i + 16 * 6 + 8;  x[((tmp[6] >> (8 * 0)) & 0x7f)] = i + 16 * 6;  
            x[((tmp[5] >> (8 * 1)) & 0x7f)] = i + 16 * 5 + 8;  x[((tmp[5] >> (8 * 0)) & 0x7f)] = i + 16 * 5;  
            x[((tmp[4] >> (8 * 1)) & 0x7f)] = i + 16 * 4 + 8;  x[((tmp[4] >> (8 * 0)) & 0x7f)] = i + 16 * 4;  
            x[((tmp[3] >> (8 * 1)) & 0x7f)] = i + 16 * 3 + 8;  x[((tmp[3] >> (8 * 0)) & 0x7f)] = i + 16 * 3;  
            x[((tmp[2] >> (8 * 1)) & 0x7f)] = i + 16 * 2 + 8;  x[((tmp[2] >> (8 * 0)) & 0x7f)] = i + 16 * 2;  
            x[((tmp[1] >> (8 * 1)) & 0x7f)] = i + 16 * 1 + 8;  x[((tmp[1] >> (8 * 0)) & 0x7f)] = i + 16 * 1;  
            x[((tmp[0] >> (8 * 1)) & 0x7f)] = i + 16 * 0 + 8;  x[((tmp[0] >> (8 * 0)) & 0x7f)] = i + 16 * 0;  

            tmm[0] = _mm_slli_epi16(tmm[0], 1);
            tmm[1] = _mm_slli_epi16(tmm[1], 1);
            tmm[2] = _mm_slli_epi16(tmm[2], 1);
            tmm[3] = _mm_slli_epi16(tmm[3], 1);
            tmm[4] = _mm_slli_epi16(tmm[4], 1);
            tmm[5] = _mm_slli_epi16(tmm[5], 1);
            tmm[6] = _mm_slli_epi16(tmm[6], 1);
            tmm[7] = _mm_slli_epi16(tmm[7], 1);
        }
    }
    else if (N == 8)
    {
        __m128i xmm[8];
        __m128i tmm[8];
        for (int k = 1; k >= 0; k--)
        {
            for (int i = 0; i < 8; i++)
            {
                xmm[i] = _mm_loadu_si128(((__m128i *)bit_slice.data()) + i * 2 + k);
            }
            tmm[0] = _mm_unpacklo_epi8(xmm[0], xmm[1]); // 1_7  0_7  1_6  0_6  1_5  0_5  1_4  0_4  1_3  0_3  1_2  0_2  1_1  0_1  1_0  0_0
            tmm[1] = _mm_unpackhi_epi8(xmm[0], xmm[1]); // 1_f  0_f  1_e  0_e  1_d  0_d  1_c  0_c  1_b  0_b  1_a  0_a  1_9  0_9  1_8  0_8
            tmm[2] = _mm_unpacklo_epi8(xmm[2], xmm[3]); // 3_7  2_7  3_6  2_6  3_5  2_5  3_4  2_4  3_3  2_3  3_2  2_2  3_1  2_1  3_0  2_0
            tmm[3] = _mm_unpackhi_epi8(xmm[2], xmm[3]); // 3_f  2_f  3_e  2_e  3_d  2_d  3_c  2_c  3_b  2_b  3_a  2_a  3_9  2_9  3_8  2_8
            tmm[4] = _mm_unpacklo_epi8(xmm[4], xmm[5]); // 5_7  4_7  5_6  4_6  5_5  4_5  5_4  4_4  5_3  4_3  5_2  4_2  5_1  4_1  5_0  4_0
            tmm[5] = _mm_unpackhi_epi8(xmm[4], xmm[5]); // 5_f  4_f  5_e  4_e  5_d  4_d  5_c  4_c  5_b  4_b  5_a  4_a  5_9  4_9  5_8  4_8
            tmm[6] = _mm_unpacklo_epi8(xmm[6], xmm[7]); // 7_7  6_7  7_6  6_6  7_5  6_5  7_4  6_4  7_3  6_3  7_2  6_2  7_1  6_1  7_0  6_0
            tmm[7] = _mm_unpackhi_epi8(xmm[6], xmm[7]); // 7_f  6_f  7_e  6_e  7_d  6_d  7_c  6_c  7_b  6_b  7_a  6_a  7_9  6_9  7_8  6_8

            xmm[0] = _mm_unpacklo_epi16(tmm[0], tmm[2]); // 3_3  2_3  1_3  0_3  3_2  2_2  1_2  0_2  3_1  2_1  1_1  0_1  3_0  2_0  1_0  0_0
            xmm[1] = _mm_unpackhi_epi16(tmm[0], tmm[2]); // 3_7  2_7  1_7  0_7  3_6  2_6  1_6  0_6  3_5  2_5  1_5  0_5  3_4  2_4  1_4  0_4
            xmm[2] = _mm_unpacklo_epi16(tmm[1], tmm[3]); // 3_b  2_b  1_b  0_b  3_a  2_a  1_a  0_a  3_9  2_9  1_9  0_9  3_8  2_8  1_8  0_8
            xmm[3] = _mm_unpackhi_epi16(tmm[1], tmm[3]); // 3_f  2_f  1_f  0_f  3_e  2_e  1_e  0_e  3_d  2_d  1_d  0_d  3_c  2_c  1_c  0_c
            xmm[4] = _mm_unpacklo_epi16(tmm[4], tmm[6]); // 7_3  6_3  5_3  4_3  7_2  6_2  5_2  4_2  7_1  6_1  5_1  4_1  7_0  6_0  5_0  4_0
            xmm[5] = _mm_unpackhi_epi16(tmm[4], tmm[6]); // 7_7  6_7  5_7  4_7  7_6  6_6  5_6  4_6  7_5  6_5  5_5  4_5  7_4  6_4  5_4  4_4
            xmm[6] = _mm_unpacklo_epi16(tmm[5], tmm[7]); // 7_b  6_b  5_b  4_b  7_a  6_a  5_a  4_a  7_9  6_9  5_9  4_9  7_8  6_8  5_8  4_8
            xmm[7] = _mm_unpackhi_epi16(tmm[5], tmm[7]); // 7_f  6_f  5_f  4_f  7_e  6_e  5_e  4_e  7_d  6_d  5_d  4_d  7_c  6_c  5_c  4_c

            tmm[0] = _mm_unpacklo_epi32(xmm[0], xmm[4]); // 7_1  6_1  5_1  4_1  3_1  2_1  1_1  0_1  7_0  6_0  5_0  4_0  3_0  2_0  1_0  0_0
            tmm[1] = _mm_unpackhi_epi32(xmm[0], xmm[4]); // 7_3  6_3  5_3  4_3  3_3  2_3  1_3  0_3  7_2  6_2  5_2  4_2  3_2  2_2  1_2  0_2
            tmm[2] = _mm_unpacklo_epi32(xmm[1], xmm[5]); // 7_5  6_5  5_5  4_5  3_5  2_5  1_5  0_5  7_4  6_4  5_4  4_4  3_4  2_4  1_4  0_4
            tmm[3] = _mm_unpackhi_epi32(xmm[1], xmm[5]); // 7_7  6_7  5_7  4_7  3_7  2_7  1_7  0_7  7_6  6_6  5_6  4_6  3_6  2_6  1_6  0_6
            tmm[4] = _mm_unpacklo_epi32(xmm[2], xmm[6]); // 7_9  6_9  5_9  4_9  3_9  2_9  1_9  0_9  7_8  6_8  5_8  4_8  3_8  2_8  1_8  0_8
            tmm[5] = _mm_unpackhi_epi32(xmm[2], xmm[6]); // 7_b  6_b  5_b  4_b  3_b  2_b  1_b  0_b  7_a  6_a  5_a  4_a  3_a  2_a  1_a  0_a
            tmm[6] = _mm_unpacklo_epi32(xmm[3], xmm[7]); // 7_d  6_d  5_d  4_d  3_d  2_d  1_d  0_d  7_c  6_c  5_c  4_c  3_c  2_c  1_c  0_c
            tmm[7] = _mm_unpackhi_epi32(xmm[3], xmm[7]); // 7_f  6_f  5_f  4_f  3_f  2_f  1_f  0_f  7_e  6_e  5_e  4_e  3_e  2_e  1_e  0_e

            int tmp[8];
            for (int i = 7; i >= 0; i--)
            {
                tmp[0] = _mm_movemask_epi8(tmm[0]);
                tmp[1] = _mm_movemask_epi8(tmm[1]);
                tmp[2] = _mm_movemask_epi8(tmm[2]);
                tmp[3] = _mm_movemask_epi8(tmm[3]);
                tmp[4] = _mm_movemask_epi8(tmm[4]);
                tmp[5] = _mm_movemask_epi8(tmm[5]);
                tmp[6] = _mm_movemask_epi8(tmm[6]);
                tmp[7] = _mm_movemask_epi8(tmm[7]);

                x[((tmp[7] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 7 + 8;  x[((tmp[7] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 7;  
                x[((tmp[6] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 6 + 8;  x[((tmp[6] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 6;  
                x[((tmp[5] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 5 + 8;  x[((tmp[5] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 5;  
                x[((tmp[4] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 4 + 8;  x[((tmp[4] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 4;  
                x[((tmp[3] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 3 + 8;  x[((tmp[3] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 3;  
                x[((tmp[2] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 2 + 8;  x[((tmp[2] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 2;  
                x[((tmp[1] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 1 + 8;  x[((tmp[1] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 1;  
                x[((tmp[0] >> (8 * 1)) & 0xff)] = 128 * k + i + 16 * 0 + 8;  x[((tmp[0] >> (8 * 0)) & 0xff)] = 128 * k + i + 16 * 0;  

                tmm[0] = _mm_slli_epi16(tmm[0], 1);
                tmm[1] = _mm_slli_epi16(tmm[1], 1);
                tmm[2] = _mm_slli_epi16(tmm[2], 1);
                tmm[3] = _mm_slli_epi16(tmm[3], 1);
                tmm[4] = _mm_slli_epi16(tmm[4], 1);
                tmm[5] = _mm_slli_epi16(tmm[5], 1);
                tmm[6] = _mm_slli_epi16(tmm[6], 1);
                tmm[7] = _mm_slli_epi16(tmm[7], 1);
            }
        }
    }
    else
    {
        cout << "bit_slice_to_iLUT():: Not support for N > 8." << endl;
    }
  }
  
  #define EQU(w, u)  (_mm_testz_si128(one_128, _mm_xor_si128(w, u)) == 1)
  #define PERM(w) (_mm_extract_epi16(_mm_cmpestrm(w, 16, S4_I, 16, 0x00), 0) == 0xffff)

  bool is_permutation() const
  {
    if (N == 4)
    {
      return PERM(LUT[0]);
    }
    else
    {
      const uint8_t * LUTp = (uint8_t *) LUT;

      bit_slice_l_t<N> ind = { {0} };
 
      for (int i = 0; i < LUT_UNIT_N; i++)
      {
        uint8_t y = LUTp[i];
        if (OPs<N>.get_bit(ind, y) == 1)
        {
          return false;
        }
        OPs<N>.set_bit_inplace(ind, y);
      }
      return true;
    }
  }

  void inverse(uint8_t iS[LUT_UNIT_N]) const
	{
		if (is_permutation())
		{
      uint8_t * S = (uint8_t *) LUT;
			for (int x = 0; x < LUT_UNIT_N; x++)
			{
				iS[S[x]] = x;
			}
		}
		else
		{
      for (int x = 0; x < LUT_UNIT_N; x++)
			{
				iS[x] = 0;
			}
		}
	};

  function_t<N> inverse() const
	{
    function_t<N> res;
    uint8_t * iS = (uint8_t *) res.LUT;
		if (is_permutation())
		{
      uint8_t * S = (uint8_t *) LUT;
			for (int x = 0; x < LUT_UNIT_N; x++)
			{
				iS[S[x]] = x;
			}
		}
    else
		{
      for (int x = 0; x < LUT_UNIT_N; x++)
			{
				iS[x] = 0;
			}
		}
    res.LUT_to_bit_slice();
		return res;
	};

  bool is_involution() const
  {
    if (N == 4)
    {
      __m128i iLUT[LUT_XMM_N];
      inverse((uint8_t *)iLUT);
      return EQU(LUT[0], iLUT[0]);
    }
    else
    {
      uint8_t iS[LUT_UNIT_N];
      uint8_t * S = (uint8_t *)LUT;
      inverse(iS);
      for (int i = 0; i < LUT_UNIT_N; i++)
      {
        if (S[i] != iS[i]) return false;
      }
      return true;
    }
  }

  void difference_distribution_matrix(int DDT[LUT_UNIT_N][LUT_UNIT_N], int & Diff, int DDT_spectrum[LUT_UNIT_N+1], int & Diff1, int DDT1_spectrum[LUT_UNIT_N+1]) const
  {
    memset(DDT[0], 0, sizeof(int) * LUT_UNIT_N * LUT_UNIT_N);
    Diff = 0;
    memset(DDT_spectrum, 0, sizeof(int) * (LUT_UNIT_N+1));
    Diff1 = 0;
    memset(DDT1_spectrum, 0, sizeof(int) * (LUT_UNIT_N+1));

    if (N == 4)
    {
	    __m128i ton;
	    __m128i t1;
	    __m128i t2;
	    int cnt;

      #undef TestOd_1_1
      #define TestOd_1_1(id, od)                       \
      {                                                \
      	ton = _mm_cmpeq_epi8(x##od, t2);               \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(ton));  \
        DDT[0x##id][0x##od] = cnt;                     \
        Diff = cnt > Diff ? cnt : Diff;                \
        DDT_spectrum[cnt]++;                           \
        Diff1 = cnt > Diff1 ? cnt : Diff1;             \
        DDT1_spectrum[cnt]++;                          \
      }

      #undef TestOd
      #define TestOd(id, od)                           \
      {                                                \
      	ton = _mm_cmpeq_epi8(x##od, t2);               \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(ton));  \
        DDT[0x##id][0x##od] = cnt;                     \
        Diff = cnt > Diff ? cnt : Diff;                \
        DDT_spectrum[cnt]++;                           \
      }

      #undef TestId_1
      #define TestId_1(id)                   \
      {												               \
      	t1 = _mm_xor_si128(x##id, x);				 \
      	t2 = _mm_shuffle_epi8(LUT[0], t1);	 \
      	t2 = _mm_xor_si128(LUT[0], t2);			 \
      	TestOd_1_1(id, 1);									 \
      	TestOd_1_1(id, 2);									 \
      	TestOd_1_1(id, 4);									 \
      	TestOd_1_1(id, 8);									 \
        TestOd(id, 0);									     \
      	TestOd(id, 3);									     \
      	TestOd(id, 5);									     \
      	TestOd(id, 6);									     \
      	TestOd(id, 9);									     \
      	TestOd(id, a);									     \
      	TestOd(id, c);									     \
      	TestOd(id, 7);									     \
      	TestOd(id, b);									     \
      	TestOd(id, d);									     \
      	TestOd(id, e);									     \
      	TestOd(id, f);									     \
      }

      #undef TestId_n
      #define TestId_n(id)                     \
      {												                 \
      	t1 = _mm_xor_si128(x##id, x);				   \
      	t2 = _mm_shuffle_epi8(LUT[0], t1);		 \
      	t2 = _mm_xor_si128(LUT[0], t2);				 \
        TestOd(id, 0);									       \
      	TestOd(id, 1);									       \
      	TestOd(id, 2);									       \
      	TestOd(id, 4);									       \
      	TestOd(id, 8);									       \
      	TestOd(id, 3);									       \
      	TestOd(id, 5);									       \
      	TestOd(id, 6);									       \
      	TestOd(id, 9);									       \
      	TestOd(id, a);									       \
      	TestOd(id, c);									       \
      	TestOd(id, 7);									       \
      	TestOd(id, b);									       \
      	TestOd(id, d);									       \
      	TestOd(id, e);									       \
      	TestOd(id, f);									       \
      }

      TestId_1(1);	
	    TestId_1(2);	
	    TestId_1(4);	
	    TestId_1(8);	
	    TestId_n(3);	
	    TestId_n(5);	
	    TestId_n(6);	
	    TestId_n(9);
	    TestId_n(a);	
	    TestId_n(c);	
	    TestId_n(7);	
	    TestId_n(b);	
	    TestId_n(d);	
	    TestId_n(e);	
	    TestId_n(f);
      DDT[0][0] = 16;
      DDT_spectrum[0] += 15;
      DDT_spectrum[16]++;
    }
    else if (N <= 8)
    {
      int cnt;

      uint8_t * S = (uint8_t *)LUT;
      for (int i = 1; i <= N; i++)
      {
        int id = HWorder<N>[i];

        for (int x = 0; x < LUT_UNIT_N; x++)
        {
          int od = S[x] ^ S[x ^ id];
          DDT[id][od]++;
        }
        for (int o = 1; o <= N; o++)
        {
          int od = HWorder<N>[o];
          cnt = DDT[id][od];
          Diff = cnt > Diff ? cnt : Diff;
          DDT_spectrum[cnt]++;
          Diff1 = cnt > Diff1 ? cnt : Diff1;
          DDT1_spectrum[cnt]++;
        }
        for (int o = N + 1; o <= LUT_UNIT_N; o++)
        {
          int od = HWorder<N>[o%LUT_UNIT_N];
          cnt = DDT[id][od];
          DDT_spectrum[cnt]++;
          Diff = cnt > Diff ? cnt : Diff;
        }
      }
      for (int i = N + 1; i < LUT_UNIT_N; i++)
      {
        int id = HWorder<N>[i];
        for (int x = 0; x < LUT_UNIT_N; x++)
        {
          int od = S[x] ^ S[x ^ id];
          DDT[id][od]++;
        }
        for (int od = 0; od < LUT_UNIT_N; od++)
        {
          cnt = DDT[id][od];
          DDT_spectrum[cnt]++;
          Diff = cnt > Diff ? cnt : Diff;
        }
      }
      DDT[0][0] = LUT_UNIT_N;
      DDT_spectrum[LUT_UNIT_N]++;
      DDT_spectrum[0] += LUT_UNIT_N - 1;
    }
    else
    {
      // cout << "difference_distribution_matrix():: not support for N < 3 and N > 8" << endl;
    }
  }

  string show_difference_distribution_matrix() const
  {
    int DDT[LUT_UNIT_N][LUT_UNIT_N];
    int Diff;
    int DDT_spectrum[LUT_UNIT_N+1];
    int Diff1;
    int DDT1_spectrum[LUT_UNIT_N+1];
    difference_distribution_matrix(DDT, Diff, DDT_spectrum, Diff1, DDT1_spectrum);

    stringstream ss;
    ss << "DDT = " << endl;
    //ss << OPs<N>.show_matrix(DDT[0], LUT_UNIT_N, LUT_UNIT_N);
    ss << OPs<N>.show_matrix_HWorder(DDT);
    ss << "Diff: " << Diff << ", " << "DDT_spectrum: {";
    for (int i = 0; i < LUT_UNIT_N+1; i++)
    {
      if (DDT_spectrum[i]!=0) ss << i << ":" << DDT_spectrum[i] << ", ";
    }
    ss << "};" << endl;
    ss << "Diff1: " << Diff1 << ", " << "DDT1_spectrum: {";
    for (int i = 0; i < LUT_UNIT_N+1; i++)
    {
      if (DDT1_spectrum[i]!=0) ss << i << ":" << DDT1_spectrum[i] << ", ";
    }
    ss << "};" << endl;
    return ss.str();
  }

  void show_difference_distribution_matrix(ofstream & fout) const
  {
    fout << show_difference_distribution_matrix();
  }

  bool difference_distribution_matrix_test(int MaxDiff_Bound, int MaxDiff1_Bound, int DiffFreq_Bound, int CardD1_Bound) const
  {
    bool computeAll = false;

    if (MaxDiff_Bound == -1) MaxDiff_Bound = LUT_UNIT_N;
    else computeAll = true;

    if (DiffFreq_Bound == -1) DiffFreq_Bound = LUT_UNIT_N * LUT_UNIT_N;
    else computeAll = true;

    if (MaxDiff1_Bound == -1) MaxDiff1_Bound = MaxDiff_Bound;
    if (CardD1_Bound == -1) CardD1_Bound = N * N;

    int Diff = 0;
    int CardD1 = 0;
    int DDT_spectrum[LUT_UNIT_N+1] = {0};

    if (N == 4)
    {
	    __m128i ton;
	    __m128i t1;
	    __m128i t2;
	    int cnt;

      #undef TestOd_1_1
      #define TestOd_1_1(id, od)                                 \
      {                                                          \
      	ton = _mm_cmpeq_epi8(x##od, t2);                         \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(ton));            \
        if (cnt > MaxDiff1_Bound) return false;                  \
        CardD1 += (cnt == 0) ? 0 : 1;                            \
        if (CardD1 > CardD1_Bound) return false;                 \
        Diff = cnt > Diff ? cnt : Diff;                          \
        DDT_spectrum[cnt]++;                                     \
      }

      #undef TestOd
      #define TestOd(id, od)                           \
      {                                                \
      	ton = _mm_cmpeq_epi8(x##od, t2);               \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(ton));  \
        if (cnt > MaxDiff_Bound) return false;         \
        Diff = cnt > Diff ? cnt : Diff;                \
        DDT_spectrum[cnt]++;                           \
      }

      #undef TestId_1
      #define TestId_1(id)                   \
      {												               \
      	t1 = _mm_xor_si128(x##id, x);				 \
      	t2 = _mm_shuffle_epi8(LUT[0], t1);	 \
      	t2 = _mm_xor_si128(LUT[0], t2);			 \
      	TestOd_1_1(id, 1);									 \
      	TestOd_1_1(id, 2);									 \
      	TestOd_1_1(id, 4);									 \
      	TestOd_1_1(id, 8);									 \
        if (computeAll)                      \
        {                                    \
          TestOd(id, 0);									   \
      	  TestOd(id, 3);									   \
      	  TestOd(id, 5);									   \
      	  TestOd(id, 6);									   \
      	  TestOd(id, 9);									   \
      	  TestOd(id, a);									   \
      	  TestOd(id, c);									   \
      	  TestOd(id, 7);									   \
      	  TestOd(id, b);									   \
      	  TestOd(id, d);									   \
      	  TestOd(id, e);									   \
      	  TestOd(id, f);									   \
        }                                    \
      }

      #undef TestId_n
      #define TestId_n(id)                     \
      {												                 \
      	t1 = _mm_xor_si128(x##id, x);				   \
      	t2 = _mm_shuffle_epi8(LUT[0], t1);		 \
      	t2 = _mm_xor_si128(LUT[0], t2);				 \
        TestOd(id, 0);									       \
      	TestOd(id, 1);									       \
      	TestOd(id, 2);									       \
      	TestOd(id, 4);									       \
      	TestOd(id, 8);									       \
      	TestOd(id, 3);									       \
      	TestOd(id, 5);									       \
      	TestOd(id, 6);									       \
      	TestOd(id, 9);									       \
      	TestOd(id, a);									       \
      	TestOd(id, c);									       \
      	TestOd(id, 7);									       \
      	TestOd(id, b);									       \
      	TestOd(id, d);									       \
      	TestOd(id, e);									       \
      	TestOd(id, f);									       \
      }

      TestId_1(1);	
	    TestId_1(2);	
	    TestId_1(4);	
	    TestId_1(8);
      if (computeAll)
      {
	      TestId_n(3);	
	      TestId_n(5);	
	      TestId_n(6);	
	      TestId_n(9);
	      TestId_n(a);	
	      TestId_n(c);	
	      TestId_n(7);	
	      TestId_n(b);	
	      TestId_n(d);	
	      TestId_n(e);	
	      TestId_n(f);
        if (DDT_spectrum[Diff] > DiffFreq_Bound) return false;
      }
      return true;
    }
    else if (N <= 8)
    {
      int DDT_id[LUT_UNIT_N];
      int cnt;

      uint8_t * S = (uint8_t *)LUT;
      for (int i = 1; i <= N; i++)
      {
        int id = HWorder<N>[i];
        memset(DDT_id, 0, sizeof(int) * LUT_UNIT_N);
        for (int x = 0; x < LUT_UNIT_N; x++)
        {
          int od = S[x] ^ S[x ^ id];
          DDT_id[od]++;
        }
        for (int o = 1; o <= N; o++)
        {
          int od = HWorder<N>[o];
          cnt = DDT_id[od];
          if (cnt > MaxDiff1_Bound) return false;
          CardD1 += (cnt == 0) ? 0 : 1;
          if (CardD1 > CardD1_Bound) return false;
          Diff = cnt > Diff ? cnt : Diff;
          DDT_spectrum[cnt]++;
        }
        if (computeAll)
        {
          for (int o = N + 1; o <= LUT_UNIT_N; o++)
          {
            int od = HWorder<N>[o%LUT_UNIT_N];
            cnt = DDT_id[od];
            if (cnt > MaxDiff_Bound) return false;
            Diff = cnt > Diff ? cnt : Diff;
            DDT_spectrum[cnt]++;
          }
        }
      }
      if (computeAll)
      {
        for (int i = N + 1; i < LUT_UNIT_N; i++)
        {
          int id = HWorder<N>[i];
          memset(DDT_id, 0, sizeof(int) * LUT_UNIT_N);
          for (int x = 0; x < LUT_UNIT_N; x++)
          {
            int od = S[x] ^ S[x ^ id];
            DDT_id[od]++;
          }
          for (int od = 0; od < LUT_UNIT_N; od++)
          {
            cnt = DDT_id[od];
            if (cnt > MaxDiff_Bound) return false;
            Diff = cnt > Diff ? cnt : Diff;
            DDT_spectrum[cnt]++;
          }
        }
        if (DDT_spectrum[Diff] > DiffFreq_Bound) return false;
      }
      return true;
    }
    else
    {
      // cout << "linear_approximation_matrix():: not support for N < 3 and N > 8" << endl;
    }
  }

  void boomerang_connection_table(int BCT[LUT_UNIT_N][LUT_UNIT_N], int & BCT_uniformity, int BCT_spectrum[LUT_UNIT_N+1]) const
  {
    if (!is_permutation())
    {
      return;
    }
    memset(BCT[0], 0, sizeof(int) * LUT_UNIT_N * LUT_UNIT_N);
    BCT_uniformity = 0;
    memset(BCT_spectrum, 0, sizeof(int) * (LUT_UNIT_N+1));

    if ((N >= 3) && (N <= 8))
    {
      int cnt;
      uint8_t * S = (uint8_t *)LUT;
      uint8_t iS[LUT_UNIT_N];
      inverse(iS);
      
      vector<uint8_t> T[LUT_UNIT_N];
      for (int yi = 0; yi < LUT_UNIT_N; yi++) T[yi].reserve(LUT_UNIT_N); 
      for (int od = 0; od < LUT_UNIT_N; od++)
      {        
        for (int x = 0; x < LUT_UNIT_N; x++)
        {
          uint8_t y = x ^ iS[S[x] ^ od];
          T[y].push_back(x);
        }
        for (int yi = 0; yi < LUT_UNIT_N; yi++)
        {
          for (int xii = 0; xii < T[yi].size(); xii++)
          {
            uint8_t xi = T[yi][xii];
            for (int xji = 0; xji < T[yi].size(); xji++)
            {
              uint8_t xj = T[yi][xji];
              uint8_t id = xi ^ xj;
              BCT[id][od]++;
            }
          }
          T[yi].clear();
        }
        for (int id = 0; id < LUT_UNIT_N; id++)
        {
          if ((id != 0) && (od != 0)) {BCT_uniformity = BCT[id][od] > BCT_uniformity ? BCT[id][od] : BCT_uniformity;}
          BCT_spectrum[BCT[id][od]]++; 
        }
      }
    }
    else
    {
      // cout << "boomerang_connection_table():: not support for N < 3 and N > 8" << endl;
    }
  }

  string show_boomerang_connection_table() const
  {
    if (!is_permutation())
    {
      return "The Sbox is not a permutation and I can not compute its BCT.";
    }
    int BCT[LUT_UNIT_N][LUT_UNIT_N];
    int BCT_uniformity;
    int BCT_spectrum[LUT_UNIT_N+1];
    boomerang_connection_table(BCT, BCT_uniformity, BCT_spectrum);

    stringstream ss;
    ss << "BCT = " << endl;
    //ss << OPs<N>.show_matrix(BCT[0], LUT_UNIT_N, LUT_UNIT_N);
    ss << OPs<N>.show_matrix_HWorder(BCT);
    ss << "BCT_uniformity: " << BCT_uniformity << ", " << "BCT_spectrum: {";
    for (int i = 0; i < LUT_UNIT_N+1; i++)
    {
      if (BCT_spectrum[i]!=0) ss << i << ":" << BCT_spectrum[i] << ", ";
    }
    ss << "};" << endl;
    return ss.str();
  }

  void show_boomerang_connection_table(ofstream & fout) const
  {
    fout << show_boomerang_connection_table();
  }

  bool boomerang_connection_table_test(int BCT_uniformity_Bound, int BCT_uniformity_Freq_Bound) const
  {
    int BCT[LUT_UNIT_N][LUT_UNIT_N];
    int BCT_uniformity;
    int BCT_spectrum[LUT_UNIT_N+1];

    memset(BCT[0], 0, sizeof(int) * LUT_UNIT_N * LUT_UNIT_N);
    BCT_uniformity = 0;
    memset(BCT_spectrum, 0, sizeof(int) * (LUT_UNIT_N+1));

    if ((N >= 3) && (N <= 8))
    {
      int cnt;
      uint8_t * S = (uint8_t *)LUT;
      uint8_t iS[LUT_UNIT_N];
      inverse(iS);
      
      vector<uint8_t> T[LUT_UNIT_N];
      for (int yi = 0; yi < LUT_UNIT_N; yi++) T[yi].reserve(LUT_UNIT_N); 
      for (int od = 0; od < LUT_UNIT_N; od++)
      {        
        for (int x = 0; x < LUT_UNIT_N; x++)
        {
          uint8_t y = x ^ iS[S[x] ^ od];
          T[y].push_back(x);
        }
        for (int yi = 0; yi < LUT_UNIT_N; yi++)
        {
          for (int xii = 0; xii < T[yi].size(); xii++)
          {
            uint8_t xi = T[yi][xii];
            for (int xji = 0; xji < T[yi].size(); xji++)
            {
              uint8_t xj = T[yi][xji];
              uint8_t id = xi ^ xj;
              BCT[id][od]++;
            }
          }
          T[yi].clear();
        }
        for (int id = 0; id < LUT_UNIT_N; id++)
        {
          if ((id != 0) && (od != 0))
          {
            BCT_uniformity = BCT[id][od] > BCT_uniformity ? BCT[id][od] : BCT_uniformity;
          }
          if (BCT_uniformity > BCT_uniformity_Bound) return false;
          BCT_spectrum[BCT[id][od]]++;
        }
      }
      if (BCT_spectrum[BCT_uniformity] > BCT_uniformity_Freq_Bound) return false;
      return true;
    }
    else
    {
      // cout << "boomerang_connection_table():: not support for N < 3 and N > 8" << endl;
      return false;
    }   
  }

  void linear_approximation_matrix(int LAT[LUT_UNIT_N][LUT_UNIT_N], int & Lin, int LAT_spectrum[LUT_UNIT_N+1], int & Lin1, int LAT1_spectrum[LUT_UNIT_N+1]) const
  {
    memset(LAT[0], 0, sizeof(int) * LUT_UNIT_N * LUT_UNIT_N);
    Lin = 0;
    memset(LAT_spectrum, 0, sizeof(int) * (LUT_UNIT_N+1));
    Lin1 = 0;
    memset(LAT1_spectrum, 0, sizeof(int) * (LUT_UNIT_N+1));

    if (N == 4)
    {
	    __m128i t1;
	    __m128i t2;
	    int cnt;

      #undef TestOm_1_1
      #define TestOm_1_1(im, om)                                               \
      {                                                                        \
      	t2 = _mm_and_si128(x##om, LUT[0]);                                     \
      	t2 = _mm_xor_si128(t1, t2);                                            \
      	t2 = _mm_xor_si128(_mm_slli_epi16(t2, 4), _mm_slli_epi16(t2, 5));      \
      	t2 = _mm_xor_si128(t2, _mm_slli_epi16(t2, 2));                         \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(t2));                           \
        cnt = 2 * abs(cnt - 8);                                                \
        LAT[0x##im][0x##om] = cnt;                                             \
        Lin = Lin >= cnt ? Lin : cnt;                                          \
        LAT_spectrum[cnt]++;                                                   \
        Lin1 = Lin1 >= cnt ? Lin1 : cnt;                                       \
        LAT1_spectrum[cnt]++;                                                  \
      }

      #undef TestOm
      #define TestOm(im, om)                                                   \
      {                                                                        \
      	t2 = _mm_and_si128(x##om, LUT[0]);                                     \
      	t2 = _mm_xor_si128(t1, t2);                                            \
      	t2 = _mm_xor_si128(_mm_slli_epi16(t2, 4), _mm_slli_epi16(t2, 5));      \
      	t2 = _mm_xor_si128(t2, _mm_slli_epi16(t2, 2));                         \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(t2));                           \
        cnt = 2 * abs(cnt - 8);                                                \
        LAT[0x##im][0x##om] = cnt;                                             \
        Lin = Lin >= cnt ? Lin : cnt;                                          \
        LAT_spectrum[cnt]++;                                                   \
      }

      #undef TestIm_1
      #define TestIm_1(id)							   \
      {												             \
      	t1 = _mm_and_si128(x##id, x);			 \
      	TestOm_1_1(id, 1);								 \
      	TestOm_1_1(id, 2);								 \
      	TestOm_1_1(id, 4);								 \
      	TestOm_1_1(id, 8);								 \
      	TestOm(id, 3);									   \
      	TestOm(id, 5);									   \
      	TestOm(id, 6);									   \
      	TestOm(id, 9);									   \
      	TestOm(id, a);									   \
      	TestOm(id, c);									   \
      	TestOm(id, 7);									   \
      	TestOm(id, b);									   \
      	TestOm(id, d);									   \
      	TestOm(id, e);									   \
      	TestOm(id, f);									   \
      }

      #undef TestIm_n
      #define TestIm_n(id)								 \
      {												             \
      	t1 = _mm_and_si128(x##id, x);			 \
      	TestOm(id, 1);									   \
      	TestOm(id, 2);									   \
      	TestOm(id, 4);									   \
      	TestOm(id, 8);									   \
      	TestOm(id, 3);									   \
      	TestOm(id, 5);									   \
      	TestOm(id, 6);									   \
      	TestOm(id, 9);									   \
      	TestOm(id, a);									   \
      	TestOm(id, c);									   \
      	TestOm(id, 7);									   \
      	TestOm(id, b);									   \
      	TestOm(id, d);									   \
      	TestOm(id, e);									   \
      	TestOm(id, f);									   \
      }

      TestIm_n(0);
      TestIm_1(1);	
	    TestIm_1(2);	
	    TestIm_1(4);	
	    TestIm_1(8);	
	    TestIm_n(3);	
	    TestIm_n(5);	
	    TestIm_n(6);	
	    TestIm_n(9);	
	    TestIm_n(a);	
	    TestIm_n(c);	
	    TestIm_n(7);	
	    TestIm_n(b);	
	    TestIm_n(d);	
	    TestIm_n(e);	
	    TestIm_n(f);
      LAT[0][0] = 16;
      LAT_spectrum[0] += 15;
      LAT_spectrum[16]++;
    }
    else if (N <= 8)
    {
      int cnt;

      uint8_t * S = (uint8_t *)LUT;
      for (int i = 1; i <= N; i++)
      {
        int im = HWorder<N>[i];
        for (int o = 1; o <= N; o++)
        {
          int om = HWorder<N>[o];
          for (int x = 0; x < LUT_UNIT_N; x++)
          {
            int ia = im & x;
            int oa = om & S[x];
            int p = _mm_popcnt_u32(ia ^ oa) & 1;
            LAT[im][om] += p;
          }
          cnt = 2 * abs(LAT[im][om] - (int)(LUT_UNIT_N / 2));
          LAT[im][om] = cnt;
          Lin = Lin >= cnt ? Lin : cnt;          
          LAT_spectrum[cnt]++;
          Lin1 = Lin1 >= cnt ? Lin1 : cnt;          
          LAT1_spectrum[cnt]++;
        }
        for (int o = N + 1; o < LUT_UNIT_N; o++)
        {
          int om = HWorder<N>[o];
          for (int x = 0; x < LUT_UNIT_N; x++)
          {
            int ia = im & x;
            int oa = om & S[x];
            int p = _mm_popcnt_u32(ia ^ oa) & 1;
            LAT[im][om] += p;
          }
          cnt = 2 * abs(LAT[im][om] - (int)(LUT_UNIT_N / 2));
          LAT[im][om] = cnt;
          Lin = Lin >= cnt ? Lin : cnt;          
          LAT_spectrum[cnt]++;
        }
      }
      for (int i = N + 1; i <= LUT_UNIT_N; i++)
      {
        int im = HWorder<N>[i%LUT_UNIT_N];
        for (int om = 1; om < LUT_UNIT_N; om++)
        {
          for (int x = 0; x < LUT_UNIT_N; x++)
          {
            int ia = im & x;
            int oa = om & S[x];
            int p = _mm_popcnt_u32(ia ^ oa) & 1;
            LAT[im][om] += p;
          }
          cnt = 2 * abs(LAT[im][om] - (int)(LUT_UNIT_N / 2));
          LAT[im][om] = cnt;
          Lin = Lin >= cnt ? Lin : cnt;          
          LAT_spectrum[cnt]++;
        }
      }
      LAT[0][0] = LUT_UNIT_N;
      LAT_spectrum[LUT_UNIT_N]++;
      LAT_spectrum[0] += LUT_UNIT_N - 1;
    }
  }

  string show_linear_approximation_matrix() const
  {
    int LAT[LUT_UNIT_N][LUT_UNIT_N];
    int Lin;
    int LAT_spectrum[LUT_UNIT_N+1];
    int Lin1;
    int LAT1_spectrum[LUT_UNIT_N+1];
    linear_approximation_matrix(LAT, Lin, LAT_spectrum, Lin1, LAT1_spectrum);

    stringstream ss;
    ss << "LAT = " << endl;
    //ss << OPs<N>.show_matrix(LAT[0], LUT_UNIT_N, LUT_UNIT_N);
    ss << OPs<N>.show_matrix_HWorder(LAT);
    ss << "Lin: " << Lin << ", " << "LAT_spectrum: {";
    for (int i = 0; i < LUT_UNIT_N+1; i++)
    {
      if (LAT_spectrum[i] != 0) ss << i << ":" << LAT_spectrum[i] << ", ";
    }
    ss << "};" << endl;
    ss << "Lin1: " << Lin1 << ", " << "LAT1_spectrum: {";
    for (int i = 0; i < LUT_UNIT_N+1; i++)
    {
      if (LAT1_spectrum[i] != 0) ss << i << ":" << LAT1_spectrum[i] << ", ";
    }
    ss << "};" << endl;
    return ss.str();
  }

  void show_linear_approximation_matrix(ofstream & fout) const
  {
    fout << show_linear_approximation_matrix();
  }

  bool linear_approximation_matrix_test(int MaxLin_Bound, int MaxLin1_Bound, int LinFreq_Bound, int CardL1_Bound) const
  {
    bool computeAll = false;

    if (MaxLin_Bound == -1) MaxLin_Bound = LUT_UNIT_N;
    else computeAll = true;

    if (LinFreq_Bound == -1) LinFreq_Bound = LUT_UNIT_N * LUT_UNIT_N;
    else computeAll = true;

    if (MaxLin1_Bound == -1) MaxLin1_Bound = MaxLin_Bound;
    if (CardL1_Bound == -1) CardL1_Bound = N * N;

    int Lin = 0;
    int CardL1 = 0;
    int LAT_spectrum[LUT_UNIT_N+1] = {0};

    if (N == 4)
    {
	    __m128i t1;
	    __m128i t2;
	    int cnt;

      #undef TestOm_1_1
      #define TestOm_1_1(im, om)                                               \
      {                                                                        \
      	t2 = _mm_and_si128(x##om, LUT[0]);                                     \
      	t2 = _mm_xor_si128(t1, t2);                                            \
      	t2 = _mm_xor_si128(_mm_slli_epi16(t2, 4), _mm_slli_epi16(t2, 5));      \
      	t2 = _mm_xor_si128(t2, _mm_slli_epi16(t2, 2));                         \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(t2));                           \
        cnt = 2 * abs(cnt - 8);                                                \
        if (cnt > MaxLin1_Bound) return false;                                \
        CardL1 += (cnt == 0) ? 0 : 1;                                          \
        if (CardL1 > CardL1_Bound) return false;                               \
        Lin = Lin >= cnt ? Lin : cnt;                                          \
        LAT_spectrum[cnt]++;                                                   \
      }

      #undef TestOm
      #define TestOm(im, om)                                                   \
      {                                                                        \
      	t2 = _mm_and_si128(x##om, LUT[0]);                                     \
      	t2 = _mm_xor_si128(t1, t2);                                            \
      	t2 = _mm_xor_si128(_mm_slli_epi16(t2, 4), _mm_slli_epi16(t2, 5));      \
      	t2 = _mm_xor_si128(t2, _mm_slli_epi16(t2, 2));                         \
      	cnt = _mm_popcnt_u32(_mm_movemask_epi8(t2));                           \
        cnt = 2 * abs(cnt - 8);                                                \
        if (cnt > MaxLin_Bound) return false;                                  \
        Lin = Lin >= cnt ? Lin : cnt;                                          \
        LAT_spectrum[cnt]++;                                                   \
      }

      #undef TestIm_1_1
      #define TestIm_1_1(id)							 \
      {												             \
      	t1 = _mm_and_si128(x##id, x);			 \
      	TestOm_1_1(id, 1);								 \
      	TestOm_1_1(id, 2);								 \
      	TestOm_1_1(id, 4);								 \
      	TestOm_1_1(id, 8);								 \
      }

      #undef TestIm_1_n
      #define TestIm_1_n(id)							 \
      {												             \
      	t1 = _mm_and_si128(x##id, x);			 \
      	TestOm(id, 3);									   \
      	TestOm(id, 5);									   \
      	TestOm(id, 6);									   \
      	TestOm(id, 9);									   \
      	TestOm(id, a);									   \
      	TestOm(id, c);									   \
      	TestOm(id, 7);									   \
      	TestOm(id, b);									   \
      	TestOm(id, d);									   \
      	TestOm(id, e);									   \
      	TestOm(id, f);									   \
      }

      #undef TestIm_n
      #define TestIm_n(id)								 \
      {												             \
      	t1 = _mm_and_si128(x##id, x);			 \
      	TestOm(id, 1);									   \
      	TestOm(id, 2);									   \
      	TestOm(id, 4);									   \
      	TestOm(id, 8);									   \
      	TestOm(id, 3);									   \
      	TestOm(id, 5);									   \
      	TestOm(id, 6);									   \
      	TestOm(id, 9);									   \
      	TestOm(id, a);									   \
      	TestOm(id, c);									   \
      	TestOm(id, 7);									   \
      	TestOm(id, b);									   \
      	TestOm(id, d);									   \
      	TestOm(id, e);									   \
      	TestOm(id, f);									   \
      }

      TestIm_1_1(1);	
	    TestIm_1_1(2);	
	    TestIm_1_1(4);	
	    TestIm_1_1(8);
      if (computeAll)
      {
        TestIm_1_n(1);	
	      TestIm_1_n(2);	
	      TestIm_1_n(4);	
	      TestIm_1_n(8);
        TestIm_n(0);
        TestIm_n(3);	
	      TestIm_n(5);	
	      TestIm_n(6);	
	      TestIm_n(9);	
	      TestIm_n(a);	
	      TestIm_n(c);	
	      TestIm_n(7);	
	      TestIm_n(b);	
	      TestIm_n(d);	
	      TestIm_n(e);	
	      TestIm_n(f);
        if (LAT_spectrum[Lin] > LinFreq_Bound) return false;
      }
      return true;
    }
    else if (N <= 8)
    {
      int cnt;
      uint8_t * S = (uint8_t *)LUT;
      for (int i = 1; i <= N; i++)
      {
        int im = HWorder<N>[i];
        for (int o = 1; o <= N; o++)
        {
          int om = HWorder<N>[o];
          cnt = 0;
          for (int x = 0; x < LUT_UNIT_N; x++)
          {
            int ia = im & x;
            int oa = om & S[x];
            int p = _mm_popcnt_u32(ia ^ oa) & 1;
            cnt += p;
          }
          cnt = 2 * abs(cnt - (LUT_UNIT_N / 2));
          if (cnt > MaxLin1_Bound) return false;
          CardL1 += (cnt == 0) ? 0 : 1;
          if (CardL1 > CardL1_Bound) return false;
          Lin = cnt > Lin ? cnt : Lin;
          LAT_spectrum[cnt]++;
        }
      }
      if (computeAll)
      {
        for (int i = 1; i <= N; i++)
        {
          int im = HWorder<N>[i];
          for (int o = N + 1; o < LUT_UNIT_N; o++)
          {
            int om = HWorder<N>[o];
            cnt = 0;
            for (int x = 0; x < LUT_UNIT_N; x++)
            {
              int ia = im & x;
              int oa = om & S[x];
              int p = _mm_popcnt_u32(ia ^ oa) & 1;
              cnt += p;
            }
            cnt = 2 * abs(cnt - (LUT_UNIT_N / 2));
            if (cnt > MaxLin_Bound) return false;
            Lin = cnt > Lin ? cnt : Lin;
            LAT_spectrum[cnt]++;
          }
        }
        for (int i = N + 1; i <= LUT_UNIT_N; i++)
        {
          int im = HWorder<N>[i%LUT_UNIT_N];
          for (int om = 1; om < LUT_UNIT_N; om++)
          {
            cnt = 0;
            for (int x = 0; x < LUT_UNIT_N; x++)
            {
              int ia = im & x;
              int oa = om & S[x];
              int p = _mm_popcnt_u32(ia ^ oa) & 1;
              cnt += p;
            }
            cnt = 2 * abs(cnt - (LUT_UNIT_N / 2));
            if (cnt > MaxLin_Bound) return false;
            Lin = cnt > Lin ? cnt : Lin;
            LAT_spectrum[cnt]++;
          }
        }
        if (LAT_spectrum[Lin] > LinFreq_Bound) return false;
      }
      return true;
    }
  }

  void get_coordinates_ANF(bit_slice_t<N> & coordinates_ANF) const
  {
    if (N == 3)
    {
      __m128i coor_ANF = _mm_loadu_si128((__m128i *)bit_slice.data());

      coor_ANF = _mm_and_si128(coor_ANF, S3_valv_mask8);

      // compute ANF for coordinates
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 4), S3_valv_mask4));      
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 2), S3_valv_mask2));
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 1), S3_valv_mask1));

      // ANF for coordinates
      coordinates_ANF[0][0] = (UINT_<N>)_mm_extract_epi8(coor_ANF, 0);
	    coordinates_ANF[1][0] = (UINT_<N>)_mm_extract_epi8(coor_ANF, 1);
	    coordinates_ANF[2][0] = (UINT_<N>)_mm_extract_epi8(coor_ANF, 2);
    }
    else if (N == 4)
    {
      __m128i coor_ANF = _mm_loadu_si128((__m128i *)bit_slice.data());

      coor_ANF = _mm_and_si128(coor_ANF, S4_valv_maskg);

      // compute ANF for coordinates
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 8), S4_valv_mask8));
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 4), S4_valv_mask4));      
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 2), S4_valv_mask2));
      coor_ANF = _mm_xor_si128(coor_ANF, _mm_and_si128(_mm_slli_epi16(coor_ANF, 1), S4_valv_mask1));

      // ANF for coordinates
      coordinates_ANF[0][0] = (UINT_<N>)_mm_extract_epi16(coor_ANF, 0);
	    coordinates_ANF[1][0] = (UINT_<N>)_mm_extract_epi16(coor_ANF, 1);
	    coordinates_ANF[2][0] = (UINT_<N>)_mm_extract_epi16(coor_ANF, 2);
	    coordinates_ANF[3][0] = (UINT_<N>)_mm_extract_epi16(coor_ANF, 3);
    }
    else if (N <= 8)
    {
      coordinates_ANF = bit_slice;
      // compute ANF for coordinates
      for (int i = 0; i < N; i++)
      {
        for (int j = N - 1; j >= 0; j--)
        {
          coordinates_ANF[i] = coordinates_ANF[i] ^ (OPs<N>.sll(coordinates_ANF[i], (1ULL<<j)) & masks<N>[j]);
        }
      }
    }
    else
    {
      // cout << "get_coordinates_ANF():: not support for N < 3 and N > 8" << endl;
    }
  }

  string show_coordinates_ANF() const
  {
    array<bit_slice_l_t<N>, N> coordinates_ANF;
    get_coordinates_ANF(coordinates_ANF);

    int related;
    int deg, term_n, related_n;  

    int degs[N] = {0};
    int maxdeg = 0;
    for (int coori = 0; coori < N; coori++)
    {
      degs[coori] = degree_from_ANF(coordinates_ANF[coori]);
      maxdeg = degs[coori] > maxdeg ? degs[coori] : maxdeg;
    }
    int endoff = (maxdeg == N) ? (1<<N) : HWorder_off<N>[maxdeg+1];
    
    stringstream ss;
    ss << "ANF of coordinates:" << endl;
    for (int coori = 0; coori < N; coori++)
    {
      deg = 0; term_n = 0; related_n = 0; related = 0;      
      string coorstr = "y" + std::to_string(coori) + " = ";
      coorstr += (OPs<N>.get_bit(coordinates_ANF[coori], 0) == 1) ? "1 + " : "  + ";
      for (int termi = 1; termi < endoff; termi++)
      {
        int termx = HWorder<N>[termi];
        int width = _mm_popcnt_u32(termx);

        if (OPs<N>.get_bit(coordinates_ANF[coori], termx) == 1)
        {
          for (int biti = 0; biti < N; biti++)
          {
            if (((termx >> biti) & 1) == 1)
            {
              related |= (1 << biti);
              coorstr += "x" + std::to_string(biti);
            }
          }
        }
        else
        {
          for (int spacei = 0; spacei < width; spacei++) coorstr = coorstr + "  ";
        }
        coorstr += " + ";
      }
      coorstr.pop_back();coorstr.pop_back();coorstr.pop_back();
      ss << coorstr << ";    ";
      deg = degs[coori];
      term_n = OPs<N>.cnt_1(coordinates_ANF[coori]);
      related_n = _mm_popcnt_u32(related);

      ss << "deg = " << setw((int)log10(N)+1) << deg << ", term_n = " << setw((int)log10(1<<N)+1) << term_n << ", related_n = " << setw((int)log10(N)) << related_n;
      ss << endl;
    }
    return ss.str();
  }

  void show_coordinates_ANF(ofstream & fout) const
  {
    fout << show_coordinates_ANF();
  }

  void get_components_ANF(array<bit_slice_l_t<N>, 1<<N> & components_ANF) const
  {
    bit_slice_t<N> coordinates_ANF;
    get_coordinates_ANF(coordinates_ANF);
    if (N == 3)
    {
      UINT_<N> a, b, c;
      a = coordinates_ANF[0][0];
      b = coordinates_ANF[1][0];
      c = coordinates_ANF[2][0];

      // ANF for components
      *((UINT_<N> *)(components_ANF[7].data())) = c ^ b ^ a;
      *((UINT_<N> *)(components_ANF[6].data())) = c ^ b;
      *((UINT_<N> *)(components_ANF[5].data())) = c     ^ a;
      *((UINT_<N> *)(components_ANF[4].data())) = c;
      *((UINT_<N> *)(components_ANF[3].data())) = b ^ a;
      *((UINT_<N> *)(components_ANF[2].data())) = b;
      *((UINT_<N> *)(components_ANF[1].data())) = a;
      *((UINT_<N> *)(components_ANF[0].data())) = 0;
    }
    else if (N == 4)
    {
      UINT_<N> a, b, c, d;
      // ANF for coordinates
      a = coordinates_ANF[0][0];
	    b = coordinates_ANF[1][0];
	    c = coordinates_ANF[2][0];
	    d = coordinates_ANF[3][0];

      // ANF for components
      *((UINT_<N> *)(components_ANF[15].data())) = d ^ c ^ b ^ a;
      *((UINT_<N> *)(components_ANF[14].data())) = d ^ c ^ b;
      *((UINT_<N> *)(components_ANF[13].data())) = d ^ c     ^ a;
      *((UINT_<N> *)(components_ANF[12].data())) = d ^ c;
      *((UINT_<N> *)(components_ANF[11].data())) = d     ^ b ^ a;
      *((UINT_<N> *)(components_ANF[10].data())) = d     ^ b;
      *((UINT_<N> *)(components_ANF[ 9].data())) = d         ^ a;
      *((UINT_<N> *)(components_ANF[ 8].data())) = d;
      *((UINT_<N> *)(components_ANF[ 7].data())) = c ^ b ^ a;
      *((UINT_<N> *)(components_ANF[ 6].data())) = c ^ b;
      *((UINT_<N> *)(components_ANF[ 5].data())) = c     ^ a;
      *((UINT_<N> *)(components_ANF[ 4].data())) = c;
      *((UINT_<N> *)(components_ANF[ 3].data())) = b ^ a;
      *((UINT_<N> *)(components_ANF[ 2].data())) = b;
      *((UINT_<N> *)(components_ANF[ 1].data())) = a;
      *((UINT_<N> *)(components_ANF[ 0].data())) = 0;
    }
    else if (N <= 8)
    {
      bit_slice_t<N> coordinates_ANF;
      get_coordinates_ANF(coordinates_ANF);
      for (int i = 0; i < (1<<N); i++)
      {
        components_ANF[i].fill(0);
        for (int j = 0; j < N; j++)
        {
          if ( ((i>>j) & 1) ) components_ANF[i] = components_ANF[i] ^ coordinates_ANF[j];
        }
      }
    }
    else
    {
      // cout << "get_coordinates_ANF():: not support for N < 3 and N > 8" << endl;
    }
  }

  string show_components_ANF() const
  {
    array<bit_slice_l_t<N>, 1<<N> components_ANF;
    get_components_ANF(components_ANF);

    int related;
    int deg, term_n, related_n;  
    int deg_spectrum[N+1] = {0};
    int max_degree = 0;
    int min_degree = N;

    int degs[1<<N] = {0};
    for (int compi = 1; compi < (1<<N); compi++)
    {
      degs[compi] = degree_from_ANF(components_ANF[compi]);
      max_degree = degs[compi] > max_degree ? degs[compi] : max_degree;
      min_degree = degs[compi] < min_degree ? degs[compi] : min_degree;
      deg_spectrum[degs[compi]]++;
    }
    int endoff = (max_degree == N) ? (1<<N) : HWorder_off<N>[max_degree+1];
    
    stringstream ss;
    ss << "ANF of components:" << endl;
    for (int compi = 1; compi < (1<<N); compi++)
    {
      deg = 0; term_n = 0; related_n = 0; related = 0;      
      string compstr = "";
      for (int biti = 0; biti < N; biti++)
      {
        compstr = compstr + (((compi >> biti) & 1) ? ("y" + std::to_string(biti)) : "  ") + " + ";
      }
      compstr.pop_back();compstr.pop_back();compstr.pop_back();
      compstr += " = ";
      compstr += (OPs<N>.get_bit(components_ANF[compi], 0) == 1) ? "1 + " : "  + ";
      for (int termi = 1; termi < endoff; termi++)
      {
        int termx = HWorder<N>[termi];
        int width = _mm_popcnt_u32(termx);

        if (OPs<N>.get_bit(components_ANF[compi], termx) == 1)
        {
          for (int biti = 0; biti < N; biti++)
          {
            if (((termx >> biti) & 1) == 1)
            {
              related |= (1 << biti);
              compstr += "x" + std::to_string(biti);
            }
          }
        }
        else
        {
          for (int spacei = 0; spacei < width; spacei++) compstr = compstr + "  ";
        }
        compstr += " + ";
      }
      compstr.pop_back();compstr.pop_back();compstr.pop_back();
      ss << compstr << ";    ";
      deg = degs[compi];
      term_n = OPs<N>.cnt_1(components_ANF[compi]);
      related_n = _mm_popcnt_u32(related);

      ss << "deg = " << setw(digits(N)) << deg << ", term_n = " << setw(digits(1<<N)) << term_n << ", related_n = " << setw((int)log10(N)) << related_n;
      ss << endl;
    }
    ss 
      << "Max degree: " << setw(digits(N)) << max_degree << ", "
      << "Min degree: " << setw(digits(N)) << min_degree << ", "
      << "Spectrum of degree: " << "{ ";
    for (int i = 0; i <= N; i++) if (deg_spectrum[i] != 0) ss << i << ":" << deg_spectrum[i] << ", ";
    ss << "};" << endl;
    return ss.str();
  }

  void show_components_ANF(ofstream & fout) const
  {
    fout << show_components_ANF();
  }

  bool degree_test(const int max_degree_bound, const int min_degree_bound) const
  {
    int max_degree = 0;
    int min_degree = numeric_limits<int>::max();
    
    if (N == 3)
    {
      __m128i deg3_ind;
      __m128i deg2_ind;
      __m128i deg1_ind;

      int deg3_cnt = 0; 
      int deg2_cnt = 0; 
      int deg1_cnt = 0; 
      int deg0_cnt = 0;

      // Compute ANF for components
      array<bit_slice_l_t<N>, 1<<N> ALIGNED_(16) components_ANF = {{0}};
      get_components_ANF(components_ANF);
      __m128i components_ANF_all = _mm_loadl_epi64((__m128i *)((*components_ANF.data()).data()));

      deg3_ind = _mm_cmpeq_epi8(_mm_and_si128(components_ANF_all, S3_comps_deg_mask3), zero_128); // is not deg3 0xffff, is deg3 0x0000
      deg3_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg3_ind, S3_comps_ind_mask), S3_comps_ind_mask)));     

      components_ANF_all = _mm_and_si128(components_ANF_all, deg3_ind); // if is not deg3
      deg2_ind = _mm_cmpeq_epi8(_mm_and_si128(components_ANF_all, S3_comps_deg_mask2), zero_128); // is not deg2 0xffff, is deg2 0x0000
      deg2_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg2_ind, S3_comps_ind_mask), S3_comps_ind_mask)));

      components_ANF_all = _mm_and_si128(components_ANF_all, deg2_ind); // if is not deg2
      deg1_ind = _mm_cmpeq_epi8(_mm_and_si128(components_ANF_all, S3_comps_deg_mask1), zero_128); // is not deg1 0xffff, is deg1 0x0000
      deg1_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg1_ind, S3_comps_ind_mask), S3_comps_ind_mask)));

      deg0_cnt =  (1 << N) - deg3_cnt - deg2_cnt - deg1_cnt;

      if (deg3_cnt != 0) max_degree = 3;
      else if (deg2_cnt != 0) max_degree = 2;
      else if (deg1_cnt != 0) max_degree = 1;
      else max_degree = 0;

      if (deg0_cnt != 1) min_degree = 0;
      else if (deg1_cnt != 0) min_degree = 1;
      else if (deg2_cnt != 0) min_degree = 2;
      else min_degree = 3;

      if ((max_degree >= max_degree_bound) && (min_degree >= min_degree_bound))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else if (N == 4)
    {
      __m256i deg4_ind;
      __m256i deg3_ind;
      __m256i deg2_ind;
      __m256i deg1_ind;

      int deg4_cnt = 0; 
      int deg3_cnt = 0; 
      int deg2_cnt = 0; 
      int deg1_cnt = 0; 
      int deg0_cnt = 0;

      // Compute ANF for components
      array<bit_slice_l_t<N>, 1<<N> ALIGNED_(32) components_ANF = {{0}};
      get_components_ANF(components_ANF);
      __m256i components_ANF_all = _mm256_load_si256((__m256i *)(components_ANF.data()));

      deg4_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask4), zero_256); // is not deg4 0xffff, is deg4 0x0000
      deg4_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg4_ind, S4_comps_ind_mask), S4_comps_ind_mask)));

      components_ANF_all = _mm256_and_si256(components_ANF_all, deg4_ind); // if is not deg4
      deg3_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask3), zero_256); // is not deg3 0xffff, is deg3 0x0000
      deg3_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg3_ind, S4_comps_ind_mask), S4_comps_ind_mask)));     

      components_ANF_all = _mm256_and_si256(components_ANF_all, deg3_ind); // if is not deg3
      deg2_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask2), zero_256); // is not deg2 0xffff, is deg2 0x0000
      deg2_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg2_ind, S4_comps_ind_mask), S4_comps_ind_mask)));

      components_ANF_all = _mm256_and_si256(components_ANF_all, deg2_ind); // if is not deg2
      deg1_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask1), zero_256); // is not deg1 0xffff, is deg1 0x0000
      deg1_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg1_ind, S4_comps_ind_mask), S4_comps_ind_mask)));

      deg0_cnt =  (1 << N) - deg4_cnt - deg3_cnt - deg2_cnt - deg1_cnt;

      if (deg4_cnt != 0) max_degree = 4;
      else if (deg3_cnt != 0) max_degree = 3;
      else if (deg2_cnt != 0) max_degree = 2;
      else if (deg1_cnt != 0) max_degree = 1;
      else max_degree = 0;

      if (deg0_cnt != 1) min_degree = 0;
      else if (deg1_cnt != 0) min_degree = 1;
      else if (deg2_cnt != 0) min_degree = 2;
      else if (deg3_cnt != 0) min_degree = 3;
      else min_degree = 4;

      if ((max_degree >= max_degree_bound) && (min_degree >= min_degree_bound))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else if (N <= 8)
    {
      // Compute ANF for components
      array<bit_slice_l_t<N>, 1<<N> components_ANF = {{0}};
      get_components_ANF(components_ANF);

      int deg_ind[N + 1] = { 0 };
      for (int i = 1; i < (1 << N); i++)
      {
        memset(deg_ind, 0, sizeof(int)*(N + 1));
        for (int j = 0; j < (1 << N); j++)
        {
          deg_ind[_mm_popcnt_u32(j)] |= OPs<N>.get_bit(components_ANF[i], j);
        }
        int deg_cur = 0;
        for (int k = N; k >= 0; k--)
        {
          if (deg_ind[k] == 1)
          {
            deg_cur = k;
            break;
          } 
        }
        max_degree = deg_cur > max_degree ? deg_cur : max_degree;
        min_degree = deg_cur < min_degree ? deg_cur : min_degree;
      }

      if ((max_degree >= max_degree_bound) && (min_degree >= min_degree_bound))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }

  bool coordinates_degree_test(const int max_degree_bound, const int min_degree_bound) const
  {
    int max_degree = 0;
    int min_degree = numeric_limits<int>::max();

    if (N == 3)
    {
      uint8_t a, b, c;

      __m128i deg3_ind;
      __m128i deg2_ind;
      __m128i deg1_ind;

      int deg3_cnt = 0; 
      int deg2_cnt = 0; 
      int deg1_cnt = 0; 
      int deg0_cnt = 0; 

      // Compute ANF for coordinates
      array<bit_slice_l_t<N>, N> ALIGNED_(16) coordinates_ANF = {{0}};
      get_coordinates_ANF(coordinates_ANF);
      __m128i coordinates_ANF_all = _mm_load_si128((__m128i *)((*coordinates_ANF.data()).data()));
      coordinates_ANF_all = _mm_xor_si128(coordinates_ANF_all, S3_valv_mask8);

      deg3_ind = _mm_cmpeq_epi8(_mm_and_si128(coordinates_ANF_all, S3_coors_deg_mask3), zero_128); // is not deg3 0xffff, is deg3 0x0000
      deg3_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg3_ind, S3_coors_ind_mask), S3_coors_ind_mask)));     

      coordinates_ANF_all = _mm_and_si128(coordinates_ANF_all, deg3_ind); // if is not deg3
      deg2_ind = _mm_cmpeq_epi8(_mm_and_si128(coordinates_ANF_all, S3_coors_deg_mask2), zero_128); // is not deg2 0xffff, is deg2 0x0000
      deg2_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg2_ind, S3_coors_ind_mask), S3_coors_ind_mask)));

      coordinates_ANF_all = _mm_and_si128(coordinates_ANF_all, deg2_ind); // if is not deg2
      deg1_ind = _mm_cmpeq_epi8(_mm_and_si128(coordinates_ANF_all, S3_coors_deg_mask1), zero_128); // is not deg1 0xffff, is deg1 0x0000
      deg1_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg1_ind, S3_coors_ind_mask), S3_coors_ind_mask)));

      deg0_cnt =  N - deg3_cnt - deg2_cnt - deg1_cnt;

      if (deg3_cnt != 0) { max_degree = 3; if (max_degree > max_degree_bound) return false;}
      else if (deg2_cnt != 0) { max_degree = 2; if (max_degree > max_degree_bound) return false;}
      else if (deg1_cnt != 0) { max_degree = 1; if (max_degree > max_degree_bound) return false;}
      else max_degree = 0;

      if (deg0_cnt != 0) { min_degree = 0; if (min_degree < min_degree_bound) return false;}
      else if (deg1_cnt != 0) { min_degree = 1; if (min_degree < min_degree_bound) return false;}
      else if (deg2_cnt != 0) { min_degree = 2; if (min_degree < min_degree_bound) return false;}
      else { min_degree = 3; if (min_degree < min_degree_bound) return false;}
      return true;
    }
    else if (N == 4)
    {
      __m128i deg4_ind;
      __m128i deg3_ind;
      __m128i deg2_ind;
      __m128i deg1_ind;

      int deg4_cnt = 0; 
      int deg3_cnt = 0; 
      int deg2_cnt = 0; 
      int deg1_cnt = 0; 
      int deg0_cnt = 0;

      // compute ANF for coordinates
      array<bit_slice_l_t<N>, N> ALIGNED_(16) coordinates_ANF = {{0}};
      get_coordinates_ANF(coordinates_ANF);
      __m128i coordinates_ANF_all = _mm_load_si128((__m128i *)((*coordinates_ANF.data()).data()));
      coordinates_ANF_all = _mm_xor_si128(coordinates_ANF_all, S4_valv_maskg);

      deg4_ind = _mm_cmpeq_epi16(_mm_and_si128(coordinates_ANF, S4_coors_deg_mask4), zero_128); // is not deg4 0xffff, is deg4 0x0000
      deg4_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg4_ind, S4_coors_ind_mask), S4_coors_ind_mask)));

      coordinates_ANF = _mm_and_si128(coordinates_ANF, deg4_ind); // if is not deg4
      deg3_ind = _mm_cmpeq_epi16(_mm_and_si128(coordinates_ANF, S4_coors_deg_mask3), zero_128); // is not deg3 0xffff, is deg3 0x0000
      deg3_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg3_ind, S4_coors_ind_mask), S4_coors_ind_mask)));     

      coordinates_ANF = _mm_and_si128(coordinates_ANF, deg3_ind); // if is not deg3
      deg2_ind = _mm_cmpeq_epi16(_mm_and_si128(coordinates_ANF, S4_coors_deg_mask2), zero_128); // is not deg2 0xffff, is deg2 0x0000
      deg2_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg2_ind, S4_coors_ind_mask), S4_coors_ind_mask)));

      coordinates_ANF = _mm_and_si128(coordinates_ANF, deg2_ind); // if is not deg2
      deg1_ind = _mm_cmpeq_epi16(_mm_and_si128(coordinates_ANF, S4_coors_deg_mask1), zero_128); // is not deg1 0xffff, is deg1 0x0000
      deg1_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg1_ind, S4_coors_ind_mask), S4_coors_ind_mask)));

      deg0_cnt =  N - deg4_cnt - deg3_cnt - deg2_cnt - deg1_cnt;

      if (deg4_cnt != 0) {max_degree = 4; if (max_degree > max_degree_bound) return false;}
      else if (deg3_cnt != 0) {max_degree = 3; if (max_degree > max_degree_bound) return false;}
      else if (deg2_cnt != 0) {max_degree = 2; if (max_degree > max_degree_bound) return false;}
      else if (deg1_cnt != 0) {max_degree = 1; if (max_degree > max_degree_bound) return false;}
      else max_degree = 0;
      

      if (deg0_cnt != 0) {min_degree = 0; if (min_degree < min_degree_bound) return false;}
      else if (deg1_cnt != 0) {min_degree = 1; if (min_degree < min_degree_bound) return false;}
      else if (deg2_cnt != 0) {min_degree = 2; if (min_degree < min_degree_bound) return false;}
      else if (deg3_cnt != 0) {min_degree = 3; if (min_degree < min_degree_bound) return false;}
      else {min_degree = 4; if (min_degree < min_degree_bound) return false;}
      return true;
    }
    else if (N <= 8)
    {
      bit_slice_t<N> coordinates_ANF = bit_slice;
      get_coordinates_ANF(coordinates_ANF);

      int deg_ind[N + 1] = { 0 };
      for (int i = 0; i < N; i++)
      {
        memset(deg_ind, 0, sizeof(int)*(N + 1));
        for (int j = 0; j < (1 << N); j++)
        {
          deg_ind[_mm_popcnt_u32(j)] |= OPs<N>.get_bit(coordinates_ANF[i], j);
        }
        int deg_cur = 0;
        for (int k = N; k >= 0; k--)
        {
          if (deg_ind[k] == 1)
          {
            deg_cur = k;
            break;
          } 
        }
        max_degree = deg_cur > max_degree ? deg_cur : max_degree;
        if (max_degree > max_degree_bound) return false;
        min_degree = deg_cur < min_degree ? deg_cur : min_degree;
        if (min_degree < min_degree_bound) return false;
      }
      return true;
    }
  }

  void degree(int deg_spectrum[N+1], int & max_degree, int & min_degree) const
  {
    memset(deg_spectrum, 0, sizeof(int) * (N + 1));
    max_degree = 0;
    min_degree = numeric_limits<int>::max();

    if (N == 3)
    {
      __m128i deg3_ind;
      __m128i deg2_ind;
      __m128i deg1_ind;

      int deg3_cnt = 0; 
      int deg2_cnt = 0; 
      int deg1_cnt = 0; 

      // Compute ANF for components
      array<bit_slice_l_t<N>, 1<<N> ALIGNED_(16) components_ANF = {{0}};
      get_components_ANF(components_ANF);
      __m128i components_ANF_all = _mm_loadl_epi64((__m128i *)((*components_ANF.data()).data()));

      deg3_ind = _mm_cmpeq_epi8(_mm_and_si128(components_ANF_all, S3_comps_deg_mask3), zero_128); // is not deg3 0xffff, is deg3 0x0000
      deg3_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg3_ind, S3_comps_ind_mask), S3_comps_ind_mask)));     

      components_ANF_all = _mm_and_si128(components_ANF_all, deg3_ind); // if is not deg3
      deg2_ind = _mm_cmpeq_epi8(_mm_and_si128(components_ANF_all, S3_comps_deg_mask2), zero_128); // is not deg2 0xffff, is deg2 0x0000
      deg2_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg2_ind, S3_comps_ind_mask), S3_comps_ind_mask)));

      components_ANF_all = _mm_and_si128(components_ANF_all, deg2_ind); // if is not deg2
      deg1_ind = _mm_cmpeq_epi8(_mm_and_si128(components_ANF_all, S3_comps_deg_mask1), zero_128); // is not deg1 0xffff, is deg1 0x0000
      deg1_cnt = _mm_popcnt_u32(_mm_movemask_epi8(_mm_xor_si128(_mm_and_si128(deg1_ind, S3_comps_ind_mask), S3_comps_ind_mask)));

      deg_spectrum[3] =  deg3_cnt;
      deg_spectrum[2] =  deg2_cnt;
      deg_spectrum[1] =  deg1_cnt;
      deg_spectrum[0] =  (1 << N) - deg3_cnt - deg2_cnt - deg1_cnt;

      if (deg3_cnt != 0) max_degree = 3;
      else if (deg2_cnt != 0) max_degree = 2;
      else if (deg1_cnt != 0) max_degree = 1;
      else max_degree = 0;

      if (deg_spectrum[0] != 1) min_degree = 0;
      else if (deg1_cnt != 0) min_degree = 1;
      else if (deg2_cnt != 0) min_degree = 2;
      else min_degree = 3;
    }
    else if (N == 4)
    {
      __m256i deg4_ind;
      __m256i deg3_ind;
      __m256i deg2_ind;
      __m256i deg1_ind;

      int deg4_cnt = 0; 
      int deg3_cnt = 0; 
      int deg2_cnt = 0; 
      int deg1_cnt = 0; 

      // Compute ANF for components
      array<bit_slice_l_t<N>, 1<<N > ALIGNED_(32) components_ANF = {{0}};
      get_components_ANF(components_ANF);
      __m256i components_ANF_all = _mm256_load_si256((__m256i *)(components_ANF.data()));

      deg4_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask4), zero_256); // is not deg4 0xffff, is deg4 0x0000
      deg4_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg4_ind, S4_comps_ind_mask), S4_comps_ind_mask)));

      components_ANF_all = _mm256_and_si256(components_ANF_all, deg4_ind); // if is not deg4
      deg3_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask3), zero_256); // is not deg3 0xffff, is deg3 0x0000
      deg3_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg3_ind, S4_comps_ind_mask), S4_comps_ind_mask)));     

      components_ANF_all = _mm256_and_si256(components_ANF_all, deg3_ind); // if is not deg3
      deg2_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask2), zero_256); // is not deg2 0xffff, is deg2 0x0000
      deg2_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg2_ind, S4_comps_ind_mask), S4_comps_ind_mask)));

      components_ANF_all = _mm256_and_si256(components_ANF_all, deg2_ind); // if is not deg2
      deg1_ind = _mm256_cmpeq_epi16(_mm256_and_si256(components_ANF_all, S4_comps_deg_mask1), zero_256); // is not deg1 0xffff, is deg1 0x0000
      deg1_cnt = _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_xor_si256(_mm256_and_si256(deg1_ind, S4_comps_ind_mask), S4_comps_ind_mask)));

      deg_spectrum[4] =  deg4_cnt;
      deg_spectrum[3] =  deg3_cnt;
      deg_spectrum[2] =  deg2_cnt;
      deg_spectrum[1] =  deg1_cnt;
      deg_spectrum[0] =  (1 << N) - deg4_cnt - deg3_cnt - deg2_cnt - deg1_cnt;

      if (deg4_cnt != 0) max_degree = 4;
      else if (deg3_cnt != 0) max_degree = 3;
      else if (deg2_cnt != 0) max_degree = 2;
      else if (deg1_cnt != 0) max_degree = 1;
      else max_degree = 0;

      if (deg_spectrum[0] != 1) min_degree = 0;
      else if (deg1_cnt != 0) min_degree = 1;
      else if (deg2_cnt != 0) min_degree = 2;
      else if (deg3_cnt != 0) min_degree = 3;
      else min_degree = 4;
    }
    else if (N <= 8)
    {
      // Compute ANF for components
      array<bit_slice_l_t<N>, 1<<N> components_ANF = {{0}};
      get_components_ANF(components_ANF);

      int deg_ind[N + 1] = { 0 };
      for (int i = 1; i < (1 << N); i++)
      {
        memset(deg_ind, 0, sizeof(int)*(N + 1));
        for (int j = 0; j < (1 << N); j++)
        {
          deg_ind[_mm_popcnt_u32(j)] |= OPs<N>.get_bit(components_ANF[i], j);
        }
        int deg_cur = 0;
        for (int k = N; k >= 0; k--)
        {
          if (deg_ind[k] == 1)
          {
            deg_cur = k;
            break;
          } 
        }
        deg_spectrum[deg_cur]++;
        max_degree = deg_cur > max_degree ? deg_cur : max_degree;
        min_degree = deg_cur < min_degree ? deg_cur : min_degree;
      }
    }
  }

  int degree_from_ANF(bit_slice_l_t<N> & bf_ANF) const
  {
    int deg = 0;
    for (int j = (1 << N) - 1; j > 0; j--)
    {
      if (OPs<N>.get_bit(bf_ANF, HWorder<N>[j]) == 1)
      {
        deg = _mm_popcnt_u32(HWorder<N>[j]);
        break;
      }
    }
    return deg;
  }

  int degree_from_value(bit_slice_l_t<N> & bf) const
  {
    bit_slice_l_t<N> bf_ANF = bf;

    // compute ANF
    for (int j = N - 1; j >= 0; j--)
    {
      bf_ANF = bf_ANF ^ (OPs<N>.sll(bf_ANF, (1ULL<<j)) & masks<N>[j]);
    }

    return degree_from_ANF(bf_ANF);
  }

  int k_product_degree(const int k) const
  {
    int max_degree = 0;
    bit_slice_l_t<N> prod_bf = {{}};
    int end_off = (k == N) ? (1<<N) : HWorder_off<N>[k+1];

    for (int i = 1; i < end_off; i++)
    {
      int sel = HWorder<N>[i];

      OPs<N>.set_one_inplace(prod_bf);
      for (int bi = 0; bi < N; bi++)
      {
        if (((sel >> bi) & 1) == 1)
        {
            prod_bf = prod_bf & bit_slice[bi];
        }
      }
      int deg = degree_from_value(prod_bf);
      max_degree = deg > max_degree ? deg : max_degree;
    }
    return max_degree;
  }

  void k_product_degree_table(int dkT[N+1]) const
  {
    dkT[0] = 0;
    for (int k = 1; k <= N; k++)
    {
      dkT[k] = k_product_degree(k);
    }
  }

  string show_k_product_degree_table() const
  {
    int dkT[N+1];
    k_product_degree_table(dkT);

    stringstream ss;
    ss << "d_k table of the S-box:" << endl;
    ss << setw(4) << "k" << "|";
    for (int k = 1; k <= N; k++)
    {
      ss << setw(4) << k << "|";
    }
    ss << endl;

    ss << setw(4) << "dk" << "|";
    for (int k = 1; k <= N; k++)
    {
      ss << setw(4) << dkT[k] << "|";
    }
    ss << endl;
    return ss.str();
  }

  void show_k_product_degree_table(ofstream & fout) const
  {
    fout << show_k_product_degree_table();
  }

  void get_prec(vector<uint8_t> & uPrec, const uint8_t u) const
  {
    int hw = _mm_popcnt_u32(((uint32_t)u) & 0xffU);
    int offset = hw < N ? HWorder_off<N>[hw+1] : (1<<N) + 1;
    for (int i = 0; i < offset; i++)
    {
      uint8_t x = HWorder<N>[i];
      if ( (x ^ (x & u)) == 0)
      {
        uPrec.push_back(x);
      }      
    }
  }

  void get_succ(vector<uint8_t> & uSucc, const uint8_t u) const
  {
    int hw = _mm_popcnt_u32(((uint32_t)u) & 0xffU);
    int offset = HWorder_off<N>[hw];
    for (int i = offset; i < (1<<N) + 1; i++)
    {
      uint8_t x = HWorder<N>[i];
      if ( (u ^ (x & u)) == 0)
      {
        uSucc.push_back(x);
      }
    }
  }

  void from_value_to_ANF_inplace(bit_slice_l_t<N> & f_anf) const
  {
    for (int j = N - 1; j >= 0; j--)
    {
      f_anf = f_anf ^ (OPs<N>.sll(f_anf, (1ULL<<j)) & masks<N>[j]);
    }
  }

  void from_value_to_ANF(bit_slice_l_t<N> & f_anf, const bit_slice_l_t<N> f_val) const
  {
    f_anf = f_val;
    from_value_to_ANF_inplace(f_anf);
  }

  void get_product_of_coordinates_ANF(bit_slice_l_t<N> & S_w, const uint8_t w) const
  {
    OPs<N>.set_one_inplace(S_w);

    for (int i = 0; i < N; i++)
    {
      if (((w>>i) & 1) == 1)
      {
        S_w = S_w & bit_slice[i];
      }
    }
    from_value_to_ANF_inplace(S_w);
  }

  bool is_in_ANF(bit_slice_l_t<N> & S_w, uint8_t u) const
  {
    if (OPs<N>.get_bit(S_w, u) == 1) return true;
    return false;
  }

  void V_S_table(int VST[LUT_UNIT_N][LUT_UNIT_N]) const
  {
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      for (int j = 0; j < LUT_UNIT_N; j++)
      {
        VST[i][j] = 0;
      }
    }

    for (int u = 0; u < LUT_UNIT_N; u++)
    {
      for (int w = 0; w < LUT_UNIT_N; w++)
      {
        bit_slice_l_t<N> S_w;
        get_product_of_coordinates_ANF(S_w, w);
        if (is_in_ANF(S_w, u))
        {
          VST[u][w] = 1;
        }
      }
    }

    vector<uint8_t> uSucc;
    uSucc.reserve(LUT_UNIT_N);
    for (int ui = LUT_UNIT_N - 1; ui >= 0; ui--)
    {
      uint8_t u = HWorder<N>[ui];

      get_succ(uSucc, u);
      for (auto v : uSucc)
      {
        for (int w = 0; w < LUT_UNIT_N; w++)
        {
          if (VST[v][w] == 1)
          {
            VST[u][w] = 1;
          }
        }
      }
      uSucc.clear();
    }
  }

  string show_V_S_table() const
  {
    int VST[LUT_UNIT_N][LUT_UNIT_N];
    V_S_table(VST);
    stringstream ss;
    ss << "VST = " << endl;
    ss << OPs<N>.show_indicate_matrix_HWorder(VST);
    return ss.str();
  }

  void show_V_S_table(ofstream & fout) const
  {
    fout << show_V_S_table() << endl;
  }

  bool is_linear() const
  {
    return coordinates_degree_test(1, 1);
  }

  bool is_linear(const array<uint8_t, LUT_UNIT_N> & LUT_array)  const
  {
    function_t<N> func;
    memcpy(func.LUT, LUT_array.data(), sizeof(uint8_t) * LUT_UNIT_N);
    func.LUT_to_bit_slice();
    return func.coordinates_degree_test(1, 1);
  }


  void autocorrelation_matrix(int ACT[LUT_UNIT_N][LUT_UNIT_N]) const
  {
    int DDT[LUT_UNIT_N][LUT_UNIT_N];
    int Diff;
    int DDT_spectrum[LUT_UNIT_N+1];
    int Diff1;
    int DDT1_spectrum[LUT_UNIT_N+1];

    difference_distribution_matrix(DDT, Diff, DDT_spectrum, Diff1, DDT1_spectrum);
    OPs<N>.mulHM(ACT, DDT);
  }

  string show_autocorrelation_matrix() const
  {
    int ACT[LUT_UNIT_N][LUT_UNIT_N];
    autocorrelation_matrix(ACT);

    stringstream ss;
    ss << "ACT = " << endl;
    //ss << OPs<N>.show_matrix(ACT[0], LUT_UNIT_N, LUT_UNIT_N);
    ss << OPs<N>.show_matrix_HWorder(ACT);
    return ss.str();
  }

  void show_autocorrelation_matrix(ofstream & fout) const
  {
    fout << show_autocorrelation_matrix();
  }

  int linear_structures(vector<tuple<int, int, int> > & LS) const
  {
    LS.clear();
    int ACT[LUT_UNIT_N][LUT_UNIT_N];
    autocorrelation_matrix(ACT);

    for (int ci = 1; ci < (1<<N); ci++)
    {
      for (int ri = 1; ri < (1<<N); ri++)
      {
        if (abs(ACT[ri][ci]) == (1<<N))
        {
          int c = ((1 - (ACT[ri][ci] >> N)) >> 1);
          LS.push_back(make_tuple(ci, ri, c));
        }
      }
    }
    return LS.size();
  }

  string show_linear_structures() const
  {
    vector<tuple<int, int, int> > LS;
    linear_structures(LS);

    stringstream ss;
    ss << "LS = { /* size " << LS.size() << " */" << endl;
    for (auto ls: LS)
    {
      ss << "{" 
            << (*(new bitset<N>(std::get<0>(ls)))).to_string() << ","
            << (*(new bitset<N>(std::get<1>(ls)))).to_string() << ","
            << setw(1) << std::get<2>(ls) << ","
            << "}, " << endl;
    }
    ss << "};" << endl;
    return ss.str();
  }

  void show_linear_structures(ofstream & fout) const
  {
    fout << show_linear_structures();
  }

  int number_linear_structures() const
  {
    vector<tuple<int, int, int> > LS;
    linear_structures(LS);
    return LS.size();
  }

  // require: bf be the ANF of a Boolean function, instead of the value vector
  bool is_linear(const bit_slice_l_t<N> & bf) const
  {
    int deg2_offset = HWorder_off<N>[2];
    for (int i = deg2_offset; i < (1 << N); i++)
    {
      int j = HWorder<N>[i];
      if (OPs<N>.get_bit(bf, j) == 1)
      {
        return false;
      }
    }
    return true;
  }

  void get_coordinates_LUT(array<array<uint8_t, LUT_UNIT_N>, N> & coordinates_LUT) const
  {
    #if 1
    for (int yi = 0; yi < N; yi++)
    {
      for (int xi = 0; xi < LUT_UNIT_N; xi++)
      {
        uint8_t ybit = OPs<N>.get_bit(bit_slice[yi], xi);
        coordinates_LUT[yi][xi] = ybit;
      }
    }
    #endif

    #if 0
    uint8_t * yp = (uint8_t *) LUT;
    for (int xi = 0; xi < LUT_UNIT_N; xi++)
    {
      uint8_t y = yp[xi];
      for (int yi = 0; yi < N; yi++)
      {
        uint8_t ybit = (y >> yi) & 1;
        coordinates_LUT[yi][xi] = ybit;
      }
    }
    #endif
  }

  void get_components_LUT(array<array<uint8_t, LUT_UNIT_N>, 1<<N > & components_LUT) const
  {
    for (int i = 0; i < (1<<N); i++) components_LUT[i].fill(0);

    array<array<uint8_t, LUT_UNIT_N>, N> coordinates_LUT;
    get_coordinates_LUT(coordinates_LUT);

    for (int i = 1; i < (1<<N); i++)
    {
      for (int j = 0; j < N; j++)
      {
        if ( ((i>>j) & 1) ) components_LUT[i] = components_LUT[i] ^ coordinates_LUT[j];
      }
    }
  }

  string show_components_LUT() const
  {
    array<array<uint8_t, LUT_UNIT_N>, 1<<N > components_LUT;
    get_components_LUT(components_LUT);

    stringstream ss;
    ss << hex << setfill('0');
    for (int i = (1<<N)-1; i >= 0 ; i--)
    {
      ss << "{ 0x" << setw(2) << i << ", {";
      for (int j = LUT_UNIT_N - 1; j >= 0; j--)
      {
        ss << components_LUT[i][j] + '\0' << ", ";
      }
      ss << "}, " << endl;
    }
    return ss.str();
  }

  void v_w_linear(vector<pair<vector<uint8_t>, vector<uint8_t>> > & VW, int VW_n[N][N]) const
  {
    memset(VW_n, 0, N * N * sizeof(int));

    array<array<uint8_t, LUT_UNIT_N>, 1<<N > components_LUT;
    get_components_LUT(components_LUT);

    bool testnextV = false;

    vector<uint8_t> WholeSpaces;
    for (int i = 0; i < (1<<N); i++)
    {
      WholeSpaces.push_back(i);
    }

    array<bool, N> existWwithDw;
    vector<uint8_t> V;
    vector<uint8_t> V_span;
    vector<uint8_t> cV_span;
    array<bool, 1<<N > components_isLinear;
    set<uint8_t> linearComponents;
    vector<vector<uint8_t> > Ws;
    vector<uint8_t> appended;

    for (int v = N - 1; v > 0; v--)
    { // enumerate on the dimension of V
      int vn = SubSpaces<N>[v - 1].size();
      // for (int vi = 0; vi < vn; vi++)
      for (int vi = 0; vi < vn; vi++)
      { // enumerate Subspaces V with dimension vi
        
        existWwithDw.fill(false);
        testnextV = false;
        
        V.clear();
        V_span.clear();

        V = SubSpaces<N>[v - 1][vi];
        V_span.push_back(0);
        for (int x = 1; x < (1 << v); x++)
        {
          uint8_t V_element = 0;
          for (int i = 0; i < v; i++)
          {
            if ((x >> i) & 1) V_element ^= V[i];
          }
          V_span.push_back(V_element);
        }
        std::sort(V_span.begin(), V_span.end());

        cV_span.clear();
        cV_span.reserve((1<<N) - (1<<v) + 1);
        cV_span[0] = 0;
        std::set_difference(WholeSpaces.begin(), WholeSpaces.end(), V_span.begin(), V_span.end(), cV_span.begin() + 1);

        int isLinearCompN = (1<<N) - 1;
        components_isLinear.fill(true);

        for (int cvi = 0; cvi <= ((1<<N) - (1<<v)); cvi++)
        { // forall a, generate coset $ a + V $, at the same time, testing whether each component is linear on this coset
          uint8_t cV_element = cV_span[cvi];

          for (int avi = 1; avi < (1 << v); avi++)
          {
            uint8_t aV_element = 0;
            for (int i = 0; i < v; i++)
            {
              if ((avi >> i) & 1)
              {
                for (int compi = 1; compi < (1<<N); compi++)
                {
                  if (components_isLinear[compi] == true)
                  {
                    if (
                    (components_LUT[compi][cV_element] ^ components_LUT[compi][cV_element ^ aV_element] ^ components_LUT[compi][cV_element ^ V[i]]) !=
                     components_LUT[compi][cV_element ^ aV_element ^ V[i]])
                    {
                      components_isLinear[compi] = false;
                      isLinearCompN--;
                      if (isLinearCompN == 0)
                      {
                        testnextV = true;
                        break;
                      }
                    }
                  }
                }
                if (testnextV) break;
                aV_element ^= V[i];
              }
            }
            if (testnextV) break;
          }
          if (testnextV) break;          
        }

        if (testnextV == false)
        { // after testing all coset of V, there exist componets remains linear

          linearComponents.clear();
          for (int compi = 1; compi < (1<<N); compi++)
          {
            if (components_isLinear[compi])
            {
              linearComponents.insert(compi);
            }
          }

          Ws.clear();
          while (linearComponents.size() != 0)
          {
            auto linearComp_it = linearComponents.begin();
            uint8_t curComp = *linearComp_it;

            if (Ws.size() == 0)
            {
              Ws.push_back({curComp});
              linearComponents.erase(curComp);
            }
            else
            {
              bool indp = true;
              auto Ws_it = Ws.begin();
              while (Ws_it != Ws.end())
              {
                bool canFormSubspace = true;
                appended.clear();
                appended.push_back(curComp);

                auto W_it = (*Ws_it).begin();
                while (W_it != (*Ws_it).end())
                {
                  uint8_t preComp = *W_it;
                  uint8_t linearComb = curComp ^ preComp;

                  auto linearCombp = linearComponents.find(linearComb);
                  if (linearCombp == linearComponents.end())
                  {
                    canFormSubspace = false;
                    break;
                  }
                  else
                  {
                    appended.push_back(linearComb);
                  }
                  W_it++;
                }
                if (canFormSubspace)
                {
                  indp = false;
                  auto new_it = appended.begin();
                  while (new_it != appended.end())
                  {
                    uint8_t new_elem = *new_it;
                    (*Ws_it).push_back(new_elem);
                    linearComponents.erase(new_elem);
                    new_it++;
                  }
                  appended.clear();
                  appended.shrink_to_fit();
                  break;
                }
                else
                {
                  appended.clear();
                  appended.shrink_to_fit();
                }
                Ws_it++;
              }

              if (indp)
              {
                Ws.push_back({curComp});
                linearComponents.erase(curComp);
              }
            }            
          }

          auto Ws_it = Ws.begin();
          while (Ws_it != Ws.end())
          {
            int Wn = (*Ws_it).size() + 1;
            float wf = log2((float)Wn);
            float wf_intpart;
            float wf_fracpart = modf(wf, &wf_intpart);
            int w = (int)wf_intpart;
            VW.push_back(pair<vector<uint8_t>, vector<uint8_t> > (V, (*Ws_it)));

            for (int wi = 1; wi <= w; wi++)
            {
              existWwithDw[wi] = true;
            }
            Ws_it++;
          }
          
          Ws.clear();
          for (int w = 1; w < N; w++)
          {
            if (existWwithDw[w]) VW_n[v][w]++;
          }
        }
      }
    }
  }

  string show_v_w_linear() const
  {
    vector<pair<vector<uint8_t>, vector<uint8_t>> > VW;
    int VW_n[N][N];
    v_w_linear(VW, VW_n);

    stringstream ss;
    ss << "(v, w)-linearity: number N_(v, w) of subspaces V of dimension v for which there exists a w-dimensional W such that the S-box is (v, w)-linear with respect to (V, W)" << endl;
    ss << "    ";
    for (int w = 1; w < N; w++) ss << "   w=" << setw(1) << w << ", ";
    ss << endl;

    for (int v = 1; v < N; v++)
    {
      ss << "v=" << setw(1) << v << " ";
      for (int w = 1; w < N; w++)
      {
        ss << setw(6) << VW_n[v][w] << ", ";
      }
      ss << endl;
    }

    ss << "(V_basis, W_spaned) pairs" << endl;
    ss << hex << setfill('0');
    auto VW_it = VW.begin();
    while (VW_it != VW.end())
    {
      vector<uint8_t> V = (*VW_it).first;
      vector<uint8_t> W = (*VW_it).second;
      ss << "{";
      ss << "{";
      for (auto vx: V)
      {
        ss << "0x" << setw(2) << vx + '\0' << ",";
      }
      ss << "}, ";
      ss << "{";
      for (auto wx: W)
      {
        ss << "0x" << setw(2) << wx + '\0' << ",";
      }
      ss << "}";
      ss << "}," << endl;
      VW_it++;
    }
    return ss.str();
  }

  void show_v_w_linear(ofstream & fout) const
  {
    fout << show_v_w_linear();
  }

  void max_v_w_linear(pair<int, int> & max_v, pair<int, int> & max_w) const
  {
    vector<pair<vector<uint8_t>, vector<uint8_t>> > VW;
    int VW_n[N][N];
    v_w_linear(VW, VW_n);

    max_v.first = 0;
    max_v.second = 0;
    bool find = false;
    for (int v = N - 1; v > 0; v--)
    {
      for (int w = N - 1; w > 0; w--)
      {
        if (VW_n[v][w] != 0)
        {
          find = true;
          max_v.first = v;
          max_v.second = w;
          break;
        }
      }
      if (find) break;
    }

    max_w.first = 0;
    max_w.second = 0;
    find = false;
    for (int w = N - 1; w > 0; w--)
    {
      for (int v = N - 1; v > 0; v--)
      {
        if (VW_n[v][w] != 0)
        {
          find = true;
          max_w.first = v;
          max_w.second = w;
          break;
        }
      }
      if (find) break;
    }
  }

  bool is_LE(const uint8_t S2[LUT_UNIT_N], uint8_t A[LUT_UNIT_N], uint8_t B[LUT_UNIT_N]) const
  {
    if (!is_permutation())
    {
      return false;
    }

    typedef bit_slice_l_t<N> Set_t;

    uint8_t * S1 = (uint8_t *) LUT;
    if (((S1[0] == 0) && (S2[0] != 0)) || ((S1[0] != 0) && (S2[0] == 0))) return false;

    uint8_t iS1[LUT_UNIT_N];
    uint8_t iS2[LUT_UNIT_N];
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
        iS1[S1[i]] = i;
        iS2[S2[i]] = i;
    }

    array<uint8_t, LUT_UNIT_N> LA = {0};
    array<uint8_t, LUT_UNIT_N> LB = {0};

    Set_t Z = { {0} };

    Set_t UA = {{ 0 }};
    Set_t UB = {{ 0 }};
    Set_t CA = {{ 0 }};
    Set_t CB = {{ 0 }};
    Set_t NA = {{ 0 }};
    Set_t NB = {{ 0 }};
    Set_t GA = {{ 0 }};
    Set_t GB = {{ 0 }};
    uint8_t GS_old = 0;

    stack<Set_t> UA_stack;
    stack<Set_t> UB_stack;
    stack<Set_t> CA_stack;
    stack<Set_t> CB_stack;
    stack<Set_t> GA_stack;
    stack<Set_t> GB_stack;
    stack<uint8_t> GS_stack;
    stack<array<uint8_t, LUT_UNIT_N> > LA_stack;
    stack<array<uint8_t, LUT_UNIT_N> > LB_stack;

    OPs<N>.set_one_inplace(UA); OPs<N>.unset_bit_inplace(UA, 0);
    OPs<N>.set_one_inplace(UB); OPs<N>.unset_bit_inplace(UB, 0);
    OPs<N>.set_bit_inplace(CA, 0);
    OPs<N>.set_bit_inplace(CB, 0);
    OPs<N>.set_one_inplace(GA); OPs<N>.unset_bit_inplace(GA, 0);
    OPs<N>.set_one_inplace(GB); OPs<N>.unset_bit_inplace(GB, 0);

    memset(LA.data(), 0, sizeof(uint8_t) * LUT_UNIT_N);
    memset(LB.data(), 0, sizeof(uint8_t) * LUT_UNIT_N);

    UA_stack.push(UA);
    UB_stack.push(UB);
    CA_stack.push(CA);
    CB_stack.push(CB);
    GA_stack.push(GA);
    GB_stack.push(GB);
    LA_stack.push(LA);
    LB_stack.push(LB);
    GS_stack.push(0);

    bool all_rejected = false;
    bool last_rejected = false;

    while (((UA != Z) && (UB != Z)) || all_rejected)
    {
      if (all_rejected)
      {
        if (UA_stack.size() == 1)
        {
          return false;
        } 
        PRINT(cout << "if (all_rejected)" << endl);
        UA_stack.pop();
        UB_stack.pop();
        CA_stack.pop();
        CB_stack.pop();
        GA_stack.pop();
        GB_stack.pop();
        LA_stack.pop();
        LB_stack.pop();
        NA = Z;
        NB = Z;
        all_rejected = false;
        last_rejected = true;
      }

      if ((NA == Z) && (NB == Z))
      {
        if (last_rejected)
        {
          PRINT(cout << "if (last_rejected)" << endl);
          UA = UA_stack.top();
          UB = UB_stack.top();
          CA = CA_stack.top();
          CB = CB_stack.top();
          GA = GA_stack.top();
          GB = GB_stack.top();
          LA = LA_stack.top();
          LB = LB_stack.top();

          GS_old = GS_stack.top(); GS_stack.pop();

          uint8_t x = OPs<N>.lsb_idx(UA);
          PRINT(cout << "Reject Guess: LA[" << x + '\0' << "] = " << GS_old + '\0'  << endl);       
          int next_guess = OPs<N>.next_lsb_idx(GA, GS_old);
          if (next_guess == -1)
          {
            PRINT(cout << "next_guess == -1" << endl);
            all_rejected = true;
            continue;
          }
          else
          {
            LA[x] = next_guess;
            PRINT(cout << "Guess: LA[" << x + '\0'  << "] = " << LA[x] + '\0'  << endl);
            GS_stack.push(next_guess);
            OPs<N>.unset_bit_inplace(GA, next_guess);
            OPs<N>.set_bit_inplace(NA, x);
            OPs<N>.unset_bit_inplace(UA, x);
          }
          last_rejected = false;
        }
        else
        {
          UA_stack.push(UA);
          UB_stack.push(UB);
          CA_stack.push(CA);
          CB_stack.push(CB);
          GA_stack.push(GA);
          GB_stack.push(GB);
          LA_stack.push(LA);
          LB_stack.push(LB);

          uint8_t x = OPs<N>.lsb_idx(UA);
          uint8_t y = OPs<N>.lsb_idx(GA);
          LA[x] = y;
          PRINT(cout << "Guess: LA[" << x + '\0'  << "] = " << y + '\0'  << endl);
          GS_stack.push(y);
          OPs<N>.unset_bit_inplace(GA, y);
          OPs<N>.set_bit_inplace(NA, x);
          OPs<N>.unset_bit_inplace(UA, x);
        }
      }

      Set_t NA_tmp = NA;
      while (NA != Z)
      {
        uint8_t x = OPs<N>.unset_lsb_idx_inplace(NA);
        Set_t CA_tmp = CA;
        while (CA_tmp != Z)
        {
          uint8_t xa = OPs<N>.unset_lsb_idx_inplace(CA_tmp);
          uint8_t xax = xa ^ x;
          uint8_t xay = LA[xa] ^ LA[x];
          int determinedA = OPs<N>.get_bit(CA, xax);
          int determinedNA = determinedA | OPs<N>.get_bit(NA_tmp, xax);
          if (((determinedNA == 1) && (LA[xax] != xay)) || ((determinedNA == 0) && (OPs<N>.get_bit(GA, xay) == 0)))
          {
              PRINT(cout << "((OPs<N>.get_bit(CA, xax) == 1) && (LA[xax] != xay)) || (OPs<N>.get_bit(GA, xay) == 0)" << endl);
              NA = Z;
              NB = Z;
              last_rejected = true;
              break;
          }
          if (determinedA == 0)
          {
            LA[xax] = xay;
            PRINT(cout << hex 
            << "From LA[" << xa + '\0' << "] = " << LA[xa] + '\0' 
            << " and LA[" << x + '\0' << "] = " << LA[x] + '\0' << ", " 
            << " get " << "LA[" << xax + '\0' << "] = " << xay + '\0' << dec << endl);
            OPs<N>.unset_bit_inplace(GA, xay);
            OPs<N>.set_bit_inplace(CA, xax);
            uint8_t xb = S2[xax];
            uint8_t yb = S1[xay];
            int determinedNB = OPs<N>.get_bit(CB, xb) | OPs<N>.get_bit(NB, xb);
            if (((determinedNB == 1) && (LB[xb] != yb)) || ((determinedNB == 0) && (OPs<N>.get_bit(GB, yb) == 0)))
            {
              PRINT(cout << "((determinedNB == 1) && (LB[xb] != yb)) || ((determinedNB == 0) && (OPs<N>.get_bit(GB, yb) == 0)))" << endl);
              NA = Z;
              NB = Z;
              last_rejected = true;
              break;
            }
            if (determinedNB == 0)
            {
              LB[xb] = yb;
              PRINT(cout << hex 
              << "From S2[" << xax + '\0' << "] = " << xb + '\0' 
              << " and LA[" << xax + '\0' << "] = " << xay + '\0' 
              << " and S1[" << xay + '\0' << "] = " << yb + '\0' 
              << ", " << "get " << "LB[" << xb + '\0' << "] = " << yb + '\0' << dec << endl);
              OPs<N>.set_bit_inplace(NB, xb);
              OPs<N>.unset_bit_inplace(GB, yb);
            }
          }
        }
        if (last_rejected) break;

        double NBsize = (double)(OPs<N>.cnt_1(NB));
        double CBsize = (double)(OPs<N>.cnt_1(CB));
        PRINT(cout << "NBsize + log2(CBsize): " << NBsize + log2(CBsize) << endl);
        if (NBsize + log2(CBsize) >= N)
        {
          PRINT(cout << "NBsize + log2(CBsize) >= N" << endl);
          Set_t NBtmp = NB;
          while (NB != Z)
          {
            uint8_t nx = OPs<N>.unset_lsb_idx_inplace(NB);
            Set_t CBtmp = CB;
            while (CBtmp != Z)
            { 
              uint8_t cx = OPs<N>.unset_lsb_idx_inplace(CBtmp);              
              uint8_t ncx = nx ^ cx;
              uint8_t ncy = LB[nx] ^ LB[cx];
              int determinedCB = OPs<N>.get_bit(CB, ncx);
              int determinedB = determinedCB | OPs<N>.get_bit(NBtmp, ncx);
              if (((determinedB == 1) && (LB[ncx] != ncy)) || ((determinedB == 0) && (OPs<N>.get_bit(GB, ncy) == 0)))
              {
                PRINT(cout << "((determinedB == 1) && (LB[ncx] != ncy)) || ((determinedB == 0) && (OPs<N>.get_bit(GB, ncy) == 0))" << endl);
                NA = Z;
                NB = Z;
                last_rejected = true;
                break;
              }
              if (determinedCB == 0)
              {
                LB[ncx] = ncy;
                OPs<N>.set_bit_inplace(CB, ncx);
                OPs<N>.unset_bit_inplace(GB, ncy);
                PRINT(cout << hex 
                << "From LB[" << nx + '\0' << "] = " << LB[nx] + '\0' 
                << " and LB[" << cx + '\0' << "] = " << LB[cx] + '\0' << ", " 
                << " get " << "LB[" << ncx + '\0' << "] = " << ncy + '\0' << dec << endl);
              }
            }
            if (last_rejected) break;
          }
          if (last_rejected) break;

          double NCBsize = (double)(OPs<N>.cnt_1(CB));
          if (NCBsize == LUT_UNIT_N)
          {
            if (is_linear(LB))
            {
              PRINT(cout << "is_linear(LB)" << endl);
              Set_t UAtmp = UA ^ (UA & CA);
              while (UAtmp != Z)
              {
                int ux = OPs<N>.unset_lsb_idx_inplace(UAtmp);
                LA[ux] = iS1[LB[S2[ux]]];
              }
              if (is_linear(LA))
              {
                PRINT(cout << "is_linear(LA)" << __LINE__ << endl);
                memcpy(A, LA.data(), sizeof(uint8_t) * LUT_UNIT_N);
                memcpy(B, LB.data(), sizeof(uint8_t) * LUT_UNIT_N);
                return true;
              }
              else
              {
                last_rejected = true;
                NA = Z;
                NB = Z;
              }
            }
            else
            {
              last_rejected = true;
              NA = Z;
              NB = Z;
            }
          }
        }
      }

      Set_t NB_tmp = NB;
      while (NB != Z)
      {
        uint8_t x = OPs<N>.unset_lsb_idx_inplace(NB);
        Set_t CB_tmp = CB;
        while (CB_tmp != Z)
        {
          uint8_t xb = OPs<N>.unset_lsb_idx_inplace(CB_tmp);
          uint8_t xbx = xb ^ x;
          uint8_t xby = LB[xb] ^ LB[x];
          int determinedB = OPs<N>.get_bit(CB, xbx);
          int determinedNB = determinedB | OPs<N>.get_bit(NB_tmp, xbx);
          if (((determinedNB == 1) && (LB[xbx] != xby)) || ((determinedNB == 0) && (OPs<N>.get_bit(GB, xby) == 0)))
          {
              PRINT(cout << "((determinedNB == 1) && (LB[xbx] != xby)) || ((determinedNB == 0) && (OPs<N>.get_bit(GB, xby) == 0))" << endl);
              NA = Z;
              NB = Z;
              last_rejected = true;
              break;
          }
          if (determinedB == 0)
          {
            LB[xbx] = xby;
            PRINT(cout << hex 
            << "From LB[" << xb + '\0' << "] = " << LB[xb] + '\0' 
            << " and LB[" << x + '\0' << "] = " << LB[x] + '\0' << ", " 
            << " get " << "LB[" << xbx + '\0' << "] = " << xby + '\0' << dec << endl);
            OPs<N>.unset_bit_inplace(GB, xby);
            OPs<N>.set_bit_inplace(CB, xbx);
            uint8_t xa = iS2[xbx];
            uint8_t ya = iS1[xby];
            int determinedNA = OPs<N>.get_bit(CA, xa) | OPs<N>.get_bit(NA, xa);
            if (((determinedNA == 1) && (LA[xa] != ya)) || ((determinedNA == 0) && (OPs<N>.get_bit(GA, ya) == 0)))
            {
              PRINT(cout << "((determinedA == 1) && (LA[xa] != ya)) || ((determinedA == 0) && (OPs<N>.get_bit(GA, ya) == 0))" << endl);
              NA = Z;
              NB = Z;
              last_rejected = true;
              break;
            }
            if (determinedNA == 0)
            {
              LA[xa] = ya;
              PRINT(cout << hex 
              << "From iS2[" << xbx + '\0' << "] = " << xa + '\0' 
              << " and LB[" << xbx + '\0' << "] = " << xby + '\0' 
              << " and iS1[" << xby + '\0' << "] = " << ya + '\0' 
              << ", " << "get " << "LA[" << xa + '\0' << "] = " << ya + '\0' << dec << endl);
              OPs<N>.set_bit_inplace(NA, xa);
              OPs<N>.unset_bit_inplace(GA, ya);
            }
          }
        }
        if (last_rejected) break;

        double NAsize = (double)OPs<N>.cnt_1(NA);
        double CAsize = (double)(OPs<N>.cnt_1(CA));
        PRINT(cout << "NAsize + log2(CAsize): " << NAsize + log2(CAsize) << endl);
        if (NAsize + log2(CAsize) >= N)
        {
          PRINT(cout << "NAsize + log2(CAsize) >= N" << endl);
          Set_t NAtmp = NA;
          while (NA != Z)
          {
            uint8_t nx = OPs<N>.unset_lsb_idx_inplace(NA);
            Set_t CAtmp = CA;
            while (CAtmp != Z)
            {
              uint8_t cx = OPs<N>.unset_lsb_idx_inplace(CAtmp);
              uint8_t ncx = nx ^ cx;
              uint8_t ncy = LA[nx] ^ LA[cx];
              int determinedCA = OPs<N>.get_bit(CA, ncx);
              int determinedA = determinedCA | OPs<N>.get_bit(NAtmp, ncx);
              if (((determinedA == 1) && (LA[ncx] != ncy)) || ((determinedA == 0) && (OPs<N>.get_bit(GA, ncy) == 0)))
              {
                PRINT(cout << "((determinedA == 1) && (LA[ncx] != ncy)) || ((determinedA == 0) && (OPs<N>.get_bit(GA, ncy) == 0))" << endl);
                NA = Z;
                NB = Z;
                last_rejected = true;
                break;
              }
              if (determinedCA == 0)
              {
                LA[ncx] = ncy;
                OPs<N>.set_bit_inplace(CA, ncx);
                OPs<N>.unset_bit_inplace(GA, ncy);
                PRINT(cout << hex 
                << "From LA[" << nx + '\0' << "] = " << LA[nx] + '\0' 
                << " and LA[" << cx + '\0' << "] = " << LA[cx] + '\0' << ", " 
                << " get " << "LA[" << ncx + '\0' << "] = " << ncy + '\0' << dec << endl);
              }
            }
            if (last_rejected) break;
          }
          if (last_rejected) break;

          double NCAsize = (double)(OPs<N>.cnt_1(CA));
          if (NCAsize == LUT_UNIT_N)
          {
            if (is_linear(LA))
            {
              PRINT(cout << "is_linear(LA)" << endl);
              Set_t UBtmp = UB ^ (UB & CB);
              while (UBtmp != Z)
              {
                int ux = OPs<N>.unset_lsb_idx_inplace(UBtmp);
                LB[ux] = S1[LA[iS2[ux]]];
              }
              if (is_linear(LB))
              {
                PRINT(cout << "is_linear(LB)" << endl);
                memcpy(A, LA.data(), sizeof(uint8_t) * LUT_UNIT_N);
                memcpy(B, LB.data(), sizeof(uint8_t) * LUT_UNIT_N);
                return true;
              }
              else
              {
                last_rejected = true;
                NA = Z;
                NB = Z;
              }
            }
            else
            {
              last_rejected = true;
              NA = Z;
              NB = Z;
            }
          }
        }
      }

      if (last_rejected) continue;

      UA = UA ^ (UA & CA);
      UB = UB ^ (UB & CB);
    }
    return false;
  }

  bool is_LE(const function_t & S2, function_t & A, function_t & B) const
  {
    bool isLE = is_LE((uint8_t *)S2.LUT, (uint8_t *)A.LUT, (uint8_t *)B.LUT);
    if (isLE)
    {
      A.LUT_to_bit_slice();
      B.LUT_to_bit_slice();
    }
    return isLE;
  }

  template<typename T>
  void PRINT_LUT(T RS) const
  {
    cout << hex << setfill('0');
    for (int i = 0; i < LUT_UNIT_N; i++) cout << RS[i] + '\0' << ", ";
    cout << dec << endl;
  }

  // Require: function in LUT form
  void get_PE(vector<pair<function_t<N>, int> > & PE_vec) const
  {
    map<function_t<N>, int> PEmap;
    function_t<N> func_perm_pi;
    function_t<N> func_perm_po;
    for (int ip = 0; ip < FACT_(N); ip++)
    {
      OPs<N>.composite(func_perm_pi.LUT, LUT, PE<N>.func[ip].LUT);
      for (int op = 0; op < FACT_(N); op++)
      {     
        OPs<N>.composite(func_perm_po.LUT, PE<N>.func[op].LUT, func_perm_pi.LUT);            
        func_perm_po.LUT_to_bit_slice();
        PEmap.insert(pair<function_t<N>, int >(func_perm_po, (ip<<16)|(op<<8)));
      }
    }

    for (auto map_it = PEmap.begin(); map_it != PEmap.end(); map_it++)
    {
      PE_vec.push_back(*map_it);
    }
  }

  // Require: function in LUT form
  void get_PE(string outfile) const
  {
    ofstream outf(outfile.c_str(), ios::app);
    vector<pair<function_t<N>, int> > PE_vec;
    get_PE(PE_vec);
    for (auto pe : PE_vec)
    {
      int insid = pe.second;
      string name = "iXiPoPoX_" + std::to_string((insid>>24)&0xff) + "_" + std::to_string((insid>>16)&0xff) + "_" + std::to_string((insid>>8)&0xff) + "_" + std::to_string((insid)&0xff);              
      outf << name << "," << pe.first.LUT_to_string() << endl;
    }
    outf.close();
  }

  // Require: function in LUT form
  void get_XE(vector<pair<function_t<N>, int> > & XE_vec) const
  {
    map<function_t<N>, int> XEmap;
    function_t<N> func_perm_xi;
    for (int ix = 0; ix < (1<<N); ix++)
    {
      OPs<N>.composite(func_perm_xi.LUT, LUT, XE<N>[ix].LUT);
      for (int ox = 0; ox < (1<<N); ox++)
      {
        function_t<N> func_comp;    
        OPs<N>.composite(func_comp.LUT, XE<N>[ox].LUT, func_perm_xi.LUT);       
        func_comp.LUT_to_bit_slice();
        XEmap.insert(pair<function_t<N>, int >(func_comp, (ix<<24)|ox));
      }   
    }

    for (auto map_it = XEmap.begin(); map_it != XEmap.end(); map_it++)
    {
      XE_vec.push_back(*map_it);
    }
  }

  // Require: function in LUT form
  void get_XE(string outfile) const
  {
    ofstream outf(outfile.c_str(), ios::app);
    vector<pair<function_t<N>, int> > XE_vec;
    get_XE(XE_vec);
    for (auto xe : XE_vec)
    {
      int insid = xe.second;
      string name = "iXiPoPoX_" + std::to_string((insid>>24)&0xff) + "_" + std::to_string((insid>>16)&0xff) + "_" + std::to_string((insid>>8)&0xff) + "_" + std::to_string((insid)&0xff);              
      outf << name << "," << xe.first.LUT_to_string() << endl;
    }
    outf.close();
  }

  function_t PE_representative() const
  {
    function_t func_min;
    function_t func_perm;

    OPs<N>.composite(func_perm.LUT, LUT, PE<N>.func[0].LUT);
    func_perm.LUT_to_bit_slice();
    func_perm.sort();
    func_min = func_perm;

    for (int ip = 1; ip < FACT_(N); ip++)
    {
      OPs<N>.composite(func_perm.LUT, LUT, PE<N>.func[ip].LUT);
      func_perm.LUT_to_bit_slice();
      func_perm.sort();
      func_min = (func_perm < func_min) ? func_perm : func_min;
    }
    func_min.bit_slice_to_LUT();
    return func_min;
  }

  // Require: function in LUT form
  void get_PXE(vector<pair<function_t<N>, int> > & PXE_vec) const
  {
    map<function_t<N>, int> PXEmap;
    function_t<N> func_perm_xi;
    function_t<N> func_perm_pi;
    function_t<N> func_perm_po;
    for (int ix = 0; ix < (1<<N); ix++)
    {
      for (int ip = 0; ip < FACT_(N); ip++)
      {
        OPs<N>.composite(func_perm_xi.LUT, PE<N>.func[ip].LUT, XE<N>[ix].LUT);
        OPs<N>.composite(func_perm_pi.LUT, LUT, func_perm_xi.LUT);
        for (int op = 0; op < FACT_(N); op++)
        {     
          OPs<N>.composite(func_perm_po.LUT, PE<N>.func[op].LUT, func_perm_pi.LUT);            
          for (int ox = 0; ox < (1<<N); ox++)
          {
            function_t<N> func_comp;    
            OPs<N>.composite(func_comp.LUT, XE<N>[ox].LUT, func_perm_po.LUT);       
            func_comp.LUT_to_bit_slice();
            PXEmap.insert(pair<function_t<N>, int >(func_comp, (ix<<24)|(ip<<16)|(op<<8)|ox));
          }   
        }
      }
    }

    for (auto map_it = PXEmap.begin(); map_it != PXEmap.end(); map_it++)
    {
      PXE_vec.push_back(*map_it);
    }
  }

  // Require: function in LUT form
  void get_PXE(string outfile) const
  {
    ofstream outf(outfile.c_str(), ios::app);
    vector<pair<function_t<N>, int> > PXE_vec;
    get_PXE(PXE_vec);
    for (auto pxe : PXE_vec)
    {
      int insid = pxe.second;
      string name = "iXiPoPoX_" + std::to_string((insid>>24)&0xff) + "_" + std::to_string((insid>>16)&0xff) + "_" + std::to_string((insid>>8)&0xff) + "_" + std::to_string((insid)&0xff);              
      outf << name << "," << pxe.first.LUT_to_string() << endl;
    }
    outf.close();
  }

  function_t PXE_representative() const
  {
    function_t func_min;
    func_min.bit_slice = bit_slice;
    for (int i = 0; i < LUT_XMM_N; i++) func_min.LUT[i] = LUT[i];

    function_t<N> func_perm_xi;
    function_t<N> func_perm_pi;
    function_t<N> func_perm_po;
    for (int ix = 0; ix < (1<<N); ix++)
    {
      for (int ip = 0; ip < FACT_(N); ip++)
      {
        OPs<N>.composite(func_perm_xi.LUT, PE<N>.func[ip].LUT, XE<N>[ix].LUT);
        OPs<N>.composite(func_perm_pi.LUT, LUT, func_perm_xi.LUT);
        for (int op = 0; op < FACT_(N); op++)
        {     
          OPs<N>.composite(func_perm_po.LUT, PE<N>.func[op].LUT, func_perm_pi.LUT);            
          for (int ox = 0; ox < (1<<N); ox++)
          {
            function_t<N> func_comp;    
            OPs<N>.composite(func_comp.LUT, XE<N>[ox].LUT, func_perm_po.LUT);       
            func_comp.LUT_to_bit_slice();
            func_min = (func_comp < func_min) ? func_comp : func_min;
          }   
        }
      }
    }
    return func_min;
  }

  void LE_representative(uint8_t RS[LUT_UNIT_N], uint8_t A[LUT_UNIT_N], uint8_t B[LUT_UNIT_N]) const
  {
    if (!is_permutation())
    {
      return;
    }
    PRINT(cout << __LINE__ << endl);

    typedef bit_slice_l_t<N> Set_t;

    uint8_t * S = (uint8_t *) LUT;
    uint8_t iS[LUT_UNIT_N];
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
        iS[S[i]] = i;
    }
    const Set_t Z = { {0} };

    array<uint8_t, LUT_UNIT_N> min_LR;
    array<uint8_t, LUT_UNIT_N> min_LA;
    array<uint8_t, LUT_UNIT_N> min_LB;
    min_LR.fill(numeric_limits<uint8_t>::max());

    Set_t UA, UB, UR, DA, DB, DR, CA, CB, NA, NB, GA, GB, GR;
    array<uint8_t, LUT_UNIT_N> LR, LA, LB, iLA, iLB;
    uint8_t GS_old = 0;

    vector<Set_t> UA_stack;
    vector<Set_t> UB_stack;
    vector<Set_t> UR_stack;
    vector<Set_t> DA_stack;
    vector<Set_t> DB_stack;
    vector<Set_t> DR_stack;
    vector<Set_t> CA_stack;
    vector<Set_t> CB_stack;
    vector<Set_t> GA_stack;
    vector<Set_t> GB_stack;
    vector<Set_t> GR_stack;
    vector<uint8_t> GS_stack;
    vector<array<uint8_t, LUT_UNIT_N> > LR_stack;
    vector<array<uint8_t, LUT_UNIT_N> > LA_stack;
    vector<array<uint8_t, LUT_UNIT_N> > LB_stack;
    vector<array<uint8_t, LUT_UNIT_N> > iLA_stack;
    vector<array<uint8_t, LUT_UNIT_N> > iLB_stack;

    UA.fill(UINT_MAX); OPs<N>.unset_bit_inplace(UA, 0);
    UB.fill(UINT_MAX); OPs<N>.unset_bit_inplace(UB, 0);
    DA.fill(0); OPs<N>.set_bit_inplace(DA, 0);
    DB.fill(0); OPs<N>.set_bit_inplace(DB, 0);
    GA.fill(UINT_MAX); OPs<N>.unset_bit_inplace(GA, 0);
    GB.fill(UINT_MAX); OPs<N>.unset_bit_inplace(GB, 0);
    LR.fill(0); LA.fill(0); LB.fill(0); iLA.fill(0); iLB.fill(0);

    if (S[0] == 0)
    {
      CA.fill(0); OPs<N>.set_bit_inplace(CA, 0);
      CB.fill(0); OPs<N>.set_bit_inplace(CB, 0);
      NA.fill(0);
      NB.fill(0);      
      DR.fill(0); OPs<N>.set_bit_inplace(DR, 0);
      UR.fill(UINT_MAX); OPs<N>.unset_bit_inplace(UR, 0);
      GR.fill(UINT_MAX); OPs<N>.unset_bit_inplace(GR, 0);
      LR[0] = 0;
    }
    else
    {
      CA.fill(0);
      CB.fill(0);
      NA.fill(0); OPs<N>.set_bit_inplace(NA, 0);
      NB.fill(0); OPs<N>.set_bit_inplace(NB, 0);
      DR.fill(0);
      UR.fill(UINT_MAX);
      GR.fill(UINT_MAX);
    }

    GS_stack.push_back(0);
    UA_stack.push_back(UA);
    UB_stack.push_back(UB);
    UR_stack.push_back(UR);
    DA_stack.push_back(DA);
    DB_stack.push_back(DB);
    DR_stack.push_back(DR);
    CA_stack.push_back(CA);
    CB_stack.push_back(CB);
    GA_stack.push_back(GA);
    GB_stack.push_back(GB);
    GR_stack.push_back(GR);
    LR_stack.push_back(LR);
    LA_stack.push_back(LA);
    LB_stack.push_back(LB);
    iLA_stack.push_back(iLA);
    iLB_stack.push_back(iLB);

    bool all_rejected = false;
    bool last_rejected = false;
    bool try_next = true;

    PRINT(cout << "GS_stack.size(): " << GS_stack.size() << " at line: " << __LINE__ << endl);
    PRINT(cout << "UA_stack.size(): " << UA_stack.size() << " at line: " << __LINE__ << endl);

    while ((((UA != Z) && (UB != Z)) || all_rejected) || try_next)
    {
      try_next = false;
      if (all_rejected)
      {
        PRINT(cout << "GS_stack.size(): " << GS_stack.size() << " at line: " << __LINE__  << endl);
        PRINT(cout << "UA_stack.size(): " << UA_stack.size() << " at line: " << __LINE__  << endl);
        if (GS_stack.size() == 1)
        {
          memcpy(RS, min_LR.data(), sizeof(uint8_t) * LUT_UNIT_N);
          memcpy(A, min_LA.data(), sizeof(uint8_t) * LUT_UNIT_N);
          memcpy(B, min_LB.data(), sizeof(uint8_t) * LUT_UNIT_N);
          return;
        } 
        PRINT(cout << "if (all_rejected)" << endl);
        UA_stack.pop_back();
        UB_stack.pop_back();
        UR_stack.pop_back();
        DA_stack.pop_back();
        DB_stack.pop_back();
        DR_stack.pop_back();
        CA_stack.pop_back();
        CB_stack.pop_back();
        GA_stack.pop_back();
        GB_stack.pop_back();
        GR_stack.pop_back();
        LR_stack.pop_back();
        LA_stack.pop_back();
        LB_stack.pop_back();
        iLA_stack.pop_back();
        iLB_stack.pop_back();
        NA = Z; NB = Z;
        all_rejected = false;
        last_rejected = true;
        try_next = true;
      }

      if ((NA == Z) && (NB == Z))
      {
        uint8_t x, y;

        if (last_rejected)
        {
          PRINT(cout << "if (last_rejected)" << endl);
          UA = UA_stack.back();
          UB = UB_stack.back();
          UR = UR_stack.back();
          DA = DA_stack.back();
          DB = DB_stack.back();
          DR = DR_stack.back();
          CA = CA_stack.back();
          CB = CB_stack.back();
          GA = GA_stack.back();
          GB = GB_stack.back();
          GR = GR_stack.back();
          LR = LR_stack.back();
          LA = LA_stack.back();
          LB = LB_stack.back();
          iLA = iLA_stack.back();
          iLB = iLB_stack.back();

          GS_old = GS_stack.back(); GS_stack.pop_back();

          x = OPs<N>.lsb_idx(UA);
          PRINT(cout << "Reject Guess: LA[" << x + '\0' << "] = " << GS_old + '\0'  << endl);       
          int next_guess = OPs<N>.next_lsb_idx(GA, GS_old);
          if (next_guess == -1)
          {
            PRINT(cout << "next_guess == -1" << endl);
            all_rejected = true;
            continue;
          }
          else
          {
            y = next_guess;
            last_rejected = false;
          }
        }
        else
        {
          UA_stack.push_back(UA);
          UB_stack.push_back(UB);
          UR_stack.push_back(UR);
          DA_stack.push_back(DA);
          DB_stack.push_back(DB);
          DR_stack.push_back(DR);
          CA_stack.push_back(CA);
          CB_stack.push_back(CB);
          GA_stack.push_back(GA);
          GB_stack.push_back(GB);
          GR_stack.push_back(GR);
          LR_stack.push_back(LR);
          LA_stack.push_back(LA);
          LB_stack.push_back(LB);
          iLA_stack.push_back(iLA);
          iLB_stack.push_back(iLB);

          x = OPs<N>.lsb_idx(UA);
          y = OPs<N>.lsb_idx(GA);
        }

        LA[x] = y; iLA[y] = x;
        PRINT(cout << "Guess: LA[" << x + '\0'  << "] = " << y + '\0'  << endl);
        GS_stack.push_back(y);

        PRINT(cout << "GS_stack.size(): " << GS_stack.size() << " at line: " << __LINE__ << endl);
        PRINT(cout << "UA_stack.size(): " << UA_stack.size() << " at line: " << __LINE__ << endl);

        Set_t DA_tmp = DA;
        Set_t GA_old = GA;
        while (DA_tmp != Z)
        {
          uint8_t xd = OPs<N>.unset_lsb_idx_inplace(DA_tmp);
          uint8_t yd = LA[xd];
          uint8_t xx = x ^ xd;
          uint8_t xy = y ^ yd;
          LA[xx] = xy; iLA[xy] = xx;
          PRINT(cout << hex
          << " From LA[" << x +'\0' << "] = " << y + '\0'
          << " and LA[" << xd +'\0' << "] = " << yd + '\0'
          << " get LA[" << xx +'\0' << "] = " << xy + '\0'
          << endl);
          OPs<N>.set_bit_inplace(DA, xx);
          OPs<N>.unset_bit_inplace(UA, xx);
          OPs<N>.unset_bit_inplace(GA, xy);
        }
        Set_t ADy = GA_old ^ GA;
        while (ADy != Z)
        {
          uint8_t cx = OPs<N>.unset_lsb_idx_inplace(ADy);
          uint8_t cy = iLA[cx];
          OPs<N>.set_bit_inplace(NA, cy);
        }
      }

      while (NA != Z)
      {
        uint8_t xa = OPs<N>.lsb_idx(NA);
        uint8_t yb = S[LA[xa]];
        CHECK(
        int determinedB = OPs<N>.get_bit(GB, yb);
        if (determinedB == 0)
        {
          last_rejected = true;
          NA = Z; NB = Z;
          break;
        });
        uint8_t xb = OPs<N>.lsb_idx(UB);
        LB[xb] = yb; iLB[yb] = xb;

        LR[xa] = xb;

        PRINT(cout << hex
          << " From LA[" << xa + '\0' << "] = " << LA[xa] + '\0'
          << " and S[" << LA[xa] + '\0' << "] = " << yb + '\0'
          << " and LB[" << xb + '\0' << "] = " << yb + '\0'
          << " get LR[" << xa + '\0' << "] = " << xb + '\0'
          << endl);
        PRINT(PRINT_LUT(LR));

        CHECKR(
        if ((OPs<N>.get_bit(DR, xa) == 1) || (OPs<N>.get_bit(GR, xb) == 0))
        {
          PRINT(cout << "(OPs<N>.get_bit(DR, xa) == 1) || (OPs<N>.get_bit(GR, xb) == 0)" << __LINE__ << endl);
          last_rejected = true;
          NA = Z; NB = Z;
          break;
        });
        OPs<N>.set_bit_inplace(DR, xa);
        OPs<N>.unset_bit_inplace(UR, xa);
        OPs<N>.unset_bit_inplace(GR, xb);
        int uk = OPs<N>.lsb_idx(UR);
        if (uk > xa)
        {
          if(lexicographical_compare(
            min_LR.begin(), min_LR.begin() + uk,
                LR.begin(), LR.begin() + uk))
          {
            last_rejected = true;
            NA = Z; NB = Z;
            break;
          }
        }

        Set_t GB_old = GB;
        Set_t DB_tmp = DB;
        while (DB_tmp != Z)
        {
          uint8_t xbd = OPs<N>.unset_lsb_idx_inplace(DB_tmp);
          uint8_t ybd = LB[xbd];
          uint8_t xbx = xb ^ xbd;
          uint8_t xby = yb ^ ybd;
          LB[xbx] = xby; iLB[xby] = xbx;
          PRINT(cout << hex
            << " From LB[" << xb +'\0' << "] = " << yb + '\0'
            << " and LB[" << xbd +'\0' << "] = " << ybd + '\0'
            << " get LB[" << xbx +'\0' << "] = " << xby + '\0'
            << endl);
          OPs<N>.set_bit_inplace(DB, xbx);
          OPs<N>.unset_bit_inplace(UB, xbx);
          OPs<N>.unset_bit_inplace(GB, xby); // maybe GB is not useful although the process
        }
        Set_t BDy = GB_old ^ GB;

        Set_t SAx = Z;
        Set_t NA_tmp = NA;
        while (NA_tmp != Z)
        {
          uint8_t xan = OPs<N>.unset_lsb_idx_inplace(NA_tmp);
          uint8_t ybn = S[LA[xan]];
          OPs<N>.set_bit_inplace(SAx, ybn);
        }
        Set_t cap_BD_SA = BDy & SAx;
        Set_t sub_BD_SA = BDy ^ cap_BD_SA;
        while (cap_BD_SA != Z)
        {
          uint8_t cx = OPs<N>.unset_lsb_idx_inplace(cap_BD_SA);
          uint8_t cy = iLB[cx];
          OPs<N>.set_bit_inplace(CB, cy);
          cy = iLA[iS[cx]];
          OPs<N>.set_bit_inplace(CA, cy);
          OPs<N>.unset_bit_inplace(NA, cy);
        }
        while (sub_BD_SA != Z)
        {
          uint8_t cx = OPs<N>.unset_lsb_idx_inplace(sub_BD_SA);
          uint8_t cy = iLB[cx];
          OPs<N>.set_bit_inplace(NB, cy);
        }

        while ((NA == Z) && (NB != Z))
        {
          uint8_t xbn = OPs<N>.lsb_idx(NB);
          uint8_t ya = iS[LB[xbn]];
          CHECK(
          int determinedA = OPs<N>.get_bit(GA, ya);
          if (determinedA == 0)
          {
            last_rejected = true;
            NA = Z; NB = Z;
            break;
          });
          uint8_t xau = OPs<N>.lsb_idx(UA);
          LA[xau] = ya; iLA[ya] = xau;
          PRINT(cout << hex
          << " From LB[" << xbn + '\0' << "] = " << LB[xbn] + '\0'
          << " and iS[" << LB[xbn] + '\0' << "] = " << ya + '\0'
          << " and LA[" << xau + '\0' << "] = " << ya + '\0'
          << " get LR[" << xau + '\0' << "] = " << xbn + '\0'
          << endl);

          LR[xau] = xbn;
          PRINT(PRINT_LUT(LR));

          CHECKR(
          if ((OPs<N>.get_bit(DR, xau) == 1) || (OPs<N>.get_bit(GR, xbn) == 0))
          {
            PRINT(cout << "(OPs<N>.get_bit(DR, xau) == 1) || (OPs<N>.get_bit(GR, xbn) == 0)" << __LINE__ << endl);  
            NA = Z; NB = Z;
            last_rejected = true;
            break;
          });
          OPs<N>.set_bit_inplace(DR, xau);
          OPs<N>.unset_bit_inplace(UR, xau);
          OPs<N>.unset_bit_inplace(GR, xbn);
          int uk = OPs<N>.lsb_idx(UR);
          if (uk > xau)
          {
            if(lexicographical_compare(
              min_LR.begin(), min_LR.begin() + uk,
                  LR.begin(), LR.begin() + uk))
            {
              NA = Z; NB = Z;
              last_rejected = true;
              break;
            }
          }

          Set_t DA_tmp = DA;
          Set_t GA_old = GA;
          while (DA_tmp != Z)
          {
            uint8_t xad = OPs<N>.unset_lsb_idx_inplace(DA_tmp);
            uint8_t yad = LA[xad];
            uint8_t xax = xau ^ xad;
            uint8_t xay = ya ^ yad;
            LA[xax] = xay; iLA[xay] = xax;
            PRINT(cout << hex
            << " From LA[" << xau +'\0' << "] = " << ya + '\0'
            << " and LA[" << xad +'\0' << "] = " << yad + '\0'
            << " get LA[" << xax +'\0' << "] = " << xay + '\0'
            << endl);
            OPs<N>.set_bit_inplace(DA, xax);
            OPs<N>.unset_bit_inplace(UA, xax);
            OPs<N>.unset_bit_inplace(GA, xay);
          }
          Set_t ADy = GA_old ^ GA;

          Set_t iSBx = Z;
          Set_t NB_tmp = NB;
          while (NB_tmp != Z)
          {
            uint8_t xbnt = OPs<N>.unset_lsb_idx_inplace(NB_tmp);
            uint8_t yant = iS[LB[xbnt]];
            OPs<N>.set_bit_inplace(iSBx, yant);
          }
          Set_t cap_AD_iSB = ADy & iSBx;
          Set_t sub_AD_iSB = ADy ^ cap_AD_iSB;
          while (cap_AD_iSB != Z)
          {
            uint8_t cx = OPs<N>.unset_lsb_idx_inplace(cap_AD_iSB);
            uint8_t cy = iLA[cx];
            OPs<N>.set_bit_inplace(CA, cy);
            cy = iLB[S[cx]];
            OPs<N>.set_bit_inplace(CB, cy);
            OPs<N>.unset_bit_inplace(NB, cy);
          }
          while (sub_AD_iSB != Z)
          {
            uint8_t cx = OPs<N>.unset_lsb_idx_inplace(sub_AD_iSB);
            uint8_t cy = iLA[cx];
            OPs<N>.set_bit_inplace(NA, cy);
          }
        }

        if ((UA == Z) && (UB == Z) && (!last_rejected))
        {
          try_next = true;

          Set_t DR_tmp = ~DR;
          while (DR_tmp != Z)
          {
            uint8_t x = OPs<N>.unset_lsb_idx_inplace(DR_tmp);
            uint8_t y = iLB[S[LA[x]]];
            CHECKR(
            if (OPs<N>.get_bit(GR, y) == 0)
            {
              PRINT(cout << "OPs<N>.get_bit(GR, y) == 0" << __LINE__ << endl);  
              NA = Z; NB = Z;
              last_rejected = true;
              break;
            });
            LR[x] = y;
            OPs<N>.set_bit_inplace(DR, x);
            OPs<N>.unset_bit_inplace(UR, x);
            OPs<N>.unset_bit_inplace(GR, y);

            int uk = OPs<N>.lsb_idx(UR);
            if (uk > x)
            {
              if(lexicographical_compare(
                min_LR.begin(), min_LR.begin() + uk,
                    LR.begin(), LR.begin() + uk))
              {
                NA = Z; NB = Z;
                last_rejected = true;
                break;
              }
            }
          }
          if (last_rejected) break;
          PRINT(cout << " Update min_LR: ");
          PRINT(PRINT_LUT(min_LR));
          PRINT(cout << " with LR: ");
          PRINT(PRINT_LUT(LR));
          
          min_LR = LR;
          min_LA = LA;
          min_LB = LB;
        
          last_rejected = true;
        }
      }
    }

    memcpy(RS, min_LR.data(), sizeof(uint8_t) * LUT_UNIT_N);
    memcpy(A, min_LA.data(), sizeof(uint8_t) * LUT_UNIT_N);
    memcpy(B, min_LB.data(), sizeof(uint8_t) * LUT_UNIT_N);
  }

  bool is_AE(const uint8_t S2[LUT_UNIT_N], uint8_t RS[LUT_UNIT_N], 
            uint8_t S1_L1[LUT_UNIT_N], uint8_t S1_L2[LUT_UNIT_N], uint8_t &a,
            uint8_t S2_L1[LUT_UNIT_N], uint8_t S2_L2[LUT_UNIT_N], uint8_t &b) const
  {
    PRINT(cout << __LINE__ << endl);
    if (!is_permutation())
    {
      return false;
    }

    struct LExinfo_t
    {
      array<uint8_t, LUT_UNIT_N> L1;
      array<uint8_t, LUT_UNIT_N> L2;
      uint8_t c;
    };
    
    map<array<uint8_t, LUT_UNIT_N>, LExinfo_t> S1_minRs;
    map<array<uint8_t, LUT_UNIT_N>, LExinfo_t> S2_minRs;

    function_t S1aF;
    function_t S2bF;
    function_t S2F;
    array<uint8_t, LUT_UNIT_N> R;
    uint8_t * Rp = R.data();

    for (int i = 0; i < LUT_XMM_N; i++)
    {
      S2F.LUT[i] = _mm_loadu_si128(((__m128i *)S2) + i);
    }


    for (int ai = 0; ai < (1<<N); ai++)
    {
      LExinfo_t LExinfo;
      LExinfo.c = ai;
      
      OPs<N>.composite(S1aF.LUT, LUT, XE<N>[ai].LUT);
      PRINT(cout << S1aF.LUT_to_string() << endl);
      S1aF.LE_representative(Rp, LExinfo.L1.data(), LExinfo.L2.data());
      PRINT(cout << __LINE__ << endl);
      PRINT(PRINT_LUT(R));
      S1_minRs.insert(pair<array<uint8_t, LUT_UNIT_N>, LExinfo_t>(R, LExinfo));

      OPs<N>.composite(S2bF.LUT, XE<N>[ai].LUT, S2F.LUT);
      PRINT(cout << S2bF.LUT_to_string() << endl);
      S2bF.LE_representative(Rp, LExinfo.L1.data(), LExinfo.L2.data());
      PRINT(cout << __LINE__ << endl);
      PRINT(PRINT_LUT(R));
      S2_minRs.insert(pair<array<uint8_t, LUT_UNIT_N>, LExinfo_t>(R, LExinfo));
    }

    PRINT(cout << __LINE__ << endl);

    for (const auto & each_S1_minR : S1_minRs)
    {
      PRINT(cout << __LINE__ << endl);
      auto the_S2_minR = S2_minRs.find(each_S1_minR.first);
      if (the_S2_minR != S2_minRs.end())
      {
        memcpy(RS, each_S1_minR.first.data(), sizeof(uint8_t) * LUT_UNIT_N);
        memcpy(S1_L1, each_S1_minR.second.L1.data(), sizeof(uint8_t) * LUT_UNIT_N);
        memcpy(S1_L2, each_S1_minR.second.L2.data(), sizeof(uint8_t) * LUT_UNIT_N);
        a = each_S1_minR.second.c;

        memcpy(S2_L1, (*the_S2_minR).second.L1.data(), sizeof(uint8_t) * LUT_UNIT_N);
        memcpy(S2_L2, (*the_S2_minR).second.L2.data(), sizeof(uint8_t) * LUT_UNIT_N);
        b = (*the_S2_minR).second.c;

        return true;
      }
    }
    return false;
  }

  bool is_AE(const function_t & S2, function_t & RS, 
            function_t & S1_L1, function_t & S1_L2, uint8_t &a,
            function_t & S2_L1, function_t & S2_L2, uint8_t &b) const
  {
    bool isAE_flag = is_AE((uint8_t *)S2.LUT, (uint8_t *)RS.LUT,
                  (uint8_t *)S1_L1.LUT, (uint8_t *)S1_L2.LUT, a,
                  (uint8_t *)S2_L1.LUT, (uint8_t *)S2_L2.LUT, b);
    if (isAE_flag)
    {
      RS.LUT_to_bit_slice();
      S1_L1.LUT_to_bit_slice();
      S1_L2.LUT_to_bit_slice();
      S2_L1.LUT_to_bit_slice();
      S2_L2.LUT_to_bit_slice();
    }
    return isAE_flag;
  }

  // B(Gi(A(x) + a)) + b = S(x)
  bool is_optimal(int & gi, function_t & Gi, function_t & A, function_t & B, uint8_t & a, uint8_t & b) const
  {
    if (N == 4)
    {
      if (!is_permutation())
      {
        return false;
      }

      array<uint8_t, LUT_UNIT_N> R;
      uint8_t * Rp = R.data();
      function_t SbF;
      map<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> > S_Rs;
      for (int ai = 0; ai < (1<<N); ai++)
      {
        LExinfoGx_t<N> LExinfo;
        LExinfo.c = ai;

        OPs<N>.composite(SbF.LUT, XE<N>[ai].LUT, LUT);
        SbF.LE_representative(Rp, LExinfo.L1.data(), LExinfo.L2.data());
        S_Rs.insert(pair<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> >(R, LExinfo));
      }

      function_t S_A;
      function_t S_B;
      function_t G_A;
      function_t G_B;

      for (const auto & each_S_R : S_Rs)
      {
        auto the_G_R = G_Rs<N>.find(each_S_R.first);
        if (the_G_R != G_Rs<N>.end())
        {
          gi = (*the_G_R).second.Gx;
          Gi.LUT[0] = _mm_load_si128((__m128i *)G4[gi]); Gi.LUT_to_bit_slice();

          G_A.LUT[0] = _mm_loadu_si128((__m128i *)(*the_G_R).second.L1.data());
          G_B.LUT[0] = _mm_loadu_si128((__m128i *)(*the_G_R).second.L2.data());
          G_B = G_B.inverse();
          a = (*the_G_R).second.c;

          S_A.LUT[0] = _mm_loadu_si128((__m128i *)each_S_R.second.L1.data());
          S_A = S_A.inverse();
          S_B.LUT[0] = _mm_loadu_si128((__m128i *)each_S_R.second.L2.data());
          b = each_S_R.second.c;

          OPs<N>.composite(A.LUT, G_A.LUT, S_A.LUT);  A.LUT_to_bit_slice();
          OPs<N>.composite(B.LUT, G_B.LUT, S_B.LUT);  B.LUT_to_bit_slice();

          return true;
        }
      }
      return false;
    }
    else
    {
      // cout << "is_optimal(...):: Not support for N != 4" << endl;
      return false;
    }
  }

    // B(Gi(A(x) + a)) + b = S(x)
  bool is_optimal(int & gi, uint8_t Gi[LUT_UNIT_N], uint8_t A[LUT_UNIT_N], uint8_t B[LUT_UNIT_N], uint8_t & a, uint8_t & b) const
  {
    if (N == 4)
    {
      if (!is_permutation())
      {
        return false;
      }
      function_t G_F;
      function_t A_F;
      function_t B_F;
      bool flag = is_optimal(gi, G_F, A_F, B_F, a, b);
      if (flag)
      {
        memcpy(Gi, (uint8_t *)G_F.LUT, sizeof(uint8_t)*LUT_UNIT_N);
        memcpy(A, (uint8_t *)A_F.LUT, sizeof(uint8_t)*LUT_UNIT_N);
        memcpy(B, (uint8_t *)B_F.LUT, sizeof(uint8_t)*LUT_UNIT_N);
      }
      return flag;
    }
    else
    {
      // cout << "is_optimal(...):: Not support for N != 4" << endl;
      return false;
    }
  }

  void get_AE_PEreps(set<function_t > & PEreps) const
  {
    if (N == 4)
    {
      function_t PB_S_AP;
      for (int ai = 0; ai < AFFINE_MATRIX_COL_PE_BOX4_N; ai++)
      {
        __m128i AP_LUT[LUT_XMM_N];
        AP_LUT[0] = _mm_load_si128((__m128i *)AffineMatrixColPEBox4[ai]);
        OPs<N>.composite(AP_LUT, LUT, AP_LUT);
        for (int bi = 0; bi < AFFINE_MATRIX_ROW_PE_BOX4_N; bi++)
        {
          __m128i PB_LUT[LUT_XMM_N];
          PB_LUT[0] = _mm_load_si128((__m128i *)AffineMatrixRowPEBox4[bi]);
          OPs<N>.composite(PB_S_AP.LUT, PB_LUT, AP_LUT);
          PB_S_AP.LUT_to_bit_slice();
          PEreps.insert(PB_S_AP.PE_representative());
        }
      }
    }
    else
    {
      // cout << "get_AE_PEreps(...):: Not support for N != 4" << endl;
    }
  }

  void get_AE_PEreps(string outfile) const
  {
    ofstream outf(outfile.c_str(), ios::app);
    set<function_t > PEreps;
    get_AE_PEreps(PEreps);
    int idx = 0;
    for (auto per : PEreps)
    {
      outf << idx << "," << per.LUT_to_string() << endl;
      idx++;
    }
    outf.close();
  }

  void get_AE_PXEreps(set<function_t > & PXEreps) const
  {
    if (N == 4)
    {
      function_t PB_S_AP;
      for (int ai = 0; ai < AFFINE_MATRIX_COL_PE_BOX4_N; ai++)
      {
        __m128i AP_LUT[LUT_XMM_N];
        AP_LUT[0] = _mm_load_si128((__m128i *)AffineMatrixColPEBox4[ai]);
        OPs<N>.composite(AP_LUT, LUT, AP_LUT);
        for (int bi = 0; bi < AFFINE_MATRIX_ROW_PE_BOX4_N; bi++)
        {
          __m128i PB_LUT[LUT_XMM_N];
          PB_LUT[0] = _mm_load_si128((__m128i *)AffineMatrixRowPEBox4[bi]);
          OPs<N>.composite(PB_S_AP.LUT, PB_LUT, AP_LUT);
          PB_S_AP.LUT_to_bit_slice();
          PXEreps.insert(PB_S_AP.PXE_representative());
        }
      }
    }
    else
    {
      // cout << "get_AE_PXEreps(...):: Not support for N != 4" << endl;
    }
  }

  void get_AE_PXEreps(string outfile) const
  {
    ofstream outf(outfile.c_str(), ios::app);
    set<function_t > PXEreps;
    get_AE_PXEreps(PXEreps);
    int idx = 0;
    for (auto per : PXEreps)
    {
      outf << idx << "," << per.LUT_to_string() << endl;
      idx++;
    }
    outf.close();
  }

  int which_optimal() const
  {
    if (N == 4)
    {
      if (!is_permutation())
      {
        return -1;
      }

      array<uint8_t, LUT_UNIT_N> R;
      uint8_t * Rp = R.data();
      function_t SbF;
      map<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> > S_Rs;
      for (int ai = 0; ai < (1<<N); ai++)
      {
        LExinfoGx_t<N> LExinfo;
        LExinfo.c = ai;

        OPs<N>.composite(SbF.LUT, XE<N>[ai].LUT, LUT);
        SbF.LE_representative(Rp, LExinfo.L1.data(), LExinfo.L2.data());
        S_Rs.insert(pair<array<uint8_t, LUT_UNIT_N>, LExinfoGx_t<N> >(R, LExinfo));
      }

      for (const auto & each_S_R : S_Rs)
      {
        auto the_G_R = G_Rs<N>.find(each_S_R.first);
        if (the_G_R != G_Rs<N>.end())
        {
          return (*the_G_R).second.Gx;
        }
      }
      return -1;
    }
    else
    {
      //cout << "which_optimal():: Not support for N != 4" << endl;
      return -1;
    }
  }

  string show_all_properties() const
  {
    stringstream ss;
    ss << show_LUT() << endl;
    ss << show_coordinates_ANF() << endl;
    ss << show_components_ANF() << endl;
		ss << show_difference_distribution_matrix() << endl;
    ss << show_boomerang_connection_table() << endl;
		ss << show_linear_approximation_matrix() << endl;
		ss << show_autocorrelation_matrix() << endl;
    ss << show_k_product_degree_table() << endl;
    ss << show_V_S_table() << endl;
		ss << show_linear_structures() << endl;
		ss << show_v_w_linear() << endl;
    return ss.str();
  }

  void show_all_properties(ofstream & fout) const
  {
    fout << show_all_properties();
  }
};

using namespace Peigen;
using namespace Peigen::weight;
struct Peigen::weight::bool_op_t
{
  int op_id;
  int op_cost;

  bool operator < (const bool_op_t& b) const
  {
    return op_cost < b.op_cost;
  }

  bool operator == (const bool_op_t& b) const
  {
    return op_cost == b.op_cost;
  }
};

#ifdef NDEBUG
#undef NDEBUG
#endif
#ifdef NCHECKR
#undef NCHECKR
#endif
#ifdef NCHECKAB
#undef NCHECKAB
#endif

#endif /* FUNC_H__ */
