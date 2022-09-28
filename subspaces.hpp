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

#ifndef SUBSPACES_H__
#define SUBSPACES_H__

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

/*
  generating the echelon matrices over finite fields to generate the basis of subspaces
  according to the method used in sagemath reduced_echelon_matrix_iterator() method
*/
template<int N>
const vector<vector<vector<uint8_t> > > SubSpaces = []
{
    if (N <= 8)
    {
        vector<vector<vector<uint8_t> > > subspaces;
        #define N0 N
        #define Ci(x) (C0[x+1]-1)
    
        int A0[N0 + 1], T0[N0 + 1], F0[N0 + 1], H0[N0 + 1],  C0[N0 + 1], X0, Y0, I0, L0, Z0;
        int M0;
    
        array<array<int, N>, N> m;
        vector<pair<int, int> > free_positions;
    
        vector<vector<uint8_t> > newdim_subspaces;
        vector<uint8_t> anew_subspace;
    
        for (int dim = 1; dim < N; dim++)
        {
          newdim_subspaces.clear();
    
          M0 = dim;
    
	        for (int i=0; i<=(N0-M0); i++) A0[i] = 0;	for (int i=N0-M0+1; i<=N0; i++) A0[i] = 1;
	        for (int i = 1; i<=M0; i++) { C0[i] = N0 - M0 + i; H0[N0-M0+i] = i; }
	        T0[N0-M0] = -1; T0[1] = 0; F0[N0] = N0 - M0 + 1; I0 = N0 - M0; L0 = N0;
          
          do
          {
            for (int i = 0; i < dim; i++) m[i].fill(0);
            free_positions.clear();
            for (int i = 0; i < dim; i++)
            {
              m[i][Ci(i)] = 1;
              for (int j = Ci(i) + 1; j < N; j++)
              {
                if (A0[j+1] != 1) 
                {
                  free_positions.push_back(pair<int, int>(i, j));
                }
              }
            }
    
            int num_free_pos = free_positions.size();
            for (uint64_t v = 0; v < (1<<num_free_pos); v++)
            {
              for (int fi = 0; fi < num_free_pos; fi++)
              {
                auto freepos = free_positions[fi];
                int rowi = freepos.first;
                int coli = freepos.second;
                m[rowi][coli] = (int)((v >> (uint64_t)fi) & 1ULL);
              }
              anew_subspace.clear();
              for (int ri = 0; ri < dim; ri++)
              {
                uint8_t vec = 0;
                for (int ci = 0; ci < N; ci++) vec |= (m[ri][ci] << ci);
                anew_subspace.push_back(vec);
              }
              anew_subspace.shrink_to_fit();
              newdim_subspaces.push_back(anew_subspace);
            }
    
	    			if (I0 == 0)
	    			{
	    				break;
	    			} 
	    			else
	    			{
	    				if (T0[I0] < 0) { if ((-T0[I0]) != (I0-1)){ T0[I0-1] = T0[I0]; } T0[I0] = I0-1; }
	    				if ( A0[I0]==0 )
	    				{
	    					X0 = I0; Y0 = F0[L0]; if (A0[I0-1] == 1){ F0[I0] = F0[I0 - 1]; } else { F0[I0] = I0; }
	    					if (F0[L0] == L0) { L0 = I0; I0 = T0[I0]; goto CHANGE0; }
	    					if (L0 == N0) { T0[F0[N0]] = -I0 - 1; T0[I0 + 1] = T0[I0]; I0 = F0[N0]; F0[N0] = F0[N0] + 1; goto CHANGE0; }
	    					T0[L0] = -I0-1; T0[I0+1] = T0[I0]; F0[L0] = F0[L0] + 1; I0 = L0; goto CHANGE0;
	    				}
	    				Y0 = I0;
	    				if (I0 != L0)
	    				{
	    					F0[L0] = X0 = F0[L0] - 1; F0[I0 - 1] = F0[I0];
	    					if (L0 == N0)
	    					{
	    						if (I0 == (F0[N0] - 1)) { I0 = T0[I0]; goto CHANGE0; }
	    						T0[F0[N0]-1] = -I0-1; T0[I0+1] = T0[I0]; I0 = F0[N0] - 1; goto CHANGE0;
	    					}
	    					T0[L0] = -I0 -1; T0[I0 + 1] = T0[I0]; I0 = L0; goto CHANGE0;
	    				}
	    				X0 = N0; F0[L0 - 1] = F0[L0]; F0[N0] = N0; L0 = N0;
	    				if (I0 == N0 - 1) { I0 = T0[N0 - 1]; goto CHANGE0; }
	    				T0[N0 - 1] = -I0 - 1; T0[I0 + 1] = T0[I0]; I0 = N0 - 1;
        CHANGE0:
	    				A0[X0] = 1; A0[Y0] = 0; H0[X0] = Z0 = H0[Y0]; C0[Z0] = X0;
	    			}
	    		} while (true);
    
          newdim_subspaces.shrink_to_fit();
          subspaces.push_back(newdim_subspaces);
        }
        #undef Ci
        #undef N0
        return subspaces;
    }
} ();

#endif