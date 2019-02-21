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

#ifndef FASTER_MITM_H__
#define FASTER_MITM_H__

using namespace std;
using namespace Peigen::depth;

#define NDEBUG
#include <assert.h>

#ifdef PRINT
#undef PRINT
#endif
#ifdef MAX_VEC_NODES
#undef MAX_VEC_NODES
#endif
#ifdef pre_parse_args
#undef pre_parse_args
#endif
#ifdef post_parse_args
#undef post_parse_args
#endif

#ifdef NDEBUG
  #define PRINT(fmt, ...) ((void)0)
#else
  #define PRINT(fmt, ...) printf(fmt, (unsigned long)(__LINE__), ##__VA_ARGS__)
#endif

#define MAX_VEC_NODES (max_nodes>>1UL)

#define pre_parse_args()                                      \
  int myargc = 0;                                             \
  char* myargv[50];                                           \
  string args_tmp = "x " + args;                              \
  char * args_str = new char [args_tmp.length() + 1];         \
  strcpy(args_str, args_tmp.c_str());                         \
  char * p = strtok(args_str," ");                            \
  while( p != 0)                                              \
  {                                                           \
    myargv[myargc] = p;                                       \
    p = strtok(NULL, " ");                                    \
    myargc++;                                                 \
  }                                                           \
  myargv[myargc] = NULL;                                      \
  int opt;                                                    \
  optind = 0;


#define post_parse_args()                                          \
  delete [] args_str;                                              \
  max_nodes = max_ram * 100000000UL / sizeof(bool_function_t<N>);  \
  if (gate_maoi1 == 1) gate_xor2 = 0;                              \
  if (gate_moai1 == 1) gate_xnor2 = 0;

template<int N>
void faster<N>::pre_compute(string args)
{
  clock_t t1 = clock();

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "c:vap:f:r:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'c': pre_l = abs(atoi(optarg))*100; cout << "Precompute Limit : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  genImpInfo();

  init_b(conf_file);

  pre_computing();

  reset();
  t1 = clock() - t1;
	cout << "Pre-computation takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
}

template<int N>
void faster<N>::pre_computing()
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  f1.sort();

  f1_succ.clear();
  f1_succ.insert(pair<int, set<bool_function_t<N> > >(0, {f1.coordinates[0]}));
  for (int i = 1; i < N; i++ ) f1_succ[0].insert(f1.coordinates[i]);

  count_list = 0;
  while (count_list <= pre_l)
  {
    expand(count_list);
    if(verbose && (count_list%100) == 0) cout << count_list << endl;
    count_list++;
    write_pre_bin();
  }
}

template<int N>
void faster<N>::write_pre_bin()
{
  auto find = [](const set<bool_function_t<N> > & func_set, const bit_slice_l_t<N> & bit_slice)
  {
    for(auto set_it = func_set.begin(); set_it != func_set.end(); set_it++)
    {
      if (set_it->bit_slice == bit_slice)
      {
        return set_it;
      }
    }
    return func_set.end();
  };

  ofstream fout;
  size_t bool_function_s = sizeof(bool_function_t<N>);
  for (auto map_it = f1_succ.begin(); map_it != f1_succ.end(); map_it++)
  {
    string pre_filen = "pre_" + imp_info + "_" + to_string(map_it->first) + ".bin";
    fout.open(pre_filen, ios::binary);
    for (auto set_it = map_it->second.begin(); set_it != map_it->second.end(); set_it++)
    {
      fout.write((char *)&(set_it->bit_slice), sizeof(bit_slice_l_t<N>));
      fout.write((char *)&(set_it->info_op), sizeof(uint8_t));

      bit_slice_l_t<N> opv_tmp;
      auto map2_it = f1_succ.begin();

      switch(set_it->info_op)
      {
        case NOP   : break;
        case NAND3 : 
        case NOR3  : 
        case OR3   : 
        case AND3  :
                    opv_tmp = set_it->operands[2]->bit_slice;
                    map2_it = f1_succ.begin();
                    while (map2_it->first < map_it->first)
                    {
                      auto func_t = find(map2_it->second, opv_tmp);
                      if (func_t != map2_it->second.end())
                      {
                        fout.write((char *)&(map2_it->first), sizeof(int));
                        fout.write((char *)&(opv_tmp), sizeof(bit_slice_l_t<N>));
                        break;
                      }
                      map2_it++;
                    }
                    assert(map2_it->first < map_it->first);
        case AND2  : 
        case NAND2 : 
        case OR2   : 
        case NOR2  : 
        case XOR2  : 
        case XNOR2 : 
        case MOAI1 : 
        case MAOI1 :
        case ANDN2 : 
        case ORN2  : 
                    opv_tmp = set_it->operands[1]->bit_slice;
                    map2_it = f1_succ.begin();
                    while (map2_it->first < map_it->first)
                    {
                      auto func_t = find(map2_it->second, opv_tmp);
                      if (func_t != map2_it->second.end())
                      {
                        fout.write((char *)&(map2_it->first), sizeof(int));
                        fout.write((char *)&(opv_tmp), sizeof(bit_slice_l_t<N>));
                        break;
                      }
                      map2_it++;
                    }
                    assert(map2_it->first < map_it->first);
        case NOT1  :
                    opv_tmp = set_it->operands[0]->bit_slice;
                    map2_it = f1_succ.begin();
                    while (map2_it->first < map_it->first)
                    {
                      auto func_t = find(map2_it->second, opv_tmp);
                      if (func_t != map2_it->second.end())
                      {
                        fout.write((char *)&(map2_it->first), sizeof(int));
                        fout.write((char *)&(opv_tmp), sizeof(bit_slice_l_t<N>));
                        break;
                      }
                      map2_it++;
                    }
                    assert(map2_it->first < map_it->first);
                    break;
        default    : cout << "Write file error. Exit." << endl; exit(1); 
      }
      fout.write((char *)&(set_it->area), sizeof(int));
    }
    fout.close();
  }
}

template<int N>
void faster<N>::read_pre_bin()
{
  auto find = [](const set<bool_function_t<N> > & func_set, const bit_slice_l_t<N> & bit_slice)
  {
    for(auto set_it = func_set.begin(); set_it != func_set.end(); set_it++)
    {
      if (set_it->bit_slice == bit_slice)
      {
        return set_it;
      }
    }
    return func_set.end();
  };

  ifstream fin;
  bool_function_t<N> f_t;
  int lambda = 0;
  size_t bool_function_s = sizeof(bool_function_t<N>);
  f1_succ.clear();
  while (lambda <= (pre_l + max_GE))
  {
    string pre_filen = "pre_" + imp_info + "_" + to_string(lambda) + ".bin";
    fin.open(pre_filen, ios::binary);
    if (fin)
    {
      set<bool_function_t<N> > new_list;    
      f1_succ.insert(pair<int, set<bool_function_t<N> > >(lambda, new_list));
      auto to_insert = &(f1_succ[lambda]);

      int depth;
      bit_slice_l_t<N> opv_tmp;

      while (fin.read((char*)&(f_t.bit_slice), sizeof(bit_slice_l_t<N>)))
      {        
        assert(fin);
        fin.read((char *)&(f_t.info_op), sizeof(uint8_t));
        assert(fin);
        f_t.operands[0] = NULL;
        f_t.operands[1] = NULL;
        f_t.operands[2] = NULL;
        switch(f_t.info_op)
        {
          case NOP   :
                      break;
          case NAND3 : 
          case NOR3  : 
          case OR3   : 
          case AND3  :assert(fin);
                      fin.read((char *)&(depth), sizeof(int));
                      assert(fin);
                      fin.read((char *)&(opv_tmp), sizeof(bit_slice_l_t<N>));
                      f_t.operands[2] = &(*(find(f1_succ[depth],opv_tmp)));
          case AND2  : 
          case NAND2 : 
          case OR2   : 
          case NOR2  : 
          case XOR2  : 
          case XNOR2 : 
          case MOAI1 : 
          case MAOI1 :
          case ANDN2 : 
          case ORN2  :assert(fin);
                      fin.read((char *)&(depth), sizeof(int));
                      assert(fin);
                      fin.read((char *)&(opv_tmp), sizeof(bit_slice_l_t<N>));
                      f_t.operands[1] = &(*(find(f1_succ[depth],opv_tmp)));
          case NOT1  :assert(fin);
                      fin.read((char *)&(depth), sizeof(int));
                      assert(fin);
                      fin.read((char *)&(opv_tmp), sizeof(bit_slice_l_t<N>));
                      f_t.operands[0] = &(*(find(f1_succ[depth],opv_tmp)));
                      break;
          default    :
                      cout << "Read binary file error. Exit. ";
                      fin.close();
                      exit(1);
        }
        assert(fin);
        fin.read((char *)&(f_t.area), sizeof(int));
        to_insert->insert(f_t);
      }
    }
    fin.close();
    lambda++;
  }
}

template<int N>
void faster<N>::search_batch(string args)
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  target = function_t<N>::INPUT_DEFAULT();
  string sboxesfile = "sboxes" + to_string(N) + ".txt";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "i:o:l:c:vwap:f:r:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'w': write_in_file = true; break;
      case 'i': f1.parse_function(optarg); break;
      case 'o': sboxesfile = optarg; break;
      case 'l': l = abs(atoi(optarg))*100; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  genImpInfo();

  init_b(conf_file);

  ifstream sboxes(sboxesfile);
  string instance_LUT;

  read_pre_bin();

  for(string line; getline(sboxes, line); )
  {
    istringstream instance_line(line);
    if(getline(instance_line, instance_name, ','))
    {
      if(getline(instance_line, instance_LUT))
      {
        clock_t t1 = clock();

        init_varbles();

        target.parse_function(instance_LUT);

        cout << "From : " << f1.to_string() << endl;
        cout << "To   : " << target.to_string() << endl;

        mitm(f1);

        double best_depth = (double)(shortest_path)/100;
        double best_area = (double)(smallest_area)/100;
      
        cout << "Best Depth: " << best_depth << " GE "
             << "  Area: " << best_area << " GE " << endl;
        
        statistician.open(imp_info + "__" + "statistics.csv", ios::app);
	      statistician << instance_name << "," << best_depth << "," << best_area << endl;
	      statistician.close();

        t1 = clock() - t1;
        cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
      }
    }
  }
  reset();
}

template<int N>
double faster<N>::search_single(string args)
{
  clock_t t1 = clock();

  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  target = function_t<N>::INPUT_DEFAULT();
  string instance_LUT;
  instance_name = "";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "i:o:c:l:vwap:f:r:n:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'w': write_in_file = true; break;
      case 'i': f1.parse_function(optarg); break;
      case 'o': target.parse_function(optarg); instance_LUT = optarg; break;
      case 'n': instance_name = optarg; break;
      case 'l': l = abs(atoi(optarg))*100; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  if(instance_name == "")
  {
    instance_name = instance_LUT;
  }

  genImpInfo();

  init_b(conf_file);

  read_pre_bin();

  init_varbles();

  cout << "From : " << f1.to_string() << endl;
  cout << "To   : " << target.to_string() << endl;

  mitm(f1);

  double best_depth = (double)(shortest_path)/100;
  double best_area = (double)(smallest_area)/100;

  cout << "Best Depth: " << best_depth << " GE "
       << "  Area: " << best_area << " GE " << endl;
  
  statistician.open(imp_info + "__" + "statistics.csv", ios::app);
	statistician << instance_name << "," << best_depth << "," << best_area << endl;
	statistician.close();

  reset();

  t1 = clock() - t1;
  cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;

  return best_depth;
}

template<int N>
void faster<N>::mitm(function_t<N> f1)
{
  start = f1;
  count_list = 0;

  if (pre_l < 0)
  {
    f1_succ.insert(pair<int, set<bool_function_t<N> > >(0, {f1.coordinates[0]}));
    for (int i = 1; i < N; i++) f1_succ[0].insert(f1.coordinates[i]);
  }
  else
  {
    auto map_it = f1_succ.begin();
    while (map_it != f1_succ.end())
    {
      count_list = map_it->first;
      if (count_list <= shortest_path)
      {
        for (auto it_tmp = (*map_it).second.begin(); it_tmp != (*map_it).second.end(); it_tmp++)
        {
          if(is_in_graphe_collision(count_list, 0, (*it_tmp)))
          {
            get_implementation();
          }
        }
        map_it++;
      }
      else
      {
        break;
      }   
    }

    if(shortest_path <= pre_l)
    {
      return;
    }
    count_list = pre_l + 1;
  }

  while(count_list <= l)
  {
    if (count_list > pre_l)
    {
      expand(count_list);
      pre_l = count_list;
    }

    if(verbose && (count_list%100) == 0) cout << "\033[0;32m" << count_list << "\033[0m" << endl;
    count_list++;
    if(count_list > shortest_path)
    {
      if (count_list > pre_l) write_pre_bin();
      exit_m();
      return;
    }
  }
  if(verbose) print_graphe_info();
  if (count_list > pre_l)
  {
    write_pre_bin();
    pre_l = count_list;
  }
  exit_m();
  return;
}


template<int N>
void faster<N>::expand(int lambda)
{
  auto remove_dup = [](vector<bool_function_t<N> > * succ_vecs_pt, int i)
  {
    vector<bool_function_t<N> > * tmp0 = &(succ_vecs_pt[i]);
    vector<bool_function_t<N> > * tmp1 = &(succ_vecs_pt[i^1]);
    tmp1->clear();
    sort(tmp0->begin(), tmp0->end());
    auto it0 = tmp0->begin();
    tmp1->push_back(*it0);
    it0++;
    while (it0 != tmp0->end())
    {
      if (it0->bit_slice != tmp1->back().bit_slice)
      {
        tmp1->push_back(*it0);
      }
      it0++;
    }
  };

  auto to_expand = f1_succ.find(lambda);
  if(to_expand != f1_succ.end())
  {
    for(auto bool_op : b)
    {
      int bool_op_cost = bool_op.op_cost;
      vector<bool_function_t<N> > successors[2];
      successors[0].reserve(MAX_VEC_NODES);
      successors[1].reserve(MAX_VEC_NODES);
      vector<bool_function_t<N> >* succ_vec_pt;
      int i = 0;
      succ_vec_pt = &(successors[i]);

      switch (bool_op.op_id)
      {
        case NOT1  :
                    for(auto f1 = (*to_expand).second.begin(); f1 != (*to_expand).second.end(); f1++)
                    {
                      bool_op_one_input(&(*f1), succ_vec_pt, bool_op);
                      if (succ_vec_pt->size() >= MAX_VEC_NODES)
                      {
                        PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                        remove_dup(successors, i);
                        i = i ^ 1;
                        succ_vec_pt = &(successors[i]);
                        PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                      }
                    }
                    break;
        case AND2  :
        case OR2   :
        case NAND2 :
        case NOR2  :
        case XOR2  :
        case XNOR2 :
        case MAOI1 :
        case MOAI1 : 
                    for(auto f1 = (*to_expand).second.begin(); f1 != (*to_expand).second.end(); f1++)
                    {
                      for(auto f2map = f1_succ.begin(); f2map != to_expand; f2map++)
                      {
                        for (auto f2 = (*f2map).second.begin(); f2 != (*f2map).second.end(); f2++)
                        {
                          bool_op_two_inputs(&(*f1), &(*f2), succ_vec_pt, bool_op);
                          if (succ_vec_pt->size() >= MAX_VEC_NODES)
                          {
                            PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                            remove_dup(successors, i);
                            i = i ^ 1;
                            succ_vec_pt = &(successors[i]);
                            PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                          }
                        }
                      }
                      for (auto f2 = (*to_expand).second.begin(); f2 != f1; f2++)
                      {
                        bool_op_two_inputs(&(*f1), &(*f2), succ_vec_pt, bool_op);
                        if (succ_vec_pt->size() >= MAX_VEC_NODES)
                        {
                          PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                          remove_dup(successors, i);
                          i = i ^ 1;
                          succ_vec_pt = &(successors[i]);
                          PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                        }
                      }
                    }
                    break;
        case ANDN2 :
        case ORN2  :
                    for(auto f1 = (*to_expand).second.begin(); f1 != (*to_expand).second.end(); f1++)
                    {
                      for(auto f2map = f1_succ.begin(); f2map != to_expand; f2map++)
                      {
                        for (auto f2 = (*f2map).second.begin(); f2 != (*f2map).second.end(); f2++)
                        {
                          bool_op_two_inputs(&(*f1), &(*f2), succ_vec_pt, bool_op);
                          bool_op_two_inputs(&(*f2), &(*f1), succ_vec_pt, bool_op);
                          if (succ_vec_pt->size() >= MAX_VEC_NODES)
                          {
                            PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                            remove_dup(successors, i);
                            i = i ^ 1;
                            succ_vec_pt = &(successors[i]);
                            PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                          }
                        }                        
                      }
                      for (auto f2 = (*to_expand).second.begin(); f2 != f1; f2++)
                      {
                        bool_op_two_inputs(&(*f1), &(*f2), succ_vec_pt, bool_op);
                        bool_op_two_inputs(&(*f2), &(*f1), succ_vec_pt, bool_op);
                        if (succ_vec_pt->size() >= MAX_VEC_NODES)
                        {
                          PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                          remove_dup(successors, i);
                          i = i ^ 1;
                          succ_vec_pt = &(successors[i]);
                          PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                        }
                      }
                    }
                    break;
        case AND3  : 
        case OR3   : 
        case NAND3 : 
        case NOR3  :
                    for(auto f1 = (*to_expand).second.begin(); f1 != (*to_expand).second.end(); f1++)
                    {
                      for(auto f2map = f1_succ.begin(); f2map != to_expand; f2map++)
                      {
                        for (auto f2 = (*f2map).second.begin(); f2 != (*f2map).second.end(); f2++)
                        {
                          for(auto f3map = f1_succ.begin(); f3map != f2map; f3map++)
                          {
                            for (auto f3 = (*f3map).second.begin(); f3 != (*f3map).second.end(); f3++)
                            {
                              bool_op_three_inputs(&(*f1), &(*f2), &(*f3), succ_vec_pt, bool_op);
                              if (succ_vec_pt->size() >= MAX_VEC_NODES)
                              {
                                PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                                remove_dup(successors, i);
                                i = i ^ 1;
                                succ_vec_pt = &(successors[i]);
                                PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                              }
                            }
                          }
                          for (auto f3 = (*f2map).second.begin(); f3 != f2; f3++)
                          {
                            bool_op_three_inputs(&(*f1), &(*f2), &(*f3), succ_vec_pt, bool_op);
                            if (succ_vec_pt->size() >= MAX_VEC_NODES)
                            {
                              PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                              remove_dup(successors, i);
                              i = i ^ 1;
                              succ_vec_pt = &(successors[i]);
                              PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                            }
                          }
                        }
                      }
                      for (auto f2 = (*to_expand).second.begin(); f2 != f1; f2++)
                      {
                        for (auto f3 = (*to_expand).second.begin(); f3 != f2; f3++)
                        {
                          bool_op_three_inputs(&(*f1), &(*f2), &(*f3), succ_vec_pt, bool_op);
                          if (succ_vec_pt->size() >= MAX_VEC_NODES)
                          {
                            PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                            remove_dup(successors, i);
                            i = i ^ 1;
                            succ_vec_pt = &(successors[i]);
                            PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());
                          }
                        }
                      }
                    }
                    break;
      }

      if(succ_vec_pt->size() != 0)
      { 
        PRINT("Line:: %lu: Before remove_dup successors.size(): %lu\n", succ_vec_pt->size());
        remove_dup(successors, i);
        i = i ^ 1;
        succ_vec_pt = &(successors[i]);
        PRINT("Line:: %lu: After remove_dup successors.size(): %lu\n", succ_vec_pt->size());

        auto s = f1_succ.find(lambda + bool_op_cost);
        set<bool_function_t<N> > new_list;
        v_list_process(lambda, bool_op_cost, succ_vec_pt, &new_list);

        if(s != f1_succ.end())
        {
          s->second.insert(new_list.begin(), new_list.end());     
        }
        else
        {
          f1_succ.insert(pair<int,set<bool_function_t<N> > >(lambda + bool_op_cost, new_list));
        }
      }
    }
  }
}

template<int N>
void faster<N>::v_list_process(int lambda, int op_cost, vector<bool_function_t<N> > *tmp,
                    set<bool_function_t<N> > *to_insert)
{
  PRINT("Entering v_list_process\n");

  #pragma omp parallel for num_threads(omp_nb_threads)
  for(auto it_tmp = tmp->begin(); it_tmp < tmp->end(); it_tmp++)
  {
    if(!is_in_graphe(lambda, op_cost, (*it_tmp)))
    {
      #pragma omp critical
      {
        to_insert->insert(*it_tmp);
        nodes_cmp++;
        if(nodes_cmp == max_nodes)
        {
          exit_m();
        }
      
        if(lambda > pre_l)
        {
          if(is_in_graphe_collision(lambda, op_cost, (*it_tmp)))
          {
            get_implementation();
          }
        }
      }
    }
  }
  PRINT("Leaving v_list_process\n");
}

template<int N>
bool faster<N>::is_in_graphe(int lambda, int op_cost, bool_function_t<N> f)
{
  auto find = [](const set<bool_function_t<N> > & func_set, const bit_slice_l_t<N> & bit_slice)
  {
    for(auto set_it = func_set.begin(); set_it != func_set.end(); set_it++)
    {
      if (set_it->bit_slice == bit_slice)
      {
        return set_it;
      }
    }
    return func_set.end();
  };

  for(auto it = f1_succ.begin(); it != f1_succ.end() ; it++)
  {
    auto found_f = find((*it).second, f.bit_slice);
    if(found_f != (*it).second.end())
    {
      if(((*it).first > lambda+op_cost) || (((*it).first == lambda+op_cost) && ((*found_f).area > f.area)))
      {
        #pragma omp critical (UpdateGraphe)
        {
          (*it).second.erase(found_f);
        }
        return false;
      }
      return true;
    }
  }
  return false;
}

template<int N>
bool faster<N>::is_in_graphe_collision(int lambda, int op_cost, bool_function_t<N> f)
{
  for (int i = 0; i < N; i++)
  {
    if (f.bit_slice == target.coordinates[i].bit_slice)
    {
      if(lambda + op_cost <= (int)target.depth[i])
      {
        target.depth[i] = lambda + op_cost;
        target.coordinates[i] = f;

        flag |= (1UL << i);

        if (flag == FOUND_ALL)
        {
          return true;
        }
      }
      break;
    }
  }
  return false;
}

#ifdef PRINT
#undef PRINT
#endif
#ifdef MAX_VEC_NODES
#undef MAX_VEC_NODES
#endif
#ifdef pre_parse_args
#undef pre_parse_args
#endif
#ifdef post_parse_args
#undef post_parse_args
#endif

#endif // #define FASTER_MITM_H__