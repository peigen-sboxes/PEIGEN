# PEIGEN: a Platform for Evaluation, Implementation, and Generation of S-boxes

PEIGEN is a tool for study S-boxes.

The S-box is a type of non-linearity cryptographic component, commonly used in symmetric cryptography primitives.
For a survey on studies of S-boxes and a formal introduction of PEIGEN, please refer to the paper [\[2\]](https://eprint.iacr.org/2019/209).

## Functionalities

### Evaluation

Given a set of *n*-bit S-boxes (*3 <= n <= 8*), PEIGEN evaluates:

- Differential Distribution Tables (DDT)

- Linear Approximation Tables (LAT)

- Boomerang Connectivity Tables (BCT)

- Algebraic Normal Forms (ANF)

- Linear Structures (LS)

- *(v,w)*-linearity

- A table indicating the appearance of monomials in the ANFs of produces of coordinate functions of an S-box, and

- various detailed criteria related to these tables and notions (e.g., differential uniformity, linearity, DDT_1, LAT_1, algebraic degree etc.)

Given *n*-bit S-boxes, PEIGEN can evaluate their equivalence relations, including:

- Permutation-XOR equivalence (PXE)

- Linear equivalence (LE)

- Affine equivalence (AE)

- For a *4*-bit S-box, it can partition its AE-class into PXE-classes.

### Implementation

This aspect of functionality is on the basis of another tool named [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz) presented in [\[1\]](https://eprint.iacr.org/2017/101).
More explicitly, PEIGEN is built on the basis of a sub-module of [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz) which is for finding hardware/software implementations of *4*-bit invertible S-boxes that are good in terms of hardware area/software instruction numbers. PEIGEN inherits all functionalities provided by this sub-module of [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz), and at the same time, improves the search efficiency using algorithmic-level optimizations. As a result, PEIGEN is efficient even when finding implementations for a large set of S-boxes. Besides, we add the functionality to find optimal implementations in terms of Depth Complexity. Concretely,

Given a set of *n*-bit S-boxes and the specific implementation configuration (available gates and costs for each gate), PEIGEN can generate implementations which are good in terms of

- Bitslice Gate Complexity (BGC)

- Gate Equivalent Complexity (GEC)

- Multiplicative Complexity (MC)

- Depth Complexity (Depth).

### Generation

- Given a set of criteria together with a set of S-boxes, PEIGEN filters out good S-boxes fulfilling the set of criteria, and output the evaluation results (output their security-related properties and implementations if required)

- Given merely a set of criteria, PEIGEN generates new S-boxes fulfilling the set of criteria (both security-related and implementation-related properties).

## Usage

PEIGEN is in the form of pure C++ template headers.
To use it, include the `.hpp` files and add `using namespace Peigen`.
Concrete usage are as follows.

- To evaluate a single S-box about its security-related properties:

  - add `#include "func.hpp"` and `using namespace Peigen;`

  - use `Peigen::function_t<n> S(SboxLUTstr);` to initate an object of *n*-bit
           S-box (*3 <= n <= 8*) with given lookup table (LUT), where

    - `SboxLUTstr` encodes the LUT of the S-box in Hexadecimal, e.g., `SboxLUTstr = "0c05060b09000a0d030e0f0804070102";`.

  - use `Peigen::function_t<n> S(SboxBitSlicestr);` to initate an object of *n*-bit
           S-box (*3 <= n <= 8*) with given Bitslice representationn (concatenation of the value vectors of its coordinate functions), where

    - `SboxBitSlicestr` encodes the concatenation of the *n* value vectores of its coordinate functions, each value vector uses  _2^n / 4_ Hexadecimal symbols, e.g., `SboxBitSlicestr = "0ed9_3687_a74c_659a";`

  - use `cout << S.show_xxx() << endl;` to print required properties related with `xxx`, e.g.,

    - call `S.show_coordinates_ANF()` will return a string representing the Algebraic Normal Form (*ANF*) of the coordinate functions of S-box *S*.

    - call `S.show_difference_distribution_matrix()` will return the Differential Distribution Table (*DDT*) of the S-box *S*.

    - to see more details on what properties can be evaluated by PEIGEN, please

      - find the paper [\[2\]](SoK: Peigen – a Platform for Evaluation, Implementation, and Generation of S-boxes), or
      - see the examples in the folder `\EvaluationResults\Sect5.1_CryptographicProperties\Sboxes4`, or
      - read the source code in `func.hpp`

- To simultaneously evaluate a set of S-boxes about their security-related properties:

  - add `#include "lighter.hpp"` and `using namespace Peigen;` and `using namespace Peigen::weight;`

  - use `Peigen::weight::lighter<n> sboxn_Eva;` to initate an object named `sboxn_Eva` of lighter for *n*-bit S-boxes

  - if the number of S-boxes to be evaluated is too large, use `sboxn_Eva.omp_nb_threads = x;` to use more thread (e.g., `x = 4`), generally, for 4-bit S-boxes, one thread is enough.

  - use `sboxn_Eva.evaluate("sboxesn.txt", "properties_sboxesn.csv");` to get the summarized evaluation results output, where

    - `sboxesn.txt` is the name of the file which contains the name of the S-boxes and the specification of the S-boxes to be evaluated. Note that, in the file named `sboxesn.txt`, it should be one S-box per line, name and specification are separated using `,`. The specification of the S-box can be eighter the LUT representation encoded in Hexadecimal (e.g., `0c05060b09000a0d030e0f0804070102`) or the bitsliced representation (concatenation of the *n* value vectors of its coordinate functions, e.g., `0ed9_3687_a74c_659a`).

    - `properties_sboxesn.csv` is the name of the output file which contains the summarized evaluation results in `.csv` file format (a simple file format used to store tabular data. Files in the CSV format can be imported to and exported from programs that store data in tables, such as Microsoft Excel or OpenOffice Calc)

    - please see files in the folder `\EvaluationResults\Sect5.1_CryptographicProperties\EVA` for examples

  - use `sboxn_Eva.evaluate_verbose("sboxesn.txt", "properties");` to get detailed evaluation results output in `.txt` file.

    - please see files in the folder `\EvaluationResults\Sect5.1_CryptographicProperties\Sboxes4` for examples

- To find efficient implemtations in terms of BGC/GEC/MC:

  - add `#include "lighter.hpp"` and `using namespace Peigen;` and `using namespace Peigen::weight;`

  - use `Peigen::weight::lighter<n> sboxn_GC;` to initate an object of lighter named `sboxn_GC` for *n*-bit S-boxes.

  - use `args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf";` to specify the configureation for the precomputation. These options are inheriated from [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz) with some additional arguments.

    - `-a` : Enable all gates and all implemented combinations of these gates

    - `-v` : verbose mode

    - `-c <value>` : limitation for precomputation

        suppose we use `-c 6`, this means when precomputing, all possible nodes in the graph with gate complexity no more than `6` will be expanded.

    - `-l <value>` : limitation for formal searching

        suppose we use `-l 9`, this means when searching for implementations, all possible nodes in the graph with gate complexity no more than `9` will be expanded. Note that, nodes with gate complexity more than `9` can also exist and be used, but will not be expanded further.

    - `-z <value>` : this is for resuming from Breakpoint. Suppose, during precomputation, the program was killed and all nodes in the graph with gate complexity no more than `6` has been expanded and write in binary files. Now, we want to resume from the breakpoint `6`, we can use `-z 6` to read the nodes stored in binary files and continue the precomputation from nodes with gate complexity `6`.

    - `-p <value>` : define the number of threads used in OpenMP, e.g., `-p 4` means using `4` threads to do the search in parallel.

    - `-r <value>` : define an upper bound on the RAM used by the programme, e.g., `-r 160` means using at most `16 GB` memory.

    - `-f <file>` : define which logic library you want to use, e.g., `-f TSMC65nm.conf` will use the library specified in the file `TSMC65nm.conf` indicating available gates and weight of each gate

    - `-i <function>` : in [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz), this is for defining start function. However, for PEIGEN, this parameter is usually default to be the Identity function (maps *x* to *x*).

    - `-o <file>` : in [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz), this is for defining arrival function. However, for PEIGEN, to support simultaneously finding implementations for many S-boxes, we use `-o <file>` define the name of the file which contains the name and the specification of the S-boxes to be implemented (this file is in the same format as the `sboxesn.txt` for the function `sboxn_Eva.evaluate("sboxesn.txt", "properties_sboxesn.csv");`). Note that, the specification of the S-boxes are either the LUT representation or the bit-sliced representation.

    - same as in [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz), if `[-a]` is not enabled, available logic gates should be explicitly added in this command line (and weight of each gate should be specified in the provided configure file):

      --not1

      --and2

      --nand2

      --or2

      --nor2

      --nand3

      --nor3

      --xor2

      --xnor2

      --maoi1

      --moai1

  - use `sboxn_GC.pre_compute(args);` to precompute the graph, this will expand the graph from the Identity function, with parameters encoded in `args`, and store the generated graph in binary files. For each configuration (the library of gates `-f <file>` and the limitation for precomputation `-c <value>`), this can be done once for all. Thus, if this has been done, the generated binary files are stored and available, we can directly call the search function.

  - use `sboxn_GC.search_batch_concatenate(args);` to search the implementations of a set of S-boxes with parameters encoded in `args`.

- To find efficient implementations in terms of Depth Complexity
  - add `#include "faster.hpp"` and `using namespace Peigen;` and `using namespace Peigen::depth;`

  - use `Peigen::depth::faster<n> sboxn_Depth;` to initate an object of faster named `sboxn_Depth` for *n*-bit S-boxes.

  - use a string like `args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf";` to specify the configuration for the precomputation and the search. These options are the same as that in finding implementations in terms of BGC/GEC/MC.

  - use `sboxn_Depth.pre_compute(args);` to precompute the graph of nodes that represent all possible balanced Boolean functions, with limited gate complexity and depth complexity. Again, like in finding implementations in terms of BGC/GEC/MC, for each configuration, this can be done once for all.

  - use `sboxn_Depth.search_batch(args);` to find the implementations efficient in terms of Depth of a set of S-boxes with parameters encoded in `args`.

- To evaluate a set of S-boxes and filter out those fulfilling given criteria

  - add `#include "lighter.hpp"` and `using namespace Peigen;` and `using namespace Peigen::weight;`

  - use `Peigen::weight::lighter<n> sboxn_FILTER;` to initate an object of lighter named `sboxn_FILTER` for *n*-bit S-boxes.

  - use a string like `args = "-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf -s criteria.conf -o sboxesn.txt";` to specify the configuration for the precomputation and filtering. The parameters are almost the same with that in finding implementations. The difference lies in `-s criteria.conf` which indicates the name of the file containing the user required criteria.

    - in a file like `criteria.conf`, required criteria should be presented one criterion per line, like the following (the name and the value should be separated by `=`, do not support whitespace):

      Diff=4

      Lin=8

      Diff1=0

      Lin1=4

      CardD1=4

      CardL1=8

      DiffFreq=18

      LinFreq=32

      MaxDeg=3

      MinDeg=2

      MaxDegFreq=14

      MinDegFreq=1

      Cost=10

    - Note that, if `Cost` is not set in `criteria.conf`, i.e., `Cost` is not a criterion for filtering, parameters for finding implementations can be omitted, and pre-computation for finding implementation can also be omitted. If `Cost` is set in `criteria.conf`, i.e., `Cost` is a criterion for filtering, those parameters for finding implementations are required.

  - use `sboxn_FILTER.pre_compute(args);` to pre-comptue the graph. Note that, if `Cost` is not a criterion for filtering, or if the graph under a same configuration has already been pre-computed and stored, no need to call `sboxn_FILTER.pre_compute(args);` for pre-computation.

  - use `sboxn_FILTER.evaluate_filter(args);` to evaluate and filter the given set of S-boxes. The output will be summarized results written into `.csv` file. If `Cost` is a criterion for filtering, the implementations for the S-boxes fulfilling the criteria will be generated and wrote to `.c` files.

- To generate S-boxes fulfilling given criteria

  - add `#include "lighter.hpp"` and `using namespace Peigen;` and `using namespace Peigen::weight;`

  - use `Peigen::weight::lighter<n> sboxn_GEN;` to initate an object of lighter named `sboxn_GEN` for *n*-bit S-boxes.

  - use a string like `-v -c 6 -l 6 -p 4 -r 160 --not1 --and2 --andn2 --or2 --xor2 -f software.conf -s criteria.conf";` to specify the configuration for the precomputation and generation. The parameters are almost the same with that in finding implementations. The difference lies in `-s criteria.conf` which indicates the name of the file containing the user required criteria.

    - the file like `criteria.conf`, is the same as in evaluating and filtering a set of S-box.

    - Note that, if `Cost` is not set in `criteria.conf`, i.e., `Cost` is not a criterion for filtering, parameters for finding implementations can **not** be omitted, because PEIGEN generate S-boxes by composing simple functions (each simple function is composed using basic gates). And PEIGEN only output those generated S-boxes fulfilling given criteria with the smallest implementation cost.

  - use `sboxn_GEN.pre_compute(args);` to pre-comptue the graph. Note that, if the graph under a same configuration has already been pre-computed and stored, no need to call `sboxn_FILTER.pre_compute(args);` for pre-computation.

  - use `sboxn_GEN.generate(args);` to generate a set of S-boxes fulfilling the given criteria. The output contains the following:

    - summarized results written into `.csv` file about the properties of the generated S-boxes.

    - implementations of the Permutation Equivalent (PE-) representatives for those generated S-box will be written into separated folders named after there property profile.

- To build the demo program in `main.cpp`

  - run `make all` to build all examples wrote in `main.cpp`
  - run `make evaluate` to build the example of evaluating a set of S-boxes
  - run `make evaluate_single` to build the example of evaluating a single S-box
  - run `make search_GC_n3` to build the example of finding efficient implementations in terms of BGC/GEC/MC for *3*-bit S-boxes
  - run `make search_GC_n4` to build the example of finding efficient implementations in terms of BGC/GEC/MC for *4*-bit S-boxes
  - run `make search_depth_n3` to build the example of finding efficient implementations in terms of Depth for *3*-bit S-boxes
  - run `make search_depth_n4` to build the example of finding efficient implementations in terms of Depth for *4*-bit S-boxes
  - run `make filter_n3` to build the example of evaluating and filtering a set of *3*-bit S-boxes
  - run `make filter_n4` to build the example of evaluating and filtering a set of *4*-bit S-boxes
  - run `make gen_n3` to build the example of generating *3*-bit S-boxes fulfilling given criteria
  - run `make gen_n4` to build the example of generating *4*-bit S-boxes fulfilling given criteria

- To check the generated implementations' correctness,

  - put the resulting implementations into a subfolder named say `R` of the current folder
  - run `make gencheck` to write the correct results () into a file named `results_correct.txt`.
  - run `make checklighter` to compile the implementations generated using an object of lighter and write the results into a file named `results_test.txt`. By comparing `results_correct.txt` and `results_test.txt`, one can check whether the implementations generated using the object of lighter are correct.
  - run `make checkfaster` to compile the implementations generated using an object of faster and write the results into a file named `results_test.txt`. By comparing `results_correct.txt` and `results_test.txt`, one can check whether the implementations generated using the object of faster are correct.

- To build a module which can be used in SageMath (Unfortunately, this feature has not been finished. The small functions for evaluation has not been provided an interface to be called),

  - rename the file `MakefileSage` as `Makefile` (one may backup the original `Makefile` for compiling examples)

  - compile inside the SageMath shell:

    `[...] ~/> sage -sh -c make`

  - start SageMath (sage):

    `[...] from peigen import Lighter`

    `[...] A = Lighter()`

    `[...] A.evaluate_4bit()`

  - Similarly, for Faster use: from peigen import Faster

## Cautions

- Note that, the files used as input to the functions of PEIGEN should be encoded in UTF-8 and End of Line (EOL) in Unix (LF).

- Sorry for the inconvenience, but for ease of supporting *n*-bit (*3 <= n <= 8*) S-boxes, in the string encoding the LUT of S-boxes as input to functions of PEIGEN, each element should be encoded in *2* Hexadecimal symbols (so, in total use _2 * 2^n_ Hexadecimal symbols) even though one Hexadecimal symbol per element is enough for *n=3* and *n=4*.

    For example, suppose the LUT of a 4-bit S-box is `{0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2}`, then it should be encoded in `0c05060b09000a0d030e0f0804070102`.

- Sorry for the inconvenience again, but for small efficiency gain, bitslicing of the S-boxes is done in little endian byte and little endian bit order (this is inconsistent with that in [LIGHTER](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz)): the least significant value is placed at the leftmost side in memory and, the least significant bit of the value is placed at the leftmost side in the value, e.g., suppose the LUT:

    LUT in hexadecimal (**big endian byte** order and **little endian bit** order):
    
    | 0x0 | 0x1 | 0x2 | 0x3 | 0x4 | 0x5 | 0x6 | 0x7 | 0x8 | 0x9 | 0xa | 0xb | 0xc | 0xd | 0xe | 0xf |
    | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
    | 0xc | 0x5 | 0x6 | 0xb | 0x9 | 0x0 | 0xa | 0xd | 0x3 | 0xe | 0xf | 0x8 | 0x4 | 0x7 | 0x1 | 0x2 |

    LUT in binary (**big endian byte** order and **little endian bit** order):
    
    | 0000 | 0001 | 0010 | 0011 | 0100 | 0101 | 0110 | 0111 | 1000 | 1001 | 1010 | 1011 | 1100 | 1101 | 1110 | 1111 |
    | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
    | 1100 | 0101 | 0110 | 1011 | 1001 | 0000 | 1010 | 1101 | 0011 | 1110 | 1111 | 1000 | 0100 | 0111 | 0001 | 0010 |

    In PEIGEN, bitslicing is done as follows:

    LUT in binary in memory (**little endian byte** order and **little endian bit** order):
    
    | 1111 | 1110 | 1101 | 1100 | 1011 | 1010 | 1001 | 1000 | 0111 | 0110 | 0101 | 0100 | 0011 | 0010 | 0001 | 0000 |
    | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
    | 0010 | 0001 | 0111 | 0100 | 1000 | 1111 | 1110 | 0011 | 1101 | 1010 | 0000 | 1001 | 1011 | 0110 | 0101 | 1100 |

    Bitslicing (**little endian byte** order and **little endian bit** order):
    
    | 1111111100000000 | 1111000011110000 | 1100110011001100 | 1010101010101010 |
    | ---------------- | ---------------- | ---------------- | ---------------- |
    | 0000111011011001 | 0011011010000111 | 1010011101001100 | 0110010110011010 |

    Condensed bitsliced representation (directly indicate memory):
    
    | ff00_f0f0_cccc_aaaa |
    | ------------------- |
    | 0ed9_3687_a74c_659a |

    This resulting string `0ed9_3687_a74c_659a` can be input to functions of PEIGEN to representing the S-box `{0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2}`.

## Final Remarks

For more theoretical aspects on S-boxes and more details on PEIGEN, please refer to the paper [\[2\]](https://eprint.iacr.org/2019/209).

Due to the storage limitation, we will not upload other results generated by PEIGEN.
Please see https://www.dropbox.com/sh/x6y7u8gfvi0ki72/AADW7Pgy9NXFbA5_yAxNlBbga for them (> 5 GBs).

Please kindly cite this paper when you produce any results with the help of PEIGEN.

We aim to improve PEIGEN continuously. Your contributions are welcome.

## References

[1] Jérémy Jean, Thomas Peyrin, Siang Meng Sim and Jade Tourteaux.: Optimizing Implementations of Lightweight Building Blocks. https://eprint.iacr.org/2017/101.

[2] Zhenzhen Bao, Jian Guo, San Ling and Yu Sasaki.: SoK: PEIGEN -- a Platform for Evaluation, Implementation, and Generation of S-boxes. https://eprint.iacr.org/2019/209.
