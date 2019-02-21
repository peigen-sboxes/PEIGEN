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

#ifndef FASTER_FUNC_H__
#define FASTER_FUNC_H__

#include "constants.hpp"

using namespace std;
using namespace Peigen::depth;

#define FOUND_ALL ((1<<N) - 1)

#define NOP   0
#define NOT1  1
#define AND2  2
#define NAND2 3
#define OR2   4
#define NOR2  5
#define XOR2  6
#define XNOR2 7
#define MOAI1 8
#define MAOI1 9
#define NAND3 10
#define NOR3  11
#define OR3   12
#define AND3  13
#define ANDN2 14
#define ORN2  15

#define MAX_COST (numeric_limits<int>::max())

template<int N>
struct Peigen::depth::bool_function_t
{
  /*
    the bit-sliced representation of the function
  */
  bit_slice_l_t<N> bit_slice{ {} };

  /*
    identifier for last boolean operator
  */
  uint8_t info_op = NOP;

  int area = 0;

  const bool_function_t<N> * operands[3];

  bool_function_t()
  {
    info_op = NOP;
    area = 0;
    operands[0] = NULL;
    operands[1] = NULL;
    operands[2] = NULL;
  };

  bool_function_t(const bit_slice_l_t<N> a)
  {
    bit_slice = a;
    info_op = NOP;
    area = 0;
    operands[0] = NULL;
    operands[1] = NULL;
    operands[2] = NULL;
  }

  bool_function_t(const bool_function_t& f)
  {
    bit_slice = f.bit_slice;
    info_op = f.info_op;
    area = f.area;
    operands[0] = f.operands[0];
    operands[1] = f.operands[1];
    operands[2] = f.operands[2]; 
  }

  bool operator < (const bool_function_t& f) const
  {
    return (bit_slice < f.bit_slice) || ((bit_slice == f.bit_slice) && (area < f.area));
  }

  bool operator == (const bool_function_t& f) const
  {
    return (bit_slice == f.bit_slice) && (area == f.area);
  }

  string to_bitstring() const
  {
    string str = "";

    for (int j = 0; j < UNIT_N ; j++)
    {
        bitset<UNIT_BIT_N> tmp(bit_slice[j]);
        str += tmp.to_string();
    }

    return str;
  }

  string to_string() const
  {
    stringstream ss;
    ss << hex << setfill('0');

    for (int j = UNIT_N - 1; j >= 0; j--)
    {
        ss << setw(UNIT_NIBBLE_N) << bit_slice[j] + '\0';
    }

    string res = ss.str();
    return res;
  }

  bool is_balanced() const
  {
    uint64_t sum = 0;
    uint64_t * pt = bit_slice.data();

    for (int j = 0; j < (BIT_SLICE_BITS_N>>6) - 1; j++)
    {
      sum += _mm_popcnt_u64((uint64_t)(pt[j]));
    }
    sum += _mm_popcnt_u64(
      (uint64_t)(pt[BIT_SLICE_BITS_N >> 6]) &
      ((1ULL << (BIT_SLICE_BITS_N & 0x3fULL)) - 1ULL));

    return (sum == (BIT_SLICE_BITS_N >> 1ULL));
  }
};

template<int N>
using coordinates_t = std::array<Peigen::depth::bool_function_t<N>, N>;

template<int N>
struct Peigen::depth::function_t
{
  coordinates_t<N> coordinates;

  array<int, N> depth = { };

  function_t()
  {
    for (int i = 0; i < N; i++)
    {
      depth[i] = MAX_COST;
    }
  };

  function_t(const bit_slice_t<N>& a)
  {
    for (int i = 0; i < N; i++)
    {
      coordinates[i].bit_slice = a[i];
      coordinates[i].area = 0;
      depth[i] = MAX_COST;
    }
  }

  function_t(const coordinates_t<N>& a)
  {
    coordinates = a;
    for (int i = 0; i < N; i++)
    {
      depth[i] = MAX_COST;
    }
  }

  function_t(const function_t& f)
  {
    coordinates = f.coordinates;
    depth = f.depth;
  }

  function_t(const string str)
  {
    parse_function(str);
    for (int i = 0; i < N; i++)
    {
      depth[i] = MAX_COST;
    }
  }

  static function_t INPUT_DEFAULT()
  {    
    function_t I;    
    uint8_t LUT[LUT_UNIT_N];
    for (int i = 0; i < LUT_UNIT_N; i++)
    {
      LUT[i] = i & 0xff;
    }
    I.LUT_to_bit_slice(LUT);
    return I;
  }

  void parse_function(const string str)
  {
    if(str.size() == (BIT_SLICE_NIBBLES_N * N) + (N - 1))
    {
      string_to_bit_slice(str);
    }
    else if(str.size() == (BIT_SLICE_BITS_N<<1))
    {
      uint8_t LUT[LUT_UNIT_N];
      string_to_LUT(str, LUT);
      LUT_to_bit_slice(LUT);
    }
    else
    {
      cout << "Error: Length of the string representing the function should be either "
         << (BIT_SLICE_NIBBLES_N * N) + (N - 1) << " or "
         << (BIT_SLICE_BITS_N<<1) << endl;
    }
    for (int i = 0; i < N; i++)
    {
      coordinates[i].area = 0;
      depth[i] = MAX_COST;
    }
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
          coordinates[i].bit_slice[j] = stoul(number.substr((UNIT_N - 1 - j)*UNIT_NIBBLE_N, UNIT_NIBBLE_N), nullptr, 16);
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
  void string_to_LUT(const string& str, uint8_t LUT[])
  {
    if(str.size() == (BIT_SLICE_BITS_N<<1))
    {
      string tmp;
      for (int i = 0; i < LUT_BYTE_N; i++)
      {
        tmp = str.substr(i * 2, 2);
        LUT[i] = (uint8_t) stoul(tmp, nullptr, 16);
      }
    }
    else
    {
      cout << "Error: Length of the string representing the function in LUT form should be "
           << (BIT_SLICE_BITS_N<<1) << endl;
      exit(1);
    }
  }

  // !!! Only applicable for N = 3, 4, 5, 6, 7, 8
  void LUT_to_bit_slice(uint8_t x[])
  {
    __m128i LUT[LUT_XMM_N];
    for (int i = 0; i < LUT_XMM_N; i++)
    {
      LUT[i] = _mm_loadu_si128((__m128i *)(x + i * 16));
    }

    bit_slice_t<N> bit_slice{{}};

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

    for (int i = 0; i < N; i++)
    {
      coordinates[i].bit_slice = bit_slice[i];
      coordinates[i].area = 0;
    }  
  }

  bool operator < (const function_t& f) const
  {
    return coordinates < f.coordinates;
  }

  bool operator == (const function_t& f) const
  {
    return coordinates == f.coordinates;
  }

  string to_bitstring() const
  {
    string str = "";
    
    for (int i = N - 1; i >= 0; i--)
    {
      for (int j = UNIT_N - 1; j >= 0; j--)
      {
          bitset<UNIT_BIT_N> tmp(coordinates[i].bit_slice[j]);
          str += tmp.to_string();
      }
      str += "_";
    }
    str.pop_back();
    return str;
  }

  string to_string() const
  {
    stringstream ss;
    ss << hex << setfill('0');
    
    for (int i = N - 1; i >= 0; i--)
    {
      for (int j = UNIT_N - 1; j >= 0; j--)
      {
          ss << setw(UNIT_NIBBLE_N) << coordinates[i].bit_slice[j] + '\0';
      }
      ss << "_";            
    }
    string res = ss.str();
    res.pop_back();
    return res;
  }
  /*
    Sort the lines of the function matrix
    Ascending order
    Ex :
          0101010101010101
          0011001100110011
          0000111100001111
          0000000000000000
  */
  void sort()
  {
    StaticSort<N> staticSort;
    staticSort(coordinates);
  }
};

struct Peigen::depth::bool_op_t
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

#endif // #ifndef FASTER_FUNC_H__