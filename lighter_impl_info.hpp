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

#ifndef LIGHTER_IMPL_INFO_H__
#define LIGHTER_IMPL_INFO_H__

#include "func.hpp"

using namespace Peigen;
using namespace Peigen::weight;
using namespace std;

template<int N>
void lighter<N>::update_unsorted_function(function_t<N> f)
{
  auto get_diff_index = [](function_t<N> f1, function_t<N> f2)
  {
    int i = 0;
    int j = 0;
    bool found = false;
    while(i < N)
    {
      while(j < N)
      {
        if(f1.bit_slice[i] == f2.bit_slice[j])
        {
          found = true;
          break;
        }
        j++;
      }
      if(found == false) return i;
      found = false;
      i++;
      j = 0;
    }
    return -1;
  };

  int i = get_diff_index(f, unsorted_function);
  int j = get_diff_index(unsorted_function, f);

  unsorted_function.bit_slice[j] = f.bit_slice[i];

  unsorted_function.prev = f.prev;
  unsorted_function.info_line = f.info_line;
  unsorted_function.info_op = f.info_op;

  bit_slice_l_t<N> mod = f.bit_slice[f.info_line];

  for(int i = 0; i < N; i++)
    if(unsorted_function.bit_slice[i] == mod) unsorted_function.info_line = i;
}

template<int N>
string lighter<N>::get_permut(function_t<N> f1, function_t<N> f2, char a, char b)
{
  auto get_index = [](int n, function_t<N> f1, function_t<N> f2)
  {
    int i = 0;
    while( i < N )
    {
      if(f1.bit_slice[n] == f2.bit_slice[i]) return i;
      i++;
    }
    return -1;
  };

  string s;
  int i = 0;
  while( i < N )
  {
    s = s + a + "[" + to_string(i) + "] = ";
    s = s + b + "[" + to_string(get_index(i, f2, f1)) + "];\n";
    i++;
  }
  return s;
}

template<int N>
void lighter<N>::get_string_impl(string *s, function_t<N> f, bool reverse)
{
  if( !reverse ) s->insert(0, bool_op_to_string(f, f.info_op, reverse));
  else s->append(bool_op_to_string(f, f.info_op, reverse));
}

template<int N>
int lighter<N>::get_semi_impl(function_t<N> f,
                    int count_vlist,
                    map<int, set<function_t<N> > > *g,
                    string *s,
                    bool reverse)
{
  int cost = bool_op_cost(f.info_op);
  auto found = g->at(0).find(f);
  unsorted_function = f;
  while(count_vlist >= 0)
  {
    update_unsorted_function(f);
    get_string_impl(s, unsorted_function, reverse);
    f.bit_slice[f.info_line] = f.prev;
    f.sort();
    count_vlist -= bool_op_cost(f.info_op);
    if( count_vlist == 0) break;
    found = g->at(count_vlist).find(f);
    f = *found;
    cost += bool_op_cost(f.info_op);
  }

  unsorted_function.bit_slice[unsorted_function.info_line] = unsorted_function.prev;

  if( !reverse )
  {
    s->insert(0, "\n" + get_permut(start, unsorted_function, 'F', 'X') + "\n");
    s->insert(0, "// from : " + start.to_string() + "\n");
  }
  else
  {
    s->append("\n" + get_permut(unsorted_function, unsorted_arrival, 'X', 'F') + "\n");
    s->append("// to : " + unsorted_arrival.to_string() + "\n");
  }
  
  return cost;
}

template<int N>
void lighter<N>::get_implementation(function_t<N> f1,
                      function_t<N> f2,
                      int v,
                      map<int, set<function_t<N> > > *g1,
                      map<int, set<function_t<N> > > *g2)
{
  static int impl_number = 0;
  if(instance_name != old_instance_name)
  {
    impl_number = 0;
    old_instance_name = instance_name;
  }
  cost_1 = 0;
  cost_2 = 0;

  string s1, s2;
  if(g1 == &f1_succ)
  {
    cost_1 = get_semi_impl(f1, count_list+bool_op_cost(f1.info_op), g1, &s1, false);
    cost_2 = get_semi_impl(f2, v, g2, &s2, true);
  }
  else
  {
    cost_1 = get_semi_impl(f1, count_list+bool_op_cost(f1.info_op), g1, &s1, true);
    cost_2 = get_semi_impl(f2, v, g2, &s2, false);
  }
  unsorted_function.sort();
  if(cost_1+cost_2 < shortest_path)
  {
     cout << "Generating implementation " << instance_name << " " << impl_number << endl;
     shortest_path = cost_1+cost_2;
     ofstream impl(imp_info + "_" + instance_name + "_" + to_string(impl_number) + ".c");
     impl << "// Implementation info.: " << imp_info << endl;
     if(unsorted_function == start) impl << s2 << s1;
     else if(unsorted_function == arrival) impl << s1 << s2;
     impl << "// Cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE" << endl;
     cout << "(cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE)" << endl;
     
     impl.close();
     impl_number ++;
  }
}

template<int N>
int lighter<N>::get_semi_impl_concatenate(function_t<N> f,
                    int count_vlist, string *s,
                    bool reverse, int PE[])
{
  int cost = bool_op_cost(f.info_op);
  auto found = f1_succ.at(0).find(f);
  unsorted_function = f;
  while(count_vlist >= 0)
  {
    update_unsorted_function(f);
    get_string_impl(s, unsorted_function, reverse);
    f.bit_slice[f.info_line] = f.prev;
    f.sort();
    count_vlist -= bool_op_cost(f.info_op);
    if( count_vlist == 0) break;
    found = f1_succ.at(count_vlist).find(f);
    f = *found;
    cost += bool_op_cost(f.info_op);
  }

  unsorted_function.bit_slice[unsorted_function.info_line] = unsorted_function.prev;

  if( !reverse )
  {
    s->insert(0, "\n" + get_permut(start, unsorted_function, 'F', 'X') + "\n");
    s->insert(0, "// from : " + start.to_string() + "\n");

    for (int i = 0; i < N; i++) s->append("X[" + to_string(i) + "] = F[" + to_string(PE[i]) + "];\n");
    for (int i = 0; i < N; i++) s->append("F[" + to_string(i) + "] = X[" + to_string(i) + "];\n");
  }
  else
  {
    s->append("\n" + get_permut(unsorted_function, start, 'X', 'F') + "\n");
    for (int i = 0; i < N; i++) s->append("F[" + to_string(i) + "] = X[" + to_string(PE[i]) + "];\n");
    for (int i = 0; i < N; i++) s->append("X[" + to_string(i) + "] = F[" + to_string(i) + "];\n");

    s->append("// to : " + unsorted_arrival.to_string() + "\n");
  }
  
  return cost;
}

template<int N>
void lighter<N>::get_implementation_concatenate(function_t<N> f1, int v1, function_t<N> f2, int v2, int PEm[])
{
  static int impl_number = 0;
  if(instance_name != old_instance_name)
  {
    impl_number = 0;
    old_instance_name = instance_name;
  }
  cost_1 = 0;
  cost_2 = 0;

  string s1, s2;
  cost_1 = get_semi_impl_concatenate(f1, v1, &s1, false, PEm);
  cost_2 = get_semi_impl_concatenate(f2, v2, &s2, true, PEo);

  unsorted_function.sort();

  cout << "Generating implementation " << instance_name << " " << impl_number << endl;
  if (shortest_path != cost_1+cost_2)
  {
    cout << "There must be wrong!!!" << endl;
  }
  ofstream impl(imp_info + "_" + instance_name + "_" + to_string(impl_number) + ".c");
  impl << "// Implementation info.: " << imp_info << endl;
  impl << s1 << s2;
  impl << "// Cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE" << endl;
  cout << "(cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE)" << endl;

  impl.close();
  impl_number ++;
}

template<int N>
int lighter<N>::get_semi_impl_good(
                    function_t<N> f,
                    int count_vlist,
                    string *s)
{
  function_t<N> f_org = f;
  int cost = bool_op_cost(f.info_op);
  auto found = f1_succ.at(0).find(f);
  unsorted_function = f;
  while(count_vlist >= 0)
  {
    update_unsorted_function(f);
    get_string_impl(s, unsorted_function, false);
    f.bit_slice[f.info_line] = f.prev;
    f.sort();
    count_vlist -= bool_op_cost(f.info_op);
    if( count_vlist == 0) break;
    found = f1_succ.at(count_vlist).find(f);
    f = *found;
    cost += bool_op_cost(f.info_op);
  }

  unsorted_function.bit_slice[unsorted_function.info_line] = unsorted_function.prev;

  s->insert(0, "\n" + get_permut(start, unsorted_function, 'F', 'X') + "\n");
  s->insert(0, "// from : " + start.to_string() + "\n");

  for (int i = 0; i < N; i++) s->append("X[" + to_string(i) + "] = F[" + to_string(i) + "];\n");
  s->append("// to : " + f_org.to_string() + "\n");

  return cost;
}

template<int N>
int lighter<N>::get_semi_impl_good(
                    function_t<N> f,
                    int count_vlist, string *s,
                    const function_t<N> & f_p, int pi)
{
  function_t<N> f_org = f;
  int cost = bool_op_cost(f.info_op);
  auto found = f1_succ.at(0).find(f);
  unsorted_function = f;
  while(count_vlist >= 0)
  {
    update_unsorted_function(f);
    get_string_impl(s, unsorted_function, false);
    f.bit_slice[f.info_line] = f.prev;
    f.sort();
    count_vlist -= bool_op_cost(f.info_op);
    if( count_vlist == 0) break;
    found = f1_succ.at(count_vlist).find(f);
    f = *found;
    cost += bool_op_cost(f.info_op);
  }

  unsorted_function.bit_slice[unsorted_function.info_line] = unsorted_function.prev;

  s->insert(0, "\n" + get_permut(start, unsorted_function, 'F', 'X') + "\n");
  s->insert(0, "// from : " + start.to_string() + "\n");
  s->append("// to : " + f_org.to_string() + "\n");
  for (int i = 0; i < N; i++) s->append("X[" + to_string(i) + "] = F[" + to_string(PE<N>.idx[pi][i]) + "];\n"); 
  s->append("// perm : " + f_p.to_string() + "\n");

  return cost;
}

template<int N>
void lighter<N>::get_implementation_good(
                      function_t<N> f1, int v1,
                      const function_t<N> & fp, int pi,
                      function_t<N> f2, int v2,
                      function_t<N> f3, int good_idx)
{
  int tid = omp_get_thread_num();
  string outfn = imp_info + "__" + "statistics_Part" + to_string(tid) + ".csv";
  ifstream inf(outfn.c_str());
  string thisSbox = f1.to_string();
  string existSbox;
  for (string line; getline(inf, line);)
  {
    istringstream is_line(line);
    getline(is_line, existSbox, ',');
    getline(is_line, existSbox, ',');
    getline(is_line, existSbox, ',');
    if (existSbox == thisSbox)
    {
      inf.close();
      return; // A duplicate S-box
    }
  }
  inf.close();

  string name = to_string(good_idx);
  string is_best = "-";
  int v = v1 + v2;
  if (v <= (2*(l+min_GE) - max_GE)) is_best = "Best";
  else is_best = "Unknown";
  evaluator<N> Eva(f3, name, v, is_best);

  ofstream outf(outfn.c_str(), ios::app);
  outf << Eva.show();
  outf.close();

  string fn_prefix = Eva.hash();

  #pragma omp critical
  {
    struct stat dirstate;
    if (stat(fn_prefix.c_str(), &dirstate) != 0 )
    {
      const int dir_err = mkdir(fn_prefix.c_str(), 0777);
      if (-1 == dir_err) cout << "Error creating directory!"  << endl;
    }

    string subSboxesfilen = fn_prefix + "/" + sboxesfile;
    ofstream sboxf(subSboxesfilen, ios::app);
    sboxf << name << "," << f3.LUT_to_string() << endl;
    sboxf.close();

    function_t<N> f3PErep = f3.PE_representative();
    string f3PErep_str = fn_prefix + "/" + f3PErep.to_string() + ".c";
    struct stat filestate;
    if (stat(f3PErep_str.c_str(), &filestate) != 0)
    {
      good_PE_number++;
      string PEsboxfn = imp_info + "_PEsboxes.txt";
      ofstream PEsboxf(PEsboxfn.c_str(), ios::app);
      PEsboxf << good_PE_number << "," << f3.LUT_to_string() << endl;
      PEsboxf.close();

      cost_1 = 0;
      cost_2 = 0;
      string s1, s2;

      cost_1 = get_semi_impl_good(f1, v1, &s1, fp, pi);
      cost_2 = get_semi_impl_good(f2, v2, &s2);
      s2.append("// final : " + f3.to_string() + "\n");

      ofstream impl(f3PErep_str);
      impl << "// Implementation info.: " << imp_info << endl;
      impl << s1 << s2;
      impl << "// Cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE" << endl;
      impl.close();

      cout << "Generating implementation " << name << endl;
      cout << "(cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE)" << endl;
    }  
  }
}

template<int N>
void lighter<N>::get_implementation_good(
                      function_t<N> f1, int v, int good_idx)
{
  int tid = omp_get_thread_num();
  string outfn = imp_info + "__" + "statistics_Part" + to_string(tid) + ".csv";
  ifstream inf(outfn.c_str());
  string thisSbox = f1.to_string();
  string existSbox;
  for (string line; getline(inf, line);)
  {
    istringstream is_line(line);
    getline(is_line, existSbox, ',');
    getline(is_line, existSbox, ',');
    getline(is_line, existSbox, ',');
    if (existSbox == thisSbox)
    {
      inf.close();
      return; // A duplicate S-box
    }
  }
  inf.close();

  string name = to_string(good_idx);
  string is_best = "-";
  if (v <= l+min_GE) is_best = "Best";
  else is_best = "Unknown";
  evaluator<N> Eva(f1, name, v, is_best);

  ofstream outf(outfn.c_str(), ios::app);
  outf << Eva.show();
  outf.close();

  string fn_prefix = Eva.hash();

  #pragma omp critical
  {
    struct stat dirstate;
    if (stat(fn_prefix.c_str(), &dirstate) != 0 )
    {
      const int dir_err = mkdir(fn_prefix.c_str(), 0777);
      if (-1 == dir_err) cout << "Error creating directory!"  << endl;
    }

    string subSboxesfilen = fn_prefix + "/" + sboxesfile;
    ofstream sboxf(subSboxesfilen, ios::app);
    sboxf << name << "," << f1.LUT_to_string() << endl;
    sboxf.close();

    function_t<N> f1PErep = f1.PE_representative();
    string f1PErep_str = fn_prefix + "/" + f1PErep.to_string() + ".c";
    struct stat filestate;
    if (stat(f1PErep_str.c_str(), &filestate) != 0)
    {
      good_PE_number++;
      string PEsboxfn = imp_info + "_PEsboxes.txt";
      ofstream PEsboxf(PEsboxfn.c_str(), ios::app);
      PEsboxf << good_PE_number << "," << f1.LUT_to_string() << endl;
      PEsboxf.close();

      cost_1 = 0;
      cost_2 = 0;
      string s1, s2;

      cost_1 = get_semi_impl_good(f1, v, &s1);

      ofstream impl(f1PErep_str);
      impl << "// Implementation info.: " << imp_info << endl;
      impl << s1;
      impl << "// Cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE" << endl;
      impl.close();

      cout << "Generating implementation " << name << endl;
      cout << "(cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE)" << endl;
    }  
  }
}

#endif // LIGHTER_IMPL_INFO_H__
