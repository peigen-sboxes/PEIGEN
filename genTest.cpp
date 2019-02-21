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

#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream files("results.c");
    ofstream codes("testcode.cpp");
    ofstream correct("results_correct.txt");
    for (string line; getline(files, line); )
    {
        codes << "I = function_t<n>::INPUT_DEFAULT(); for (int i = 0; i < n; i++) X[i] = I.coordinates[i].bit_slice;" << endl;
        codes << "#include " << "\"" << line << "\"" << endl;
        codes << "for (int i = 0; i < n; i++) I.coordinates[i].bit_slice = F[i]; fout << I.to_string() << endl;" << endl;
        codes << endl;

        ifstream result(line.c_str());
        for (string resline; getline(result, resline); )
        {
            string findstr = "// to : ";
            if (resline.find(findstr) != std::string::npos)
            {
                resline.erase(resline.begin(), resline.begin()+findstr.length());
                correct << resline << endl;
                break;
            }                
        }
        result.close();
    }
    correct.close();
    files.close();
    codes.close();
    
    return 0;
}