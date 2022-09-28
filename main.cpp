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

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <array>
#include <cstdint>
#include <bitset>
#include <cstring>
#include <stdio.h>
#include <time.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Woverflow"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#include "faster.hpp"
#include "func.hpp"
#include "evaluator.hpp"
#include "lighter.hpp"

#pragma GCC diagnostic pop

using namespace std;
using namespace Peigen;
using namespace Peigen::depth;
using namespace Peigen::weight;

int main(int argc, char *argv[])
{
	clock_t t0, t1;
	std::string mainargs;
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			mainargs += argv[i];
			mainargs += " ";
		}
		mainargs.pop_back();
	}
	string args = mainargs;

#if defined(EVA)
	// Evaluate Cryptographic Properties: evaluate() and evaluate_verbose()
	cout << "Evaluating Cryptographic Properties" << endl;
	t0 = clock();
	Peigen::weight::lighter<3> sbox3_Eva;
	sbox3_Eva.omp_nb_threads = 1;
	sbox3_Eva.evaluate("sboxes3.txt", "properties_sboxes3.csv");
	sbox3_Eva.evaluate_verbose("sboxes3.txt", "properties");
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;

	t0 = clock();
	Peigen::weight::lighter<4> sbox4_Eva;
	sbox4_Eva.omp_nb_threads = 1;
	sbox4_Eva.evaluate("sboxes4.txt", "properties_sboxes4.csv");
	sbox4_Eva.evaluate_verbose("sboxes4.txt", "properties");
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;

	t0 = clock();
	Peigen::weight::lighter<5> sbox5_Eva;
	sbox5_Eva.omp_nb_threads = 1;
	sbox5_Eva.evaluate("sboxes5.txt", "properties_sboxes5.csv");
	sbox5_Eva.evaluate_verbose("sboxes5.txt", "properties");
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;

	t0 = clock();
	Peigen::weight::lighter<6> sbox6_Eva;
	sbox6_Eva.omp_nb_threads = 1;
	sbox6_Eva.evaluate("sboxes6.txt", "properties_sboxes6.csv");
	sbox6_Eva.evaluate_verbose("sboxes6.txt", "properties");
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;

	t0 = clock();
	Peigen::weight::lighter<7> sbox7_Eva;
	sbox7_Eva.omp_nb_threads = 1;
	sbox7_Eva.evaluate("sboxes7.txt", "properties_sboxes7.csv");
	sbox7_Eva.evaluate_verbose("sboxes7.txt", "properties");
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;

	t0 = clock();
	Peigen::weight::lighter<8> sbox8_Eva;
	sbox8_Eva.omp_nb_threads = 1;
	sbox8_Eva.evaluate("sboxes8.txt", "properties_sboxes8.csv");
	sbox8_Eva.evaluate_verbose("sboxes8.txt", "properties");
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif

#if defined(EVA_Single)
	string SboxName;
	string SboxLUTstr;
	if (argc <= 2)
	{
 		//SboxName = "SERPENT_S0";
 		//SboxLUTstr = "03080f010a06050b0e0d04020700090c";
		SboxName = "PRESENT";
 		SboxLUTstr = "0c05060b09000a0d030e0f0804070102";
	}
	else
	{
		SboxName = argv[1];
		SboxLUTstr = argv[2];
	}
 	Peigen::function_t<4> S(SboxLUTstr);
	Peigen::function_t<4> iS;	 
	// Or initilized by:
	// uint8_t S_LUT[16] = {3,8,15,1,10,6,5,11,14,13,4,2,7,0,9,12};
	// function_t<4> S(S_LUT);
	string fn = "properties_" + SboxName + ".txt";
	ofstream fout(fn.c_str());
	fout << SboxName << ", " << S.to_string() << endl << endl;
	// Call the following for evaluating concerned properties
    fout << S.show_LUT() << endl;
    fout << S.show_coordinates_ANF() << endl;
    fout << S.show_components_ANF() << endl;
	fout << S.show_difference_distribution_matrix() << endl;
	fout << S.show_boomerang_connection_table() << endl;
	fout << S.show_linear_approximation_matrix() << endl;
	fout << S.show_autocorrelation_matrix() << endl;
	fout << S.show_k_product_degree_table() << endl;
	fout << S.show_V_S_table() << endl;
	fout << S.show_linear_structures() << endl;
	fout << S.show_v_w_linear() << endl;
	// Or call the following to show all properties
	// S.show_all_properties(fout);	
	if (S.is_permutation())
	{
		iS = S.inverse();
		fout << SboxName << " inverse, " << iS.to_string() << endl << endl;
    	fout << iS.show_LUT() << endl;
    	fout << iS.show_coordinates_ANF() << endl;
    	fout << iS.show_components_ANF() << endl;
		fout << iS.show_difference_distribution_matrix() << endl;
		fout << iS.show_boomerang_connection_table() << endl;
		fout << iS.show_linear_approximation_matrix() << endl;
		fout << iS.show_autocorrelation_matrix() << endl;
    	fout << iS.show_k_product_degree_table() << endl;
		fout << iS.show_V_S_table() << endl;
		fout << iS.show_linear_structures() << endl;
		fout << iS.show_v_w_linear() << endl;
		// Or call the following to show all properties
		// iS.show_all_properties(fout);
	}

	// Evaluating equvalence classes for a single S-box
	Peigen::function_t<4> Gi;
	Peigen::function_t<4> A;
	Peigen::function_t<4> B;
	int gi;
	uint8_t a;
	uint8_t b;
	if(S.is_optimal(gi, Gi, A, B, a, b))
	{
		fout << "Belongs to optimal class: G" << gi << endl;
		fout << "G: " << Gi.to_string() << ", " << Gi.LUT_to_string() << endl;
		fout << "A: " << A.to_string() << ", " << A.LUT_to_string() << endl;
		fout << "B: " << B.to_string() << ", " << B.LUT_to_string() << endl;
		fout << "a: " << a + '\0' << endl;
		fout << "b: " << b + '\0' << endl;
		fout << endl;
		iS.is_optimal(gi, Gi, A, B, a, b);
		fout << "Inverse S-box belongs to optimal class: G" << gi << endl;
		fout << "G: " << Gi.to_string() << ", " << Gi.LUT_to_string() << endl;
		fout << "A: " << A.to_string() << ", " << A.LUT_to_string() << endl;
		fout << "B: " << B.to_string() << ", " << B.LUT_to_string() << endl;
		fout << "a: " << a + '\0' << endl;
		fout << "b: " << b + '\0' << endl;
	}
	else
	{
		fout << "Is not optimal." << endl;
	}
	fout << endl;
	fout.close();

	S.get_PE((SboxName + "_PE.txt").c_str());
	S.get_XE((SboxName + "_XE.txt").c_str());
	S.get_PXE((SboxName + "_PXE.txt").c_str());
	// Partition the affine equvalent class will take about 15 mins, and only support for 4-bit S-box
	// so is commented here, uncomment it if need 
	// S.get_AE_PXEreps((SboxName + "_AE_PXEreps.txt").c_str());

	// Testing two S-boxes for Affine Equivalence
	string S1str = "5473_f322_131f_62c7"; /* PRINCE */
	string S2str = "6f48_16f8_e2e4_b86a"; /* G13 */
	Peigen::function_t<4> S1(S1str);
	Peigen::function_t<4> S2(S2str);
	Peigen::function_t<4> RS;
	Peigen::function_t<4> S1_L1, S1_L2;
	Peigen::function_t<4> S2_L1, S2_L2;
	uint8_t S1_c;
	uint8_t S2_c;

	fout.open((S1str + "_AE_" + S2str + ".txt").c_str());
	fout << "S1: " << S1.to_string() << ", " << S1.LUT_to_string() << endl;
	fout << "S2: " << S2.to_string() << ", " << S2.LUT_to_string() << endl;
	if(S1.is_AE(S2, RS, S1_L1, S1_L2, S1_c, S2_L1, S2_L2, S2_c))
	{
		fout << "RS: " << RS.to_string() << ", " << RS.LUT_to_string() << endl;
		fout << "S1_L1: " << S1_L1.to_string() << ", " << S1_L1.LUT_to_string() << endl;
		fout << "S1_L2: " << S1_L2.to_string() << ", " << S1_L2.LUT_to_string() << endl;
		fout << "S1_c: " << S1_c + '\0' << endl;
		fout << "S2_L1: " << S2_L1.to_string() << ", " << S2_L1.LUT_to_string() << endl;
		fout << "S2_L2: " << S2_L2.to_string() << ", " << S2_L2.LUT_to_string() << endl;
		fout << "S2_c: " << S2_c + '\0' << endl;
	}
	else
	{
		fout << "Is not AE." << endl;
	}
	fout.close();
#endif


#if defined(GC)
	cout << "Finding BGC/GEC/MC implementations" << endl;
#if(SIZEINBIT == 3)
	t0 = clock();
	Peigen::weight::lighter<3> sbox3_GC;
	if (argc <= 1)
	{
		args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf";
		//args = "-v -c 7 -l 7 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf";
		//args = "-av -c 7 -l 7 -p 4 -r 160 -f TSMC65nm.conf";
	}
	sbox3_GC.pre_compute(args);
	sbox3_GC.search_batch_concatenate(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#if(SIZEINBIT == 4)
	t0 = clock();
	Peigen::weight::lighter<4> sbox4_GC;
	if (argc <= 1)
	{
		// Here, the limit (-c) for pre-compute and the limit (-l) for finding implementations
		// are set to be a little bit smaller than that set to get the results in the paper.
		// In the paper, -c and -l for software is set to be 7, for TSMC65nm and UMC180nm they are set to be 12.
		// That will cost reletively longer time and large memory.
		// So, to run as a demon on a PC, we recommend to set them smaller
		// and set them larger to find better solutions when running on a server
		// The following settings for ``generation'' are similiar.
		args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf";
		//args = "-v -c 9 -l 9 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf";
		//args = "-av -c 9 -l 9 -p 4 -r 160 -f TSMC65nm.conf";
	}
	sbox4_GC.pre_compute(args);
	sbox4_GC.search_batch_concatenate(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#endif

#if defined(DEPTH)
	// Find Depth-optimal implementation
	cout << "Finding Depth-optimal implementation" << endl;

#if(SIZEINBIT == 3)
	// Depth for 3-bit S-boxes;
	t0 = clock();
	Peigen::depth::faster<3> sbox3_Depth;
	if (argc <= 1)
	{
		args = "-v -c 4 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf";
		//args = "-v -c 4 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf";
		//args = "-av -c 4 -p 4 -r 160 -f TSMC65nm.conf";
		//args = "-v -c 4 -p 4 -r 160 --not1 --and2 --nand2 --or2 --nor2 --xor2 --xnor2 -f STM65nm.conf";
		//args = "-v -c 4 -p 4 -r 160 --not1 --and2 --nand2 --or2 --nor2 --xor2 --xnor2 -f gatec.conf";
	}
	sbox3_Depth.pre_compute(args);
	sbox3_Depth.search_batch(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#if(SIZEINBIT == 4)
	// Depth for 4-bit S-boxes;
	t0 = clock();
	Peigen::depth::faster<4> sbox4_Depth;
	if (argc <= 1)
	{
		args = "-v -c 4 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf";
		//args = "-v -c 3 -l 3 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf";
		//args = "-av -c 3 -l 3 -p 4 -r 160 -f TSMC65nm.conf";
		//args = "-v -c 4 -l 4 -p 4 -r 160 --not1 --and2 --nand2 --or2 --nor2 --xor2 --xnor2 -f STM65nm.conf";
		//args = "-v -c 4 -l 4 -p 4 -r 160 --not1 --and2 --nand2 --or2 --nor2 --xor2 --xnor2 -f gatec.conf";
	}
	sbox4_Depth.pre_compute(args);
	sbox4_Depth.search_batch(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#endif

#if defined(FILTER)
	cout << "Evaluating and filtering " << endl;

#if(SIZEINBIT == 3)
	t0 = clock();
	Peigen::weight::lighter<3> sbox3_FILTER;
	if (argc <= 1)
	{
		args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf -s criteria.conf -o sboxes3.txt";
		//args = "-v -c 7 -l 7 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf -s criteria.conf -o sboxes3.txt";
		//args = "-av -c 7 -l 7 -p 4 -r 160 -f TSMC65nm.conf -s criteria.conf -o sboxes3.txt";
	}
	// Note, if the graph under a same configuration has already been pre computed and stored,
	// or if ``Cost'' is not set in ``criteria.conf'', i.e., ``Cost'' is not a criterion for filtering,
	// The line ``pre_compute(args)'' can be commented.
	sbox3_FILTER.pre_compute(args);
	sbox3_FILTER.evaluate_filter(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#if(SIZEINBIT == 4)
	// Provide the set of criteria in a file, e.g., ``criteria_filter.conf''
	// An example is provided in ``criteria_filter.conf'':
	t0 = clock();
	Peigen::weight::lighter<4> sbox4_FILTER;
	if (argc <= 1)
	{
		args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf -s criteria_filter.conf -o sboxes4.txt";
		//args = "-v -c 9 -l 9 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf -s criteria_filter.conf -o sboxes4.txt";
		//args = "-av -c 9 -l 9 -p 4 -r 160 -f TSMC65nm.conf -s criteria_filter.conf -o sboxes4.txt";
	}
	// Note, if ``Cost'' is set in ``criteria_filter.conf'', i.e., ``Cost'' is a criterion for filtering,
	// and if the graph under a same configuration has not been pre computed,
	// The following line ``pre_compute(args)'' should be uncommented.
	// sbox4_FILTER.pre_compute(args);
	sbox4_FILTER.evaluate_filter(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#endif

#if defined(GEN)
	cout << "Generating " << endl;

#if(SIZEINBIT == 3)
	t0 = clock();
	Peigen::weight::lighter<3> sbox3_GEN;
	if (argc <= 1)
	{
		args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf -s criteria.conf";
		//args = "-v -c 7 -l 7 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf -s criteria.conf";
		//args = "-av -c 7 -l 7 -p 4 -r 160 -f TSMC65nm.conf -s criteria.conf";
	}
	// Note, if the graph under a same configuration has already been pre computed and stored,
	// The line ``pre_compute(args)'' can be commented.
	sbox3_GEN.pre_compute(args);
	sbox3_GEN.generate(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#if(SIZEINBIT == 4)
	t0 = clock();
	Peigen::weight::lighter<4> sbox4_GEN;
	if (argc <= 1)
	{
		args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf -s criteria.conf";
		//args = "-v -c 9 -l 9 -p 4 -r 160 --not1 --nand2 --nor2 --and2 --or2 --xor2 --xnor2 --andn2 --orn2 --nand3 --nor3 --maoi1 --moai1 -f UMC180nm.conf -s criteria.conf";
		//args = "-av -c 9 -l 9 -p 4 -r 160 -f TSMC65nm.conf -s criteria.conf";
	}
	// Note, if the graph under a same configuration has already been pre computed and stored,
	// The line ``pre_compute(args)'' can be commented.
	sbox4_GEN.pre_compute(args);
	sbox4_GEN.generate(args);
	t1 = clock() - t0;
	cout << "Total time (mins): " << (((double)t1) / (CLOCKS_PER_SEC * 60.0)) << endl;
#endif
#endif

	return 0;
}