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

from peigen cimport CppLighter3
from peigen cimport CppLighter4
from peigen cimport CppLighter5
from peigen cimport CppLighter6
from peigen cimport CppLighter7
from peigen cimport CppLighter8

from peigen cimport CppFaster3
from peigen cimport CppFaster4
from peigen cimport CppFaster5
from peigen cimport CppFaster6
from peigen cimport CppFaster7
from peigen cimport CppFaster8

cdef class Lighter(object):

	cdef CppLighter3* lighter3
	cdef CppLighter4* lighter4
	cdef CppLighter5* lighter5
	cdef CppLighter6* lighter6
	cdef CppLighter7* lighter7
	cdef CppLighter8* lighter8

	def __cinit__(self):
		self.lighter3 = NULL
		self.lighter4 = NULL
		self.lighter5 = NULL
		self.lighter6 = NULL
		self.lighter7 = NULL
		self.lighter8 = NULL

	def __dealloc__(self):
		del self.lighter3
		del self.lighter4
		del self.lighter5
		del self.lighter6
		del self.lighter7
		del self.lighter8

	def __init__(self):
		self.lighter3 = new CppLighter3()
		self.lighter4 = new CppLighter4()
		self.lighter5 = new CppLighter5()
		self.lighter6 = new CppLighter6()
		self.lighter7 = new CppLighter7()
		self.lighter8 = new CppLighter8()

	##########################################################
	# void evaluate(string sboxesfile, string outputfile)
	##########################################################
	def evaluate_3bit(self, str1='sboxes3.txt', str2='sboxes3_eva.csv'):
		self.lighter3.evaluate(str1, str2)

	def evaluate_4bit(self, str1='sboxes4.txt', str2='sboxes4_eva.csv'):
		self.lighter4.evaluate(str1, str2)

	def evaluate_5bit(self, str1='sboxes5.txt', str2='sboxes5_eva.csv'):
		self.lighter5.evaluate(str1, str2)

	def evaluate_6bit(self, str1='sboxes6.txt', str2='sboxes6_eva.csv'):
		self.lighter6.evaluate(str1, str2)

	def evaluate_7bit(self, str1='sboxes7.txt', str2='sboxes7_eva.csv'):
		self.lighter7.evaluate(str1, str2)

	def evaluate_8bit(self, str1='sboxes8.txt', str2='sboxes8_eva.csv'):
		self.lighter8.evaluate(str1, str2)

	##########################################################
	# void evaluate_verbose(string sboxesfile, string outputfile_prefix)
	##########################################################
	def evaluate_verbose_3bit(self, str1='sboxes3.txt', str2='sboxes3_evav_'):
		self.lighter3.evaluate_verbose(str1, str2)

	def evaluate_verbose_4bit(self, str1='sboxes4.txt', str2='sboxes4_evav_'):
		self.lighter4.evaluate_verbose(str1, str2)

	def evaluate_verbose_5bit(self, str1='sboxes5.txt', str2='sboxes5_evav_'):
		self.lighter5.evaluate_verbose(str1, str2)

	def evaluate_verbose_6bit(self, str1='sboxes6.txt', str2='sboxes6_evav_'):
		self.lighter6.evaluate_verbose(str1, str2)

	def evaluate_verbose_7bit(self, str1='sboxes7.txt', str2='sboxes7_evav_'):
		self.lighter7.evaluate_verbose(str1, str2)

	def evaluate_verbose_8bit(self, str1='sboxes8.txt', str2='sboxes8_evav_'):
		self.lighter8.evaluate_verbose(str1, str2)

	##########################################################
	# pre_computation
	##########################################################
	def pre_compute_3bit(self, str='-av'):
		self.lighter3.pre_compute(str)

	def pre_compute_4bit(self, str='-av'):
		self.lighter4.pre_compute(str)

	def pre_compute_5bit(self, str='-av'):
		self.lighter5.pre_compute(str)

	def pre_compute_6bit(self, str='-av'):
		self.lighter6.pre_compute(str)

	def pre_compute_7bit(self, str='-av'):
		self.lighter7.pre_compute(str)

	def pre_compute_8bit(self, str='-av'):
		self.lighter8.pre_compute(str)

	##########################################################
	# search in batch mode (a set of S-boxes)
	##########################################################

	def search_batch_3bit(self, str='-av'):
		self.lighter3.search_batch(str)

	def search_batch_4bit(self, str='-av'):
		self.lighter4.search_batch(str)

	def search_batch_5bit(self, str='-av'):
		self.lighter5.search_batch(str)

	def search_batch_6bit(self, str='-av'):
		self.lighter6.search_batch(str)

	def search_batch_7bit(self, str='-av'):
		self.lighter7.search_batch(str)

	def search_batch_8bit(self, str='-av'):
		self.lighter8.search_batch(str)

	##########################################################
	# search in single mode (one S-boxes)
	##########################################################

	def search_single_3bit(self, str='-av'):
		return self.lighter3.search_single(str)

	def search_single_4bit(self, str='-av'):
		return self.lighter4.search_single(str)

	def search_single_5bit(self, str='-av'):
		return self.lighter5.search_single(str)

	def search_single_6bit(self, str='-av'):
		return self.lighter6.search_single(str)

	def search_single_7bit(self, str='-av'):
		return self.lighter7.search_single(str)

	def search_single_8bit(self, str='-av'):
		return self.lighter8.search_single(str)

	##########################################################
	# search in batch mode using concatnation method (a set of S-boxes)
	##########################################################

	def search_batch_concatenate_3bit(self, str='-av'):
		self.lighter3.search_batch_concatenate(str)

	def search_batch_concatenate_4bit(self, str='-av'):
		self.lighter4.search_batch_concatenate(str)

	def search_batch_concatenate_5bit(self, str='-av'):
		self.lighter5.search_batch_concatenate(str)

	def search_batch_concatenate_6bit(self, str='-av'):
		self.lighter6.search_batch_concatenate(str)

	def search_batch_concatenate_7bit(self, str='-av'):
		self.lighter7.search_batch_concatenate(str)

	def search_batch_concatenate_8bit(self, str='-av'):
		self.lighter8.search_batch_concatenate(str)

	##########################################################
	# search in single mode using concatnation method (one S-boxes)
	##########################################################

	def search_single_concatenate_3bit(self, str='-av'):
		return self.lighter3.search_single_concatenate(str)

	def search_single_concatenate_4bit(self, str='-av'):
		return self.lighter4.search_single_concatenate(str)

	def search_single_concatenate_5bit(self, str='-av'):
		return self.lighter5.search_single_concatenate(str)

	def search_single_concatenate_6bit(self, str='-av'):
		return self.lighter6.search_single_concatenate(str)

	def search_single_concatenate_7bit(self, str='-av'):
		return self.lighter7.search_single_concatenate(str)

	def search_single_concatenate_8bit(self, str='-av'):
		return self.lighter8.search_single_concatenate(str)

	##########################################################
	# void evaluate_filter(string args)
	##########################################################

	def evaluate_filter_3bit(self, str='-av'):
		return self.lighter3.evaluate_filter(str)

	def evaluate_filter_4bit(self, str='-av'):
		return self.lighter4.evaluate_filter(str)

	def evaluate_filter_5bit(self, str='-av'):
		return self.lighter5.evaluate_filter(str)

	def evaluate_filter_6bit(self, str='-av'):
		return self.lighter6.evaluate_filter(str)

	def evaluate_filter_7bit(self, str='-av'):
		return self.lighter7.evaluate_filter(str)

	def evaluate_filter_8bit(self, str='-av'):
		return self.lighter8.evaluate_filter(str)

	##########################################################
	# void generate(string args)
	##########################################################

	def generate_3bit(self, str='-av'):
		return self.lighter3.generate(str)

	def generate_4bit(self, str='-av'):
		return self.lighter4.generate(str)

	def generate_5bit(self, str='-av'):
		return self.lighter5.generate(str)

	def generate_6bit(self, str='-av'):
		return self.lighter6.generate(str)

	def generate_7bit(self, str='-av'):
		return self.lighter7.generate(str)

	def generate_8bit(self, str='-av'):
		return self.lighter8.generate(str)

cdef class Faster(object):

	cdef CppFaster3* faster3
	cdef CppFaster4* faster4
	cdef CppFaster5* faster5
	cdef CppFaster6* faster6
	cdef CppFaster7* faster7
	cdef CppFaster8* faster8

	def __cinit__(self):
		self.faster3 = NULL
		self.faster4 = NULL
		self.faster5 = NULL
		self.faster6 = NULL
		self.faster7 = NULL
		self.faster8 = NULL

	def __dealloc__(self):
		del self.faster3
		del self.faster4
		del self.faster5
		del self.faster6
		del self.faster7
		del self.faster8

	def __init__(self):
		self.faster3 = new CppFaster3()
		self.faster4 = new CppFaster4()
		self.faster5 = new CppFaster5()
		self.faster6 = new CppFaster6()
		self.faster7 = new CppFaster7()
		self.faster8 = new CppFaster8()

	###########################################
	# pre_computation
	###########################################
	def pre_compute_3bit(self, str='-av'):
		self.faster3.pre_compute(str)

	def pre_compute_4bit(self, str='-av'):
		self.faster4.pre_compute(str)

	def pre_compute_5bit(self, str='-av'):
		self.faster5.pre_compute(str)

	def pre_compute_6bit(self, str='-av'):
		self.faster6.pre_compute(str)

	def pre_compute_7bit(self, str='-av'):
		self.faster7.pre_compute(str)

	def pre_compute_8bit(self, str='-av'):
		self.faster8.pre_compute(str)

	###########################################
	# search in batch mode (a set of S-boxes)
	###########################################

	def search_batch_3bit(self, str='-av'):
		self.faster3.search_batch(str)

	def search_batch_4bit(self, str='-av'):
		self.faster4.search_batch(str)

	def search_batch_5bit(self, str='-av'):
		self.faster5.search_batch(str)

	def search_batch_6bit(self, str='-av'):
		self.faster6.search_batch(str)

	def search_batch_7bit(self, str='-av'):
		self.faster7.search_batch(str)

	def search_batch_8bit(self, str='-av'):
		self.faster8.search_batch(str)

	###########################################
	# search in single mode (one S-boxes)
	###########################################

	def search_single_3bit(self, str='-av'):
		return self.faster3.search_single(str)

	def search_single_4bit(self, str='-av'):
		return self.faster4.search_single(str)

	def search_single_5bit(self, str='-av'):
		return self.faster5.search_single(str)

	def search_single_6bit(self, str='-av'):
		return self.faster6.search_single(str)

	def search_single_7bit(self, str='-av'):
		return self.faster7.search_single(str)

	def search_single_8bit(self, str='-av'):
		return self.faster8.search_single(str)