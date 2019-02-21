##########################################################
# PEIGEN: a Platform for Evaluation, Implementation, and Generation of S-boxes
#
#  Copyright 2019 by
#    Zhenzhen Bao <baozhenzhen10[at]gmail.com>
#    Jian Guo <guojian[at]ntu.edu.sg>
#    San Ling <lingsan[at]ntu.edu.sg>
#    Yu Sasaki <yu[dot]sasaki[dot]sk@hco.ntt.co.jp>
# 
# This platform is developed based on the open source application
# <http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz>
# Optimizing Implementations of Lightweight Building Blocks
#
#  Copyright 2017 by
#    Jade Tourteaux <Jade[dot]Tourteaux[at]gmail.com>
#    Jérémy Jean <Jean[dot]Jeremy[at]gmail.com>
# 
# We follow the same copyright policy.
# 
# This file is part of some open source application.
# 
# Some open source application is free software: you can redistribute 
# it and/or modify it under the terms of the GNU General Public 
# License as published by the Free Software Foundation, either 
# version 3 of the License, or (at your option) any later version.
# 
# Some open source application is distributed in the hope that it will 
# be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#
# @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
##########################################################

#include 'libcpp/string.pxd'

from cython.parallel cimport parallel
cimport openmp
from libcpp.string cimport string

cdef extern from 'lighter.hpp' namespace "Peigen::weight":

	cdef cppclass CppLighter3 "lighter<3>":
		CppLighter3()
		void evaluate(string sboxesfile, string outputfile)
		void evaluate_verbose(string sboxesfile, string outputfile_prefix)
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)
		void search_batch_concatenate(string argvs)
		double search_single_concatenate(string argvs)
		void evaluate_filter(string args)
		void generate(string args)

	cdef cppclass CppLighter4 "lighter<4>":
		CppLighter4()
		void evaluate(string sboxesfile, string outputfile)
		void evaluate_verbose(string sboxesfile, string outputfile_prefix)
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)
		void search_batch_concatenate(string argvs)
		double search_single_concatenate(string argvs)
		void evaluate_filter(string args)
		void generate(string args)

	cdef cppclass CppLighter5 "lighter<5>":
		CppLighter5()
		void evaluate(string sboxesfile, string outputfile)
		void evaluate_verbose(string sboxesfile, string outputfile_prefix)
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)
		void search_batch_concatenate(string argvs)
		double search_single_concatenate(string argvs)
		void evaluate_filter(string args)
		void generate(string args)

	cdef cppclass CppLighter6 "lighter<6>":
		CppLighter6()
		void evaluate(string sboxesfile, string outputfile)
		void evaluate_verbose(string sboxesfile, string outputfile_prefix)
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)
		void search_batch_concatenate(string argvs)
		double search_single_concatenate(string argvs)
		void evaluate_filter(string args)
		void generate(string args)

	cdef cppclass CppLighter7 "lighter<7>":
		CppLighter7()
		void evaluate(string sboxesfile, string outputfile)
		void evaluate_verbose(string sboxesfile, string outputfile_prefix)
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)
		void search_batch_concatenate(string argvs)
		double search_single_concatenate(string argvs)
		void evaluate_filter(string args)
		void generate(string args)

	cdef cppclass CppLighter8 "lighter<8>":
		CppLighter8()
		void evaluate(string sboxesfile, string outputfile)
		void evaluate_verbose(string sboxesfile, string outputfile_prefix)
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)
		void search_batch_concatenate(string argvs)
		double search_single_concatenate(string argvs)
		void evaluate_filter(string args)
		void generate(string args)



cdef extern from 'faster.hpp' namespace "Peigen::depth":

	cdef cppclass CppFaster3 "faster<3>":
		CppFaster3()
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)


	cdef cppclass CppFaster4 "faster<4>":
		CppFaster4()
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)

	cdef cppclass CppFaster5 "faster<5>":
		CppFaster5()
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)

	cdef cppclass CppFaster6 "faster<6>":
		CppFaster6()
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)

	cdef cppclass CppFaster7 "faster<7>":
		CppFaster7()
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)

	cdef cppclass CppFaster8 "faster<8>":
		CppFaster8()
		void pre_compute(string argvs)
		void search_batch(string argvs)
		double search_single(string argvs)