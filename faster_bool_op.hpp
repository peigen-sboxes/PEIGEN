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

#ifndef FASTER_BOOL_OP_H__
#define FASTER_BOOL_OP_H__

using namespace std;
using namespace Peigen::depth;

template<int N>
void faster<N>::all_gates()
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
  gate_andn2 = true;
  gate_orn2  = true;
}

template<int N>
void faster<N>::genImpInfo()
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
int faster<N>::bool_op_cost(uint8_t op)
{
  switch (op)
  {
    case NOT1  : return not1_cost;
    case AND2  : return and2_cost;
    case NAND2 : return nand2_cost;
    case OR2   : return or2_cost;
    case NOR2  : return nor2_cost;
    case XOR2  : return xor2_cost;
    case XNOR2 : return xnor2_cost;
    case MOAI1 : return moai1_cost;
    case MAOI1 : return maoi1_cost;
    case NAND3 : return nand3_cost;
    case NOR3  : return nor3_cost;
    case OR3   : return or3_cost;
    case AND3  : return and3_cost;
    case ANDN2 : return andn2_cost;
    case ORN2  : return orn2_cost;
    case NOP   : return 0;
    default    : return -1;
  }
}

template<int N>
string faster<N>::bool_op_str(uint8_t op)
{
  switch (op)
  {
    case NOT1  : return "NOT1";
    case AND2  : return "AND2";
    case NAND2 : return "NAND2";
    case OR2   : return "OR2";
    case NOR2  : return "NOR2";
    case XOR2  : return "XOR2";
    case XNOR2 : return "XNOR2";
    case MOAI1 : return "MOAI1";
    case MAOI1 : return "MAOI1";
    case NAND3 : return "NAND3";
    case NOR3  : return "NOR3";
    case OR3   : return "OR3";
    case AND3  : return "AND3";
    case ANDN2 : return "ANDN2";
    case ORN2  : return "ORN2";

    default    : return "--";
  }
}


template<int N>
void faster<N>::init_b(string conf_file)
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
        if(key == "andn2_cost") andn2_cost = stod(value)*100;
        if(key == "orn2_cost") orn2_cost = stod(value)*100;
        if (N >= 4)
        {
            if(key == "nand3_cost") nand3_cost = stod(value)*100;
            if(key == "nor3_cost") nor3_cost = stod(value)*100;
            if(key == "and3_cost") and3_cost = stod(value)*100;
            if(key == "or3_cost") or3_cost = stod(value)*100;
        }

      }
    }
  }

  /*
    initilisation of the set of bolean operations
  */
  if(gate_not1 ) b.push_back({NOT1 , not1_cost});
  if(gate_and2 ) b.push_back({AND2 , and2_cost});
  if(gate_nand2) b.push_back({NAND2, nand2_cost});
  if(gate_or2  ) b.push_back({OR2  , or2_cost});
  if(gate_nor2 ) b.push_back({NOR2 , nor2_cost});
  if(gate_xor2 ) b.push_back({XOR2 , xor2_cost});
  if(gate_xnor2) b.push_back({XNOR2, xnor2_cost});
  if(gate_moai1) b.push_back({MOAI1, moai1_cost});
  if(gate_maoi1) b.push_back({MAOI1, maoi1_cost});
  if(gate_andn2) b.push_back({ANDN2, andn2_cost});
  if(gate_orn2 ) b.push_back({ORN2 , orn2_cost});
  if (N >= 4)
  {
    if(gate_nand3) b.push_back({NAND3, nand3_cost});
    if(gate_nor3 ) b.push_back({NOR3 , nor3_cost});
    if(gate_or3  ) b.push_back({OR3  , or3_cost});
    if(gate_and3 ) b.push_back({AND3 , and3_cost});    
  }

  sort(b.begin(), b.end());

  max_GE = (b.back()).op_cost;
}

template<int N>
void faster<N>::bool_op_one_input(const bool_function_t<N> *f, vector<bool_function_t<N> > *v, bool_op_t bool_op)
{
  int operation = bool_op.op_id;
  int cost = bool_op.op_cost;

  bool_function_t<N> fun_tmp;

  if (operation == NOT1)
  {
    fun_tmp.bit_slice = ~((*f).bit_slice);
    fun_tmp.area = (*f).area + cost;    
    fun_tmp.info_op = (uint8_t)operation;
    fun_tmp.operands[0] = f;
    v->push_back(fun_tmp);
  }
}

template<int N>
void faster<N>::bool_op_two_inputs(const bool_function_t<N> *f1, const bool_function_t<N> *f2, vector<bool_function_t<N> > *v, bool_op_t bool_op)
{
  int operation = bool_op.op_id;
  int cost = bool_op.op_cost;

  bool_function_t<N> fun_tmp;

  switch(operation)
  {
    case AND2  : fun_tmp.bit_slice =    (*f1).bit_slice & (*f2).bit_slice ; break;
    case OR2   : fun_tmp.bit_slice =    (*f1).bit_slice | (*f2).bit_slice ; break;
    case NAND2 : fun_tmp.bit_slice =  ~((*f1).bit_slice & (*f2).bit_slice); break;
    case NOR2  : fun_tmp.bit_slice =  ~((*f1).bit_slice | (*f2).bit_slice); break;
    case ANDN2 : fun_tmp.bit_slice = (~(*f1).bit_slice) & (*f2).bit_slice ; break;
    case ORN2  : fun_tmp.bit_slice = (~(*f1).bit_slice) | (*f2).bit_slice ; break;
    case MAOI1 :
    case XOR2  : fun_tmp.bit_slice =   (*f1).bit_slice ^ (*f2).bit_slice ; break;
    case MOAI1 :
    case XNOR2 : fun_tmp.bit_slice = ~((*f1).bit_slice ^ (*f2).bit_slice); break;
  }
  fun_tmp.area = (*f1).area + (*f2).area + cost;
  fun_tmp.info_op = (uint8_t)operation;
  fun_tmp.operands[0] = f1;
  fun_tmp.operands[1] = f2;
  v->push_back(fun_tmp);
}

template<int N>
void faster<N>::bool_op_three_inputs(const bool_function_t<N> *f1, const bool_function_t<N> *f2, const bool_function_t<N> *f3, vector<bool_function_t<N> > *v, bool_op_t bool_op)
{
  int operation = bool_op.op_id;
  int cost = bool_op.op_cost;

  bool_function_t<N> fun_tmp;

  switch(operation)
  {
    case AND3  : fun_tmp.bit_slice =   (*f1).bit_slice & (*f2).bit_slice & (*f3).bit_slice; break;
    case OR3   : fun_tmp.bit_slice =   (*f1).bit_slice | (*f2).bit_slice | (*f3).bit_slice; break;
    case NAND3 : fun_tmp.bit_slice = ~((*f1).bit_slice & (*f2).bit_slice & (*f3).bit_slice); break;
    case NOR3  : fun_tmp.bit_slice = ~((*f1).bit_slice | (*f2).bit_slice | (*f3).bit_slice); break;
  }

  fun_tmp.area = (*f1).area + (*f2).area + (*f3).area + cost;
  fun_tmp.info_op = (uint8_t)operation;
  fun_tmp.operands[0] = f1;
  fun_tmp.operands[1] = f2;
  fun_tmp.operands[2] = f3;
  v->push_back(fun_tmp);
}

#endif // #ifndef FASTER_BOOL_OP_H__