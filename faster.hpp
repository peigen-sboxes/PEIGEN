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

#ifndef FASTER_H__
#define FASTER_H__

#include "faster_func.hpp"

using namespace Peigen::depth;
using namespace std;

template<int N>
class Peigen::depth::faster
{
public:
    int pre_l = -1;

    string imp_info = "";
    ofstream statistician;
    string old_instance_name = "";
    string instance_name = "";

    int max_GE = 0;
    vector<bool_op_t> b; // The boolean instructions (B in the paper)

    map<int, set<bool_function_t<N>> > f1_succ;
    
    function_t<N> start;
    function_t<N> target;

    uint32_t flag = 0UL;

    int count_list;
    int shortest_path = MAX_COST;
    int smallest_area = MAX_COST;
    set<const bool_function_t<N>* > area_sum;

    bool verbose = false;
    int l = 1200;

    bool write_in_file = false;
    int omp_nb_threads = 20;
    string conf_file = "TSMC65nm.conf";
    long max_ram = 160;
    long nodes_cmp = 0;
    long max_nodes;
    int cost_1;
    
    int gate_not1  = false;
    int gate_and2  = false;
    int gate_nand2 = false;
    int gate_or2   = false;
    int gate_nor2  = false;
    int gate_xor2  = false;
    int gate_xnor2 = false;
    int gate_moai1 = false;
    int gate_maoi1 = false;
    int gate_nand3 = false;
    int gate_nor3  = false;
    int gate_or3   = false;
    int gate_and3  = false;
    int gate_andn2  = false;
    int gate_orn2   = false;

    struct option longopts[16] = {
    { "not1", no_argument, &gate_not1, 1},
    { "and2", no_argument, &gate_and2, 1},
    { "nand2", no_argument, &gate_nand2, 1},
    { "or2", no_argument, &gate_or2, 1},
    { "nor2", no_argument, &gate_nor2, 1},
    { "xor2", no_argument, &gate_xor2, 1},
    { "xnor2", no_argument, &gate_xnor2, 1},
    { "moai1", no_argument, &gate_moai1, 1},
    { "maoi1", no_argument, &gate_maoi1, 1},
    { "nand3", no_argument, &gate_nand3, 1},
    { "nor3", no_argument, &gate_nor3, 1},
    { "and3", no_argument, &gate_and3, 1},
    { "or3", no_argument, &gate_or3, 1},
    { "andn2", no_argument, &gate_andn2, 1},
    { "orn2", no_argument, &gate_orn2, 1},
    {0, 0, 0, 0}
    };
    
    int not1_cost;
    int and2_cost;
    int nand2_cost;
    int or2_cost;
    int nor2_cost;
    int xor2_cost;
    int xnor2_cost;
    int maoi1_cost;
    int moai1_cost;
    int nand3_cost;
    int nor3_cost;
    int and3_cost;
    int or3_cost;
    int andn2_cost;
    int orn2_cost;

    void init_varbles()
    {
    	shortest_path = MAX_COST;
      smallest_area = MAX_COST;
      area_sum.clear();
      if (pre_l < 0)
      {
        f1_succ.clear();
      }
      flag = 0UL;
    }

    void reset()
    {
      pre_l = -1;
      imp_info = "";
      instance_name = "";
      old_instance_name = "";

      start = function_t<N>::INPUT_DEFAULT();
      target = function_t<N>::INPUT_DEFAULT();
      flag = 0UL;

      max_GE = 0;
      b.clear();
      f1_succ.clear();
      shortest_path = MAX_COST;
      smallest_area = MAX_COST;
      area_sum.clear();
      verbose = false;
      l = 1200;
      write_in_file = false;
      omp_nb_threads = 20;
      conf_file = "TSMC65nm.conf";
      max_ram = 160;
      nodes_cmp = 0;

      gate_not1  = false;
      gate_and2  = false;
      gate_nand2 = false;
      gate_or2   = false;
      gate_nor2  = false;
      gate_xor2  = false;
      gate_xnor2 = false;
      gate_moai1 = false;
      gate_maoi1 = false;
      gate_nand3 = false;
      gate_nor3  = false;
      gate_or3   = false;
      gate_and3  = false;
      gate_andn2  = false;
      gate_orn2   = false;
    }
 
    int cur_shortest_path()
    {
      return shortest_path;
    }

    int cur_smallest_area()
    {
      return smallest_area;
    }

    void all_gates();

    void genImpInfo();

    int bool_op_cost(uint8_t op);

    string bool_op_str(uint8_t op);

    void init_b(string conf_file);

    void bool_op_one_input(const bool_function_t<N> *f, vector<bool_function_t<N> > *v, bool_op_t bool_op);

    void bool_op_two_inputs(const bool_function_t<N> *f1, const bool_function_t<N> *f2, vector<bool_function_t<N> > *v, bool_op_t bool_op);

    void bool_op_three_inputs(const bool_function_t<N> *f1, const bool_function_t<N> *f2, const bool_function_t<N> *f3, vector<bool_function_t<N> > *v, bool_op_t bool_op);

    string get_implementation(bool_function_t<N> f1);

    void get_implementation();

    void print_graphe_info();

    void exit_m();

    void graphe_to_file(map<int, set<bool_function_t<N> > > *graphe,
                    string graphe_name);
    
    void print_uint16(uint16_t n);
    
    void print_uint8(uint8_t n);
    
    string uint8_to_string(uint8_t n);
    
    void print_uint32(uint32_t n);

    void mitm(function_t<N> f1);

    void expand(int lambda);

    void v_list_process(int lambda, int op_cost, vector<bool_function_t<N> > *tmp,
                    set<bool_function_t<N> > *to_insert);

    bool is_in_graphe(int lambda, int op_cost, bool_function_t<N> f);
   
    bool is_in_graphe_collision(int lambda, int op_cost, bool_function_t<N> f);

    void pre_compute(string args);
    void pre_computing();
    void write_pre_bin();
    void read_pre_bin();
    void search_batch(string args);
    double search_single(string args);

    faster(){};

    ~faster(){};
};

#include "faster_bool_op.hpp"
#include "faster_mitm.hpp"
#include "faster_utils.hpp"
#include "faster_impl_info.hpp"

#undef NOP   
#undef NOT1  
#undef AND2  
#undef NAND2 
#undef OR2   
#undef NOR2  
#undef XOR2  
#undef XNOR2 
#undef MOAI1 
#undef MAOI1 
#undef NAND3 
#undef NOR3  
#undef OR3   
#undef AND3  
#undef ANDN2 
#undef ORN2  
#undef MAX_COST
#undef FOUND_ALL

#endif /* FASTER_H__ */


