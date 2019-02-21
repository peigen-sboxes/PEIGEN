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

#ifndef LIGHTER_BOOL_OP_H__
#define LIGHTER_BOOL_OP_H__

using namespace Peigen;
using namespace Peigen::weight;
using namespace std;

template<int N>
void lighter<N>::all_gates()
{
  gate_not1  = true;
  gate_and2  = true;
  gate_nand2 = true;
  gate_or2   = true;
  gate_nor2  = true;
  gate_xor2  = true;
  gate_xnor2 = true;
  gate_moai1 = true;
  gate_maoi1 = true;
  gate_nand3 = true;
  gate_nor3  = true;
  gate_or3   = true;
  gate_and3  = true;
  gate_andn2  = true;
  gate_orn2   = true;
}

template<int N>
void lighter<N>::genImpInfo()
{
  imp_info = "";
  if (gate_not1  ) imp_info += "-not1";
  if (gate_and2  ) imp_info += "-and2";
  if (gate_nand2 ) imp_info += "-nand2";
  if (gate_or2   ) imp_info += "-or2";
  if (gate_nor2  ) imp_info += "-nor2";
  if (gate_xor2  ) imp_info += "-xor2";
  if (gate_xnor2 ) imp_info += "-xnor2";
  if (gate_moai1 ) imp_info += "-moai1";
  if (gate_maoi1 ) imp_info += "-maoi1";
  if (gate_nand3 ) imp_info += "-nand3";
  if (gate_nor3  ) imp_info += "-nor3";
  if (gate_or3   ) imp_info += "-or3";
  if (gate_and3  ) imp_info += "-and3";
  if (gate_andn2 ) imp_info += "-andn2";
  if (gate_orn2  ) imp_info += "-orn2";

  imp_info += "-f";
  imp_info += conf_file.substr(0, conf_file.rfind('.'));
  imp_info += "-N";
  imp_info += to_string(N);
}

template<int N>
int lighter<N>::bool_op_cost(uint8_t op)
{
  switch (op)
  {
    case NOT1              : return not1_cost;
    case XOR2              : return xor2_cost;
    case XNOR2             : return xnor2_cost;
    case MAOI1             : return maoi1_cost;
    case MOAI1             : return moai1_cost;
    case MOAI1_NAND2       : return moai1_cost + nand2_cost;
    case MOAI1_NOR2        : return moai1_cost + nor2_cost;
    case MOAI1_AND2        : return moai1_cost + and2_cost;
    case MOAI1_OR2         : return moai1_cost + or2_cost;
    case MOAI1_NAND3       : return moai1_cost + nand3_cost;
    case MOAI1_NOR3        : return moai1_cost + nor3_cost;
    case MAOI1_NAND3       : return maoi1_cost + nand3_cost;
    case MAOI1_NOR3        : return maoi1_cost + nor3_cost;
    case MOAI1_AND2_NAND2  : return moai1_cost + and2_cost + nand2_cost;
    case MOAI1_OR2_NOR2    : return moai1_cost + or2_cost + nor2_cost;
    case MOAI1_NAND2_NAND2 : return moai1_cost + 2*nand2_cost;
    case MOAI1_NOR2_NOR2   : return moai1_cost + 2*nor2_cost;
    case MOAI1_NAND2_OR2   : return moai1_cost + nand2_cost + or2_cost;
    case MOAI1_NOR2_AND2   : return moai1_cost + nor2_cost + and2_cost;
    case MOAI1_NAND2_NOR2  : return moai1_cost + nand2_cost + nor2_cost;
    case MOAI1_NOR2_NAND2  : return moai1_cost + nor2_cost + nand2_cost;
    case MOAI1_AND2_OR2    : return moai1_cost + and2_cost + or2_cost;
    case MOAI1_OR2_AND2    : return moai1_cost + or2_cost + and2_cost;
    case MOAI1_AND2_NOR2   : return moai1_cost + and2_cost + nor2_cost;
    case MOAI1_OR2_NAND2   : return moai1_cost + or2_cost + nand2_cost;
    case MAOI1_NAND2_NAND2 : return maoi1_cost + nand2_cost + nand2_cost;
    case MAOI1_NOR2_NOR2   : return maoi1_cost + nor2_cost + nor2_cost;
    case MAOI1_AND2_NAND2  : return maoi1_cost + and2_cost + nand2_cost;
    case MAOI1_OR2_NOR2    : return maoi1_cost + or2_cost + nor2_cost;
    case MAOI1_AND2_OR2    : return maoi1_cost + and2_cost + or2_cost;
    case MAOI1_OR2_AND2    : return maoi1_cost + or2_cost + and2_cost;
    case MAOI1_AND2_NOR2   : return maoi1_cost + and2_cost + nor2_cost;
    case MAOI1_OR2_NAND2   : return maoi1_cost + or2_cost + nand2_cost;
    case MAOI1_NAND2_OR2   : return maoi1_cost + nand2_cost + or2_cost;
    case MAOI1_NOR2_AND2   : return maoi1_cost + nor2_cost + and2_cost;
    case MAOI1_NAND2_NOR2  : return maoi1_cost + nand2_cost + nor2_cost;
    case MAOI1_NOR2_NAND2  : return maoi1_cost + nor2_cost + nand2_cost;
    case MAOI1_NAND2       : return maoi1_cost + nand2_cost;
    case MAOI1_NOR2        : return maoi1_cost + nor2_cost;
    case MAOI1_AND2        : return maoi1_cost + and2_cost;
    case MAOI1_OR2         : return maoi1_cost + or2_cost;
    case MAOI1_AND3        : return maoi1_cost + and3_cost;
    case MOAI1_NAND2_AND2  : return moai1_cost + nand2_cost + and2_cost;
    case MAOI1_AND2_AND2   : return maoi1_cost + and2_cost + and2_cost;
    case MAOI1_OR3         : return maoi1_cost + or3_cost;
    case MOAI1_NOR2_OR2    : return moai1_cost + nor2_cost + or2_cost;
    case MAOI1_OR2_OR2     : return maoi1_cost + or2_cost + or2_cost;
    case MOAI1_AND3        : return moai1_cost + and3_cost;
    case MAOI1_NAND2_AND2  : return maoi1_cost + nand2_cost + and2_cost;
    case MOAI1_AND2_AND2   : return moai1_cost + and2_cost + and2_cost;
    case MOAI1_OR3         : return moai1_cost + or3_cost;
    case MAOI1_NOR2_OR2    : return maoi1_cost + nor2_cost + or2_cost;
    case MOAI1_OR2_OR2     : return moai1_cost + or2_cost + or2_cost;

	  case MOAI1_ANDN2L        : return moai1_cost + andn2_cost;
	  case MOAI1_ANDN2R        : return moai1_cost + andn2_cost;
	  case MOAI1_ORN2L         : return moai1_cost + orn2_cost;
	  case MOAI1_ORN2R         : return moai1_cost + orn2_cost;
	  case MOAI1_AND2_ANDN2L   : return moai1_cost + and2_cost  + andn2_cost;
	  case MOAI1_AND2_ANDN2R   : return moai1_cost + and2_cost  + andn2_cost;
	  case MOAI1_AND2_ORN2L    : return moai1_cost + and2_cost  + orn2_cost;
	  case MOAI1_AND2_ORN2R    : return moai1_cost + and2_cost  + orn2_cost;
	  case MOAI1_OR2_ANDN2L    : return moai1_cost + or2_cost   + andn2_cost;
	  case MOAI1_OR2_ANDN2R    : return moai1_cost + or2_cost   + andn2_cost;
	  case MOAI1_OR2_ORN2L     : return moai1_cost + or2_cost   + orn2_cost;
	  case MOAI1_OR2_ORN2R     : return moai1_cost + or2_cost   + orn2_cost;
	  case MOAI1_NAND2_ANDN2L  : return moai1_cost + nand2_cost + andn2_cost;
	  case MOAI1_NAND2_ANDN2R  : return moai1_cost + nand2_cost + andn2_cost;
	  case MOAI1_NAND2_ORN2L   : return moai1_cost + nand2_cost + orn2_cost;
	  case MOAI1_NAND2_ORN2R   : return moai1_cost + nand2_cost + orn2_cost;
	  case MOAI1_NOR2_ANDN2L   : return moai1_cost + nor2_cost  + andn2_cost;
	  case MOAI1_NOR2_ANDN2R   : return moai1_cost + nor2_cost  + andn2_cost;
	  case MOAI1_NOR2_ORN2L    : return moai1_cost + nor2_cost  + orn2_cost;
	  case MOAI1_NOR2_ORN2R    : return moai1_cost + nor2_cost  + orn2_cost;
	  case MOAI1_ANDN2L_AND2   : return moai1_cost + andn2_cost + and2_cost;
	  case MOAI1_ANDN2L_OR2    : return moai1_cost + andn2_cost + or2_cost;
	  case MOAI1_ANDN2L_NAND2  : return moai1_cost + andn2_cost + nand2_cost;
	  case MOAI1_ANDN2L_NOR2   : return moai1_cost + andn2_cost + nor2_cost;
	  case MOAI1_ANDN2L_ANDN2L : return moai1_cost + andn2_cost + andn2_cost;
	  case MOAI1_ANDN2L_ANDN2R : return moai1_cost + andn2_cost + andn2_cost;
	  case MOAI1_ANDN2L_ORN2L  : return moai1_cost + andn2_cost + orn2_cost;
	  case MOAI1_ANDN2L_ORN2R  : return moai1_cost + andn2_cost + orn2_cost;
	  case MOAI1_ANDN2R_AND2   : return moai1_cost + andn2_cost + and2_cost;
	  case MOAI1_ANDN2R_OR2    : return moai1_cost + andn2_cost + or2_cost;
	  case MOAI1_ANDN2R_NAND2  : return moai1_cost + andn2_cost + nand2_cost;
	  case MOAI1_ANDN2R_NOR2   : return moai1_cost + andn2_cost + nor2_cost;
	  case MOAI1_ANDN2R_ANDN2L : return moai1_cost + andn2_cost + andn2_cost;
	  case MOAI1_ANDN2R_ANDN2R : return moai1_cost + andn2_cost + andn2_cost;
	  case MOAI1_ANDN2R_ORN2L  : return moai1_cost + andn2_cost + orn2_cost;
	  case MOAI1_ANDN2R_ORN2R  : return moai1_cost + andn2_cost + orn2_cost;
	  case MOAI1_ORN2L_AND2    : return moai1_cost + orn2_cost  + and2_cost;
	  case MOAI1_ORN2L_OR2     : return moai1_cost + orn2_cost  + or2_cost;
	  case MOAI1_ORN2L_NAND2   : return moai1_cost + orn2_cost  + nand2_cost;
	  case MOAI1_ORN2L_NOR2    : return moai1_cost + orn2_cost  + nor2_cost;
	  case MOAI1_ORN2L_ANDN2L  : return moai1_cost + orn2_cost  + andn2_cost;
	  case MOAI1_ORN2L_ANDN2R  : return moai1_cost + orn2_cost  + andn2_cost;
	  case MOAI1_ORN2L_ORN2L   : return moai1_cost + orn2_cost  + orn2_cost;
	  case MOAI1_ORN2L_ORN2R   : return moai1_cost + orn2_cost  + orn2_cost;
	  case MOAI1_ORN2R_AND2    : return moai1_cost + orn2_cost  + and2_cost;
	  case MOAI1_ORN2R_OR2     : return moai1_cost + orn2_cost  + or2_cost;
	  case MOAI1_ORN2R_NAND2   : return moai1_cost + orn2_cost  + nand2_cost;
	  case MOAI1_ORN2R_NOR2    : return moai1_cost + orn2_cost  + nor2_cost;
	  case MOAI1_ORN2R_ANDN2L  : return moai1_cost + orn2_cost  + andn2_cost;
	  case MOAI1_ORN2R_ANDN2R  : return moai1_cost + orn2_cost  + andn2_cost;
	  case MOAI1_ORN2R_ORN2L   : return moai1_cost + orn2_cost  + orn2_cost;
	  case MOAI1_ORN2R_ORN2R   : return moai1_cost + orn2_cost  + orn2_cost;
	  case MAOI1_ANDN2L        : return maoi1_cost + andn2_cost;
	  case MAOI1_ANDN2R        : return maoi1_cost + andn2_cost;
	  case MAOI1_ORN2L         : return maoi1_cost + orn2_cost;
	  case MAOI1_ORN2R         : return maoi1_cost + orn2_cost;
	  case MAOI1_AND2_ANDN2L   : return maoi1_cost + and2_cost  + andn2_cost;
	  case MAOI1_AND2_ANDN2R   : return maoi1_cost + and2_cost  + andn2_cost;
	  case MAOI1_AND2_ORN2L    : return maoi1_cost + and2_cost  + orn2_cost;
	  case MAOI1_AND2_ORN2R    : return maoi1_cost + and2_cost  + orn2_cost;
	  case MAOI1_OR2_ANDN2L    : return maoi1_cost + or2_cost   + andn2_cost;
	  case MAOI1_OR2_ANDN2R    : return maoi1_cost + or2_cost   + andn2_cost;
	  case MAOI1_OR2_ORN2L     : return maoi1_cost + or2_cost   + orn2_cost;
	  case MAOI1_OR2_ORN2R     : return maoi1_cost + or2_cost   + orn2_cost;
	  case MAOI1_NAND2_ANDN2L  : return maoi1_cost + nand2_cost + andn2_cost;
	  case MAOI1_NAND2_ANDN2R  : return maoi1_cost + nand2_cost + andn2_cost;
	  case MAOI1_NAND2_ORN2L   : return maoi1_cost + nand2_cost + orn2_cost;
	  case MAOI1_NAND2_ORN2R   : return maoi1_cost + nand2_cost + orn2_cost;
	  case MAOI1_NOR2_ANDN2L   : return maoi1_cost + nor2_cost  + andn2_cost;
	  case MAOI1_NOR2_ANDN2R   : return maoi1_cost + nor2_cost  + andn2_cost;
	  case MAOI1_NOR2_ORN2L    : return maoi1_cost + nor2_cost  + orn2_cost;
	  case MAOI1_NOR2_ORN2R    : return maoi1_cost + nor2_cost  + orn2_cost;
	  case MAOI1_ANDN2L_AND2   : return maoi1_cost + andn2_cost + and2_cost;
	  case MAOI1_ANDN2L_OR2    : return maoi1_cost + andn2_cost + or2_cost;
	  case MAOI1_ANDN2L_NAND2  : return maoi1_cost + andn2_cost + nand2_cost;
	  case MAOI1_ANDN2L_NOR2   : return maoi1_cost + andn2_cost + nor2_cost;
	  case MAOI1_ANDN2L_ANDN2L : return maoi1_cost + andn2_cost + andn2_cost;
	  case MAOI1_ANDN2L_ANDN2R : return maoi1_cost + andn2_cost + andn2_cost;
	  case MAOI1_ANDN2L_ORN2L  : return maoi1_cost + andn2_cost + orn2_cost;
	  case MAOI1_ANDN2L_ORN2R  : return maoi1_cost + andn2_cost + orn2_cost;
	  case MAOI1_ANDN2R_AND2   : return maoi1_cost + andn2_cost + and2_cost;
	  case MAOI1_ANDN2R_OR2    : return maoi1_cost + andn2_cost + or2_cost;
	  case MAOI1_ANDN2R_NAND2  : return maoi1_cost + andn2_cost + nand2_cost;
	  case MAOI1_ANDN2R_NOR2   : return maoi1_cost + andn2_cost + nor2_cost;
	  case MAOI1_ANDN2R_ANDN2L : return maoi1_cost + andn2_cost + andn2_cost;
	  case MAOI1_ANDN2R_ANDN2R : return maoi1_cost + andn2_cost + andn2_cost;
	  case MAOI1_ANDN2R_ORN2L  : return maoi1_cost + andn2_cost + orn2_cost;
	  case MAOI1_ANDN2R_ORN2R  : return maoi1_cost + andn2_cost + orn2_cost;
	  case MAOI1_ORN2L_AND2    : return maoi1_cost + orn2_cost  + and2_cost;
	  case MAOI1_ORN2L_OR2     : return maoi1_cost + orn2_cost  + or2_cost;
	  case MAOI1_ORN2L_NAND2   : return maoi1_cost + orn2_cost  + nand2_cost;
	  case MAOI1_ORN2L_NOR2    : return maoi1_cost + orn2_cost  + nor2_cost;
	  case MAOI1_ORN2L_ANDN2L  : return maoi1_cost + orn2_cost  + andn2_cost;
	  case MAOI1_ORN2L_ANDN2R  : return maoi1_cost + orn2_cost  + andn2_cost;
	  case MAOI1_ORN2L_ORN2L   : return maoi1_cost + orn2_cost  + orn2_cost;
	  case MAOI1_ORN2L_ORN2R   : return maoi1_cost + orn2_cost  + orn2_cost;
	  case MAOI1_ORN2R_AND2    : return maoi1_cost + orn2_cost  + and2_cost;
	  case MAOI1_ORN2R_OR2     : return maoi1_cost + orn2_cost  + or2_cost;
	  case MAOI1_ORN2R_NAND2   : return maoi1_cost + orn2_cost  + nand2_cost;
	  case MAOI1_ORN2R_NOR2    : return maoi1_cost + orn2_cost  + nor2_cost;
	  case MAOI1_ORN2R_ANDN2L  : return maoi1_cost + orn2_cost  + andn2_cost;
	  case MAOI1_ORN2R_ANDN2R  : return maoi1_cost + orn2_cost  + andn2_cost;
	  case MAOI1_ORN2R_ORN2L   : return maoi1_cost + orn2_cost  + orn2_cost;
	  case MAOI1_ORN2R_ORN2R   : return maoi1_cost + orn2_cost  + orn2_cost;

    case XOR2_AND2         : return xor2_cost + and2_cost;
    case XOR2_OR2          : return xor2_cost + or2_cost;
    case XOR2_ANDN2L       : return xor2_cost + andn2_cost;
    case XOR2_ANDN2R       : return xor2_cost + andn2_cost;
    case XOR2_ORN2L        : return xor2_cost + orn2_cost;
    case XOR2_ORN2R        : return xor2_cost + orn2_cost;

    case XOR2_AND2_AND2    : return xor2_cost + 2*and2_cost;
    case XOR2_AND2_OR2     : return xor2_cost + and2_cost + or2_cost;
    case XOR2_AND2_ANDN2L  : return xor2_cost + and2_cost + andn2_cost;
    case XOR2_AND2_ANDN2R  : return xor2_cost + and2_cost + andn2_cost;
    case XOR2_AND2_ORN2L   : return xor2_cost + and2_cost + orn2_cost;
    case XOR2_AND2_ORN2R   : return xor2_cost + and2_cost + orn2_cost;

    case XOR2_OR2_AND2     : return xor2_cost + and2_cost + or2_cost;
    case XOR2_OR2_OR2      : return xor2_cost + 2*or2_cost;
    case XOR2_OR2_ANDN2L   : return xor2_cost + andn2_cost + or2_cost;
    case XOR2_OR2_ANDN2R   : return xor2_cost + andn2_cost + or2_cost;
    case XOR2_OR2_ORN2L    : return xor2_cost + or2_cost + orn2_cost;
    case XOR2_OR2_ORN2R    : return xor2_cost + or2_cost + orn2_cost;

    case XOR2_ANDN2L_AND2    : return xor2_cost + andn2_cost + and2_cost;
    case XOR2_ANDN2R_AND2    : return xor2_cost + andn2_cost + and2_cost;
    case XOR2_ANDN2L_OR2     : return xor2_cost + andn2_cost + or2_cost;
    case XOR2_ANDN2R_OR2     : return xor2_cost + andn2_cost + or2_cost;
    case XOR2_ANDN2L_ANDN2L  : return xor2_cost + 2*andn2_cost;
    case XOR2_ANDN2L_ANDN2R  : return xor2_cost + 2*andn2_cost;
    case XOR2_ANDN2R_ANDN2L  : return xor2_cost + 2*andn2_cost;
    case XOR2_ANDN2R_ANDN2R  : return xor2_cost + 2*andn2_cost;
    case XOR2_ANDN2L_ORN2L   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ANDN2L_ORN2R   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ANDN2R_ORN2L   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ANDN2R_ORN2R   : return xor2_cost + andn2_cost + orn2_cost;

    case XOR2_ORN2L_AND2     : return xor2_cost + and2_cost + orn2_cost;
    case XOR2_ORN2R_AND2     : return xor2_cost + and2_cost + orn2_cost;
    case XOR2_ORN2L_OR2      : return xor2_cost + orn2_cost + or2_cost;
    case XOR2_ORN2R_OR2      : return xor2_cost + orn2_cost + or2_cost;
    case XOR2_ORN2L_ANDN2L   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ORN2L_ANDN2R   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ORN2R_ANDN2L   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ORN2R_ANDN2R   : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ORN2L_ORN2L    : return xor2_cost + 2*orn2_cost;
    case XOR2_ORN2L_ORN2R    : return xor2_cost + 2*orn2_cost;
    case XOR2_ORN2R_ORN2L    : return xor2_cost + 2*orn2_cost;
    case XOR2_ORN2R_ORN2R    : return xor2_cost + 2*orn2_cost;

    default                : return -1;
  }
}


template<int N>
void lighter<N>::init_b(string conf_file)
{
  ifstream input(conf_file);
  string key, value;
  for(string line; getline( input, line ); )
  {
    istringstream is_line(line);
    if(getline(is_line, key, '='))
    {
      if(getline(is_line, value))
      {
        if(key == "not1_cost") not1_cost = stod(value)*100;
        if(key == "and2_cost") and2_cost = stod(value)*100;
        if(key == "nand2_cost") nand2_cost = stod(value)*100;
        if(key == "or2_cost") or2_cost = stod(value)*100;
        if(key == "nor2_cost") nor2_cost = stod(value)*100;
        if(key == "xor2_cost") xor2_cost = stod(value)*100;
        if(key == "xnor2_cost") xnor2_cost = stod(value)*100;
        if(key == "maoi1_cost") maoi1_cost = stod(value)*100;
        if(key == "moai1_cost") moai1_cost = stod(value)*100;
        if (N >= 4)
        {
            if(key == "nand3_cost") nand3_cost = stod(value)*100;
            if(key == "nor3_cost") nor3_cost = stod(value)*100;
            if(key == "and3_cost") and3_cost = stod(value)*100;
            if(key == "or3_cost") or3_cost = stod(value)*100;
        }
        if(key == "andn2_cost") andn2_cost = stod(value)*100;
        if(key == "orn2_cost") orn2_cost = stod(value)*100;
      }
    }
  }

  /*
    initilisation of the set of bolean operations
  */
  if(gate_not1) b.push_back({NOT1, bool_op_cost(NOT1)});
  if(gate_xor2)
  {
    b.push_back({XOR2, bool_op_cost(XOR2)});
    if(gate_and2)
    {
      b.push_back({XOR2_AND2, bool_op_cost(XOR2_AND2)});
      if (N >= 4)
      {
          b.push_back({XOR2_AND2_AND2, bool_op_cost(XOR2_AND2_AND2)});
          if(gate_or2) b.push_back({XOR2_AND2_OR2, bool_op_cost(XOR2_AND2_OR2)});
          if(gate_andn2)
          {
            b.push_back({XOR2_AND2_ANDN2L, bool_op_cost(XOR2_AND2_ANDN2L)});
            b.push_back({XOR2_AND2_ANDN2R, bool_op_cost(XOR2_AND2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({XOR2_AND2_ORN2L, bool_op_cost(XOR2_AND2_ORN2L)});
            b.push_back({XOR2_AND2_ORN2R, bool_op_cost(XOR2_AND2_ORN2R)});
          }
      }
    }
    if(gate_or2)
    {
      b.push_back({XOR2_OR2, bool_op_cost(XOR2_OR2)});
      if (N >= 4)
      {
          b.push_back({XOR2_OR2_OR2, bool_op_cost(XOR2_OR2_OR2)});
          if(gate_and2) b.push_back({XOR2_OR2_AND2, bool_op_cost(XOR2_OR2_AND2)});
          if(gate_andn2) 
          {
            b.push_back({XOR2_OR2_ANDN2L, bool_op_cost(XOR2_OR2_ANDN2L)});
            b.push_back({XOR2_OR2_ANDN2R, bool_op_cost(XOR2_OR2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({XOR2_OR2_ORN2L, bool_op_cost(XOR2_OR2_ORN2L)});
            b.push_back({XOR2_OR2_ORN2R, bool_op_cost(XOR2_OR2_ORN2R)});
          }
      }
    }
    if(gate_andn2)
    {
      b.push_back({XOR2_ANDN2L, bool_op_cost(XOR2_ANDN2L)});
      b.push_back({XOR2_ANDN2R, bool_op_cost(XOR2_ANDN2R)});
      if (N >= 4)
      {
          b.push_back({XOR2_ANDN2L_ANDN2L, bool_op_cost(XOR2_ANDN2L_ANDN2L)});
          b.push_back({XOR2_ANDN2L_ANDN2R, bool_op_cost(XOR2_ANDN2L_ANDN2R)});
          b.push_back({XOR2_ANDN2R_ANDN2L, bool_op_cost(XOR2_ANDN2R_ANDN2L)});
          b.push_back({XOR2_ANDN2R_ANDN2R, bool_op_cost(XOR2_ANDN2R_ANDN2R)});

          if(gate_or2)
          {
            b.push_back({XOR2_ANDN2L_OR2, bool_op_cost(XOR2_ANDN2L_OR2)});
            b.push_back({XOR2_ANDN2R_OR2, bool_op_cost(XOR2_ANDN2R_OR2)});
          }
          if(gate_and2)
          {
            b.push_back({XOR2_ANDN2L_AND2, bool_op_cost(XOR2_ANDN2L_AND2)});
            b.push_back({XOR2_ANDN2R_AND2, bool_op_cost(XOR2_ANDN2R_AND2)});
          }
          if(gate_orn2)
          {
            b.push_back({XOR2_ANDN2L_ORN2L, bool_op_cost(XOR2_ANDN2L_ORN2L)});
            b.push_back({XOR2_ANDN2L_ORN2R, bool_op_cost(XOR2_ANDN2L_ORN2R)});
            b.push_back({XOR2_ANDN2R_ORN2L, bool_op_cost(XOR2_ANDN2R_ORN2L)});
            b.push_back({XOR2_ANDN2R_ORN2R, bool_op_cost(XOR2_ANDN2R_ORN2R)});
          }       
      }
    }
    if(gate_orn2)
    {
      b.push_back({XOR2_ORN2L, bool_op_cost(XOR2_ORN2L)});
      b.push_back({XOR2_ORN2R, bool_op_cost(XOR2_ORN2R)});
      if (N >= 4)
      {
          b.push_back({XOR2_ORN2L_ORN2L, bool_op_cost(XOR2_ORN2L_ORN2L)});
          b.push_back({XOR2_ORN2L_ORN2R, bool_op_cost(XOR2_ORN2L_ORN2R)});
          b.push_back({XOR2_ORN2R_ORN2L, bool_op_cost(XOR2_ORN2R_ORN2L)});
          b.push_back({XOR2_ORN2R_ORN2R, bool_op_cost(XOR2_ORN2R_ORN2R)});

          if(gate_and2)
          {
            b.push_back({XOR2_ORN2L_AND2, bool_op_cost(XOR2_ORN2L_AND2)});
            b.push_back({XOR2_ORN2R_AND2, bool_op_cost(XOR2_ORN2R_AND2)});
          }
          if(gate_or2)
          {
            b.push_back({XOR2_ORN2L_OR2, bool_op_cost(XOR2_ORN2L_OR2)});
            b.push_back({XOR2_ORN2R_OR2, bool_op_cost(XOR2_ORN2R_OR2)});
          }       
          if(gate_andn2)
          {
            b.push_back({XOR2_ORN2L_ANDN2L, bool_op_cost(XOR2_ORN2L_ANDN2L)});
            b.push_back({XOR2_ORN2L_ANDN2R, bool_op_cost(XOR2_ORN2L_ANDN2R)});
            b.push_back({XOR2_ORN2R_ANDN2L, bool_op_cost(XOR2_ORN2R_ANDN2L)});
            b.push_back({XOR2_ORN2R_ANDN2R, bool_op_cost(XOR2_ORN2R_ANDN2R)});
          }
      }
    }
  }
  if(gate_xnor2) b.push_back({XNOR2, bool_op_cost(XNOR2)});

  if(gate_maoi1)
  {
    b.push_back({MAOI1, bool_op_cost(MAOI1)});
    
    if (N >= 4)
    {
        if(gate_and3) b.push_back({MAOI1_AND3, bool_op_cost(MAOI1_AND3)});
        if(gate_or3) b.push_back({MAOI1_OR3, bool_op_cost(MAOI1_OR3)});
        if(gate_nor3) b.push_back({MAOI1_NOR3, bool_op_cost(MAOI1_NOR3)});
        if(gate_nand3) b.push_back({MAOI1_NAND3, bool_op_cost(MAOI1_NAND3)});
    }

    if(gate_nand2)
    {
      b.push_back({MAOI1_NAND2, bool_op_cost(MAOI1_NAND2)});
      if (N >= 4)
      {
          b.push_back({MAOI1_NAND2_NAND2, bool_op_cost(MAOI1_NAND2_NAND2)});
          if(gate_nor2) b.push_back({MAOI1_NAND2_NOR2, bool_op_cost(MAOI1_NAND2_NOR2)});
          if(gate_and2) b.push_back({MAOI1_NAND2_AND2, bool_op_cost(MAOI1_NAND2_AND2)});
          if(gate_or2) b.push_back({MAOI1_NAND2_OR2, bool_op_cost(MAOI1_NAND2_OR2)});
          if(gate_andn2)
          {
            b.push_back({MAOI1_NAND2_ANDN2L, bool_op_cost(MAOI1_NAND2_ANDN2L)});
            b.push_back({MAOI1_NAND2_ANDN2R, bool_op_cost(MAOI1_NAND2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MAOI1_NAND2_ORN2L, bool_op_cost(MAOI1_NAND2_ORN2L)});
            b.push_back({MAOI1_NAND2_ORN2R, bool_op_cost(MAOI1_NAND2_ORN2R)});
          }
      }
    }
    if(gate_nor2)
    {
      b.push_back({MAOI1_NOR2, bool_op_cost(MAOI1_NOR2)});
      if (N >= 4)
      {
          b.push_back({MAOI1_NOR2_NOR2, bool_op_cost(MAOI1_NOR2_NOR2)});
          if(gate_and2) b.push_back({MAOI1_NOR2_AND2, bool_op_cost(MAOI1_NOR2_AND2)});
          if(gate_or2) b.push_back({MAOI1_NOR2_OR2, bool_op_cost(MAOI1_NOR2_OR2)});
          if(gate_nand2) b.push_back({MAOI1_NOR2_NAND2, bool_op_cost(MAOI1_NOR2_NAND2)});
          if(gate_andn2)
          {
            b.push_back({MAOI1_NOR2_ANDN2L, bool_op_cost(MAOI1_NOR2_ANDN2L)});
            b.push_back({MAOI1_NOR2_ANDN2R, bool_op_cost(MAOI1_NOR2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MAOI1_NOR2_ORN2L, bool_op_cost(MAOI1_NOR2_ORN2L)});
            b.push_back({MAOI1_NOR2_ORN2R, bool_op_cost(MAOI1_NOR2_ORN2R)});
          }        
      }
    }
    if(gate_and2)
    {
      b.push_back({MAOI1_AND2, bool_op_cost(MAOI1_AND2)});
      if (N >= 4)
      {
          b.push_back({MAOI1_AND2_AND2, bool_op_cost(MAOI1_AND2_AND2)});
          if(gate_nand2) b.push_back({MAOI1_AND2_NAND2, bool_op_cost(MAOI1_AND2_NAND2)});
          if(gate_nor2) b.push_back({MAOI1_AND2_NOR2, bool_op_cost(MAOI1_AND2_NOR2)});
          if(gate_or2) b.push_back({MAOI1_AND2_OR2, bool_op_cost(MAOI1_AND2_OR2)});        
          if(gate_andn2)
          {
            b.push_back({MAOI1_AND2_ANDN2L, bool_op_cost(MAOI1_AND2_ANDN2L)});
            b.push_back({MAOI1_AND2_ANDN2R, bool_op_cost(MAOI1_AND2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MAOI1_AND2_ORN2L, bool_op_cost(MAOI1_AND2_ORN2L)});
            b.push_back({MAOI1_AND2_ORN2R, bool_op_cost(MAOI1_AND2_ORN2R)});
          }        
      }
    }
    if(gate_or2)
    {
      b.push_back({MAOI1_OR2, bool_op_cost(MAOI1_OR2)});
      if (N >= 4)
      {
          b.push_back({MAOI1_OR2_OR2, bool_op_cost(MAOI1_OR2_OR2)});
          if(gate_and2) b.push_back({MAOI1_OR2_AND2, bool_op_cost(MAOI1_OR2_AND2)});
          if(gate_nor2) b.push_back({MAOI1_OR2_NOR2, bool_op_cost(MAOI1_OR2_NOR2)});
          if(gate_nand2) b.push_back({MAOI1_OR2_NAND2, bool_op_cost(MAOI1_OR2_NAND2)});
          if(gate_andn2)
          {
            b.push_back({MAOI1_OR2_ANDN2L, bool_op_cost(MAOI1_OR2_ANDN2L)});
            b.push_back({MAOI1_OR2_ANDN2R, bool_op_cost(MAOI1_OR2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MAOI1_OR2_ORN2L, bool_op_cost(MAOI1_OR2_ORN2L)});
            b.push_back({MAOI1_OR2_ORN2R, bool_op_cost(MAOI1_OR2_ORN2R)});
          }        
      }
    }
    if(gate_andn2)
    {
      b.push_back({MAOI1_ANDN2L, bool_op_cost(MAOI1_ANDN2L)});
      b.push_back({MAOI1_ANDN2R, bool_op_cost(MAOI1_ANDN2R)});
      if (N >= 4)
      {
          b.push_back({MAOI1_ANDN2L_ANDN2L, bool_op_cost(MAOI1_ANDN2L_ANDN2L)});
          b.push_back({MAOI1_ANDN2L_ANDN2R, bool_op_cost(MAOI1_ANDN2L_ANDN2R)});
          b.push_back({MAOI1_ANDN2R_ANDN2L, bool_op_cost(MAOI1_ANDN2R_ANDN2L)});
          b.push_back({MAOI1_ANDN2R_ANDN2R, bool_op_cost(MAOI1_ANDN2R_ANDN2R)});

          if(gate_nor2)
          {
            b.push_back({MAOI1_ANDN2L_NOR2, bool_op_cost(MAOI1_ANDN2L_NOR2)});
            b.push_back({MAOI1_ANDN2R_NOR2, bool_op_cost(MAOI1_ANDN2R_NOR2)});
          }
          if(gate_nand2)
          {
            b.push_back({MAOI1_ANDN2L_NAND2, bool_op_cost(MAOI1_ANDN2L_NAND2)});
            b.push_back({MAOI1_ANDN2R_NAND2, bool_op_cost(MAOI1_ANDN2R_NAND2)});
          }
          if(gate_or2)
          {
            b.push_back({MAOI1_ANDN2L_OR2, bool_op_cost(MAOI1_ANDN2L_OR2)});
            b.push_back({MAOI1_ANDN2R_OR2, bool_op_cost(MAOI1_ANDN2R_OR2)});
          }
          if(gate_and2)
          {
            b.push_back({MAOI1_ANDN2L_AND2, bool_op_cost(MAOI1_ANDN2L_AND2)});
            b.push_back({MAOI1_ANDN2R_AND2, bool_op_cost(MAOI1_ANDN2R_AND2)});
          }
          if(gate_orn2)
          {
            b.push_back({MAOI1_ANDN2L_ORN2L, bool_op_cost(MAOI1_ANDN2L_ORN2L)});
            b.push_back({MAOI1_ANDN2L_ORN2R, bool_op_cost(MAOI1_ANDN2L_ORN2R)});
            b.push_back({MAOI1_ANDN2R_ORN2L, bool_op_cost(MAOI1_ANDN2R_ORN2L)});
            b.push_back({MAOI1_ANDN2R_ORN2R, bool_op_cost(MAOI1_ANDN2R_ORN2R)});
          }       
      }
    }    
    if(gate_orn2)
    {
      b.push_back({MAOI1_ORN2L, bool_op_cost(MAOI1_ORN2L)});
      b.push_back({MAOI1_ORN2R, bool_op_cost(MAOI1_ORN2R)});
      if (N >= 4)
      {
          b.push_back({MAOI1_ORN2L_ORN2L, bool_op_cost(MAOI1_ORN2L_ORN2L)});
          b.push_back({MAOI1_ORN2L_ORN2R, bool_op_cost(MAOI1_ORN2L_ORN2R)});
          b.push_back({MAOI1_ORN2R_ORN2L, bool_op_cost(MAOI1_ORN2R_ORN2L)});
          b.push_back({MAOI1_ORN2R_ORN2R, bool_op_cost(MAOI1_ORN2R_ORN2R)});

          if(gate_nor2)
          {
            b.push_back({MAOI1_ORN2L_NOR2, bool_op_cost(MAOI1_ORN2L_NOR2)});
            b.push_back({MAOI1_ORN2R_NOR2, bool_op_cost(MAOI1_ORN2R_NOR2)});
          }
          if(gate_nand2)
          {
            b.push_back({MAOI1_ORN2L_NAND2, bool_op_cost(MAOI1_ORN2L_NAND2)});
            b.push_back({MAOI1_ORN2R_NAND2, bool_op_cost(MAOI1_ORN2R_NAND2)});
          }
          if(gate_or2)
          {
            b.push_back({MAOI1_ORN2L_OR2, bool_op_cost(MAOI1_ORN2L_OR2)});
            b.push_back({MAOI1_ORN2R_OR2, bool_op_cost(MAOI1_ORN2R_OR2)});
          }
          if(gate_and2)
          {
            b.push_back({MAOI1_ORN2L_AND2, bool_op_cost(MAOI1_ORN2L_AND2)});
            b.push_back({MAOI1_ORN2R_AND2, bool_op_cost(MAOI1_ORN2R_AND2)});
          }
          if(gate_andn2)
          {
            b.push_back({MAOI1_ORN2L_ANDN2L, bool_op_cost(MAOI1_ORN2L_ANDN2L)});
            b.push_back({MAOI1_ORN2L_ANDN2R, bool_op_cost(MAOI1_ORN2L_ANDN2R)});
            b.push_back({MAOI1_ORN2R_ANDN2L, bool_op_cost(MAOI1_ORN2R_ANDN2L)});
            b.push_back({MAOI1_ORN2R_ANDN2R, bool_op_cost(MAOI1_ORN2R_ANDN2R)});
          }       
      }
    }    
  }
  if(gate_moai1)
  {
    b.push_back({MOAI1, bool_op_cost(MOAI1)});
    if (N >= 4)
    {
        if(gate_and3) b.push_back({MOAI1_AND3, bool_op_cost(MOAI1_AND3)});
        if(gate_or3) b.push_back({MOAI1_OR3, bool_op_cost(MOAI1_OR3)});
        if(gate_nor3) b.push_back({MOAI1_NOR3, bool_op_cost(MOAI1_NOR3)});
        if(gate_nand3) b.push_back({MOAI1_NAND3, bool_op_cost(MOAI1_NAND3)});      
    }
    if(gate_nand2)
    {
      b.push_back({MOAI1_NAND2, bool_op_cost(MOAI1_NAND2)});
      if (N >= 4)
      {
          b.push_back({MOAI1_NAND2_NAND2, bool_op_cost(MOAI1_NAND2_NAND2)});
          if(gate_nor2) b.push_back({MOAI1_NAND2_NOR2, bool_op_cost(MOAI1_NAND2_NOR2)});        
          if(gate_and2) b.push_back({MOAI1_NAND2_AND2, bool_op_cost(MOAI1_NAND2_AND2)});
          if(gate_or2) b.push_back({MOAI1_NAND2_OR2, bool_op_cost(MOAI1_NAND2_OR2)});
          if(gate_andn2)
          {
            b.push_back({MOAI1_NAND2_ANDN2L, bool_op_cost(MOAI1_NAND2_ANDN2L)});
            b.push_back({MOAI1_NAND2_ANDN2R, bool_op_cost(MOAI1_NAND2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MOAI1_NAND2_ORN2L, bool_op_cost(MOAI1_NAND2_ORN2L)});
            b.push_back({MOAI1_NAND2_ORN2R, bool_op_cost(MOAI1_NAND2_ORN2R)});
          }
      }
    }
    if(gate_nor2)
    {
      b.push_back({MOAI1_NOR2, bool_op_cost(MOAI1_NOR2)});
      if (N >= 4)
      {
          b.push_back({MOAI1_NOR2_NOR2, bool_op_cost(MOAI1_NOR2_NOR2)});
          if(gate_nand2) b.push_back({MOAI1_NOR2_NAND2, bool_op_cost(MOAI1_NOR2_NAND2)});        
          if(gate_and2) b.push_back({MOAI1_NOR2_AND2, bool_op_cost(MOAI1_NOR2_AND2)});
          if(gate_or2) b.push_back({MOAI1_NOR2_OR2, bool_op_cost(MOAI1_NOR2_OR2)});
          if(gate_andn2)
          {
            b.push_back({MOAI1_NOR2_ANDN2L, bool_op_cost(MOAI1_NOR2_ANDN2L)});
            b.push_back({MOAI1_NOR2_ANDN2R, bool_op_cost(MOAI1_NOR2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MOAI1_NOR2_ORN2L, bool_op_cost(MOAI1_NOR2_ORN2L)});
            b.push_back({MOAI1_NOR2_ORN2R, bool_op_cost(MOAI1_NOR2_ORN2R)});
          }        
      }
    }
    if(gate_and2)
    {
      b.push_back({MOAI1_AND2, bool_op_cost(MOAI1_AND2)});
      if (N >= 4)
      {
          b.push_back({MOAI1_AND2_AND2, bool_op_cost(MOAI1_AND2_AND2)});
          if(gate_nand2) b.push_back({MOAI1_AND2_NAND2, bool_op_cost(MOAI1_AND2_NAND2)});
          if(gate_nor2) b.push_back({MOAI1_AND2_NOR2, bool_op_cost(MOAI1_AND2_NOR2)});        
          if(gate_or2) b.push_back({MOAI1_AND2_OR2, bool_op_cost(MOAI1_AND2_OR2)});
          if(gate_andn2)
          {
            b.push_back({MOAI1_AND2_ANDN2L, bool_op_cost(MOAI1_AND2_ANDN2L)});
            b.push_back({MOAI1_AND2_ANDN2R, bool_op_cost(MOAI1_AND2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MOAI1_AND2_ORN2L, bool_op_cost(MOAI1_AND2_ORN2L)});
            b.push_back({MOAI1_AND2_ORN2R, bool_op_cost(MOAI1_AND2_ORN2R)});
          }        
      }
    }
    if(gate_or2)
    {
      b.push_back({MOAI1_OR2, bool_op_cost(MOAI1_OR2)});
      if (N >= 4)
      {
          b.push_back({MOAI1_OR2_OR2, bool_op_cost(MOAI1_OR2_OR2)});
          if(gate_nand2) b.push_back({MOAI1_OR2_NAND2, bool_op_cost(MOAI1_OR2_NAND2)});        
          if(gate_nor2) b.push_back({MOAI1_OR2_NOR2, bool_op_cost(MOAI1_OR2_NOR2)});
          if(gate_and2) b.push_back({MOAI1_OR2_AND2, bool_op_cost(MOAI1_OR2_AND2)});
          if(gate_andn2)
          {
            b.push_back({MOAI1_OR2_ANDN2L, bool_op_cost(MOAI1_OR2_ANDN2L)});
            b.push_back({MOAI1_OR2_ANDN2R, bool_op_cost(MOAI1_OR2_ANDN2R)});
          }
          if(gate_orn2)
          {
            b.push_back({MOAI1_OR2_ORN2L, bool_op_cost(MOAI1_OR2_ORN2L)});
            b.push_back({MOAI1_OR2_ORN2R, bool_op_cost(MOAI1_OR2_ORN2R)});
          }        
      }
    }
    if(gate_andn2)
    {
      b.push_back({MOAI1_ANDN2L, bool_op_cost(MOAI1_ANDN2L)});
      b.push_back({MOAI1_ANDN2R, bool_op_cost(MOAI1_ANDN2R)});
      if (N >= 4)
      {
          b.push_back({MOAI1_ANDN2L_ANDN2L, bool_op_cost(MOAI1_ANDN2L_ANDN2L)});
          b.push_back({MOAI1_ANDN2L_ANDN2R, bool_op_cost(MOAI1_ANDN2L_ANDN2R)});
          b.push_back({MOAI1_ANDN2R_ANDN2L, bool_op_cost(MOAI1_ANDN2R_ANDN2L)});
          b.push_back({MOAI1_ANDN2R_ANDN2R, bool_op_cost(MOAI1_ANDN2R_ANDN2R)});

          if(gate_nor2)
          {
            b.push_back({MOAI1_ANDN2L_NOR2, bool_op_cost(MOAI1_ANDN2L_NOR2)});
            b.push_back({MOAI1_ANDN2R_NOR2, bool_op_cost(MOAI1_ANDN2R_NOR2)});
          }
          if(gate_nand2)
          {
            b.push_back({MOAI1_ANDN2L_NAND2, bool_op_cost(MOAI1_ANDN2L_NAND2)});
            b.push_back({MOAI1_ANDN2R_NAND2, bool_op_cost(MOAI1_ANDN2R_NAND2)});
          }
          if(gate_or2)
          {
            b.push_back({MOAI1_ANDN2L_OR2, bool_op_cost(MOAI1_ANDN2L_OR2)});
            b.push_back({MOAI1_ANDN2R_OR2, bool_op_cost(MOAI1_ANDN2R_OR2)});
          }
          if(gate_and2)
          {
            b.push_back({MOAI1_ANDN2L_AND2, bool_op_cost(MOAI1_ANDN2L_AND2)});
            b.push_back({MOAI1_ANDN2R_AND2, bool_op_cost(MOAI1_ANDN2R_AND2)});
          }
          if(gate_orn2)
          {
            b.push_back({MOAI1_ANDN2L_ORN2L, bool_op_cost(MOAI1_ANDN2L_ORN2L)});
            b.push_back({MOAI1_ANDN2L_ORN2R, bool_op_cost(MOAI1_ANDN2L_ORN2R)});
            b.push_back({MOAI1_ANDN2R_ORN2L, bool_op_cost(MOAI1_ANDN2R_ORN2L)});
            b.push_back({MOAI1_ANDN2R_ORN2R, bool_op_cost(MOAI1_ANDN2R_ORN2R)});
          }       
      }
    }    
    if(gate_orn2)
    {
      b.push_back({MOAI1_ORN2L, bool_op_cost(MOAI1_ORN2L)});
      b.push_back({MOAI1_ORN2R, bool_op_cost(MOAI1_ORN2R)});
      if (N >= 4)
      {
          b.push_back({MOAI1_ORN2L_ORN2L, bool_op_cost(MOAI1_ORN2L_ORN2L)});
          b.push_back({MOAI1_ORN2L_ORN2R, bool_op_cost(MOAI1_ORN2L_ORN2R)});
          b.push_back({MOAI1_ORN2R_ORN2L, bool_op_cost(MOAI1_ORN2R_ORN2L)});
          b.push_back({MOAI1_ORN2R_ORN2R, bool_op_cost(MOAI1_ORN2R_ORN2R)});

          if(gate_nor2)
          {
            b.push_back({MOAI1_ORN2L_NOR2, bool_op_cost(MOAI1_ORN2L_NOR2)});
            b.push_back({MOAI1_ORN2R_NOR2, bool_op_cost(MOAI1_ORN2R_NOR2)});
          }
          if(gate_nand2)
          {
            b.push_back({MOAI1_ORN2L_NAND2, bool_op_cost(MOAI1_ORN2L_NAND2)});
            b.push_back({MOAI1_ORN2R_NAND2, bool_op_cost(MOAI1_ORN2R_NAND2)});
          }
          if(gate_or2)
          {
            b.push_back({MOAI1_ORN2L_OR2, bool_op_cost(MOAI1_ORN2L_OR2)});
            b.push_back({MOAI1_ORN2R_OR2, bool_op_cost(MOAI1_ORN2R_OR2)});
          }
          if(gate_and2)
          {
            b.push_back({MOAI1_ORN2L_AND2, bool_op_cost(MOAI1_ORN2L_AND2)});
            b.push_back({MOAI1_ORN2R_AND2, bool_op_cost(MOAI1_ORN2R_AND2)});
          }
          if(gate_andn2)
          {
            b.push_back({MOAI1_ORN2L_ANDN2L, bool_op_cost(MOAI1_ORN2L_ANDN2L)});
            b.push_back({MOAI1_ORN2L_ANDN2R, bool_op_cost(MOAI1_ORN2L_ANDN2R)});
            b.push_back({MOAI1_ORN2R_ANDN2L, bool_op_cost(MOAI1_ORN2R_ANDN2L)});
            b.push_back({MOAI1_ORN2R_ANDN2R, bool_op_cost(MOAI1_ORN2R_ANDN2R)});
          }       
      }
    }    
  }

  sort(b.begin(), b.end());

  b.erase(remove_if(b.begin(), b.end(),
      [this](bool_op_t bool_op) -> bool
      {
        const function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
        vector<function_t<N> > f;
        bool_op_fun(bool_op.op_id, f1, &f);
        sort(f.begin(), f.end());
        auto check = check_tt.insert(f);
        return !check.second;
      }), b.end());
      
  check_tt.clear();

  for(auto bool_op : b)
  {
    if(bool_op.op_cost > max_GE) max_GE = bool_op.op_cost;
    if(bool_op.op_cost < min_GE) min_GE = bool_op.op_cost;
  }
}

template<int N>
void lighter<N>::init_critiera(string criteria_file)
{
  ifstream input(criteria_file);
  string key, value;
  for(string line; getline( input, line ); )
  {
    istringstream is_line(line);
    if(getline(is_line, key, '='))
    {
      if(getline(is_line, value))
      {
        if(key == "Cost") Cost_Criteria = stod(value)*100;
        if(key == "Involution") Involution_Criteria = stoi(value);
        if(key == "Diff") Diff_Criteria = stoi(value);
        if(key == "DiffFreq") DiffFreq_Criteria = stoi(value);
        if(key == "Diff1") Diff1_Criteria = stoi(value);
        if(key == "CardD1") CardD1_Criteria = stoi(value);
        if(key == "Lin") Lin_Criteria = stoi(value);
        if(key == "LinFreq") LinFreq_Criteria = stoi(value);
        if(key == "Lin1") Lin1_Criteria = stoi(value);
        if(key == "CardL1") CardL1_Criteria = stoi(value);
        if(key == "MaxDeg") MaxDeg_Criteria = stoi(value);
        if(key == "MinDeg") MinDeg_Criteria = stoi(value);
        if(key == "MaxDegFreq") MaxDegFreq_Criteria = stoi(value);
        if(key == "MinDegFreq") MinDegFreq_Criteria = stoi(value);
      }
    }
  }
}

template<int N>
void lighter<N>::bool_op_simple_gates(function_t<N> f, vector<function_t<N> > *v, int operation)
{
  function_t<N> fun_tmp = f;

  if(operation == NOT1)
  {
    for (int i = 0; i < N; i++)
    {
      fun_tmp.bit_slice[i] = ~(f.bit_slice[i]);
      fun_tmp.info_line = (uint8_t)i;
      fun_tmp.info_op = (uint8_t)operation;
      fun_tmp.prev = f.bit_slice[i];
      fun_tmp.sort();
      v->push_back(fun_tmp);
      fun_tmp = f;
    }
  }
  else
  {
    for(int i = 0; i < N; i++)
    {
      for(int j = 0; j < N; j++)
      {
        if(i != j)
        {
          switch (operation)
          {
            case MAOI1 :
            case XOR2  : fun_tmp.bit_slice[i] =   f.bit_slice[i] ^ f.bit_slice[j]; break;
            case MOAI1 :
            case XNOR2 : fun_tmp.bit_slice[i] = ~(f.bit_slice[i] ^ f.bit_slice[j]); break;
          }
          fun_tmp.prev = f.bit_slice[i];
          fun_tmp.info_line = (uint8_t)i;
          fun_tmp.info_op = (uint8_t)operation;
          fun_tmp.sort();
          v->push_back(fun_tmp);
          fun_tmp = f;
        }
      }
      fun_tmp = f;
    }
  }
}

template<int N>
void lighter<N>::bool_op_double_gates(function_t<N> f, vector<function_t<N> > *v, int operation)
{
  auto two_inputs = [](const function_t<N> f, int i, int a, int b, int operation)
  {
    auto fun_tmp = f;
    switch (operation)
    {
      case MAOI1_NAND2  :
      case MOAI1_AND2   : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N] & f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;
      case MAOI1_NOR2   :    
      case MOAI1_OR2    : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N] | f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;
      case XOR2_AND2    :    
      case MAOI1_AND2   :    
      case MOAI1_NAND2  : fun_tmp.bit_slice[i] =  (f.bit_slice[(i+a)%N] & f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;
      case XOR2_OR2     :
      case MAOI1_OR2    :
      case MOAI1_NOR2   : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] | f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;      

      case MOAI1_ORN2R  :
      case MAOI1_ANDN2L :
      case XOR2_ANDN2L  : fun_tmp.bit_slice[i] = (~f.bit_slice[(i+a)%N] & f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;

      case MOAI1_ORN2L  :
      case MAOI1_ANDN2R :
      case XOR2_ANDN2R  : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] &~f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;

      case MOAI1_ANDN2R :
      case MAOI1_ORN2L  :
      case XOR2_ORN2L   : fun_tmp.bit_slice[i] = (~f.bit_slice[(i+a)%N] | f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;

      case MOAI1_ANDN2L :
      case MAOI1_ORN2R  :
      case XOR2_ORN2R   : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] |~f.bit_slice[(i+b)%N]) ^ f.bit_slice[i];break;
   }
    fun_tmp.prev = f.bit_slice[i];
    fun_tmp.info_line = i;
    fun_tmp.info_op = operation;
    fun_tmp.sort();
    return fun_tmp;
  };

  auto three_inputs = [](const function_t<N> f, int i, int a, int b, int c, int operation)
  {
    auto fun_tmp = f;
    switch (operation)
    {
      case XOR2_AND2_AND2     :
      case MAOI1_AND3         :
      case MOAI1_NAND2_AND2   :
      case MAOI1_AND2_AND2    :
      case MOAI1_ORN2L_NAND2  :
      case MAOI1_ANDN2R_NAND2 :
      case MOAI1_NAND3        : fun_tmp.bit_slice[i] =  (f.bit_slice[(i+a)%N] & f.bit_slice[(i+b)%N] & f.bit_slice[(i+c)%N]) ^ f.bit_slice[i];break;
      
      case XOR2_OR2_OR2       :      
      case MAOI1_OR3          :      
      case MOAI1_NOR2_OR2     :      
      case MAOI1_OR2_OR2      : 
      case MOAI1_ANDN2L_NOR2  :  
      case MAOI1_ORN2R_NOR2   :
      case MOAI1_NOR3         : fun_tmp.bit_slice[i] =  (f.bit_slice[(i+a)%N] | f.bit_slice[(i+b)%N] | f.bit_slice[(i+c)%N]) ^ f.bit_slice[i];break;
      
      case MOAI1_AND3         :      
      case MAOI1_NAND2_AND2   :      
      case MOAI1_AND2_AND2    :
      case MOAI1_ANDN2R_NAND2 :
      case MAOI1_ORN2L_NAND2  :      
      case MAOI1_NAND3        : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N] & f.bit_slice[(i+b)%N] & f.bit_slice[(i+c)%N]) ^ f.bit_slice[i];break;
      
      case MOAI1_OR3          :      
      case MAOI1_NOR2_OR2     :      
      case MOAI1_OR2_OR2      :      
      case MAOI1_ANDN2L_NOR2  :
      case MOAI1_ORN2R_NOR2   : 
      case MAOI1_NOR3         : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N] | f.bit_slice[(i+b)%N] | f.bit_slice[(i+c)%N]) ^ f.bit_slice[i];break;
    }
    fun_tmp.prev = f.bit_slice[i];
    fun_tmp.info_line = (uint8_t)i;
    fun_tmp.info_op = (uint8_t)operation;
    fun_tmp.sort();
    return fun_tmp;
  };

  switch (operation)
  {
    case MAOI1_AND3        :
    case MOAI1_NAND2_AND2  :
    case MAOI1_AND2_AND2   :
    case MAOI1_OR3         :
    case MOAI1_NOR2_OR2    :
    case MAOI1_OR2_OR2     :
    case MOAI1_AND3        :
    case MAOI1_NAND2_AND2  :
    case MOAI1_AND2_AND2   :
    case MOAI1_OR3         :
    case MAOI1_NOR2_OR2    :
    case MOAI1_OR2_OR2     :
    case MOAI1_NAND3       :
    case MOAI1_NOR3        :
    case MAOI1_NAND3       :
    case MAOI1_NOR3        : 
    case XOR2_AND2_AND2    :
    case XOR2_OR2_OR2      :
    case MOAI1_ANDN2L_NOR2 :  
    case MAOI1_ORN2R_NOR2  :     
    case MOAI1_ORN2R_NOR2  :  
    case MAOI1_ANDN2L_NOR2 :  
    case MOAI1_ORN2L_NAND2 :
    case MAOI1_ANDN2R_NAND2:
    case MOAI1_ANDN2R_NAND2:
    case MAOI1_ORN2L_NAND2 :
                           for (int i = 0; i < N; i++)
                           {
                             for (int j = 1; j < N - 2; j++)
                             {
                               for (int k = j + 1; k < N - 1; k++)
                               {
                                 for (int u = k + 1; u < N; u++)
                                 {
                                    v->push_back(three_inputs(f, i, j, k, u, operation));
                                 }
                               }
                             }
                           }
                           break;
    case MAOI1_NAND2  :
    case MAOI1_NOR2   :
    case MAOI1_AND2   :
    case MAOI1_OR2    :
    case MOAI1_NAND2  :
    case MOAI1_NOR2   :
    case MOAI1_AND2   :
    case MOAI1_OR2    :
    case XOR2_AND2    :
    case XOR2_OR2     :
    case MAOI1_ANDN2L :
    case MAOI1_ANDN2R :
    case MAOI1_ORN2L  :
    case MAOI1_ORN2R  :
    case MOAI1_ANDN2L :
    case MOAI1_ANDN2R :
    case MOAI1_ORN2L  :
    case MOAI1_ORN2R  :
    case XOR2_ANDN2L  :
    case XOR2_ANDN2R  :
    case XOR2_ORN2L   :
    case XOR2_ORN2R   :
                       for (int i = 0; i < N; i++)
                       {
                         for (int j = 1; j < N - 1; j++)
                         {
                           for (int k = j + 1; k < N; k++)
                           {
                              v->push_back(two_inputs(f, i, j, k, operation));
                           }
                         }
                       }
                       break;
  }
}

template<int N>
void lighter<N>::bool_op_triple_gates(function_t<N> f, vector<function_t<N> > *v, int operation)
{
  auto triple_gates = [](const function_t<N> f, int i, int a, int b, int c, int operation)
  {
    auto fun_tmp = f;
    switch (operation)
    {
      case MOAI1_NAND2_OR2      :
      case MAOI1_AND2_OR2       :
      case MOAI1_ORN2L_NOR2     :
      case MAOI1_ANDN2R_NOR2    :
      case XOR2_AND2_OR2        : fun_tmp.bit_slice[i] = ((f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;

      case MOAI1_NOR2_AND2      :
      case MAOI1_OR2_AND2       :
      case MOAI1_ANDN2L_NAND2   :
      case MAOI1_ORN2R_NAND2    :
      case XOR2_OR2_AND2        : fun_tmp.bit_slice[i] = ((f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;

      case MOAI1_ANDN2R_NOR2   :
      case MAOI1_ORN2L_NOR2    :
      case MAOI1_NAND2_OR2     :
      case MOAI1_AND2_OR2      : fun_tmp.bit_slice[i] = ~((f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;

      case MOAI1_OR2_AND2       :
      case MAOI1_NOR2_AND2      :
      case MOAI1_ORN2R_NAND2    :
      case MAOI1_ANDN2L_NAND2   : fun_tmp.bit_slice[i] = ~((f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;

      case MOAI1_NAND2_ANDN2R   :
      case MOAI1_ORN2L_ORN2L    :
      case MAOI1_AND2_ANDN2R    :
      case MAOI1_ANDN2R_ORN2L   :
      case XOR2_AND2_ANDN2R     :
      case XOR2_ANDN2R_ORN2L    : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] &  ( f.bit_slice[(i+b)%N] & ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MOAI1_NAND2_ORN2R    :
      case MOAI1_ORN2L_ANDN2L   :
      case MAOI1_AND2_ORN2R     :
      case MAOI1_ANDN2R_ANDN2L  :
      case XOR2_AND2_ORN2R      :
      case XOR2_ANDN2R_ANDN2L   : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] &  ( f.bit_slice[(i+b)%N] | ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MOAI1_NOR2_ANDN2L    :
      case MOAI1_ANDN2L_ORN2R   :
      case MAOI1_OR2_ANDN2L     :
      case MAOI1_ORN2R_ORN2R    :
      case XOR2_OR2_ANDN2L      :
      case XOR2_ORN2R_ORN2R     : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] |  (~f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      
      case MOAI1_NOR2_ANDN2R    :
      case MOAI1_ANDN2L_ORN2L   :
      case MAOI1_OR2_ANDN2R     :
      case MAOI1_ORN2R_ORN2L    :
      case XOR2_OR2_ANDN2R      :
      case XOR2_ORN2R_ORN2L     : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] |  ( f.bit_slice[(i+b)%N] & ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MOAI1_NOR2_ORN2L     :
      case MOAI1_ANDN2L_ANDN2R  :
      case MAOI1_OR2_ORN2L      :
      case MAOI1_ORN2R_ANDN2R   :
      case XOR2_OR2_ORN2L       :
      case XOR2_ORN2R_ANDN2R    :  fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] |  (~f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MOAI1_NOR2_ORN2R     :
      case MOAI1_ANDN2L_ANDN2L  :
      case MAOI1_OR2_ORN2R      :
      case MAOI1_ORN2R_ANDN2L   :
      case XOR2_OR2_ORN2R       :
      case XOR2_ORN2R_ANDN2L    : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] |  ( f.bit_slice[(i+b)%N] | ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
   
      ///////////NOR2_NAND2 ~(A | ~(B & C)) = (~A & (B & C)) ANDN2L_AND2
      case MAOI1_NOR2_NAND2    :
      case MAOI1_ANDN2L_AND2   :
      case MOAI1_OR2_NAND2     :
      case MOAI1_ORN2R_AND2    :
      case XOR2_ANDN2L_AND2    : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] &  ( f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////NOR2_NOR2 ~(A | ~(B | C)) = (~A & (B | C)) ANDN2L_OR2
      case MAOI1_NOR2_NOR2     :
      case MAOI1_ANDN2L_OR2    :
      case MOAI1_OR2_NOR2      :
      case MOAI1_ORN2R_OR2     :
      case XOR2_ANDN2L_OR2     : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] &  ( f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////NOR2_ORN2R ~(A | (B | ~C)) = ~A & (~B & C) ANDN2L_ANDN2L
      case MAOI1_NOR2_ORN2R    :
      case MAOI1_ANDN2L_ANDN2L :
      case MOAI1_OR2_ORN2R     :
      case MOAI1_ORN2R_ANDN2L  :
      case XOR2_ANDN2L_ANDN2L  : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] &  (~f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////NOR2_ORN2L ~(A | (~B | C)) = ~A & (B & ~C) ANDN2L_ANDN2R
      case MAOI1_NOR2_ORN2L    :
      case MAOI1_ANDN2L_ANDN2R :
      case MOAI1_OR2_ORN2L     :
      case MOAI1_ORN2R_ANDN2R  :
      case XOR2_ANDN2L_ANDN2R  : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] &  ( f.bit_slice[(i+b)%N] & ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////NOR2_ANDN2R ~(A | (B & ~C)) = ~A & (~B | C) ANDN2L_ORN2L
      case MAOI1_NOR2_ANDN2R   :
      case MAOI1_ANDN2L_ORN2L  :
      case MOAI1_OR2_ANDN2R    :
      case MOAI1_ORN2R_ORN2L   :
      case XOR2_ANDN2L_ORN2L   : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] &  (~f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////NOR2_ANDN2L ~(A | (~B & C)) = ~A & (B | ~C) ANDN2L_ORN2R
      case MAOI1_NOR2_ANDN2L   :
      case MAOI1_ANDN2L_ORN2R  :
      case MOAI1_OR2_ANDN2L    :
      case MOAI1_ORN2R_ORN2R   :
      case XOR2_ANDN2L_ORN2R   : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] &  ( f.bit_slice[(i+b)%N] | ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////AND2_NAND2 (A & ~(B & C)) = ~(~A | (B & C))
      case MAOI1_AND2_NAND2    :
      case MAOI1_ANDN2R_AND2   :
      case MOAI1_NAND2_NAND2   :
      case MOAI1_ORN2L_AND2    :
      case XOR2_ANDN2R_AND2    : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] & ~( f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////AND2_NAND2 (A & ~(B | C)) = ~(~A | (B | C))
      case MAOI1_AND2_NOR2     :
      case MAOI1_ANDN2R_OR2    :
      case MOAI1_NAND2_NOR2    :
      case MOAI1_ORN2L_OR2     :
      case XOR2_ANDN2R_OR2     : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] & ~( f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MAOI1_AND2_ORN2L    :
      case MAOI1_ANDN2R_ANDN2R :
      case MOAI1_NAND2_ORN2L   :
      case MOAI1_ORN2L_ANDN2R  :
      case XOR2_AND2_ORN2L     :
      case XOR2_ANDN2R_ANDN2R  : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] & ~( f.bit_slice[(i+b)%N] & ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MAOI1_AND2_ANDN2L   :
      case MAOI1_ANDN2R_ORN2R  :
      case MOAI1_NAND2_ANDN2L  :
      case MOAI1_ORN2L_ORN2R   :
      case XOR2_AND2_ANDN2L    :
      case XOR2_ANDN2R_ORN2R   : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] & ~( f.bit_slice[(i+b)%N] | ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ///////////NAND2_NAND2 ~(A & ~(B & C)) = ~A | (B & C) ORN2L_AND2
      case MAOI1_NAND2_NAND2   :
      case MAOI1_ORN2L_AND2    :
      case MOAI1_AND2_NAND2    :
      case MOAI1_ANDN2R_AND2   :
      case XOR2_ORN2L_AND2     : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] |  ( f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MOAI1_AND2_NOR2     :
      case MAOI1_NAND2_NOR2    :
      case MOAI1_ANDN2R_OR2    :
      case MAOI1_ORN2L_OR2     :
      case XOR2_ORN2L_OR2      : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] |  ( f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ////NAND2_ORN2R ~(A & (B | ~C)) = ~A | (~B & C) ORN2L_ANDN2L  
      case MAOI1_NAND2_ORN2R   :
      case MAOI1_ORN2L_ANDN2L  : 
      case MOAI1_AND2_ORN2R    :
      case MOAI1_ANDN2R_ANDN2L :
      case XOR2_ORN2L_ANDN2L   : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] |  (~f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ////NAND2_ORN2L ~(A & (~B | C)) = ~A | (B & ~C) ORN2L_ANDN2R
      case MAOI1_NAND2_ORN2L   :
      case MAOI1_ORN2L_ANDN2R  :
      case MOAI1_AND2_ORN2L    :
      case MOAI1_ANDN2R_ANDN2R :
      case XOR2_ORN2L_ANDN2R   : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] |  ( f.bit_slice[(i+b)%N] & ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ////NAND2_ANDN2R ~(A & (B & ~C)) = ~A | (~B | C) ORN2L_ORN2L
      case MAOI1_NAND2_ANDN2R  :
      case MAOI1_ORN2L_ORN2L   :
      case MOAI1_AND2_ANDN2R   :
      case MOAI1_ANDN2R_ORN2L  :
      case XOR2_ORN2L_ORN2L    : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] |  (~f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      ////NAND2_ANDN2L ~(A & (~B & C)) = ~A | (B | ~C) ORN2L_ORN2R
      case MOAI1_AND2_ANDN2L    :
      case MOAI1_ANDN2R_ORN2R   :
      case MAOI1_NAND2_ANDN2L   :
      case MAOI1_ORN2L_ORN2R    :
      case XOR2_ORN2L_ORN2R     : fun_tmp.bit_slice[i] =  ( ~f.bit_slice[(i+a)%N] |  ( f.bit_slice[(i+b)%N] | ~f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MAOI1_OR2_NAND2     :
      case MAOI1_ORN2R_AND2    :
      case MOAI1_NOR2_NAND2    :
      case MOAI1_ANDN2L_AND2   :
      case XOR2_ORN2R_AND2     : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] | ~( f.bit_slice[(i+b)%N] &  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;

      case MOAI1_NOR2_NOR2      :
      case MAOI1_OR2_NOR2       :
      case MOAI1_ANDN2L_OR2     :
      case MAOI1_ORN2R_OR2      :
      case XOR2_ORN2R_OR2       : fun_tmp.bit_slice[i] =  (  f.bit_slice[(i+a)%N] | ~( f.bit_slice[(i+b)%N] |  f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
    }
    fun_tmp.prev = f.bit_slice[i];
    fun_tmp.info_line = (uint8_t)i;
    fun_tmp.info_op = (uint8_t)operation;
    fun_tmp.sort();
    return fun_tmp;
  };

  switch (operation)
  {
      case MOAI1_NAND2_OR2      :
      case MAOI1_AND2_OR2       :
      case MOAI1_ORN2L_NOR2     :
      case MAOI1_ANDN2R_NOR2    :
      case XOR2_AND2_OR2        :

      case MOAI1_NOR2_AND2      :
      case MAOI1_OR2_AND2       :
      case MOAI1_ANDN2L_NAND2   :
      case MAOI1_ORN2R_NAND2    :
      case XOR2_OR2_AND2        :
      
      case MOAI1_AND2_OR2       :
      case MAOI1_NAND2_OR2      :
      case MOAI1_ANDN2R_NOR2    :
      case MAOI1_ORN2L_NOR2     :
      
      case MOAI1_OR2_AND2       :
      case MAOI1_NOR2_AND2      :
      case MOAI1_ORN2R_NAND2    :
      case MAOI1_ANDN2L_NAND2   :

      case MOAI1_AND2_NOR2      :
      case MAOI1_NAND2_NOR2     :
      case MOAI1_ANDN2R_OR2     :
      case MAOI1_ORN2L_OR2      :
      case XOR2_ORN2L_OR2       :

      case MOAI1_NAND2_ANDN2R   :
      case MOAI1_ORN2L_ORN2L    :
      case MAOI1_AND2_ANDN2R    :
      case MAOI1_ANDN2R_ORN2L   :
      case XOR2_AND2_ANDN2R     :
      case XOR2_ANDN2R_ORN2L    :

      case MOAI1_NAND2_ORN2L    :
      case MOAI1_ORN2L_ANDN2R   :
      case MAOI1_AND2_ORN2L     :
      case MAOI1_ANDN2R_ANDN2R  :
      case XOR2_AND2_ORN2L      :
      case XOR2_ANDN2R_ANDN2R   :

      case MOAI1_NAND2_ORN2R    :
      case MOAI1_ORN2L_ANDN2L   :
      case MAOI1_AND2_ORN2R     :
      case MAOI1_ANDN2R_ANDN2L  :
      case XOR2_AND2_ORN2R      :
      case XOR2_ANDN2R_ANDN2L   :

      case MOAI1_NOR2_ANDN2L    :
      case MOAI1_ANDN2L_ORN2R   :
      case MAOI1_OR2_ANDN2L     :
      case MAOI1_ORN2R_ORN2R    :
      case XOR2_OR2_ANDN2L      :
      case XOR2_ORN2R_ORN2R     :

      case MOAI1_NOR2_ANDN2R    :
      case MOAI1_ANDN2L_ORN2L   :
      case MAOI1_OR2_ANDN2R     :
      case MAOI1_ORN2R_ORN2L    :
      case XOR2_OR2_ANDN2R      :
      case XOR2_ORN2R_ORN2L     :

      case MOAI1_NOR2_ORN2L     :
      case MOAI1_ANDN2L_ANDN2R  :
      case MAOI1_OR2_ORN2L      :
      case MAOI1_ORN2R_ANDN2R   :
      case XOR2_OR2_ORN2L       :
      case XOR2_ORN2R_ANDN2R    :

      case MOAI1_NOR2_ORN2R     :
      case MOAI1_ANDN2L_ANDN2L  :
      case MAOI1_OR2_ORN2R      :
      case MAOI1_ORN2R_ANDN2L   :
      case XOR2_OR2_ORN2R       :
      case XOR2_ORN2R_ANDN2L    :

      case MOAI1_OR2_NAND2      :
      case MAOI1_NOR2_NAND2     :
      case MOAI1_ORN2R_AND2     :
      case MAOI1_ANDN2L_AND2    :
      case XOR2_ANDN2L_AND2     :

      case MOAI1_OR2_NOR2       :
      case MAOI1_NOR2_NOR2      :
      case MOAI1_ORN2R_OR2      :
      case MAOI1_ANDN2L_OR2     :
      case XOR2_ANDN2L_OR2      :

      case MOAI1_OR2_ORN2R      :
      case MOAI1_ORN2R_ANDN2L   :
      case MAOI1_NOR2_ORN2R     :
      case MAOI1_ANDN2L_ANDN2L  :
      case XOR2_ANDN2L_ANDN2L   :

      case MOAI1_OR2_ORN2L      :
      case MOAI1_ORN2R_ANDN2R   :
      case MAOI1_NOR2_ORN2L     :
      case MAOI1_ANDN2L_ANDN2R  :
      case XOR2_ANDN2L_ANDN2R   :

      case MOAI1_OR2_ANDN2R     :
      case MOAI1_ORN2R_ORN2L    :
      case MAOI1_NOR2_ANDN2R    :
      case MAOI1_ANDN2L_ORN2L   :
      case XOR2_ANDN2L_ORN2L    :

      case MOAI1_OR2_ANDN2L     :
      case MOAI1_ORN2R_ORN2R    :
      case MAOI1_NOR2_ANDN2L    :
      case MAOI1_ANDN2L_ORN2R   :
      case XOR2_ANDN2L_ORN2R    :

      case MOAI1_NAND2_NAND2    :
      case MAOI1_AND2_NAND2     :
      case MOAI1_ORN2L_AND2     :
      case MAOI1_ANDN2R_AND2    :
      case XOR2_ANDN2R_AND2     :

      case MOAI1_NAND2_NOR2     :
      case MAOI1_AND2_NOR2      :
      case MOAI1_ORN2L_OR2      :
      case MAOI1_ANDN2R_OR2     :
      case XOR2_ANDN2R_OR2      :

      case MOAI1_NAND2_ANDN2L   :
      case MOAI1_ORN2L_ORN2R    :
      case MAOI1_AND2_ANDN2L    :
      case MAOI1_ANDN2R_ORN2R   :
      case XOR2_AND2_ANDN2L     :
      case XOR2_ANDN2R_ORN2R    :

      case MOAI1_AND2_NAND2     :
      case MAOI1_NAND2_NAND2    :
      case MOAI1_ANDN2R_AND2    :
      case MAOI1_ORN2L_AND2     :
      case XOR2_ORN2L_AND2      :

      case MOAI1_AND2_ORN2R     :
      case MOAI1_ANDN2R_ANDN2L  :
      case MAOI1_NAND2_ORN2R    :
      case MAOI1_ORN2L_ANDN2L   :
      case XOR2_ORN2L_ANDN2L    :

      case MOAI1_AND2_ORN2L     :
      case MOAI1_ANDN2R_ANDN2R  :
      case MAOI1_NAND2_ORN2L    :
      case MAOI1_ORN2L_ANDN2R   :
      case XOR2_ORN2L_ANDN2R    :

      case MOAI1_AND2_ANDN2R    :
      case MOAI1_ANDN2R_ORN2L   :
      case MAOI1_NAND2_ANDN2R   :
      case MAOI1_ORN2L_ORN2L    :
      case XOR2_ORN2L_ORN2L     :

      case MOAI1_AND2_ANDN2L    :
      case MOAI1_ANDN2R_ORN2R   :
      case MAOI1_NAND2_ANDN2L   :
      case MAOI1_ORN2L_ORN2R    :
      case XOR2_ORN2L_ORN2R     :

      case MOAI1_NOR2_NAND2     :
      case MAOI1_OR2_NAND2      :
      case MOAI1_ANDN2L_AND2    :
      case MAOI1_ORN2R_AND2     :
      case XOR2_ORN2R_AND2      :

      case MOAI1_NOR2_NOR2      :
      case MAOI1_OR2_NOR2       :
      case MOAI1_ANDN2L_OR2     :
      case MAOI1_ORN2R_OR2      :
      case XOR2_ORN2R_OR2       :
                                for (int i = 0; i < N; i++)
                                {
                                  for (int j = 1; j < N; j++ )
                                  {
                                    for (int k = j + 1; k < j + N - 1; k++)
                                    {
                                      if(k != N)
                                      {
                                        for (int u = k + 1; u < j + N; u++)
                                        {
                                          if (u != N)
                                          {
                                            v->push_back(triple_gates(f, i, j, k, u, operation));
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                                break;
  }
}


template<int N>
void lighter<N>::bool_op_fun(int op_id, function_t<N> f, vector<function_t<N> > *v)
{
  switch (op_id)
  {
    case NOT1  :
    case XOR2  :
    case XNOR2 :
    case MAOI1 :
    case MOAI1 : bool_op_simple_gates(f, v, op_id); break;

    case MOAI1_NAND2       :
    case MOAI1_NOR2        :
    case MOAI1_AND2        :
    case MOAI1_OR2         :
    case MAOI1_NAND3       :
    case MAOI1_NOR3        :
    case MOAI1_NAND3       :
    case MOAI1_NOR3        :
    case XOR2_AND2         :
    case XOR2_OR2          :
    case MAOI1_ANDN2L      :
    case MAOI1_ANDN2R      :
    case MAOI1_ORN2L       :
    case MAOI1_ORN2R       :
    case MOAI1_ANDN2L      :
    case MOAI1_ANDN2R      :
    case MOAI1_ORN2L       :
    case MOAI1_ORN2R       :
    case XOR2_ANDN2L       :
    case XOR2_ANDN2R       :
    case XOR2_ORN2L        :
    case XOR2_ORN2R        :
    case XOR2_OR2_OR2      :

    case MOAI1_NAND2_AND2     :
    case MAOI1_AND2_AND2      :
    case MOAI1_ORN2L_NAND2    :
    case MAOI1_ANDN2R_NAND2   :
    case XOR2_AND2_AND2       :
 
    case MAOI1_NAND2       :
    case MAOI1_NOR2        :
    case MAOI1_AND2        :
    case MAOI1_OR2         :
    case MAOI1_AND3        :

    case MAOI1_OR3         :
    case MOAI1_NOR2_OR2    :
    case MAOI1_OR2_OR2     :
    case MOAI1_AND3        :
    case MAOI1_NAND2_AND2  :
    case MOAI1_AND2_AND2   :
    case MOAI1_OR3         :
    case MAOI1_NOR2_OR2    :
    case MOAI1_OR2_OR2     : 
    case MOAI1_ANDN2L_NOR2 : 
    case MAOI1_ORN2R_NOR2  :  
    case MOAI1_ORN2R_NOR2  : 
    case MAOI1_ANDN2L_NOR2 :
    case MOAI1_ANDN2R_NAND2:
    case MAOI1_ORN2L_NAND2 : bool_op_double_gates(f, v, op_id); break;

    case MOAI1_AND2_OR2       :
    case MAOI1_NAND2_OR2      :
    case MOAI1_ANDN2R_NOR2    :
    case MAOI1_ORN2L_NOR2     :
    
    case MOAI1_OR2_AND2       :
    case MAOI1_NOR2_AND2      :
    case MOAI1_ORN2R_NAND2    :
    case MAOI1_ANDN2L_NAND2   :

    case MOAI1_AND2_NOR2      :
    case MAOI1_NAND2_NOR2     :
    case MOAI1_ANDN2R_OR2     :
    case MAOI1_ORN2L_OR2      :
    case XOR2_ORN2L_OR2       :
     
    case MOAI1_NAND2_OR2      :
    case MAOI1_AND2_OR2       :
    case MOAI1_ORN2L_NOR2     :
    case MAOI1_ANDN2R_NOR2    :
    case XOR2_AND2_OR2        :

    case MOAI1_NOR2_AND2      :
    case MAOI1_OR2_AND2       :
    case MOAI1_ANDN2L_NAND2   :
    case MAOI1_ORN2R_NAND2    :
    case XOR2_OR2_AND2        :

    case MOAI1_NAND2_ANDN2R   :
    case MOAI1_ORN2L_ORN2L    :
    case MAOI1_AND2_ANDN2R    :
    case MAOI1_ANDN2R_ORN2L   :
    case XOR2_AND2_ANDN2R     :
    case XOR2_ANDN2R_ORN2L    :

    case MOAI1_NAND2_ORN2L    :
    case MOAI1_ORN2L_ANDN2R   :
    case MAOI1_AND2_ORN2L     :
    case MAOI1_ANDN2R_ANDN2R  :
    case XOR2_AND2_ORN2L      :
    case XOR2_ANDN2R_ANDN2R   :

    case MOAI1_NAND2_ORN2R    :
    case MOAI1_ORN2L_ANDN2L   :
    case MAOI1_AND2_ORN2R     :
    case MAOI1_ANDN2R_ANDN2L  :
    case XOR2_AND2_ORN2R      :
    case XOR2_ANDN2R_ANDN2L   :

    case MOAI1_NOR2_ANDN2L    :
    case MOAI1_ANDN2L_ORN2R   :
    case MAOI1_OR2_ANDN2L     :
    case MAOI1_ORN2R_ORN2R    :
    case XOR2_OR2_ANDN2L      :
    case XOR2_ORN2R_ORN2R     :

    case MOAI1_NOR2_ANDN2R    :
    case MOAI1_ANDN2L_ORN2L   :
    case MAOI1_OR2_ANDN2R     :
    case MAOI1_ORN2R_ORN2L    :
    case XOR2_OR2_ANDN2R      :
    case XOR2_ORN2R_ORN2L     :

    case MOAI1_NOR2_ORN2L     :
    case MOAI1_ANDN2L_ANDN2R  :
    case MAOI1_OR2_ORN2L      :
    case MAOI1_ORN2R_ANDN2R   :
    case XOR2_OR2_ORN2L       :
    case XOR2_ORN2R_ANDN2R    :

    case MOAI1_NOR2_ORN2R     :
    case MOAI1_ANDN2L_ANDN2L  :
    case MAOI1_OR2_ORN2R      :
    case MAOI1_ORN2R_ANDN2L   :
    case XOR2_OR2_ORN2R       :
    case XOR2_ORN2R_ANDN2L    :
    
    case MOAI1_OR2_NAND2      :
    case MAOI1_NOR2_NAND2     :
    case MOAI1_ORN2R_AND2     :
    case MAOI1_ANDN2L_AND2    :
    case XOR2_ANDN2L_AND2     :

    case MOAI1_OR2_NOR2       :
    case MAOI1_NOR2_NOR2      :
    case MOAI1_ORN2R_OR2      :
    case MAOI1_ANDN2L_OR2     :
    case XOR2_ANDN2L_OR2      :

    case MOAI1_OR2_ORN2R      :
    case MOAI1_ORN2R_ANDN2L   :
    case MAOI1_NOR2_ORN2R     :
    case MAOI1_ANDN2L_ANDN2L  :
    case XOR2_ANDN2L_ANDN2L   :

    case MOAI1_OR2_ORN2L      :
    case MOAI1_ORN2R_ANDN2R   :
    case MAOI1_NOR2_ORN2L     :
    case MAOI1_ANDN2L_ANDN2R  :
    case XOR2_ANDN2L_ANDN2R   :

    case MOAI1_OR2_ANDN2R     :
    case MOAI1_ORN2R_ORN2L    :
    case MAOI1_NOR2_ANDN2R    :
    case MAOI1_ANDN2L_ORN2L   :
    case XOR2_ANDN2L_ORN2L    :

    case MOAI1_OR2_ANDN2L     :
    case MOAI1_ORN2R_ORN2R    :
    case MAOI1_NOR2_ANDN2L    :
    case MAOI1_ANDN2L_ORN2R   :
    case XOR2_ANDN2L_ORN2R    :

    case MOAI1_NAND2_NAND2    :
    case MAOI1_AND2_NAND2     :
    case MOAI1_ORN2L_AND2     :
    case MAOI1_ANDN2R_AND2    :
    case XOR2_ANDN2R_AND2     :

    case MOAI1_NAND2_NOR2     :
    case MAOI1_AND2_NOR2      :
    case MOAI1_ORN2L_OR2      :
    case MAOI1_ANDN2R_OR2     :
    case XOR2_ANDN2R_OR2      :

    case MOAI1_NAND2_ANDN2L   :
    case MOAI1_ORN2L_ORN2R    :
    case MAOI1_AND2_ANDN2L    :
    case MAOI1_ANDN2R_ORN2R   :
    case XOR2_AND2_ANDN2L     :
    case XOR2_ANDN2R_ORN2R    :

    case MOAI1_AND2_NAND2     :
    case MAOI1_NAND2_NAND2    :
    case MOAI1_ANDN2R_AND2    :
    case MAOI1_ORN2L_AND2     :
    case XOR2_ORN2L_AND2      :

    case MOAI1_AND2_ORN2R     :
    case MOAI1_ANDN2R_ANDN2L  :
    case MAOI1_NAND2_ORN2R    :
    case MAOI1_ORN2L_ANDN2L   :
    case XOR2_ORN2L_ANDN2L    :

    case MOAI1_AND2_ORN2L     :
    case MOAI1_ANDN2R_ANDN2R  :
    case MAOI1_NAND2_ORN2L    :
    case MAOI1_ORN2L_ANDN2R   :
    case XOR2_ORN2L_ANDN2R    :

    case MOAI1_AND2_ANDN2R    :
    case MOAI1_ANDN2R_ORN2L   :
    case MAOI1_NAND2_ANDN2R   :
    case MAOI1_ORN2L_ORN2L    :
    case XOR2_ORN2L_ORN2L     :

    case MOAI1_AND2_ANDN2L    :
    case MOAI1_ANDN2R_ORN2R   :
    case MAOI1_NAND2_ANDN2L   :
    case MAOI1_ORN2L_ORN2R    :
    case XOR2_ORN2L_ORN2R     :

    case MOAI1_NOR2_NAND2     :
    case MAOI1_OR2_NAND2      :
    case MOAI1_ANDN2L_AND2    :
    case MAOI1_ORN2R_AND2     :
    case XOR2_ORN2R_AND2      :

    case MOAI1_NOR2_NOR2      :
    case MAOI1_OR2_NOR2       :
    case MOAI1_ANDN2L_OR2     :
    case MAOI1_ORN2R_OR2      :
    case XOR2_ORN2R_OR2       :
                              bool_op_triple_gates(f, v, op_id); break;
  }
}

#endif // LIGHTER_BOOL_OP_H__