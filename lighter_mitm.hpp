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

#ifndef LIGHTER_MITM_H__
#define LIGHTER_MITM_H__

using namespace Peigen;
using namespace Peigen::weight;
using namespace std;

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


#define post_parse_args()                                     \
  delete [] args_str;                                         \
  max_nodes = max_ram * 100000000UL / sizeof(function_t<N>);  \
  if (gate_maoi1 == 1) gate_xor2 = 0;                         \
  if (gate_moai1 == 1) gate_xnor2 = 0;


template<int N>
void lighter<N>::pre_compute(string args)
{
  clock_t t1 = clock();

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "c:z:vap:f:r:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'c': pre_l = abs(atoi(optarg))*100; cout << "Precompute Limit : "<< optarg << endl; break;
      case 'z': cntpoint = abs(atoi(optarg))*100; cout << "Continue from breakpoint : "<< optarg << endl; break;
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
void lighter<N>::pre_computing()
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  f1.sort(); f1.bit_slice_to_LUT();
  f1_succ.clear();
  f2_succ.clear();

  if (cntpoint != -1)
  {
    read_pre_bin(cntpoint);
    count_list = cntpoint;
    while (count_list <= pre_l)
    {
      expand(&f1_succ, &f2_succ, count_list);
      if (verbose) cout << count_list << endl;
      write_pre_bin(count_list+min_GE);
      count_list++;
    }
  }
  else
  {
    f1_succ.insert(pair<int, set<function_t<N> > >(0, {f1}));
    write_pre_bin(0);
    count_list = 0;
    while (count_list <= pre_l)
    {
      expand(&f1_succ, &f2_succ, count_list);
      if (verbose) cout << count_list << endl;
      write_pre_bin(count_list+min_GE);
      count_list++;
    }
  }
}

template<int N>
void lighter<N>::write_pre_bin(int lambda)
{
  ofstream fout;
  size_t function_s = sizeof(function_t<N>);
  auto map_it = f1_succ.find(lambda);
  while (map_it != f1_succ.end())
  {
    string pre_filen = "pre_" + imp_info + "_" + to_string(map_it->first) + ".bin";
    fout.open(pre_filen, ios::binary);
    for (auto set_it = map_it->second.begin(); set_it != map_it->second.end(); set_it++)
    {
      fout.write((char *)&(*set_it), function_s);
    }
    fout.close();
    map_it++;
  }
}

template<int N>
void lighter<N>::read_pre_bin(int completed)
{
  ifstream fin;
  function_t<N> f_t;
  int lambda = 0;
  size_t function_s = sizeof(function_t<N>);

  f1_succ.clear();
  while (lambda <= (completed + max_GE))
  {
    string pre_filen = "pre_" + imp_info + "_" + to_string(lambda) + ".bin";
    fin.open(pre_filen, ios::binary);
    if (fin)
    {
      set<function_t<N> > new_list;    
      f1_succ.insert(pair<int, set<function_t<N> > >(lambda, new_list));
      auto to_insert = &(f1_succ[lambda]);
      while (fin.read((char*)(&f_t), function_s))
      {
        (*to_insert).insert(f_t);
      }
    }
    fin.close();
    lambda++;
  }
  if(verbose) print_graphe_info();
}

template<int N>
void lighter<N>::search_batch(string args)
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  function_t<N> f2 = function_t<N>::INPUT_DEFAULT();
  string sboxesfile = "sboxes" + to_string(N) + ".txt";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "i:o:l:c:vuwap:f:r:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'u': one_expand = true; break;
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
  read_pre_bin(pre_l);

  ifstream sboxes(sboxesfile);
  string instance_LUT;
  for(string line; getline(sboxes, line); )
  {
    istringstream instance_line(line);
    if(getline(instance_line, instance_name, ','))
    {
      if(getline(instance_line, instance_LUT))
      {
        init_varbles();

        f2.parse_function(instance_LUT);

        if (f2.is_permutation())
        {
          clock_t t1 = clock();

          cout << "From : " << f1.to_string() << endl;
          cout << "To   : " << f2.to_string() << endl;
    
          mitm(f1, f2);
    
          cout << "Best : " << (double)(shortest_path)/100 << " GE" << endl;
  
          string is_best;
          if (shortest_path <= (2*(l+min_GE) - max_GE)) is_best = "Best";
          else is_best = "Unknown";

          statistician.open(imp_info + "__" + "statistics.csv", ios::app);
          statistician << instance_name << "," << double(shortest_path) / 100 << "," << is_best << "," << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
          statistician.close();

          t1 = clock() - t1;
          cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
        }
      }
    }
  }
  reset();
}

template<int N>
double lighter<N>::search_single(string args)
{
  clock_t t1 = clock();

  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  function_t<N> f2 = function_t<N>::INPUT_DEFAULT();
  string instance_LUT;
  instance_name = "";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "i:o:c:l:vuwap:f:r:n:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'u': one_expand = true; break;
      case 'w': write_in_file = true; break;
      case 'i': f1.parse_function(optarg); break;
      case 'o': f2.parse_function(optarg); instance_LUT = optarg; break;
      case 'n': instance_name = optarg; break;
      case 'l': l = abs(atoi(optarg))*100; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  if(instance_name == "") instance_name = instance_LUT;
  genImpInfo();

  init_b(conf_file);
  init_varbles();

  read_pre_bin(pre_l);

  cout << "From : " << f1.to_string() << endl;
  cout << "To   : " << f2.to_string() << endl;

  mitm(f1, f2);

  double best = (double)(shortest_path)/100;
  cout << "Best : " << best << " GE" << endl;

  string is_best;
  if (shortest_path <= (2*(l+min_GE) - max_GE)) is_best = "Best";
  else is_best = "Unknown";

  statistician.open(imp_info + "__" + "statistics.csv", ios::app);
  statistician << instance_name << "," << best << "," << is_best << "," << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
  statistician.close();

  reset();

  t1 = clock() - t1;
  cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
  return best;
}

template<int N>
void lighter<N>::mitm(function_t<N> f1, function_t<N> f2)
{
  /*
    initilisation of the two graphs
    we start by adding the function f and the identity
  */
  unsorted_arrival = f2;
  f1.sort();
  f1.bit_slice_to_LUT();
  f2.sort();
  f2.bit_slice_to_LUT();

  if (pre_l < 0) f1_succ.insert(pair<int, set<function_t<N> > >(0, {f1}));
  f2_succ.insert(pair<int, set<function_t<N> > >(0, {f2}));
  start = f1;
  arrival = f2;

  /*
    we expand the two graphs
  */
  count_list = 0;

  while(count_list <= l)
  {
    if (count_list > pre_l)
    {
      expand(&f1_succ, &f2_succ, count_list);
      pre_l = count_list;
    }
    else if (one_expand || (count_list == 0))
    {
      function_t<N>  fun_found;
      int vect_found;

      if(is_in_graphe_collision(count_list, 0, f2, &fun_found, &vect_found, &f1_succ))
      {
        get_implementation(f2, fun_found, vect_found, &f2_succ, &f1_succ);
      }
    }

    if(!one_expand)
    {
      expand(&f2_succ, &f1_succ, count_list);
    }

    if(verbose && (count_list%100) == 0) cout << "\033[0;32m" << count_list << "\033[0m" << endl;
    count_list++;
    if(2*count_list > shortest_path)
    {
      if (count_list > pre_l) write_pre_bin(0);
      exit_m();
      return;
    }
  }
  if(verbose) print_graphe_info();
  if (count_list > pre_l)
  {
    write_pre_bin(0);
  }
  exit_m();
  return;
}

template<int N>
void lighter<N>::expand(map<int, set<function_t<N> > > *current,
            map<int, set<function_t<N> > > *opposite,
            int lambda)
{
  auto to_expand = current->find(lambda);
  if(to_expand != current->end())
  {
    for(auto bool_op : b)
    {
      int bool_op_cost = bool_op.op_cost;
      vector<function_t<N> > successors;

      for(auto fun : (*to_expand).second)
      {
        bool_op_fun(bool_op.op_id, fun, &successors);
        if (successors.size() >= MAX_VEC_NODES)
        {
          sort(successors.begin(), successors.end());
          successors.erase(unique(successors.begin(), successors.end()), successors.end());
        }
      }
        
      if(successors.size() != 0)
      {
        sort(successors.begin(), successors.end());
        successors.erase(unique(successors.begin(), successors.end()), successors.end());
        
        auto s = current->find(lambda+bool_op.op_cost);
        set<function_t<N> > new_list;
        v_list_process(lambda, bool_op_cost, &successors, current, &new_list, opposite);
        if(s != current->end())
        {
          s->second.insert(new_list.begin(), new_list.end());
        }
        else
        {
          current->insert(pair<int,set<function_t<N> > >(lambda+bool_op.op_cost, new_list));
        }
      }
    }
  }
}

template<int N>
void lighter<N>::v_list_process(int lambda, int op_cost, vector<function_t<N> > *tmp,
                    map<int, set<function_t<N> > > *current,
                    set<function_t<N> > *to_insert, map<int,set<function_t<N> > > *opposite)
{
  function_t<N>  fun_found;
  int vect_found;

  #pragma omp parallel for private(fun_found, vect_found) num_threads(omp_nb_threads)
  for(auto it_tmp = tmp->begin(); it_tmp < tmp->end(); it_tmp++)
  {
    if(!is_in_graphe(lambda, op_cost, (*it_tmp), current))
    {
      #pragma omp critical
      {
        (*it_tmp).bit_slice_to_LUT();
        to_insert->insert(*it_tmp);
        nodes_cmp++;
        if(nodes_cmp == max_nodes)
        {
          exit_m();
        }
      }
      if(is_in_graphe_collision(lambda, op_cost, (*it_tmp), &fun_found, &vect_found, opposite))
      {
        #pragma omp critical
        {
          //system("notify-send 'Collision'");
          get_implementation((*it_tmp), fun_found, vect_found, current, opposite);
        }
      }
    }
  }
}

template<int N>
bool lighter<N>::is_in_graphe(int lambda, int op_cost, function_t<N> f, map<int, set<function_t<N> > > *g)
{
  for(auto it = g->begin(); it != g->end() ; it++)
  {
    if((*it).second.find(f) != (*it).second.end())
    {
      if((*it).first > lambda+op_cost)
      {
        #pragma omp critical
        {
          (*it).second.erase(f);
        }
        return false;
      }
      return true;
    }
  }
  return false;
}

template<int N>
bool lighter<N>::is_in_graphe_collision(int lambda, int op_cost, function_t<N> f,
                            function_t<N> *fun_found,
                            int *vect_found,
                            map<int, set<function_t<N> > > *g)
{
  for(auto it = g->begin(); it != g->end(); it++)
  {
    if (it->first >= lambda - max_GE)
    {
      if( lambda + (*it).first + op_cost >= shortest_path ) return false;
      if((*it).second.find(f) != (*it).second.end())
      {
        *vect_found = (*it).first;
        *fun_found = *((*it).second.find(f));
        return true;
      }
    }
  }
  return false;
}

template<int N>
double lighter<N>::search_single_concatenate(string args)
{
  clock_t t1 = clock();

  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  function_t<N> f2 = function_t<N>::INPUT_DEFAULT();
  string instance_LUT;
  instance_name = "";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "i:o:c:l:vuwap:f:r:n:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'u': one_expand = true; break;
      case 'w': write_in_file = true; break;
      case 'i': f1.parse_function(optarg); break;
      case 'o': f2.parse_function(optarg); instance_LUT = optarg; break;
      case 'n': instance_name = optarg; break;
      case 'l': l = abs(atoi(optarg))*100; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  if(instance_name == "") instance_name = instance_LUT;

  genImpInfo();
  init_b(conf_file);
  init_varbles();

  read_pre_bin(pre_l);

  cout << "From : " << f1.to_string() << endl;
  cout << "To   : " << f2.to_string() << endl;

  concatenate(f1, f2);

  double best = (double)shortest_path/100.0;
  cout << "Best : " << best << " GE" << endl;

  string is_best;
  if (shortest_path <= (2*(l+min_GE) - max_GE)) is_best = "Best";
  else is_best = "Unknown";

  statistician.open(imp_info + "__" + "statistics.csv", ios::app);
  statistician << instance_name << "," << best << "," << is_best << "," << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
  statistician.close();

  reset();

  t1 = clock() - t1;
  cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
  return best;
}

template<int N>
void lighter<N>::search_batch_concatenate(string args)
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  function_t<N> f2 = function_t<N>::INPUT_DEFAULT();
  string sboxesfile = "sboxes" + to_string(N) + ".txt";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "i:o:l:c:vuwap:f:r:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'u': one_expand = true; break;
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
  string statisticianfn = imp_info + "-o" + sboxesfile.substr(0, sboxesfile.rfind('.')) + "__" + "statistics.csv";
  statistician.open(statisticianfn.c_str(), ios::app);
  evaluate_writeTitleLine(statistician);         
  statistician.close();

  init_b(conf_file);

  read_pre_bin(pre_l);

  ifstream sboxes(sboxesfile);
  string instance_LUT;
  for(string line; getline(sboxes, line); )
  {
    istringstream instance_line(line);
    if(getline(instance_line, instance_name, ','))
    {
      if(getline(instance_line, instance_LUT))
      {
        clock_t t1 = clock();

        init_varbles();

        f2.parse_function(instance_LUT);
        string is_best = "-";
        if (f2.is_permutation())
        {
          cout << "From : " << f1.to_string() << endl;
          cout << "To   : " << f2.to_string() << endl;

          concatenate(f1, f2);

          cout << "Best : " << to_string((double)(shortest_path)/100.0) << " GE" << endl;

          if (shortest_path <= (2*(l+min_GE) - max_GE)) is_best = "Best";
          else is_best = "Unknown";
        }

        statistician.open(statisticianfn.c_str(), ios::app);
        evaluate(instance_name, f2, shortest_path, is_best, statistician);
        statistician.close();
        t1 = clock() - t1;
        cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
      }
    }
  }
  reset();
}

template<int N>
void lighter<N>::concatenate(function_t<N> f1, function_t<N> f2)
{
  unsorted_arrival = f2;

  f1.sort(); f1.bit_slice_to_LUT();
  f2.sort(PEo); f2.bit_slice_to_LUT();
  start = f1;
  arrival = f2;

  if (pre_l < 0) f1_succ.insert(pair<int, set<function_t<N> > >(0, {f1}));

  count_list = 0;
  while (count_list <= l + max_GE)
  {
    if ((count_list > pre_l) && (count_list <= l))
    {
      expand(count_list);
      match(count_list);
      write_pre_bin(count_list+min_GE);
      pre_l = count_list;
    }
    else
    {
      match(count_list);
    }

    if(verbose && (count_list%100) == 0) cout << count_list << endl;
    count_list++;

    if(2*count_list - max_GE + 1 > shortest_path)
    {
      exit_m();
      return;
    }
  }

  if (verbose) print_graphe_info();
  exit_m();
  return;
}

template<int N>
void lighter<N>::expand(int lambda)
{
  auto to_expand = f1_succ.find(lambda);
  if(to_expand != f1_succ.end())
  {
    for(auto bool_op : b)
    {
      int bool_op_cost = bool_op.op_cost;
      vector<function_t<N> > successors;

      for(auto fun : (*to_expand).second)
      {
        bool_op_fun(bool_op.op_id, fun, &successors);
        if (successors.size() >= MAX_VEC_NODES)
        {
          sort(successors.begin(), successors.end());
          successors.erase(unique(successors.begin(), successors.end()), successors.end());
        }
      }

      if(successors.size() != 0)
      {
        sort(successors.begin(), successors.end());
        successors.erase(unique(successors.begin(), successors.end()), successors.end());
        
        auto s = f1_succ.find(lambda+bool_op.op_cost);
        set<function_t<N> > new_list;
        v_list_process(lambda, bool_op_cost, &successors, &new_list);
        if(s != f1_succ.end())
        {
          s->second.insert(new_list.begin(), new_list.end());
        }
        else
        {
          f1_succ.insert(pair<int,set<function_t<N> > >(lambda+bool_op.op_cost, new_list));
        }
      }
    }
  }
}

template<int N>
void lighter<N>::v_list_process(int lambda, int op_cost, vector<function_t<N> > *tmp,
                    set<function_t<N> > *to_insert)
{
  function_t<N>  fun_found;
  int vect_found;

  #pragma omp parallel for private(fun_found, vect_found) num_threads(omp_nb_threads)
  for(auto it_tmp = tmp->begin(); it_tmp < tmp->end(); it_tmp++)
  {
    if(!is_in_graphe(lambda, op_cost, (*it_tmp)))
    {
      #pragma omp critical
      {
        (*it_tmp).bit_slice_to_LUT();
        to_insert->insert(*it_tmp);
        nodes_cmp++;
        if(nodes_cmp == max_nodes)
        {
          exit_m();
        }
      }
    }
  }
}

template<int N>
bool lighter<N>::is_in_graphe(int lambda, int op_cost, function_t<N> f)
{
  for(auto it = f1_succ.begin(); it != f1_succ.end() ; it++)
  {
    if((*it).second.find(f) != (*it).second.end())
    {
      if((*it).first > lambda+op_cost)
      {
        #pragma omp critical
        {
          (*it).second.erase(f);
        }
        return false;
      }
      return true;
    }
  }
  return false;
}

template<int N>
void lighter<N>::match(int c1)
{
  auto f1_it = f1_succ.find(c1);
  if (f1_it != f1_succ.end())
  {
    vector<const function_t<N>* > tmp_vec;
    for (auto set1_it = f1_it->second.begin(); set1_it != f1_it->second.end(); set1_it++)
	  {
	  	tmp_vec.push_back( &(*set1_it) );
	  }

    //for (auto set1_it = f1_it->second.begin(); set1_it != f1_it->second.end(); set1_it++)
    #pragma omp parallel for num_threads(omp_nb_threads)
    for (auto vec1_it = tmp_vec.begin(); vec1_it < tmp_vec.end(); vec1_it++)
    {
      #pragma omp flush
      if (2 * c1 - max_GE + 1 < shortest_path)
      {
        //auto func1 = *set1_it;
        auto func1 = **vec1_it;

		    bit_slice_t<N> cp_bit_slice = OPs<N>.composite(func1.LUT, arrival.LUT);
        function_t<N> func_comp(cp_bit_slice);

        int PEm[N];
        func_comp.sort(PEm);

        auto f2_it = f1_succ.begin();
        while ((f2_it != f1_succ.end()) && (f2_it->first <= (c1 + max_GE)))
        {
          #pragma omp flush
          if ((c1 + f2_it->first) < shortest_path)
          {
            int c2;
            c2 = f2_it->first;
            if (c2 > (c1 - max_GE))
            {
              auto vec2_it = f2_it->second.find(func_comp);
              if (vec2_it != f2_it->second.end())
              {
                auto func2 = *vec2_it;

                #pragma omp flush
                #pragma omp critical
                {
                  if (shortest_path > (c1 + c2))
                  {
                    shortest_path = c1 + c2;
                    get_implementation_concatenate(func2, c2, func1, c1, PEm);
                  }
                }
              }
            }
            f2_it++;
          }
          else
          {
            break;
          }
        }
      }
    }
  }
}

template<int N>
void lighter<N>::generate(string args)
{  
  clock_t t1 = clock();

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "o:l:c:vwap:f:r:s:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'w': write_in_file = true; break;
      case 'o': sboxesfile = optarg; break;
      case 'l': l = abs(atoi(optarg))*100; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 's': criteria_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  genImpInfo();

  for (int tid = 0; tid < omp_nb_threads; tid++)
  {
    string outfn = imp_info + "__" + "statistics.csv";
    string outfn_sub = "_Part" + to_string(tid);
    outfn.insert(outfn.rfind('.'), outfn_sub);
    ofstream outf(outfn.c_str(), ios::app);
    evaluate_writeTitleLine(outf);
    outf.close();
  }

  // statistician.open(imp_info + "__" + "statistics.csv", ios::app);
  // evaluate_writeTitleLine(statistician);
  // statistician.close();

  init_b(conf_file);
  init_critiera(criteria_file);

  read_pre_bin(pre_l);

  start = function_t<N>::INPUT_DEFAULT();

  generate();

  reset();

  t1 = clock() - t1;
  cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
}

#if 0
template<int N>
void lighter<N>::generate()
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  f1.sort();
  f1.bit_slice_to_LUT();

  if (pre_l < 0) f1_succ.insert(pair<int, set<function_t<N> > >(0, {f1}));

  count_list = 0;
  while (count_list <= l + max_GE)
  {
    if ((count_list > pre_l) && (count_list <= l))
    {
      expand(count_list);
      filter(count_list);
      write_pre_bin(count_list+min_GE);
      pre_l = count_list;
    }
    else
    {
      filter(count_list);
    }

    if(verbose && (count_list%100) == 0) cout << count_list << endl;
    count_list++;

    if(2*count_list - max_GE + 1 > Cost_Criteria)
    {
      exit_m();
      return;
    }
  }

  if(verbose) print_graphe_info();
  exit_m();
  return;  
}
#endif

template<int N>
void lighter<N>::generate()
{
  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  f1.sort();
  f1.bit_slice_to_LUT();

  if (pre_l < 0)
  {
    f1_succ.insert(pair<int, set<function_t<N> > >(0, {f1}));
  }

  count_list = 0;
  while (count_list <= l + max_GE)
  {
    if(verbose && (count_list%100) == 0) cout << count_list << endl;
    
    if ((count_list > pre_l) && (count_list <= l))
    {
      expand(count_list);
      filter_pre(count_list);      
    }
    else
    {
      filter_pre(count_list);
    }

    count_list++;

    // All S-boxes which can be implemented with <= l + min_GE
    // have been stored in the precomputed graph
    // Thus, if (l + min_GE >= Cost_Criteria) and all precomputed
    // S-boxes has been checked, no need to expand the graph further.
    if((count_list <= l + min_GE) && (count_list > Cost_Criteria))
    {
      exit_m();
      return;
    }
    else if ((count_list > l) && (count_list > Cost_Criteria))
    {
      break;
    }
  }

  #if 0
  // c1 + c2 <= c1 + c1 + max_GE; c1 + c2 > l + min_GE;
  // c1 + c1 + max_GE > l + min_GE
  int c1LowerBound = (l + min_GE >= max_GE ? (((l + min_GE - max_GE) / 2) + 1) : 0);
  count_list = c1LowerBound;
  while ((count_list <= l + max_GE) &&
         (count_list + count_list - max_GE + 1 <= Cost_Criteria)) // c1 + c2 >= c1 + c1 - max_GE + 1
  {
    filter(count_list);

    if(verbose && (count_list%100) == 0)
    {
      cout 
        << (count_list + count_list - max_GE + 1) << " ~ "
        << (count_list + count_list + max_GE) << endl;
    }
    count_list++;
  }
  #endif

  // c1 + c2 <= c1 + c1 + max_GE; c1 + c2 > l + min_GE;
  // c1 + c1 + max_GE > l + min_GE
  int c1LowerBound = (l + min_GE >= max_GE ? (((l + min_GE - max_GE) / 2) + 1) : 0);
  count_list = l + max_GE;
  while (count_list >= c1LowerBound) // c1 + c2 >= c1 + c1 - max_GE + 1
  {
    if(verbose && (count_list%100) == 0)
    {
      cout 
        << (count_list + count_list - max_GE + 1) << " ~ "
        << (count_list + count_list + max_GE) << endl;
    }
    
    if (count_list + count_list - max_GE + 1 > Cost_Criteria)
    {
      count_list--;
      continue;
    }

    filter(count_list);


    count_list--;
  }


  if(verbose) print_graphe_info();
  exit_m();
  return;  
}

template<int N>
void lighter<N>::filter_pre(int c1)
{
  auto f1_it = f1_succ.find(c1);
  if (f1_it != f1_succ.end())
  {
    vector<const function_t<N>* > tmp_vec;
    for (auto set1_it = f1_it->second.begin(); set1_it != f1_it->second.end(); set1_it++)
	  {
	  	tmp_vec.push_back( &(*set1_it) );
	  }

    #pragma omp parallel for num_threads(omp_nb_threads)
    for (auto vec1_it = tmp_vec.begin(); vec1_it < tmp_vec.end(); vec1_it++)
    {
      int good_idx = 0;
      int good_idx_old = -1;

      #pragma omp flush
      if (c1 <= Cost_Criteria) // for parallel; Cost_Criteria may reduced by other threads
      {
        auto func1 = **vec1_it;
        if (is_good(func1))
        {
          #pragma omp critical
          {
            if (c1 <= Cost_Criteria)
            {
              if (c1 < Cost_Criteria) Cost_Criteria = c1;
              good_idx = good_number;
              good_number++;
            }
          }
          if (good_idx != good_idx_old)
          {
            get_implementation_good(func1, c1, good_idx);
            good_idx_old = good_idx;
          }        
        }
      }
    }
  }
}

template<int N>
void lighter<N>::filter(int c1)
{ // Any Implementation can be decomposed into two parts.
  // Suppose Imp = Imp1 || Imp2
  // Use the following decomposed, we can find
  // There exist Imp1 and Imp2, such that cost of Imp1 = C1 and cost of Imp2 = C2, s.t.
  // C1 <= C2 and C2 - C1 <= 2 * max_GE;
  // Init: C1 = C; C2 = 0
  // While C1 > C2: C1 -= deltai; C2 += deltai; move one Instruction in Imp1 to Imp2;
  // Finished: C1 <= C2; C1 + delta_last > C2 - delta_last; => C2 - C1 < 2*delta;
  // If C2 - C1 < delta: this case is covered by testing c1 || c2, and c1 <= c2 <= c1 + max_GE;
  // If C2 - C1 >= delta: we adjust again by setting C1 += delta; C2 -= delta; i.e. move the last instruction back to Imp1
  //                      then C1 - C2 < 2*delta - delta = delta
  //                      this case is covered by testing c1 || c2, and c1 - max_GE <= c2 < c1
  // Thus, any implementation can be decomposed into two parts, the larger part costs no more max_GE than the smaller part
  // We do not plainly considered c2 in [c1, c1 + 2*max_GE] for the sake of efficiency.
  // For fixed c1, we considered c2 in [c1 - max_GE + 1, c1 + max_GE]
  int c2LowerBound = (c1 - max_GE + 1) > (l + min_GE- c1 + 1 ) ? (c1 - max_GE + 1) : (l + min_GE - c1 + 1);
  // S-boxes which cost less than $l + min_GE$ has already been examed in filter_pre()
  // Thus, only need to exam c1 + c2 > l + min_GE => c2 > l + min_GE - c1
  // Combination c1 + (c1 - max_GE) is already examed under the 
  // combination c1' + (c1' + max_GE), where c1' = c1 - max_GE 
  int c2UpperBound = c1 + max_GE;
  int curCostLowerBound = c1 + c2LowerBound;

  auto f1_it = f1_succ.find(c1);
  if (f1_it != f1_succ.end())
  {
    vector<const function_t<N>* > tmp_vec;
    for (auto set1_it = f1_it->second.begin(); set1_it != f1_it->second.end(); set1_it++)
	  {
	  	tmp_vec.push_back( &(*set1_it) );
	  }

    #pragma omp parallel for num_threads(omp_nb_threads)
    for (auto vec1_it = tmp_vec.begin(); vec1_it < tmp_vec.end(); vec1_it++)
    {
      int good_idx = 0;
      int good_idx_old = -1;

      #pragma omp flush
      if (curCostLowerBound <= Cost_Criteria) // for parallel; Cost_Criteria may reduced by other threads
      {
        auto func1 = **vec1_it;
        vector<function_t<N> > func_perms;
        func_perms.reserve(FACT_(N));
        for (int i = 0; i < FACT_(N); i++)
        {
            function_t<N> func_perm;
            OPs<N>.composite(func_perm.LUT, PE<N>.func[i].LUT, func1.LUT);
            func_perm.LUT_to_bit_slice();
            func_perms.push_back(func_perm);
        }

        auto f2_it = f1_succ.begin();
        while ((f2_it != f1_succ.end()) && (f2_it->first <= c2UpperBound))
        {
          int c2 = f2_it->first;
          int curCost = c1 + c2;

          if (c2 < c2LowerBound)
          {
            f2_it++;
            continue;
          }
          
          #pragma omp flush
          if (curCost > Cost_Criteria)
          {
            break;
          }

          auto vec2_it = f2_it->second.begin();
          while (vec2_it != f2_it->second.end())
          {
            auto func2 = *vec2_it;

            function_t<N> func_comp;
            for (int i = 0; i < FACT_(N); i++)
            {
                OPs<N>.composite(func_comp.LUT, func2.LUT, func_perms[i].LUT);
                func_comp.LUT_to_bit_slice();
                if (is_good(func_comp))
                {
                  #pragma omp critical
                  {
                    if (curCost <= Cost_Criteria)
                    {
                      Cost_Criteria = curCost;
                      good_idx = good_number;
                      good_number++;
                    }
                  }
                  if (good_idx != good_idx_old)
                  {
                    get_implementation_good(func1, c1, func_perms[i], i, func2, c2, func_comp, good_idx);
                    good_idx_old = good_idx;
                  }                    
                }
            }
            vec2_it++;
          }            
          f2_it++;
        }
        func_perms.clear();
        func_perms.shrink_to_fit();
      }
    }
  }
}

template<int N>
void lighter<N>::evaluate_filter(string args)
{
  clock_t t1 = clock();

  function_t<N> f1 = function_t<N>::INPUT_DEFAULT();
  function_t<N> f2 = function_t<N>::INPUT_DEFAULT();
  string sboxesfile = "sboxes" + to_string(N) + ".txt";

  pre_parse_args();
  while ((opt = getopt_long(myargc, myargv, "o:l:c:vwap:f:r:s:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'c': pre_l = atoi(optarg)*100; cout << "Precomputed Limit : "<< optarg << endl; break;
      case 'a': all_gates(); break;
      case 'v': verbose = true; break;
      case 'w': write_in_file = true; break;
      case 'o': sboxesfile = optarg; break;
      case 'l': l = abs(atoi(optarg))*100; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 's': criteria_file = optarg; break;
      case 'r': max_ram = atol(optarg); break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }
  post_parse_args();

  start = function_t<N>::INPUT_DEFAULT();

  init_b(conf_file);
  init_critiera(criteria_file);

  if (Cost_Criteria != -1)
  {
    genImpInfo();
    read_pre_bin(pre_l);
  }
  string statisticianfn = imp_info + "-c" + criteria_file.substr(0, criteria_file.rfind('.')) + "-o" + sboxesfile.substr(0, sboxesfile.rfind('.')) + "__" + "statistics.csv";
  statistician.open(statisticianfn.c_str(), ios::app);
  evaluate_writeTitleLine(statistician);
  statistician.close();
  string is_best = "-";

  ifstream sboxes(sboxesfile);
  string instance_LUT;
  for(string line; getline(sboxes, line); )
  {
    istringstream instance_line(line);
    if(getline(instance_line, instance_name, ','))
    {
      if(getline(instance_line, instance_LUT))
      {
        f2.parse_function(instance_LUT);

        if (is_good(f2))
        {
          if (Cost_Criteria != -1)
          {
            init_varbles();
            concatenate(f1, f2);
            if (shortest_path <= (2*(l+min_GE) - max_GE)) is_best = "Best";
            else is_best = "Unknown";
            if (shortest_path <= Cost_Criteria)
            {
              statistician.open(statisticianfn.c_str(), ios::app);
              evaluate(instance_name, f2, shortest_path, is_best, statistician);
              statistician.close();
            }
          }
          else
          {
            statistician.open(statisticianfn.c_str(), ios::app);
            evaluate(instance_name, f2, 0, is_best, statistician);
            statistician.close();
          }
        }
      }
    }
  }

  reset();

  t1 = clock() - t1;
  cout << "Takes time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
}

template<int N>
void lighter<N>::evaluate_writeTitleLine(ofstream & outf)
{
  string conf = conf_file.substr(0, conf_file.rfind('.'));
  outf
    << "Cipher"          << ","
    << "LUT"             << ","
    << "bit_slice"       << ","
    << "Permutation"     << ","
    << "Involution"      << ","
    << "Diff"            << ","
    << "DiffFreq"        << ","
    << "Diff1"           << ","
    << "CardD1"          << ","
    << "Lin"             << ","
    << "LinFreq"         << ","
    << "Lin1"            << ","
    << "CardL1"          << ","
    << "max_degree"      << ","
    << "min_degree"      << ","
    << "MaxDegreeFreq"   << ","
    << "MinDegreeFreq"   << ","
    << "Max_ProductDegrees" << ","
    << "LS_number"       << ","
    << "\"max_v (v, w)-linear\"" << ","
    << "\"max_w (v, w)-linear\"" << ","
    << "Optimal_Class"   << ","
    << "Cost (GE) " + conf  << ","
    << "Cost is Best "      << ","
    << "inv_LUT"            << ","
    << "inv_bit_slice"      << ","
    << "inv_max_degree"     << ","
    << "inv_min_degree"     << ","
    << "inv_MaxDegreeFreq"  << ","
    << "inv_MinDegreeFreq"  << ","
    << "inv_Max_ProductDegrees"<< ","
    << "inv_LS_number"       << ","
    << "\"inv_max_v (v, w)-linear\"" << ","
    << "\"inv_max_w (v, w)-linear\"" << ","
    << "inv_Optimal_Class"
    << endl;
}


template<int N>
void lighter<N>::evaluate(string sboxesfile, string outputfile)
{
  ifstream sboxes(sboxesfile);
  
  vector<pair<string, string> > sboxes_vec;
  for(string line; getline(sboxes, line); )
  {
    istringstream instance_line(line);
    string name_str;
    string LUT_str;
    if(getline(instance_line, name_str, ','))
    {
      if(getline(instance_line, LUT_str))
      {
        sboxes_vec.push_back(pair<string, string>(name_str, LUT_str));
      }
    }
  }
  sboxes.close();

  ofstream *outfs = new ofstream[omp_nb_threads];
  for (int tid = 0; tid < omp_nb_threads; tid++)
  {
    string outfn = outputfile;
    string outfn_sub = "_Part" + to_string(tid);
    outfn.insert(outfn.rfind('.'), outfn_sub);
    outfs[tid].open(outfn.c_str(), ios::app);
    evaluate_writeTitleLine(outfs[tid]);
  }

  #pragma omp parallel for num_threads(omp_nb_threads)
  for (auto vec_it = sboxes_vec.begin(); vec_it < sboxes_vec.end(); vec_it++)
  {   
    string name_str = (*vec_it).first;
    string LUT_str = (*vec_it).second;
    function_t<N> f(LUT_str);
    int tid = omp_get_thread_num();
    evaluate(name_str, f, 0, "-", outfs[tid]);
  }

  for (int tid = 0; tid < omp_nb_threads; tid++)
  {
    outfs[tid].close();
  }

  delete [] outfs;
}

template<int N>
void lighter<N>::evaluate_verbose(string sboxesfile, string outputfile_prefix)
{
  ifstream sboxes(sboxesfile);
  
  vector<pair<string, string> > sboxes_vec;
  for(string line; getline(sboxes, line); )
  {
    istringstream instance_line(line);
    string name_str;
    string LUT_str;
    if(getline(instance_line, name_str, ','))
    {
      if(getline(instance_line, LUT_str))
      {
        sboxes_vec.push_back(pair<string, string>(name_str, LUT_str));
      }
    }
  }
  sboxes.close();

  #pragma omp parallel for num_threads(omp_nb_threads)
  for (auto vec_it = sboxes_vec.begin(); vec_it < sboxes_vec.end(); vec_it++)
  {   
    string name_str = (*vec_it).first;
    string LUT_str = (*vec_it).second;
    string file_str = outputfile_prefix + "_" + name_str + ".txt";
    ofstream fout(file_str.c_str());
    function_t<N> f(LUT_str);
    fout << name_str << " Sbox:" << endl;
    fout << f.show_all_properties();
    if (f.is_permutation())
    {
      function_t<N> f_inv; 
      f_inv = f.inverse();
      fout << name_str << " Inverse Sbox: " << endl;
      fout << f_inv.show_all_properties();
    }
    fout.close();
  }
}

#endif // LIGHTER_MITM_H__