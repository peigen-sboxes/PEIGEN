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

#ifndef FASTER_IMPL_INFO_H__
#define FASTER_IMPL_INFO_H__

using namespace std;
using namespace Peigen::depth;

template<int N>
string faster<N>::get_implementation(bool_function_t<N> f1)
{
  string s = "";
  uint8_t op = f1.info_op;

  switch(op)
  {
    case NOT1  : 
                s = bool_op_str(op) + "( "+ get_implementation(*f1.operands[0]) + " ) ";
                area_sum.insert(f1.operands[0]);
                break;
    case AND2  :
    case NAND2 :
    case OR2   :
    case NOR2  :
    case XOR2  :
    case XNOR2 :
    case ANDN2 :
    case ORN2  :
    case MOAI1 :
    case MAOI1 : 
                s = bool_op_str(op) + "( "+ 
                      get_implementation(*f1.operands[0]) + ", " + 
                      get_implementation(*f1.operands[1]) + " ) "; 
                area_sum.insert(f1.operands[0]);
                area_sum.insert(f1.operands[1]);
                break; 
    case NAND3 :
    case NOR3  :
    case OR3   :
    case AND3  :
                s = bool_op_str(op) + "( "+ 
                      get_implementation(*f1.operands[0]) + ", " + 
                      get_implementation(*f1.operands[1]) + ", " + 
                      get_implementation(*f1.operands[2]) + " ) ";
                area_sum.insert(f1.operands[0]);
                area_sum.insert(f1.operands[1]);
                break; 
    case NOP   :
                for (int i = 0; i < N; i++)
                {
                  if (f1 == start.coordinates[i])
                  {
                    s = "X[" + to_string(i) + "]";
                    break;
                  }
                }
   }
   return s;
}

template<int N>
void faster<N>::get_implementation()
{
  int depth_cost = 0;
  int area_cost = 0;
  area_sum.clear();
  for (int i = 0; i < N; i++)
  {
    depth_cost = target.depth[i] >= depth_cost ? target.depth[i] : depth_cost;
    area_cost += target.coordinates[i].area;
  }

  if ((depth_cost < shortest_path) || ((depth_cost == shortest_path) && (area_cost < smallest_area)))
  {
    shortest_path = depth_cost;
    smallest_area = area_cost;

    static int impl_number = 0;
    if(instance_name != old_instance_name)
    {
      impl_number = 0;
      old_instance_name = instance_name;
    }

    cout << "Generating implementation " << instance_name << " " << impl_number << endl;

    ofstream impl(imp_info + "_" + instance_name + "_" + to_string(impl_number) + ".c");
    impl << "// from : " + start.to_string() + "\n";
    area_cost = 0;
    for (int i = 0; i < N; i++)
    {
      area_cost += bool_op_cost(target.coordinates[i].info_op);
      impl << "F[" << i << "] = " << get_implementation(target.coordinates[i]) << ";" << endl;     
    }
    
    for (auto it : area_sum)
    {
      area_cost += bool_op_cost((*it).info_op);
    }
    smallest_area = area_cost;

    impl << "// to : " + target.to_string() + "\n";
    impl << "// Depth : " << double(depth_cost)/100 << endl;
    impl << "// Area : " << double(area_cost)/100 << endl;
    cout << "Depth : " << double(depth_cost)/100 << "    Area : " << double(area_cost)/100 << endl;
    impl.close();
    impl_number ++;
  }
}

#endif // #ifndef FASTER_IMPL_INFO_H__