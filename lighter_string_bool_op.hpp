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

#ifndef LIGHTER_STRING_BOOL_OP_H__
#define LIGHTER_STRING_BOOL_OP_H__

using namespace Peigen;
using namespace Peigen::weight;
using namespace std;

template<int N>
void lighter<N>::write_c(string *s, vector<int> tmp_tab)
{
  int i = 0;
  auto pos = s->find_first_of("[");
  s->insert(pos+1, to_string(tmp_tab[0]));
  while((pos = s->find_first_of("[", pos+1)) != string::npos)
  {
    s->insert(pos+1, to_string(tmp_tab[i%(tmp_tab.size())]));
    i++;
  }
}

template<int N>
string lighter<N>::bool_op_not(const function_t<N> f, string s, bool reverse)
{
  int tmp;
  function_t<N> fun_prev = f;
  if(!reverse)
  {
    fun_prev.bit_slice[f.info_line] = f.prev;
    for(int i = 0; i < N; i++) if(fun_prev.bit_slice[i] == f.prev) tmp = i;
  }
  else tmp = f.info_line;
  write_c(&s, {tmp});
  return s;
}

template<int N>
string lighter<N>::bool_op_two_inputs(const function_t<N> f, int operation, string s, bool reverse)
{
  int tmp;
  function_t<N> fun_prev = f;
  if(!reverse)
  {
    fun_prev.bit_slice[f.info_line] = f.prev;
    for(int i = 0; i < N; i++) if(fun_prev.bit_slice[i] == f.prev) tmp = i;
  }
  else tmp = f.info_line;

  bit_slice_l_t<N> a, b;

  int j;
  for(j = 0; j < N; j++)
  {
    switch (operation)
    {
      case MAOI1 :
      case XOR2  : a = fun_prev.bit_slice[tmp] ^ fun_prev.bit_slice[j]; break;
      case MOAI1 :
      case XNOR2 : a = ~(fun_prev.bit_slice[tmp] ^ fun_prev.bit_slice[j]); break;
    }
    if(!reverse) b = f.bit_slice[f.info_line];
    else b = f.prev;
    if(a == b) break;
  }
  write_c(&s, {tmp, j});
  return s;
}

template<int N>
string lighter<N>::bool_op_super_gates(const function_t<N> f, int operation, string s, bool reverse)
{
  int tmp;
  function_t<N> fun_prev = f;
  if(!reverse)
  {
    fun_prev.bit_slice[f.info_line] = f.prev;
    for(int i = 0; i < N; i++) if(fun_prev.bit_slice[i] == f.prev) tmp = i;
  }
  else tmp = f.info_line;

  bit_slice_l_t<N> a, b;
  switch(operation)
  {
    case MOAI1_AND2   :
    case MOAI1_OR2    :
    case MOAI1_NAND2  :
    case MOAI1_NOR2   :
    case XOR2_AND2    :
    case XOR2_OR2     :
    case MOAI1_ORN2R  :
    case MAOI1_ANDN2L :
    case MOAI1_ORN2L  :
    case MAOI1_ANDN2R :
    case MOAI1_ANDN2R :
    case MAOI1_ORN2L  :
    case MOAI1_ANDN2L :
    case MAOI1_ORN2R  :
    case XOR2_ANDN2L  :
    case XOR2_ANDN2R  :
    case XOR2_ORN2L   :
    case XOR2_ORN2R   :
    case MAOI1_NAND2  :
    case MAOI1_NOR2   :
    case MAOI1_AND2   :
    case MAOI1_OR2    :
          for(int k = 0; k < N; k++)
          {
            for(int j = 0; j < N; j++)
            {
              int op1, op2;
              switch(operation)
              {
                case MAOI1_NAND2  :
                case MOAI1_AND2   : a = ~( fun_prev.bit_slice[k] &  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                case MAOI1_NOR2   :  
                case MOAI1_OR2    : a = ~( fun_prev.bit_slice[k] |  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                case XOR2_AND2    : 
                case MAOI1_AND2   : 
                case MOAI1_NAND2  : a =  ( fun_prev.bit_slice[k] &  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                case XOR2_OR2     : 
                case MAOI1_OR2    : 
                case MOAI1_NOR2   : a =  ( fun_prev.bit_slice[k] |  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                
                case MOAI1_ORN2R  : a =  (~fun_prev.bit_slice[k] &  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = j; op2 = k; break;
                case MAOI1_ANDN2L :
                case XOR2_ANDN2L  : a =  (~fun_prev.bit_slice[k] &  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                
                case MOAI1_ORN2L  : a =  ( fun_prev.bit_slice[k] & ~fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                case MAOI1_ANDN2R :
                case XOR2_ANDN2R  : a =  ( fun_prev.bit_slice[k] & ~fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = j; op2 = k; break;
                
                case MOAI1_ANDN2R : a =  (~fun_prev.bit_slice[k] |  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = j; op2 = k; break;
                case MAOI1_ORN2L  :
                case XOR2_ORN2L   : a =  (~fun_prev.bit_slice[k] |  fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                
                case MOAI1_ANDN2L : a =  ( fun_prev.bit_slice[k] | ~fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = k; op2 = j; break;
                case MAOI1_ORN2R  :                
                case XOR2_ORN2R   : a =  ( fun_prev.bit_slice[k] | ~fun_prev.bit_slice[j]) ^ fun_prev.bit_slice[tmp]; op1 = j; op2 = k; break;
              }
              if(!reverse) b = f.bit_slice[f.info_line];
              else b = f.prev;
              if( a == b )
              {
                write_c(&s, {tmp, op1, op2});
                return s;
              }
            }
          }
    case XOR2_AND2_AND2     :
    case MAOI1_AND3         :
    case MOAI1_NAND2_AND2   :
    case MAOI1_AND2_AND2    :
    case MOAI1_ORN2L_NAND2  :
    case MAOI1_ANDN2R_NAND2 :
    case MOAI1_NAND3        :
    case XOR2_OR2_OR2       :
    case MAOI1_OR3          :
    case MOAI1_NOR2_OR2     :
    case MAOI1_OR2_OR2      :
    case MOAI1_ANDN2L_NOR2  :
    case MAOI1_ORN2R_NOR2   :
    case MOAI1_NOR3         :
    case MOAI1_AND3         :
    case MAOI1_NAND2_AND2   :
    case MOAI1_AND2_AND2    :
    case MOAI1_ANDN2R_NAND2 :
    case MAOI1_ORN2L_NAND2  :
    case MAOI1_NAND3        :
    case MOAI1_OR3          :
    case MAOI1_NOR2_OR2     :
    case MOAI1_OR2_OR2      :
    case MAOI1_NOR3         :
    case MOAI1_ORN2R_NOR2   :
    case MAOI1_ANDN2L_NOR2  :
          for(int k = 0; k < N; k++)
          {
            for(int j = 0; j < N; j++)
            {
              for(int l = 0; l < N; l++)
              {
                switch(operation)
                {
                  case XOR2_AND2_AND2     :
                  case MAOI1_AND3         :
                  case MOAI1_NAND2_AND2   :
                  case MAOI1_AND2_AND2    :
                  case MOAI1_ORN2L_NAND2  :
                  case MAOI1_ANDN2R_NAND2 :
                  case MOAI1_NAND3        : a =  (fun_prev.bit_slice[k] & fun_prev.bit_slice[j] & fun_prev.bit_slice[l]) ^ fun_prev.bit_slice[tmp]; break;
  
                  case XOR2_OR2_OR2       :       
                  case MAOI1_OR3          :       
                  case MOAI1_NOR2_OR2     :       
                  case MAOI1_OR2_OR2      :       
                  case MOAI1_ANDN2L_NOR2  :  
                  case MAOI1_ORN2R_NOR2   :
                  case MOAI1_NOR3         : a =  (fun_prev.bit_slice[k] | fun_prev.bit_slice[j] | fun_prev.bit_slice[l]) ^ fun_prev.bit_slice[tmp]; break;

                  case MOAI1_AND3         :      
                  case MAOI1_NAND2_AND2   :      
                  case MOAI1_AND2_AND2    :      
                  case MOAI1_ANDN2R_NAND2 :
                  case MAOI1_ORN2L_NAND2  :      
                  case MAOI1_NAND3        : a = ~(fun_prev.bit_slice[k] & fun_prev.bit_slice[j] & fun_prev.bit_slice[l]) ^ fun_prev.bit_slice[tmp]; break;

                  case MOAI1_OR3          :      
                  case MAOI1_NOR2_OR2     :      
                  case MOAI1_OR2_OR2      :      
                  case MAOI1_ANDN2L_NOR2  :
                  case MOAI1_ORN2R_NOR2   : 
                  case MAOI1_NOR3         : a = ~(fun_prev.bit_slice[k] | fun_prev.bit_slice[j] | fun_prev.bit_slice[l]) ^ fun_prev.bit_slice[tmp]; break;
                }
                if(!reverse) b = f.bit_slice[f.info_line];
                else b = f.prev;
                if( a == b )
                {
                  write_c(&s, {tmp, l, j, k});
                  return s;
                }
              }
            }
          }
  }
  return s;
}

template<int N>
string lighter<N>::bool_op_super_super_gates(const function_t<N> f, int operation, string s, bool reverse)
{
  int tmp;
  function_t<N> fun_prev = f;
  if(!reverse)
  {
    fun_prev.bit_slice[f.info_line] = f.prev;
    for(int i = 0; i < N; i++) if(fun_prev.bit_slice[i] == f.prev) tmp = i;
  }
  else tmp = f.info_line;

  bit_slice_l_t<N> a, b;
  int op1, op2, op3;
  for(int k = 0; k < N; k++)
  {
    for(int j = 0; j < N; j++)
    {
      for(int l = 0; l < N; l++)
      {
        switch(operation)
        {
          case MAOI1_NAND2_NAND2   :
          case MOAI1_AND2_NAND2    : a = ~( fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_NOR2_NOR2     :    
          case MOAI1_OR2_NOR2      : a = ~( fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_AND2_NAND2    :    
          case MOAI1_NAND2_NAND2   : a =  ( fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_OR2_NOR2      :    
          case MOAI1_NOR2_NOR2     : a =  ( fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;            
          case XOR2_AND2_OR2       :    
          case MAOI1_AND2_OR2      :    
          case MOAI1_NAND2_OR2     : a =  ( fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case XOR2_OR2_AND2       :     
          case MAOI1_OR2_AND2      :     
          case MOAI1_NOR2_AND2     : a =  ( fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_AND2_NOR2     :    
          case MOAI1_NAND2_NOR2    : a =  ( fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_OR2_NAND2     :     
          case MOAI1_NOR2_NAND2    : a =  ( fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_NAND2_OR2     :    
          case MOAI1_AND2_OR2      : a = ~( fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_NOR2_AND2     :     
          case MOAI1_OR2_AND2      : a = ~( fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_NAND2_NOR2    :    
          case MOAI1_AND2_NOR2     : a = ~( fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
          case MAOI1_NOR2_NAND2    :    
          case MOAI1_OR2_NAND2     : a = ~( fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
  
	        case MOAI1_NAND2_ANDN2L  :
          case MAOI1_AND2_ANDN2L   :
          case XOR2_AND2_ANDN2L    : a =  (  fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_NAND2_ANDN2R  :
      	  case MAOI1_AND2_ANDN2R   : 
          case XOR2_AND2_ANDN2R    : a =  (  fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_NAND2_ORN2L   :
      	  case MAOI1_AND2_ORN2L    : 
          case XOR2_AND2_ORN2L     : a =  (  fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_NAND2_ORN2R   :
      	  case MAOI1_AND2_ORN2R    : 
          case XOR2_AND2_ORN2R     : a =  (  fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
 
	        case MOAI1_AND2_ANDN2L   :
      	  case MAOI1_NAND2_ANDN2L  : a = ~(  fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_AND2_ANDN2R   :
      	  case MAOI1_NAND2_ANDN2R  : a = ~(  fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_AND2_ORN2L    :
      	  case MAOI1_NAND2_ORN2L   : a = ~(  fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_AND2_ORN2R    :
      	  case MAOI1_NAND2_ORN2R   : a = ~(  fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;

	        case MOAI1_NOR2_ANDN2L   :
      	  case MAOI1_OR2_ANDN2L    :
          case XOR2_OR2_ANDN2L     : a =  (  fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_NOR2_ANDN2R   :
      	  case MAOI1_OR2_ANDN2R    : 
          case XOR2_OR2_ANDN2R     : a =  (  fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_NOR2_ORN2L    :
      	  case MAOI1_OR2_ORN2L     : 
          case XOR2_OR2_ORN2L      : a =  (  fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_NOR2_ORN2R    :
      	  case MAOI1_OR2_ORN2R     : 
          case XOR2_OR2_ORN2R      : a =  (  fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;

	        case MOAI1_OR2_ANDN2L    :
      	  case MAOI1_NOR2_ANDN2L   : a = ~(  fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_OR2_ANDN2R    :
      	  case MAOI1_NOR2_ANDN2R   : a = ~(  fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_OR2_ORN2L     :
      	  case MAOI1_NOR2_ORN2L    : a = ~(  fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_OR2_ORN2R     :
      	  case MAOI1_NOR2_ORN2R    : a = ~(  fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;

      	  case MAOI1_ANDN2L_AND2   : 
          case XOR2_ANDN2L_AND2    : a =  ( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ANDN2L_OR2    : 
          case XOR2_ANDN2L_OR2     : a =  ( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ANDN2L_ANDN2L : 
          case XOR2_ANDN2L_ANDN2L  : a =  ( ~fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
      	  case MAOI1_ANDN2L_ANDN2R : 
          case XOR2_ANDN2L_ANDN2R  : a =  ( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ANDN2L_ORN2L  : 
          case XOR2_ANDN2L_ORN2L   : a =  ( ~fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
      	  case MAOI1_ANDN2L_ORN2R  : 
          case XOR2_ANDN2L_ORN2R   : a =  ( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ANDN2R_AND2   : 
          case XOR2_ANDN2R_AND2    : a =  (  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
      	  case MAOI1_ANDN2R_OR2    : 
          case XOR2_ANDN2R_OR2     : a =  (  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
      	  case MAOI1_ANDN2R_ANDN2L : 
          case XOR2_ANDN2R_ANDN2L  : a =  (  fun_prev.bit_slice[k] & ~(~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
      	  case MAOI1_ANDN2R_ANDN2R : 
          case XOR2_ANDN2R_ANDN2R  : a =  (  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
      	  case MAOI1_ANDN2R_ORN2L  : 
          case XOR2_ANDN2R_ORN2L   : a =  (  fun_prev.bit_slice[k] & ~(~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
      	  case MAOI1_ANDN2R_ORN2R  : 
          case XOR2_ANDN2R_ORN2R   : a =  (  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;

      	  case MAOI1_ANDN2L_NAND2  : a =  ( ~fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ANDN2R_NOR2   : a =  (  fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
   
      	  case MAOI1_ORN2L_AND2    : 
          case XOR2_ORN2L_AND2     : a =  ( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ORN2L_OR2     : 
          case XOR2_ORN2L_OR2      : a =  ( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ORN2L_ANDN2L  : 
          case XOR2_ORN2L_ANDN2L   : a =  ( ~fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
      	  case MAOI1_ORN2L_ANDN2R  : 
          case XOR2_ORN2L_ANDN2R   : a =  ( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ORN2L_ORN2L   : 
          case XOR2_ORN2L_ORN2L    : a =  ( ~fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
      	  case MAOI1_ORN2L_ORN2R   : 
          case XOR2_ORN2L_ORN2R    : a =  ( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break; 
      	  case MAOI1_ORN2R_AND2    : 
          case XOR2_ORN2R_AND2     : a =  (  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
       	  case MAOI1_ORN2R_OR2     : 
          case XOR2_ORN2R_OR2      : a =  (  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
      	  case MAOI1_ORN2R_ANDN2L  : 
          case XOR2_ORN2R_ANDN2L   : a =  (  fun_prev.bit_slice[k] | ~(~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
      	  case MAOI1_ORN2R_ANDN2R  : 
          case XOR2_ORN2R_ANDN2R   : a =  (  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
      	  case MAOI1_ORN2R_ORN2L   : 
          case XOR2_ORN2R_ORN2L    : a =  (  fun_prev.bit_slice[k] | ~(~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
      	  case MAOI1_ORN2R_ORN2R   : 
          case XOR2_ORN2R_ORN2R    : a =  (  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;

      	  case MAOI1_ORN2L_NOR2    : a =  ( ~fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
      	  case MAOI1_ORN2R_NAND2   : a =  (  fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;

	        case MOAI1_ANDN2L_AND2   : a = ~( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ANDN2L_OR2    : a = ~( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ANDN2L_NAND2  : a = ~( ~fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ANDN2L_ANDN2L : a = ~( ~fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
	        case MOAI1_ANDN2L_ANDN2R : a = ~( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ANDN2L_ORN2L  : a = ~( ~fun_prev.bit_slice[k] &  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
	        case MOAI1_ANDN2L_ORN2R  : a = ~( ~fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ANDN2R_AND2   : a = ~(  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ANDN2R_OR2    : a = ~(  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ANDN2R_NOR2   : a = ~(  fun_prev.bit_slice[k] &  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ANDN2R_ANDN2L : a = ~(  fun_prev.bit_slice[k] & ~(~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_ANDN2R_ANDN2R : a = ~(  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ANDN2R_ORN2L  : a = ~(  fun_prev.bit_slice[k] & ~(~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_ANDN2R_ORN2R  : a = ~(  fun_prev.bit_slice[k] & ~( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ORN2L_AND2    : a = ~( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ORN2L_OR2     : a = ~( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ORN2L_NOR2    : a = ~( ~fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ORN2L_ANDN2L  : a = ~( ~fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
	        case MOAI1_ORN2L_ANDN2R  : a = ~( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break;
	        case MOAI1_ORN2L_ORN2L   : a = ~( ~fun_prev.bit_slice[k] |  (~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = l; op2 = j; op1 = k; break;
	        case MOAI1_ORN2L_ORN2R   : a = ~( ~fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = j; op2 = l; op1 = k; break; 
	        case MOAI1_ORN2R_AND2    : a = ~(  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ORN2R_OR2     : a = ~(  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ORN2R_NAND2   : a = ~(  fun_prev.bit_slice[k] |  ( fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ORN2R_ANDN2L  : a = ~(  fun_prev.bit_slice[k] | ~(~fun_prev.bit_slice[j] &  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_ORN2R_ANDN2R  : a = ~(  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] & ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
	        case MOAI1_ORN2R_ORN2L   : a = ~(  fun_prev.bit_slice[k] | ~(~fun_prev.bit_slice[j] |  fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = l; op1 = j; break;
	        case MOAI1_ORN2R_ORN2R   : a = ~(  fun_prev.bit_slice[k] | ~( fun_prev.bit_slice[j] | ~fun_prev.bit_slice[l]) ) ^ fun_prev.bit_slice[tmp]; op3 = k; op2 = j; op1 = l; break;
        }
        if(!reverse) b = f.bit_slice[f.info_line];
        else b = f.prev;
        if( a == b )
        {
          write_c(&s, {tmp, op1, op2, op3});
          return s;
        }
      }
    }
  }
  return s;
}

template<int N>
string lighter<N>::bool_op_base_string(uint8_t op)
{
    string feq   = "F[] = ";
    string ff    = "F[], F[])";
    string f     = ", F[], ";
    string and2  = "AND2 (";
    string nand2 = "NAND2(";
    string or2   = "OR2  (";
    string nor2  = "NOR2 (";
    string nor3  = "NOR3 (F[], F[], F[])";
    string nand3 = "NAND3(F[], F[], F[])";
    string or3   = "OR3  (F[], F[], F[])";
    string and3  = "AND3 (F[], F[], F[])";
    string maoi1 = "MAOI1(F[], ";
    string moai1 = "MOAI1(F[], ";
    string xor2  = "XOR2 (F[], ";
    string andn2 = "ANDN2(";
    string orn2  = "ORN2 (";
    string tmp1, tmp2, tmp3;

    switch (op)
    {
      case NOT1                : return feq+"NOT1(F[]);\n";
      case XOR2                : return feq+"XOR2 (F[], F[]);\n";
      case XNOR2               : return feq+"XNOR2(F[], F[]);\n";
      case MAOI1               : return feq+"MAOI1(F[], F[] , F[] , F[]);\n";
      case MOAI1               : return feq+"MOAI1(F[], F[] , F[] , F[]);\n";
      case MOAI1_NAND2         : return feq+moai1+nand2+ff+f+nand2+ff+");\n";
      case MOAI1_NOR2          : return feq+moai1+nor2+ff+f+nor2+ff+");\n";
      case MOAI1_AND2          : return feq+moai1+and2+ff+f+and2+ff+");\n";
      case MOAI1_OR2           : return feq+moai1+or2+ff+f+or2+ff+");\n";
      case MOAI1_NAND3         : return feq+moai1+nand3+f+nand3+");\n";
      case MOAI1_NOR3          : return feq+moai1+nor3+f+nor3+");\n";
      case MAOI1_NAND3         : return feq+maoi1+nand3+f+nand3+");\n";
      case MAOI1_NOR3          : return feq+maoi1+nor3+f+nor3+");\n";
      case MOAI1_ANDN2L        :
      case MOAI1_ANDN2R        : return feq+moai1+andn2+ff+f+andn2+ff+");\n";
      case MOAI1_ORN2L         :
      case MOAI1_ORN2R         : return feq+moai1+orn2+ff+f+orn2+ff+");\n";
      case MAOI1_ANDN2L        :
      case MAOI1_ANDN2R        : return feq+maoi1+andn2+ff+f+andn2+ff+");\n";
      case MAOI1_ORN2L         :
      case MAOI1_ORN2R         : return feq+maoi1+orn2+ff+f+orn2+ff+");\n";
      case XOR2_AND2           : return feq+xor2+and2+ff+");\n";
      case XOR2_OR2            : return feq+xor2+or2+ff+");\n";
      case XOR2_ANDN2L         :
      case XOR2_ANDN2R         : return feq+xor2+andn2+ff+");\n";
      case XOR2_ORN2L          :
      case XOR2_ORN2R          : return feq+xor2+orn2+ff+");\n";
      case XOR2_AND2_AND2      : return feq+xor2+and2+and2+ff+", F[]));\n"; break;
      case XOR2_AND2_OR2       : return feq+xor2+and2+or2+ff+", F[]));\n"; break;
      case XOR2_AND2_ANDN2L    :
      case XOR2_AND2_ANDN2R    : return feq+xor2+and2+andn2+ff+", F[]));\n"; break;
      case XOR2_AND2_ORN2L     :
      case XOR2_AND2_ORN2R     : return feq+xor2+and2+orn2+ff+", F[]));\n"; break;
      case XOR2_OR2_AND2       : return feq+xor2+or2+and2+ff+", F[]));\n"; break;
      case XOR2_OR2_OR2        : return feq+xor2+or2+or2+ff+", F[]));\n"; break;
      case XOR2_OR2_ANDN2L     :
      case XOR2_OR2_ANDN2R     : return feq+xor2+or2+andn2+ff+", F[]));\n"; break;
      case XOR2_OR2_ORN2L      :
      case XOR2_OR2_ORN2R      : return feq+xor2+or2+orn2+ff+", F[]));\n"; break;
      case XOR2_ANDN2L_AND2    : return feq+xor2+andn2+"F[], "+and2+ff+"));\n"; break;
      case XOR2_ANDN2R_AND2    : return feq+xor2+andn2+and2+ff+", F[]));\n"; break;
      case XOR2_ANDN2L_OR2     : return feq+xor2+andn2+"F[], "+or2+ff+"));\n"; break;
      case XOR2_ANDN2R_OR2     : return feq+xor2+andn2+or2+ff+", F[]));\n"; break;
      case XOR2_ANDN2L_ANDN2L  :
      case XOR2_ANDN2L_ANDN2R  : return feq+xor2+andn2+"F[], "+andn2+ff+"));\n"; break;
      case XOR2_ANDN2R_ANDN2L  :
      case XOR2_ANDN2R_ANDN2R  : return feq+xor2+andn2+andn2+ff+", F[]));\n"; break;
      case XOR2_ANDN2L_ORN2L   :
      case XOR2_ANDN2L_ORN2R   : return feq+xor2+andn2+"F[], "+orn2+ff+"));\n"; break;
      case XOR2_ANDN2R_ORN2L   :
      case XOR2_ANDN2R_ORN2R   : return feq+xor2+andn2+orn2+ff+", F[]));\n"; break;
      case XOR2_ORN2L_AND2     : return feq+xor2+orn2+"F[], "+and2+ff+"));\n"; break;
      case XOR2_ORN2R_AND2     : return feq+xor2+orn2+and2+ff+", F[]));\n"; break;
      case XOR2_ORN2L_OR2      : return feq+xor2+orn2+"F[], "+or2+ff+"));\n"; break;
      case XOR2_ORN2R_OR2      : return feq+xor2+orn2+or2+ff+", F[]));\n"; break;
      case XOR2_ORN2L_ANDN2L   :
      case XOR2_ORN2L_ANDN2R   : return feq+xor2+orn2+"F[], "+andn2+ff+"));\n"; break;
      case XOR2_ORN2R_ANDN2L   :
      case XOR2_ORN2R_ANDN2R   : return feq+xor2+orn2+andn2+ff+", F[]));\n"; break;
      case XOR2_ORN2L_ORN2L    :
      case XOR2_ORN2L_ORN2R    : return feq+xor2+orn2+"F[], "+orn2+ff+"));\n"; break;
      case XOR2_ORN2R_ORN2L    :
      case XOR2_ORN2R_ORN2R    : return feq+xor2+orn2+orn2+ff+", F[]));\n"; break;
      case MAOI1_NAND2         : return feq+maoi1+nand2+ff+f+nand2+ff+");\n";
      case MAOI1_NOR2          : return feq+maoi1+nor2+ff+f+nor2+ff+");\n";
      case MAOI1_AND2          : return feq+maoi1+and2+ff+f+and2+ff+");\n";
      case MAOI1_OR2           : return feq+maoi1+or2+ff+f+or2+ff+");\n";
      case MAOI1_AND3          : return feq+maoi1+and3+f+and3+");\n";
      case MAOI1_OR3           : return feq+maoi1+or3+f+or3+");\n";
      case MOAI1_AND3          : return feq+moai1+and3+f+and3+");\n";
      case MOAI1_OR3           : return feq+moai1+or3+f+or3+");\n";
      case MOAI1_AND2_NAND2    : tmp1 = and2;   tmp2 = nand2;  tmp3 = moai1;break;
      case MOAI1_OR2_NOR2      : tmp1 = or2;    tmp2 = nor2;   tmp3 = moai1;break;
      case MOAI1_NAND2_NAND2   : tmp1 = nand2;  tmp2 = nand2;  tmp3 = moai1;break;
      case MOAI1_NOR2_NOR2     : tmp1 = nor2;   tmp2 = nor2;   tmp3 = moai1;break;
      case MOAI1_NAND2_OR2     : tmp1 = nand2;  tmp2 = or2;    tmp3 = moai1;break;
      case MOAI1_NOR2_AND2     : tmp1 = nor2;   tmp2 = and2;   tmp3 = moai1; break;
      case MOAI1_NAND2_NOR2    : tmp1 = nand2;  tmp2 = nor2;   tmp3 = moai1; break;
      case MOAI1_NOR2_NAND2    : tmp1 = nor2;   tmp2 = nand2;  tmp3 = moai1; break;
      case MOAI1_AND2_OR2      : tmp1 = and2;   tmp2 = or2;    tmp3 = moai1; break;
      case MOAI1_OR2_AND2      : tmp1 = or2;    tmp2 = and2;   tmp3 = moai1; break;
      case MOAI1_AND2_NOR2     : tmp1 = and2;   tmp2 = nor2;   tmp3 = moai1; break;
      case MOAI1_OR2_NAND2     : tmp1 = or2;    tmp2 = nand2;  tmp3 = moai1; break;
      case MAOI1_NAND2_NAND2   : tmp1 = nand2 ; tmp2 = nand2;  tmp3 = maoi1;break;
      case MAOI1_NOR2_NOR2     : tmp1 = nor2 ;  tmp2 = nor2;   tmp3 = maoi1; break;
      case MAOI1_AND2_NAND2    : tmp1 = and2;   tmp2 = nand2;  tmp3 = maoi1; break;
      case MAOI1_OR2_NOR2      : tmp1 = or2;    tmp2 = nor2;   tmp3 = maoi1; break;
      case MAOI1_AND2_OR2      : tmp1 = and2;   tmp2 = or2;    tmp3 = maoi1; break;
      case MAOI1_OR2_AND2      : tmp1 = or2;    tmp2 = and2;   tmp3 = maoi1;break;
      case MAOI1_AND2_NOR2     : tmp1 = and2;   tmp2 = nor2;   tmp3 = maoi1; break;
      case MAOI1_OR2_NAND2     : tmp1 = or2;    tmp2 = nand2;  tmp3 = maoi1; break;
      case MAOI1_NAND2_OR2     : tmp1 = nand2;  tmp2 = or2;    tmp3 = maoi1;break;
      case MAOI1_NOR2_AND2     : tmp1 = nor2;   tmp2 = and2;   tmp3 = maoi1;break;
      case MAOI1_NAND2_NOR2    : tmp1 = nand2;  tmp2 = nor2;   tmp3 = maoi1;break;
      case MAOI1_NOR2_NAND2    : tmp1 = nor2;   tmp2 = nand2;  tmp3 = maoi1;break;
      case MOAI1_NAND2_AND2    : tmp1 = nand2;  tmp2 = and2;   tmp3 = moai1; break;
      case MAOI1_AND2_AND2     : tmp1 = and2;   tmp2 = and2;   tmp3 = maoi1; break;
      case MOAI1_NOR2_OR2      : tmp1 = nor2;   tmp2 = or2;    tmp3 = moai1; break;
      case MAOI1_OR2_OR2       : tmp1 = or2;    tmp2 = or2;    tmp3 = maoi1; break;
      case MAOI1_NAND2_AND2    : tmp1 = nand2;  tmp2 = and2;   tmp3 = maoi1; break;
      case MOAI1_AND2_AND2     : tmp1 = and2;   tmp2 = and2;   tmp3 = moai1; break;
      case MAOI1_NOR2_OR2      : tmp1 = nor2;   tmp2 = or2;    tmp3 = maoi1; break;
      case MOAI1_OR2_OR2       : tmp1 = or2;    tmp2 = or2;    tmp3 = moai1; break;

      case MOAI1_AND2_ANDN2L   : return feq + moai1 + and2   + andn2  + ff + ", F[])" + f + and2   + andn2  + ff + ", F[]));\n";
      case MOAI1_AND2_ANDN2R   : return feq + moai1 + and2   + andn2  + ff + ", F[])" + f + and2   + andn2  + ff + ", F[]));\n";
      case MOAI1_AND2_ORN2L    : return feq + moai1 + and2   + orn2   + ff + ", F[])" + f + and2   + orn2   + ff + ", F[]));\n";
      case MOAI1_AND2_ORN2R    : return feq + moai1 + and2   + orn2   + ff + ", F[])" + f + and2   + orn2   + ff + ", F[]));\n";
      case MOAI1_OR2_ANDN2L    : return feq + moai1 + or2    + andn2  + ff + ", F[])" + f + or2    + andn2  + ff + ", F[]));\n";
      case MOAI1_OR2_ANDN2R    : return feq + moai1 + or2    + andn2  + ff + ", F[])" + f + or2    + andn2  + ff + ", F[]));\n";
      case MOAI1_OR2_ORN2L     : return feq + moai1 + or2    + orn2   + ff + ", F[])" + f + or2    + orn2   + ff + ", F[]));\n";
      case MOAI1_OR2_ORN2R     : return feq + moai1 + or2    + orn2   + ff + ", F[])" + f + or2    + orn2   + ff + ", F[]));\n";
      case MOAI1_NAND2_ANDN2L  : return feq + moai1 + nand2  + andn2  + ff + ", F[])" + f + nand2  + andn2  + ff + ", F[]));\n";
      case MOAI1_NAND2_ANDN2R  : return feq + moai1 + nand2  + andn2  + ff + ", F[])" + f + nand2  + andn2  + ff + ", F[]));\n";
      case MOAI1_NAND2_ORN2L   : return feq + moai1 + nand2  + orn2   + ff + ", F[])" + f + nand2  + orn2   + ff + ", F[]));\n";
      case MOAI1_NAND2_ORN2R   : return feq + moai1 + nand2  + orn2   + ff + ", F[])" + f + nand2  + orn2   + ff + ", F[]));\n";
      case MOAI1_NOR2_ANDN2L   : return feq + moai1 + nor2   + andn2  + ff + ", F[])" + f + nor2   + andn2  + ff + ", F[]));\n";
      case MOAI1_NOR2_ANDN2R   : return feq + moai1 + nor2   + andn2  + ff + ", F[])" + f + nor2   + andn2  + ff + ", F[]));\n";
      case MOAI1_NOR2_ORN2L    : return feq + moai1 + nor2   + orn2   + ff + ", F[])" + f + nor2   + orn2   + ff + ", F[]));\n";
      case MOAI1_NOR2_ORN2R    : return feq + moai1 + nor2   + orn2   + ff + ", F[])" + f + nor2   + orn2   + ff + ", F[]));\n";
      case MOAI1_ANDN2R_AND2   : return feq + moai1 + andn2  + and2   + ff + ", F[])" + f + andn2  + and2   + ff + ", F[]));\n";
      case MOAI1_ANDN2R_OR2    : return feq + moai1 + andn2  + or2    + ff + ", F[])" + f + andn2  + or2    + ff + ", F[]));\n";
      case MOAI1_ANDN2R_NAND2  : return feq + moai1 + andn2  + nand2  + ff + ", F[])" + f + andn2  + nand2  + ff + ", F[]));\n";
      case MOAI1_ANDN2R_NOR2   : return feq + moai1 + andn2  + nor2   + ff + ", F[])" + f + andn2  + nor2   + ff + ", F[]));\n";
      case MOAI1_ANDN2R_ANDN2L : return feq + moai1 + andn2  + andn2  + ff + ", F[])" + f + andn2  + andn2  + ff + ", F[]));\n";
      case MOAI1_ANDN2R_ANDN2R : return feq + moai1 + andn2  + andn2  + ff + ", F[])" + f + andn2  + andn2  + ff + ", F[]));\n";
      case MOAI1_ANDN2R_ORN2L  : return feq + moai1 + andn2  + orn2   + ff + ", F[])" + f + andn2  + orn2   + ff + ", F[]));\n";
      case MOAI1_ANDN2R_ORN2R  : return feq + moai1 + andn2  + orn2   + ff + ", F[])" + f + andn2  + orn2   + ff + ", F[]));\n";
      case MOAI1_ORN2R_AND2    : return feq + moai1 + orn2   + and2   + ff + ", F[])" + f + orn2   + and2   + ff + ", F[]));\n";
      case MOAI1_ORN2R_OR2     : return feq + moai1 + orn2   + or2    + ff + ", F[])" + f + orn2   + or2    + ff + ", F[]));\n";
      case MOAI1_ORN2R_NAND2   : return feq + moai1 + orn2   + nand2  + ff + ", F[])" + f + orn2   + nand2  + ff + ", F[]));\n";
      case MOAI1_ORN2R_NOR2    : return feq + moai1 + orn2   + nor2   + ff + ", F[])" + f + orn2   + nor2   + ff + ", F[]));\n";
      case MOAI1_ORN2R_ANDN2L  : return feq + moai1 + orn2   + andn2  + ff + ", F[])" + f + orn2   + andn2  + ff + ", F[]));\n";
      case MOAI1_ORN2R_ANDN2R  : return feq + moai1 + orn2   + andn2  + ff + ", F[])" + f + orn2   + andn2  + ff + ", F[]));\n";
      case MOAI1_ORN2R_ORN2L   : return feq + moai1 + orn2   + orn2   + ff + ", F[])" + f + orn2   + orn2   + ff + ", F[]));\n";
      case MOAI1_ORN2R_ORN2R   : return feq + moai1 + orn2   + orn2   + ff + ", F[])" + f + orn2   + orn2   + ff + ", F[]));\n";
      case MAOI1_AND2_ANDN2L   : return feq + maoi1 + and2   + andn2  + ff + ", F[])" + f + and2   + andn2  + ff + ", F[]));\n";
      case MAOI1_AND2_ANDN2R   : return feq + maoi1 + and2   + andn2  + ff + ", F[])" + f + and2   + andn2  + ff + ", F[]));\n";
      case MAOI1_AND2_ORN2L    : return feq + maoi1 + and2   + orn2   + ff + ", F[])" + f + and2   + orn2   + ff + ", F[]));\n";
      case MAOI1_AND2_ORN2R    : return feq + maoi1 + and2   + orn2   + ff + ", F[])" + f + and2   + orn2   + ff + ", F[]));\n";
      case MAOI1_OR2_ANDN2L    : return feq + maoi1 + or2    + andn2  + ff + ", F[])" + f + or2    + andn2  + ff + ", F[]));\n";
      case MAOI1_OR2_ANDN2R    : return feq + maoi1 + or2    + andn2  + ff + ", F[])" + f + or2    + andn2  + ff + ", F[]));\n";
      case MAOI1_OR2_ORN2L     : return feq + maoi1 + or2    + orn2   + ff + ", F[])" + f + or2    + orn2   + ff + ", F[]));\n";
      case MAOI1_OR2_ORN2R     : return feq + maoi1 + or2    + orn2   + ff + ", F[])" + f + or2    + orn2   + ff + ", F[]));\n";
      case MAOI1_NAND2_ANDN2L  : return feq + maoi1 + nand2  + andn2  + ff + ", F[])" + f + nand2  + andn2  + ff + ", F[]));\n";
      case MAOI1_NAND2_ANDN2R  : return feq + maoi1 + nand2  + andn2  + ff + ", F[])" + f + nand2  + andn2  + ff + ", F[]));\n";
      case MAOI1_NAND2_ORN2L   : return feq + maoi1 + nand2  + orn2   + ff + ", F[])" + f + nand2  + orn2   + ff + ", F[]));\n";
      case MAOI1_NAND2_ORN2R   : return feq + maoi1 + nand2  + orn2   + ff + ", F[])" + f + nand2  + orn2   + ff + ", F[]));\n";
      case MAOI1_NOR2_ANDN2L   : return feq + maoi1 + nor2   + andn2  + ff + ", F[])" + f + nor2   + andn2  + ff + ", F[]));\n";
      case MAOI1_NOR2_ANDN2R   : return feq + maoi1 + nor2   + andn2  + ff + ", F[])" + f + nor2   + andn2  + ff + ", F[]));\n";
      case MAOI1_NOR2_ORN2L    : return feq + maoi1 + nor2   + orn2   + ff + ", F[])" + f + nor2   + orn2   + ff + ", F[]));\n";
      case MAOI1_NOR2_ORN2R    : return feq + maoi1 + nor2   + orn2   + ff + ", F[])" + f + nor2   + orn2   + ff + ", F[]));\n";
      case MAOI1_ANDN2R_AND2   : return feq + maoi1 + andn2  + and2   + ff + ", F[])" + f + andn2  + and2   + ff + ", F[]));\n";
      case MAOI1_ANDN2R_OR2    : return feq + maoi1 + andn2  + or2    + ff + ", F[])" + f + andn2  + or2    + ff + ", F[]));\n";
      case MAOI1_ANDN2R_NAND2  : return feq + maoi1 + andn2  + nand2  + ff + ", F[])" + f + andn2  + nand2  + ff + ", F[]));\n";
      case MAOI1_ANDN2R_NOR2   : return feq + maoi1 + andn2  + nor2   + ff + ", F[])" + f + andn2  + nor2   + ff + ", F[]));\n";
      case MAOI1_ANDN2R_ANDN2L : return feq + maoi1 + andn2  + andn2  + ff + ", F[])" + f + andn2  + andn2  + ff + ", F[]));\n";
      case MAOI1_ANDN2R_ANDN2R : return feq + maoi1 + andn2  + andn2  + ff + ", F[])" + f + andn2  + andn2  + ff + ", F[]));\n";
      case MAOI1_ANDN2R_ORN2L  : return feq + maoi1 + andn2  + orn2   + ff + ", F[])" + f + andn2  + orn2   + ff + ", F[]));\n";
      case MAOI1_ANDN2R_ORN2R  : return feq + maoi1 + andn2  + orn2   + ff + ", F[])" + f + andn2  + orn2   + ff + ", F[]));\n";
      case MAOI1_ORN2R_AND2    : return feq + maoi1 + orn2   + and2   + ff + ", F[])" + f + orn2   + and2   + ff + ", F[]));\n";
      case MAOI1_ORN2R_OR2     : return feq + maoi1 + orn2   + or2    + ff + ", F[])" + f + orn2   + or2    + ff + ", F[]));\n";
      case MAOI1_ORN2R_NAND2   : return feq + maoi1 + orn2   + nand2  + ff + ", F[])" + f + orn2   + nand2  + ff + ", F[]));\n";
      case MAOI1_ORN2R_NOR2    : return feq + maoi1 + orn2   + nor2   + ff + ", F[])" + f + orn2   + nor2   + ff + ", F[]));\n";
      case MAOI1_ORN2R_ANDN2L  : return feq + maoi1 + orn2   + andn2  + ff + ", F[])" + f + orn2   + andn2  + ff + ", F[]));\n";
      case MAOI1_ORN2R_ANDN2R  : return feq + maoi1 + orn2   + andn2  + ff + ", F[])" + f + orn2   + andn2  + ff + ", F[]));\n";
      case MAOI1_ORN2R_ORN2L   : return feq + maoi1 + orn2   + orn2   + ff + ", F[])" + f + orn2   + orn2   + ff + ", F[]));\n";
      case MAOI1_ORN2R_ORN2R   : return feq + maoi1 + orn2   + orn2   + ff + ", F[])" + f + orn2   + orn2   + ff + ", F[]));\n";
      case MOAI1_ANDN2L_AND2   : return feq + moai1 + andn2  + "F[], " + and2   + ff + ")" + f + andn2  + "F[], " + and2   + ff + "));\n";
      case MOAI1_ANDN2L_OR2    : return feq + moai1 + andn2  + "F[], " + or2    + ff + ")" + f + andn2  + "F[], " + or2    + ff + "));\n";
      case MOAI1_ANDN2L_NAND2  : return feq + moai1 + andn2  + "F[], " + nand2  + ff + ")" + f + andn2  + "F[], " + nand2  + ff + "));\n";
      case MOAI1_ANDN2L_NOR2   : return feq + moai1 + andn2  + "F[], " + nor2   + ff + ")" + f + andn2  + "F[], " + nor2   + ff + "));\n";
      case MOAI1_ANDN2L_ANDN2L : return feq + moai1 + andn2  + "F[], " + andn2  + ff + ")" + f + andn2  + "F[], " + andn2  + ff + "));\n";
      case MOAI1_ANDN2L_ANDN2R : return feq + moai1 + andn2  + "F[], " + andn2  + ff + ")" + f + andn2  + "F[], " + andn2  + ff + "));\n";
      case MOAI1_ANDN2L_ORN2L  : return feq + moai1 + andn2  + "F[], " + orn2   + ff + ")" + f + andn2  + "F[], " + orn2   + ff + "));\n";
      case MOAI1_ANDN2L_ORN2R  : return feq + moai1 + andn2  + "F[], " + orn2   + ff + ")" + f + andn2  + "F[], " + orn2   + ff + "));\n";
      case MOAI1_ORN2L_AND2    : return feq + moai1 + orn2   + "F[], " + and2   + ff + ")" + f + orn2   + "F[], " + and2   + ff + "));\n";
      case MOAI1_ORN2L_OR2     : return feq + moai1 + orn2   + "F[], " + or2    + ff + ")" + f + orn2   + "F[], " + or2    + ff + "));\n";
      case MOAI1_ORN2L_NAND2   : return feq + moai1 + orn2   + "F[], " + nand2  + ff + ")" + f + orn2   + "F[], " + nand2  + ff + "));\n";
      case MOAI1_ORN2L_NOR2    : return feq + moai1 + orn2   + "F[], " + nor2   + ff + ")" + f + orn2   + "F[], " + nor2   + ff + "));\n";
      case MOAI1_ORN2L_ANDN2L  : return feq + moai1 + orn2   + "F[], " + andn2  + ff + ")" + f + orn2   + "F[], " + andn2  + ff + "));\n";
      case MOAI1_ORN2L_ANDN2R  : return feq + moai1 + orn2   + "F[], " + andn2  + ff + ")" + f + orn2   + "F[], " + andn2  + ff + "));\n";
      case MOAI1_ORN2L_ORN2L   : return feq + moai1 + orn2   + "F[], " + orn2   + ff + ")" + f + orn2   + "F[], " + orn2   + ff + "));\n";
      case MOAI1_ORN2L_ORN2R   : return feq + moai1 + orn2   + "F[], " + orn2   + ff + ")" + f + orn2   + "F[], " + orn2   + ff + "));\n";
      case MAOI1_ANDN2L_AND2   : return feq + maoi1 + andn2  + "F[], " + and2   + ff + ")" + f + andn2  + "F[], " + and2   + ff + "));\n";
      case MAOI1_ANDN2L_OR2    : return feq + maoi1 + andn2  + "F[], " + or2    + ff + ")" + f + andn2  + "F[], " + or2    + ff + "));\n";
      case MAOI1_ANDN2L_NAND2  : return feq + maoi1 + andn2  + "F[], " + nand2  + ff + ")" + f + andn2  + "F[], " + nand2  + ff + "));\n";
      case MAOI1_ANDN2L_NOR2   : return feq + maoi1 + andn2  + "F[], " + nor2   + ff + ")" + f + andn2  + "F[], " + nor2   + ff + "));\n";
      case MAOI1_ANDN2L_ANDN2L : return feq + maoi1 + andn2  + "F[], " + andn2  + ff + ")" + f + andn2  + "F[], " + andn2  + ff + "));\n";
      case MAOI1_ANDN2L_ANDN2R : return feq + maoi1 + andn2  + "F[], " + andn2  + ff + ")" + f + andn2  + "F[], " + andn2  + ff + "));\n";
      case MAOI1_ANDN2L_ORN2L  : return feq + maoi1 + andn2  + "F[], " + orn2   + ff + ")" + f + andn2  + "F[], " + orn2   + ff + "));\n";
      case MAOI1_ANDN2L_ORN2R  : return feq + maoi1 + andn2  + "F[], " + orn2   + ff + ")" + f + andn2  + "F[], " + orn2   + ff + "));\n";
      case MAOI1_ORN2L_AND2    : return feq + maoi1 + orn2   + "F[], " + and2   + ff + ")" + f + orn2   + "F[], " + and2   + ff + "));\n";
      case MAOI1_ORN2L_OR2     : return feq + maoi1 + orn2   + "F[], " + or2    + ff + ")" + f + orn2   + "F[], " + or2    + ff + "));\n";
      case MAOI1_ORN2L_NAND2   : return feq + maoi1 + orn2   + "F[], " + nand2  + ff + ")" + f + orn2   + "F[], " + nand2  + ff + "));\n";
      case MAOI1_ORN2L_NOR2    : return feq + maoi1 + orn2   + "F[], " + nor2   + ff + ")" + f + orn2   + "F[], " + nor2   + ff + "));\n";
      case MAOI1_ORN2L_ANDN2L  : return feq + maoi1 + orn2   + "F[], " + andn2  + ff + ")" + f + orn2   + "F[], " + andn2  + ff + "));\n";
      case MAOI1_ORN2L_ANDN2R  : return feq + maoi1 + orn2   + "F[], " + andn2  + ff + ")" + f + orn2   + "F[], " + andn2  + ff + "));\n";
      case MAOI1_ORN2L_ORN2L   : return feq + maoi1 + orn2   + "F[], " + orn2   + ff + ")" + f + orn2   + "F[], " + orn2   + ff + "));\n";
      case MAOI1_ORN2L_ORN2R   : return feq + maoi1 + orn2   + "F[], " + orn2   + ff + ")" + f + orn2   + "F[], " + orn2   + ff + "));\n";
    }
    return feq+tmp3+tmp1+tmp2+ff+", F[])"+f+ tmp1+tmp2+ff+", F[]));\n";
}

template<int N>
string lighter<N>::bool_op_to_string(const function_t<N> f, uint8_t op, bool reverse)
{
  switch (op)
  {
    case  NOT1 :
    return bool_op_not(f, bool_op_base_string(op), reverse);

    case XOR2  :
    case XNOR2 :
    case MAOI1 :
    case MOAI1 :
    return bool_op_two_inputs(f, op, bool_op_base_string(op), reverse);

    case MOAI1_NAND2          :
    case MOAI1_NOR2           :
    case MOAI1_AND2           :
    case MOAI1_OR2            :
    case MAOI1_NAND3          :
    case MAOI1_NOR3           :
    case MOAI1_NAND3          :
    case MOAI1_NOR3           :
    case XOR2_AND2            :
    case XOR2_OR2             :
    case MAOI1_ANDN2L         :
    case MAOI1_ANDN2R         :
    case MAOI1_ORN2L          :
    case MAOI1_ORN2R          :
    case MOAI1_ANDN2L         :
    case MOAI1_ANDN2R         :
    case MOAI1_ORN2L          :
    case MOAI1_ORN2R          :
    case XOR2_ANDN2L          :
    case XOR2_ANDN2R          :
    case XOR2_ORN2L           :
    case XOR2_ORN2R           :
    case XOR2_OR2_OR2         :

    case MOAI1_NAND2_AND2     :
    case MAOI1_AND2_AND2      :
    case MOAI1_ORN2L_NAND2    :
    case MAOI1_ANDN2R_NAND2   :
    case XOR2_AND2_AND2       :
 
    case MAOI1_NAND2          :
    case MAOI1_NOR2           :
    case MAOI1_AND2           :
    case MAOI1_OR2            :
    case MAOI1_AND3           :
   
    case MAOI1_OR3            :
    case MOAI1_NOR2_OR2       :
    case MAOI1_OR2_OR2        :
    case MOAI1_AND3           :
    case MAOI1_NAND2_AND2     :
    case MOAI1_AND2_AND2      :
    case MOAI1_OR3            :
    case MAOI1_NOR2_OR2       :
    case MOAI1_OR2_OR2        : 
    case MOAI1_ANDN2L_NOR2    : 
    case MAOI1_ORN2R_NOR2     :  
    case MOAI1_ORN2R_NOR2     : 
    case MAOI1_ANDN2L_NOR2    :
    case MOAI1_ANDN2R_NAND2   :
    case MAOI1_ORN2L_NAND2    :
    return bool_op_super_gates(f, op, bool_op_base_string(op), reverse);

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
                              return bool_op_super_super_gates(f, op, bool_op_base_string(op), reverse);
  }
  return "";
}


#endif // LIGHTER_STRING_BOOL_OP_H__

