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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fstream>
#include <string>
#include "func.hpp"

using namespace Peigen;
using namespace Peigen::weight;
using namespace std;

template<typename T>
inline T MAOI1(T x, T y, T z, T t)
{
  return (~((x & y) | (~(z | t))));
}

template<typename T>
inline T MOAI1(T x, T y, T z, T t)
{
  return (~((x | y) & (~(z & t))));
}

template<typename T>
inline T NAND2(T x, T y)
{
  return ~(x & y);
}

template<typename T>
inline T NOR2(T x, T y)
{
  return ~(x | y);
}

template<typename T>
inline T NAND3(T x, T y, T z)
{
  return ~(x & y & z);
}

template<typename T>
inline T NOR3(T x, T y, T z)
{
  return ~(x | y | z);
}

template<typename T>
inline T AND3(T x, T y, T z)
{
  return (x & y & z);
}

template<typename T>
inline T OR3(T x, T y, T z)
{
  return (x | y | z);
}

template<typename T>
inline T AND2(T x, T y)
{
  return (x & y);
}

template<typename T>
inline T ANDN2(T x, T y)
{
  return (~x & y);
}

template<typename T>
inline T OR2(T x, T y)
{
  return (x | y);
}

template<typename T>
inline T ORN2(T x, T y)
{
  return ((~x) | y);
}

template<typename T>
inline T XOR2(T x, T y)
{
  return (x ^ y);
}

template<typename T>
inline T XNOR2(T x, T y)
{
  return ~(x ^ y);
}

template<typename T>
inline T NOT1(T x)
{
  return ~x;
}

int main()
{
  ofstream fout("results_test.txt");

#define n 4
  function_t<n> I = function_t<n>::INPUT_DEFAULT();
  bit_slice_t<n> X;
  bit_slice_t<n> F;

  #include "testcode.cpp"  

  fout.close();

}
