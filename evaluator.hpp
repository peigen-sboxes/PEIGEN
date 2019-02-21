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

#ifndef EVALUATOR_H__
#define EVALUATOR_H__

#include "constants.hpp"
#include "func.hpp"

using namespace Peigen;
using namespace std;

template<int N>
struct Peigen::evaluator
{
  string name;
  string LUT_str;
  string bit_slice_str;

  bool is_perm_flag;
  bool is_inv_flag;

  int DDT[LUT_UNIT_N][LUT_UNIT_N];
  int DDT_spectrum[LUT_UNIT_N+1];
  int Diff;
  int DiffFreq;

  int DDT1_spectrum[LUT_UNIT_N+1];
  int Diff1;
  int CardD1;

  int LAT[LUT_UNIT_N][LUT_UNIT_N];
  int LAT_spectrum[LUT_UNIT_N+1];
  int Lin;
  int LinFreq;

  int LAT1_spectrum[LUT_UNIT_N+1];
  int Lin1;
  int CardL1;

  int max_degree;
  int min_degree;
  int deg_spectrum[N+1];
  int max_degree_freq;
  int min_degree_freq;

  int dkT[N+1];
  int LS_nubmer;
  pair<int, int> max_v;
  pair<int, int> max_w;

  int whichG = -1;
  double cost = -1.0;
  string costisBest = "-";

  string inv_LUT_str = "-";
  string inv_bit_slice_str = "-";
  int inv_max_degree = -1;
  int inv_min_degree = -1;
  int inv_deg_spectrum[N+1];
  int inv_max_degree_freq = -1;
  int inv_min_degree_freq = -1;
  int inv_dkT[N+1];
  int inv_LS_nubmer;
  pair<int, int> inv_max_v;
  pair<int, int> inv_max_w;

  int inv_whichG = -1;

  evaluator(const function_t<N> & f, string aname = "-", int acost = -1, string acostisBest = "Unknown")
  {
    name = aname;
    cost = (double)acost/100.0;
    costisBest = acostisBest;

    LUT_str = f.LUT_to_string();
    bit_slice_str = f.to_string();

    is_perm_flag = f.is_permutation();
    is_inv_flag = f.is_involution();

    f.difference_distribution_matrix(DDT, Diff, DDT_spectrum, Diff1, DDT1_spectrum);
    DiffFreq = DDT_spectrum[Diff];  
    CardD1 = N * N - DDT1_spectrum[0];

    f.linear_approximation_matrix(LAT, Lin, LAT_spectrum, Lin1, LAT1_spectrum);
    LinFreq = LAT_spectrum[Lin];
    CardL1 = N * N - LAT1_spectrum[0];

    f.degree(deg_spectrum, max_degree, min_degree);
    max_degree_freq = deg_spectrum[max_degree];
    min_degree_freq = deg_spectrum[min_degree];

    f.k_product_degree_table(dkT);
    LS_nubmer = f.number_linear_structures();
    f.max_v_w_linear(max_v, max_w);

    whichG = f.which_optimal();

    if (is_perm_flag)
    {
      function_t<N> f_inv; 
      f_inv = f.inverse();

      inv_LUT_str = f_inv.LUT_to_string();
      inv_bit_slice_str = f_inv.to_string();

      f_inv.degree(inv_deg_spectrum, inv_max_degree, inv_min_degree);
      inv_max_degree_freq = inv_deg_spectrum[inv_max_degree];
      inv_min_degree_freq = inv_deg_spectrum[inv_min_degree];

      f_inv.k_product_degree_table(inv_dkT);
      inv_LS_nubmer = f_inv.number_linear_structures();
      f_inv.max_v_w_linear(inv_max_v, inv_max_w);

      inv_whichG = f_inv.which_optimal();
    }
  }

  string hash() const
  {
    stringstream ss;
    ss << setfill('0')
    << setw(1)                               << (is_perm_flag ? "T" : "F") << "_"
    << setw(1)                               << (is_inv_flag ? "T" : "F")  << "_"
    << setw(digits(LUT_UNIT_N))              << Diff                       << "_"
    << setw(digits(LUT_UNIT_N * LUT_UNIT_N)) << DiffFreq                   << "_"
    << setw(digits(LUT_UNIT_N))              << Diff1                      << "_"
    << setw(digits(N * N))                   << CardD1                     << "_"
    << setw(digits(LUT_UNIT_N))              << Lin                        << "_"
    << setw(digits(LUT_UNIT_N * LUT_UNIT_N)) << LinFreq                    << "_"
    << setw(digits(LUT_UNIT_N))              << Lin1                       << "_"
    << setw(digits(N * N))                   << CardL1                     << "_"
    << setw(digits(N))                       << max_degree                 << "_"
    << setw(digits(N))                       << min_degree                 << "_"
    << setw(digits(LUT_UNIT_N))              << max_degree_freq            << "_"
    << setw(digits(LUT_UNIT_N))              << min_degree_freq            << "_"
    << setw(2)                               << whichG                     << "_"
    << setw(5)                               << cost                       << "_"
    << setw(digits(N))                       << inv_max_degree             << "_"
    << setw(digits(N))                       << inv_min_degree             << "_"
    << setw(digits(LUT_UNIT_N))              << inv_max_degree_freq        << "_"
    << setw(digits(LUT_UNIT_N))              << inv_min_degree_freq        << "_"
    << setw(2)                               << inv_whichG;
    return ss.str();
  }

  string show() const
  {
    stringstream ss;
    string dkTstr = "[";
    for (int k = 1; k <= N; k++) dkTstr += std::to_string(dkT[k]) + "|";
    dkTstr.pop_back();
    dkTstr += "]";

    ss
    << name                       << ","
    << LUT_str                    << ","
    << bit_slice_str              << ","
    << (is_perm_flag ? "True" : "False") << ","
    << (is_inv_flag ? "True" : "False")  << ","
    << Diff                       << ","
    << DiffFreq                   << ","
    << Diff1                      << ","
    << CardD1                     << ","
    << Lin                        << ","
    << LinFreq                    << ","
    << Lin1                       << ","
    << CardL1                     << ","
    << max_degree                 << ","
    << min_degree                 << ","
    << max_degree_freq            << ","
    << min_degree_freq            << ","
    << dkTstr                     << ","
    << LS_nubmer                  << ","
    << "\"(" << max_v.first << ", " << max_v.second << ")\""   << ","
    << "\"(" << max_w.first << ", " << max_w.second << ")\""   << ","
    << (whichG == -1 ? "-" : "G" + std::to_string(whichG)) << ","
    << cost                       << ","
    << costisBest                 << ",";

    if (is_perm_flag)
    {
      string inv_dkTstr = "[";
      for (int k = 1; k <= N; k++) inv_dkTstr += std::to_string(inv_dkT[k]) + "|";
      inv_dkTstr.pop_back();
      inv_dkTstr += "]";

      ss
      << inv_LUT_str                << ","
      << inv_bit_slice_str          << ","
      << inv_max_degree             << ","
      << inv_min_degree             << ","
      << inv_max_degree_freq        << ","
      << inv_min_degree_freq        << ","
      << inv_dkTstr                 << ","
      << inv_LS_nubmer              << ","
      << "\"(" << inv_max_v.first << ", " << inv_max_v.second << ")\"" << ","
      << "\"(" << inv_max_w.first << ", " << inv_max_w.second << ")\"" << ","
      << (inv_whichG == -1 ? "-" : "G" + std::to_string(inv_whichG))
      << endl;
    }
    else
    {
      ss
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-" << ","
      << "-"
      << endl;
    }
    return ss.str();
  }

  ~evaluator(){};
};

#endif // EVALUATOR_H__
