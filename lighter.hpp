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

#ifndef LIGHTER_H__
#define LIGHTER_H__

#include "func.hpp"
#include "evaluator.hpp"

#define NOT1                  1
#define XOR2                  2
#define XNOR2                 3
#define MAOI1                 4
#define MOAI1                 5
#define MOAI1_AND2            6
#define MOAI1_OR2             7
#define MOAI1_NAND2           8
#define MOAI1_NOR2            9
#define MAOI1_NAND2           10
#define MAOI1_NOR2            11
#define MAOI1_AND2            12
#define MAOI1_OR2             13
#define MOAI1_NAND3           14
#define MAOI1_AND3            15
#define MOAI1_NAND2_AND2      16
#define MAOI1_AND2_AND2       17
#define MOAI1_NOR3            18
#define MAOI1_OR3             19
#define MOAI1_NOR2_OR2        20
#define MAOI1_OR2_OR2         21
#define MAOI1_NAND3           22
#define MOAI1_AND3            23
#define MAOI1_NAND2_AND2      24
#define MOAI1_AND2_AND2       25
#define MAOI1_NOR3            26
#define MOAI1_OR3             27
#define MAOI1_NOR2_OR2        28
#define MOAI1_OR2_OR2         29
#define MOAI1_AND2_NAND2      30
#define MOAI1_OR2_NOR2        31
#define MAOI1_NAND2_NAND2     32
#define MAOI1_NOR2_NOR2       33
#define MOAI1_NAND2_NAND2     34
#define MOAI1_NOR2_NOR2       35
#define MAOI1_AND2_NAND2      36
#define MAOI1_OR2_NOR2        37
#define MOAI1_NAND2_OR2       38
#define MOAI1_NOR2_AND2       39
#define MAOI1_AND2_OR2        40
#define MAOI1_OR2_AND2        41
#define MOAI1_NAND2_NOR2      42
#define MOAI1_NOR2_NAND2      43
#define MAOI1_AND2_NOR2       44
#define MAOI1_OR2_NAND2       45
#define MOAI1_AND2_OR2        46
#define MOAI1_OR2_AND2        47
#define MAOI1_NAND2_OR2       48
#define MAOI1_NOR2_AND2       49
#define MOAI1_AND2_NOR2       50
#define MOAI1_OR2_NAND2       51
#define MAOI1_NAND2_NOR2      52
#define MAOI1_NOR2_NAND2      53

#define MOAI1_ANDN2L          54
#define MOAI1_ANDN2R          55
#define MOAI1_ORN2L           56
#define MOAI1_ORN2R           57
#define MOAI1_AND2_ANDN2L     58
#define MOAI1_AND2_ANDN2R     59
#define MOAI1_AND2_ORN2L      60
#define MOAI1_AND2_ORN2R      61
#define MOAI1_OR2_ANDN2L      62
#define MOAI1_OR2_ANDN2R      63
#define MOAI1_OR2_ORN2L       64
#define MOAI1_OR2_ORN2R       65
#define MOAI1_NAND2_ANDN2L    66
#define MOAI1_NAND2_ANDN2R    67
#define MOAI1_NAND2_ORN2L     68
#define MOAI1_NAND2_ORN2R     69
#define MOAI1_NOR2_ANDN2L     70
#define MOAI1_NOR2_ANDN2R     71
#define MOAI1_NOR2_ORN2L      72
#define MOAI1_NOR2_ORN2R      73
#define MOAI1_ANDN2L_AND2     74
#define MOAI1_ANDN2L_OR2      75
#define MOAI1_ANDN2L_NAND2    76
#define MOAI1_ANDN2L_NOR2     77
#define MOAI1_ANDN2L_ANDN2L   78
#define MOAI1_ANDN2L_ANDN2R   79
#define MOAI1_ANDN2L_ORN2L    80
#define MOAI1_ANDN2L_ORN2R    81
#define MOAI1_ANDN2R_AND2     82
#define MOAI1_ANDN2R_OR2      83
#define MOAI1_ANDN2R_NAND2    84
#define MOAI1_ANDN2R_NOR2     85
#define MOAI1_ANDN2R_ANDN2L   86
#define MOAI1_ANDN2R_ANDN2R   87
#define MOAI1_ANDN2R_ORN2L    88
#define MOAI1_ANDN2R_ORN2R    89
#define MOAI1_ORN2L_AND2      90
#define MOAI1_ORN2L_OR2       91
#define MOAI1_ORN2L_NAND2     92
#define MOAI1_ORN2L_NOR2      93
#define MOAI1_ORN2L_ANDN2L    94
#define MOAI1_ORN2L_ANDN2R    95
#define MOAI1_ORN2L_ORN2L     96
#define MOAI1_ORN2L_ORN2R     97
#define MOAI1_ORN2R_AND2      98
#define MOAI1_ORN2R_OR2       99
#define MOAI1_ORN2R_NAND2     100
#define MOAI1_ORN2R_NOR2      101
#define MOAI1_ORN2R_ANDN2L    102
#define MOAI1_ORN2R_ANDN2R    103
#define MOAI1_ORN2R_ORN2L     104
#define MOAI1_ORN2R_ORN2R     105
#define MAOI1_ANDN2L          106
#define MAOI1_ANDN2R          107
#define MAOI1_ORN2L           108
#define MAOI1_ORN2R           109
#define MAOI1_AND2_ANDN2L     110
#define MAOI1_AND2_ANDN2R     111
#define MAOI1_AND2_ORN2L      112
#define MAOI1_AND2_ORN2R      113
#define MAOI1_OR2_ANDN2L      114
#define MAOI1_OR2_ANDN2R      115
#define MAOI1_OR2_ORN2L       116
#define MAOI1_OR2_ORN2R       117
#define MAOI1_NAND2_ANDN2L    118
#define MAOI1_NAND2_ANDN2R    119
#define MAOI1_NAND2_ORN2L     120
#define MAOI1_NAND2_ORN2R     121
#define MAOI1_NOR2_ANDN2L     122
#define MAOI1_NOR2_ANDN2R     123
#define MAOI1_NOR2_ORN2L      124
#define MAOI1_NOR2_ORN2R      125
#define MAOI1_ANDN2L_AND2     126
#define MAOI1_ANDN2L_OR2      127
#define MAOI1_ANDN2L_NAND2    128
#define MAOI1_ANDN2L_NOR2     129
#define MAOI1_ANDN2L_ANDN2L   130
#define MAOI1_ANDN2L_ANDN2R   131
#define MAOI1_ANDN2L_ORN2L    132
#define MAOI1_ANDN2L_ORN2R    133
#define MAOI1_ANDN2R_AND2     134
#define MAOI1_ANDN2R_OR2      135
#define MAOI1_ANDN2R_NAND2    136
#define MAOI1_ANDN2R_NOR2     137
#define MAOI1_ANDN2R_ANDN2L   138
#define MAOI1_ANDN2R_ANDN2R   139
#define MAOI1_ANDN2R_ORN2L    140
#define MAOI1_ANDN2R_ORN2R    141
#define MAOI1_ORN2L_AND2      142
#define MAOI1_ORN2L_OR2       143
#define MAOI1_ORN2L_NAND2     144
#define MAOI1_ORN2L_NOR2      145
#define MAOI1_ORN2L_ANDN2L    146
#define MAOI1_ORN2L_ANDN2R    147
#define MAOI1_ORN2L_ORN2L     148
#define MAOI1_ORN2L_ORN2R     149
#define MAOI1_ORN2R_AND2      150
#define MAOI1_ORN2R_OR2       151
#define MAOI1_ORN2R_NAND2     152
#define MAOI1_ORN2R_NOR2      153
#define MAOI1_ORN2R_ANDN2L    154
#define MAOI1_ORN2R_ANDN2R    155
#define MAOI1_ORN2R_ORN2L     156
#define MAOI1_ORN2R_ORN2R     157

#define XOR2_AND2             158
#define XOR2_OR2              159
#define XOR2_ANDN2L           160
#define XOR2_ANDN2R           161
#define XOR2_ORN2L            162
#define XOR2_ORN2R            163
#define XOR2_AND2_AND2        164
#define XOR2_AND2_OR2         165
#define XOR2_AND2_ANDN2L      166
#define XOR2_AND2_ANDN2R      167
#define XOR2_AND2_ORN2L       168
#define XOR2_AND2_ORN2R       169
#define XOR2_OR2_AND2         170
#define XOR2_OR2_OR2          171
#define XOR2_OR2_ANDN2L       172
#define XOR2_OR2_ANDN2R       173
#define XOR2_OR2_ORN2L        174
#define XOR2_OR2_ORN2R        175
#define XOR2_ANDN2L_AND2      176
#define XOR2_ANDN2L_OR2       177
#define XOR2_ANDN2L_ANDN2L    178
#define XOR2_ANDN2L_ANDN2R    179
#define XOR2_ANDN2L_ORN2L     180
#define XOR2_ANDN2L_ORN2R     181
#define XOR2_ANDN2R_AND2      182
#define XOR2_ANDN2R_OR2       183
#define XOR2_ANDN2R_ANDN2L    184
#define XOR2_ANDN2R_ANDN2R    185
#define XOR2_ANDN2R_ORN2L     186
#define XOR2_ANDN2R_ORN2R     187
#define XOR2_ORN2L_AND2       188
#define XOR2_ORN2L_OR2        189
#define XOR2_ORN2L_ANDN2L     190
#define XOR2_ORN2L_ANDN2R     191
#define XOR2_ORN2L_ORN2L      192
#define XOR2_ORN2L_ORN2R      193
#define XOR2_ORN2R_AND2       194
#define XOR2_ORN2R_OR2        195
#define XOR2_ORN2R_ANDN2L     196
#define XOR2_ORN2R_ANDN2R     197
#define XOR2_ORN2R_ORN2L      198
#define XOR2_ORN2R_ORN2R      199

using namespace Peigen;
using namespace Peigen::weight;
using namespace std;

template<int N>
class Peigen::weight::lighter
{
public:
    int pre_l = -1;
    int cntpoint = -1;

    string imp_info = "";
    ofstream statistician;
    string old_instance_name = "";
    string instance_name = "";
 
    string sboxesfile = "new_sboxes" + to_string(N) + ".txt";

    int max_GE = 0;
    int min_GE = numeric_limits<int>::max();
    vector<bool_op_t> b; // The boolean instructions (B in the paper)
    set<vector<function_t<N> > > check_tt;
    function_t<N> start;
    function_t<N> arrival;
    function_t<N> unsorted_arrival;
    function_t<N> unsorted_function;
    int PEo[N];
    map<int, set<function_t<N>> > f1_succ;
    map<int, set<function_t<N>> > f2_succ;

    int count_list;
    int shortest_path = numeric_limits<int>::max();
    
    bool verbose = false;
    int l = 900;
    bool one_expand = false;
    bool write_in_file = false;
    int omp_nb_threads = 20;
    
    string conf_file = "TSMC65nm.conf";
    string criteria_file = "criteria.conf";
    long max_ram = 160;
    long nodes_cmp = 0;
    long max_nodes;
    int cost_1;
    int cost_2;

    int good_number = 0;
    int good_PE_number = 0;
    
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

    int Cost_Criteria = -1;
    int Involution_Criteria = -1;
    int Diff_Criteria = -1;
    int DiffFreq_Criteria = -1;
    int Diff1_Criteria = -1;
    int CardD1_Criteria = -1;
    int Lin_Criteria = -1;
    int LinFreq_Criteria = -1;
    int Lin1_Criteria = -1;
    int CardL1_Criteria = -1;
    int MaxDeg_Criteria = -1;
    int MinDeg_Criteria = -1;
    int MaxDegFreq_Criteria = -1;
    int MinDegFreq_Criteria = -1;

    // void parse_args(string args);

    void reset()
    {
      pre_l = -1;
      cntpoint = -1;
      imp_info = "";
      instance_name = "";
      old_instance_name = "";
      sboxesfile = "new_sboxes" + to_string(N) + ".txt";
      max_GE = 0;
      min_GE = numeric_limits<int>::max();
      b.clear();
      check_tt.clear();
      f1_succ.clear();
      f2_succ.clear();
      shortest_path = numeric_limits<int>::max();
      verbose = false;
      l = 900;
      one_expand = false;
      write_in_file = false;
      omp_nb_threads = 20;
      conf_file = "TSMC65nm.conf";
      criteria_file = "criteria.conf";
      max_ram = 160;
      nodes_cmp = 0;
      good_number = 0;
      good_PE_number = 0;

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

      Cost_Criteria = -1;
      Involution_Criteria = -1;
      Diff_Criteria = -1;
      DiffFreq_Criteria = -1;
      Diff1_Criteria = -1;
      CardD1_Criteria = -1;
      Lin_Criteria = -1;
      LinFreq_Criteria = -1;
      Lin1_Criteria = -1;
      CardL1_Criteria = -1;
      MaxDeg_Criteria = -1;
      MinDeg_Criteria = -1;
      MaxDegFreq_Criteria = -1;
      MinDegFreq_Criteria = -1;
    }

    void init_varbles()
    {
      if (pre_l < 0)
      {
        f1_succ.clear();
      }    	
    	f2_succ.clear();
    
    	shortest_path = numeric_limits<int>::max();
    }
    
    void all_gates();

    void genImpInfo();

    int bool_op_cost(uint8_t op);

    void init_b(string conf_file);

    void bool_op_simple_gates(function_t<N> f, vector<function_t<N> > *v, int operation);

    void bool_op_double_gates(function_t<N> f, vector<function_t<N> > *v, int operation);

    void bool_op_triple_gates(function_t<N> f, vector<function_t<N> > *v, int operation);

    void bool_op_fun(int op_id, function_t<N> f, vector<function_t<N> > *v);

    void write_c(string *s, vector<int> tmp_tab);

    string bool_op_not(const function_t<N> f, string s, bool reverse);

    string bool_op_two_inputs(const function_t<N> f, int operation, string s, bool reverse);

    string bool_op_super_gates(const function_t<N> f, int operation, string s, bool reverse);

    string bool_op_super_super_gates(const function_t<N> f, int operation, string s, bool reverse);

    string bool_op_base_string(uint8_t op);

    string bool_op_to_string(const function_t<N> f, uint8_t op, bool reverse);

    void update_unsorted_function(function_t<N> f);

    string get_permut(function_t<N> f1, function_t<N> f2, char a, char b);

    void get_string_impl(string *s, function_t<N> f, bool reverse);

    int get_semi_impl(function_t<N> f,
                    int count_vlist,
                    map<int, set<function_t<N> > > *g,
                    string *s,
                    bool reverse);

    void get_implementation(function_t<N> f1,
                      function_t<N> f2,
                      int v,
                      map<int, set<function_t<N> > > *g1,
                      map<int, set<function_t<N> > > *g2);

    //void get_implementation_concatenate(function_t<N> f1, int v1, function_t<N> f2, int v2, function_t<N> f3);

    void get_implementation_concatenate(function_t<N> f1, int v1, function_t<N> f2, int v2, int perm[]);

    int get_semi_impl_concatenate(function_t<N> f,
                    int count_vlist, string *s,
                    bool reverse, int perm[]);
    
    void print_graphe_info();

    void exit_m();

    void graphe_to_file(map<int, set<function_t<N> > > *graphe,
                    string graphe_name);

    void print_uint16(uint16_t n);
    
    void print_uint8(uint8_t n);
    
    string uint8_to_string(uint8_t n);
    
    void print_uint32(uint32_t n);

    void mitm(function_t<N> f1, function_t<N> f2);

    void expand(map<int, set<function_t<N> > > *current,
            map<int, set<function_t<N> > > *opposite,
            int lambda);

    void v_list_process(int lambda, int op_cost, vector<function_t<N> > *tmp,
                    map<int, set<function_t<N> > > *current,
                    set<function_t<N> > *to_insert, map<int,set<function_t<N> > > *opposite);

    bool is_in_graphe(int lambda, int op_cost, function_t<N> f, map<int, set<function_t<N> > > *g);
   
    bool is_in_graphe_collision(int lambda, int op_cost, function_t<N> f,
                            function_t<N> *fun_found,
                            int *vect_found,
                            map<int, set<function_t<N> > > *g);

    void pre_compute(string args);
    void pre_computing();
    void write_pre_bin(int lambda);
    void read_pre_bin(int completed);
    void search_batch(string args);
    double search_single(string args);
    double search_single_concatenate(string args);
    void search_batch_concatenate(string args);

    void concatenate(function_t<N> f1, function_t<N> f2);
    void expand(int lambda);
    void v_list_process(int lambda, int op_cost, vector<function_t<N> > *tmp,
                    set<function_t<N> > *to_insert);
    bool is_in_graphe(int lambda, int op_cost, function_t<N> f);
    void match(int cost1);
    //void check_dup();
 
    void init_critiera(string conf_file);
    void generate(string args);
    void generate();
    void filter(int c1);
    void filter_pre(int c1);

    int get_semi_impl_good(function_t<N> f, int count_vlist, string *s);

    int get_semi_impl_good(function_t<N> f,
                           int count_vlist, string *s,
                           const function_t<N> & f_p, int pi);

    void get_implementation_good(function_t<N> f1, int v1,
                                 const function_t<N> & fp, int pi,
                                 function_t<N> f2, int v2,
                                 function_t<N> f3, int good_idx);

    void get_implementation_good(function_t<N> f1, int v, int good_idx);

    void evaluate_writeTitleLine(ofstream & outf);

    void evaluate(string sboxesfile, string outputfile);

    void evaluate_verbose(string sboxesfile, string outputfile_prefix);

    void evaluate_filter(string args);
   
    // require LUT and bit_slice forms are all computed
    bool is_good(function_t<N> & func) const
    {
      int max_degree;
      int min_degree;
      int deg_spectrum[N+1];
      int max_degree_freq;
      int min_degree_freq;

      if (Involution_Criteria != -1)
      {
        if (func.is_involution() != Involution_Criteria) return false;
      }

      if ((Diff_Criteria != -1) || (Diff1_Criteria != -1) || (DiffFreq_Criteria != -1) || (CardD1_Criteria != -1))
      {
        if (!func.difference_distribution_matrix_test(Diff_Criteria, Diff1_Criteria, DiffFreq_Criteria, CardD1_Criteria)) return false;
      }

      if ((Lin_Criteria != -1) || (Lin1_Criteria != -1) || (LinFreq_Criteria != -1) || (CardL1_Criteria != -1))
      {
        if (!func.linear_approximation_matrix_test(Lin_Criteria, Lin1_Criteria, LinFreq_Criteria, CardL1_Criteria)) return false;
      }

      if ((MaxDegFreq_Criteria != -1) || (MinDegFreq_Criteria != -1) || (MaxDeg_Criteria != -1) || (MinDeg_Criteria != -1))
      {
        func.degree(deg_spectrum, max_degree, min_degree);
        max_degree_freq = deg_spectrum[max_degree];
        min_degree_freq = deg_spectrum[min_degree];
        if (MinDeg_Criteria != -1)
        {
          if (min_degree < MinDeg_Criteria) return false;
        }
        if (MinDegFreq_Criteria != -1)
        {
          if (min_degree_freq > MinDegFreq_Criteria) return false;
        }
        if (MaxDeg_Criteria != -1)
        {
          if (max_degree < MaxDeg_Criteria) return false;
        }
        if (MaxDegFreq_Criteria != -1)
        {
          if (max_degree_freq < MaxDegFreq_Criteria) return false;      
        }
      }
      return true;
    }

    void evaluate(string name, const function_t<N> & f, int Cost, string CostisBest, ofstream & outf)
    {
      evaluator<N> Eva(f, name, Cost, CostisBest);
      outf << Eva.show();
    }

    lighter(){};

    ~lighter(){};
};

#include "lighter_bool_op.hpp"
#include "lighter_string_bool_op.hpp"
#include "lighter_mitm.hpp"
#include "lighter_utils.hpp"
#include "lighter_impl_info.hpp"

#undef NOT1                  
#undef XOR2                  
#undef XNOR2                 
#undef MAOI1                 
#undef MOAI1                 
#undef MOAI1_AND2            
#undef MOAI1_OR2             
#undef MOAI1_NAND2           
#undef MOAI1_NOR2            
#undef MAOI1_NAND2           
#undef MAOI1_NOR2            
#undef MAOI1_AND2            
#undef MAOI1_OR2             
#undef MOAI1_NAND3           
#undef MAOI1_AND3            
#undef MOAI1_NAND2_AND2      
#undef MAOI1_AND2_AND2       
#undef MOAI1_NOR3            
#undef MAOI1_OR3             
#undef MOAI1_NOR2_OR2        
#undef MAOI1_OR2_OR2         
#undef MAOI1_NAND3           
#undef MOAI1_AND3            
#undef MAOI1_NAND2_AND2      
#undef MOAI1_AND2_AND2       
#undef MAOI1_NOR3            
#undef MOAI1_OR3             
#undef MAOI1_NOR2_OR2        
#undef MOAI1_OR2_OR2         
#undef MOAI1_AND2_NAND2      
#undef MOAI1_OR2_NOR2        
#undef MAOI1_NAND2_NAND2     
#undef MAOI1_NOR2_NOR2       
#undef MOAI1_NAND2_NAND2     
#undef MOAI1_NOR2_NOR2       
#undef MAOI1_AND2_NAND2      
#undef MAOI1_OR2_NOR2        
#undef MOAI1_NAND2_OR2       
#undef MOAI1_NOR2_AND2       
#undef MAOI1_AND2_OR2        
#undef MAOI1_OR2_AND2        
#undef MOAI1_NAND2_NOR2      
#undef MOAI1_NOR2_NAND2      
#undef MAOI1_AND2_NOR2       
#undef MAOI1_OR2_NAND2       
#undef MOAI1_AND2_OR2        
#undef MOAI1_OR2_AND2        
#undef MAOI1_NAND2_OR2       
#undef MAOI1_NOR2_AND2       
#undef MOAI1_AND2_NOR2       
#undef MOAI1_OR2_NAND2       
#undef MAOI1_NAND2_NOR2      
#undef MAOI1_NOR2_NAND2      

#undef MOAI1_ANDN2L          
#undef MOAI1_ANDN2R          
#undef MOAI1_ORN2L           
#undef MOAI1_ORN2R           
#undef MOAI1_AND2_ANDN2L     
#undef MOAI1_AND2_ANDN2R     
#undef MOAI1_AND2_ORN2L      
#undef MOAI1_AND2_ORN2R      
#undef MOAI1_OR2_ANDN2L      
#undef MOAI1_OR2_ANDN2R      
#undef MOAI1_OR2_ORN2L       
#undef MOAI1_OR2_ORN2R       
#undef MOAI1_NAND2_ANDN2L    
#undef MOAI1_NAND2_ANDN2R    
#undef MOAI1_NAND2_ORN2L     
#undef MOAI1_NAND2_ORN2R     
#undef MOAI1_NOR2_ANDN2L     
#undef MOAI1_NOR2_ANDN2R     
#undef MOAI1_NOR2_ORN2L      
#undef MOAI1_NOR2_ORN2R      
#undef MOAI1_ANDN2L_AND2     
#undef MOAI1_ANDN2L_OR2      
#undef MOAI1_ANDN2L_NAND2    
#undef MOAI1_ANDN2L_NOR2     
#undef MOAI1_ANDN2L_ANDN2L   
#undef MOAI1_ANDN2L_ANDN2R   
#undef MOAI1_ANDN2L_ORN2L    
#undef MOAI1_ANDN2L_ORN2R    
#undef MOAI1_ANDN2R_AND2     
#undef MOAI1_ANDN2R_OR2      
#undef MOAI1_ANDN2R_NAND2    
#undef MOAI1_ANDN2R_NOR2     
#undef MOAI1_ANDN2R_ANDN2L   
#undef MOAI1_ANDN2R_ANDN2R   
#undef MOAI1_ANDN2R_ORN2L    
#undef MOAI1_ANDN2R_ORN2R    
#undef MOAI1_ORN2L_AND2      
#undef MOAI1_ORN2L_OR2       
#undef MOAI1_ORN2L_NAND2     
#undef MOAI1_ORN2L_NOR2      
#undef MOAI1_ORN2L_ANDN2L    
#undef MOAI1_ORN2L_ANDN2R    
#undef MOAI1_ORN2L_ORN2L     
#undef MOAI1_ORN2L_ORN2R     
#undef MOAI1_ORN2R_AND2      
#undef MOAI1_ORN2R_OR2       
#undef MOAI1_ORN2R_NAND2     
#undef MOAI1_ORN2R_NOR2      
#undef MOAI1_ORN2R_ANDN2L    
#undef MOAI1_ORN2R_ANDN2R    
#undef MOAI1_ORN2R_ORN2L     
#undef MOAI1_ORN2R_ORN2R     
#undef MAOI1_ANDN2L          
#undef MAOI1_ANDN2R          
#undef MAOI1_ORN2L           
#undef MAOI1_ORN2R           
#undef MAOI1_AND2_ANDN2L     
#undef MAOI1_AND2_ANDN2R     
#undef MAOI1_AND2_ORN2L      
#undef MAOI1_AND2_ORN2R      
#undef MAOI1_OR2_ANDN2L      
#undef MAOI1_OR2_ANDN2R      
#undef MAOI1_OR2_ORN2L       
#undef MAOI1_OR2_ORN2R       
#undef MAOI1_NAND2_ANDN2L    
#undef MAOI1_NAND2_ANDN2R    
#undef MAOI1_NAND2_ORN2L     
#undef MAOI1_NAND2_ORN2R     
#undef MAOI1_NOR2_ANDN2L     
#undef MAOI1_NOR2_ANDN2R     
#undef MAOI1_NOR2_ORN2L      
#undef MAOI1_NOR2_ORN2R      
#undef MAOI1_ANDN2L_AND2     
#undef MAOI1_ANDN2L_OR2      
#undef MAOI1_ANDN2L_NAND2    
#undef MAOI1_ANDN2L_NOR2     
#undef MAOI1_ANDN2L_ANDN2L   
#undef MAOI1_ANDN2L_ANDN2R   
#undef MAOI1_ANDN2L_ORN2L    
#undef MAOI1_ANDN2L_ORN2R    
#undef MAOI1_ANDN2R_AND2     
#undef MAOI1_ANDN2R_OR2      
#undef MAOI1_ANDN2R_NAND2    
#undef MAOI1_ANDN2R_NOR2     
#undef MAOI1_ANDN2R_ANDN2L   
#undef MAOI1_ANDN2R_ANDN2R   
#undef MAOI1_ANDN2R_ORN2L    
#undef MAOI1_ANDN2R_ORN2R    
#undef MAOI1_ORN2L_AND2      
#undef MAOI1_ORN2L_OR2       
#undef MAOI1_ORN2L_NAND2     
#undef MAOI1_ORN2L_NOR2      
#undef MAOI1_ORN2L_ANDN2L    
#undef MAOI1_ORN2L_ANDN2R    
#undef MAOI1_ORN2L_ORN2L     
#undef MAOI1_ORN2L_ORN2R     
#undef MAOI1_ORN2R_AND2      
#undef MAOI1_ORN2R_OR2       
#undef MAOI1_ORN2R_NAND2     
#undef MAOI1_ORN2R_NOR2      
#undef MAOI1_ORN2R_ANDN2L    
#undef MAOI1_ORN2R_ANDN2R    
#undef MAOI1_ORN2R_ORN2L     
#undef MAOI1_ORN2R_ORN2R     

#undef XOR2_AND2             
#undef XOR2_OR2              
#undef XOR2_ANDN2L           
#undef XOR2_ANDN2R           
#undef XOR2_ORN2L            
#undef XOR2_ORN2R            
#undef XOR2_AND2_AND2        
#undef XOR2_AND2_OR2         
#undef XOR2_AND2_ANDN2L      
#undef XOR2_AND2_ANDN2R      
#undef XOR2_AND2_ORN2L       
#undef XOR2_AND2_ORN2R       
#undef XOR2_OR2_AND2         
#undef XOR2_OR2_OR2          
#undef XOR2_OR2_ANDN2L       
#undef XOR2_OR2_ANDN2R       
#undef XOR2_OR2_ORN2L        
#undef XOR2_OR2_ORN2R        
#undef XOR2_ANDN2L_AND2      
#undef XOR2_ANDN2L_OR2       
#undef XOR2_ANDN2L_ANDN2L    
#undef XOR2_ANDN2L_ANDN2R    
#undef XOR2_ANDN2L_ORN2L     
#undef XOR2_ANDN2L_ORN2R     
#undef XOR2_ANDN2R_AND2      
#undef XOR2_ANDN2R_OR2       
#undef XOR2_ANDN2R_ANDN2L    
#undef XOR2_ANDN2R_ANDN2R    
#undef XOR2_ANDN2R_ORN2L     
#undef XOR2_ANDN2R_ORN2R     
#undef XOR2_ORN2L_AND2       
#undef XOR2_ORN2L_OR2        
#undef XOR2_ORN2L_ANDN2L     
#undef XOR2_ORN2L_ANDN2R     
#undef XOR2_ORN2L_ORN2L      
#undef XOR2_ORN2L_ORN2R      
#undef XOR2_ORN2R_AND2       
#undef XOR2_ORN2R_OR2        
#undef XOR2_ORN2R_ANDN2L     
#undef XOR2_ORN2R_ANDN2R     
#undef XOR2_ORN2R_ORN2L      
#undef XOR2_ORN2R_ORN2R      

#endif /* LIGHTER_H__ */
