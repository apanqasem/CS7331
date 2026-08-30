## Benchmarks

Benchmark suites provide established workloads that can simplify
experimental design and make it easier to compare results across
platforms.

### CPU

- [SPEC CPU 2017](https://www.spec.org/benchmarks.html#cpu). Widely used
  benchmark suite for evaluating CPU performance. It is **not free**,
  but CRL has a license; experiments using SPEC must therefore be
  performed on CRL machines.

- [MiBench](https://vhosts.eecs.umich.edu/mibench/). Benchmark suite
  originally designed for embedded systems but also useful for studying
  general CPU performance.

- [PolyBench](https://github.com/MatthiasJReisinger/PolyBenchC-4.2.1.git).
  Collection of numerical kernels particularly suitable for experiments
  involving compiler optimizations and loop transformations. Relatively
  easy to build and execute.

- [NAS Parallel Benchmarks](https://github.com/GMAP/NPB-CPP). Parallel
  benchmarks derived from computational patterns used in scientific
  applications. Particularly useful for evaluating parallel performance
  and scalability.

- [PARSEC Benchmark Suite](https://github.com/bamos/parsec-benchmark).
  Suite of multithreaded applications covering a broader range of
  application domains than NAS.

- [AnghaBench](https://github.com/brenocfg/AnghaBench). Collection of
  approximately one million compilable C programs. Useful for static or
  compiler-based studies; most programs are intended for compilation and
  program-analysis experiments rather than direct execution.

### GPU

- [CUDA Samples](https://github.com/NVIDIA/cuda-samples). Example
  programs and kernels useful for evaluating NVIDIA GPU functionality
  and performance.

- [Rodinia](https://www.cs.virginia.edu/rodinia/doku.php?id=start).
  Heterogeneous benchmark suite containing implementations using CUDA,
  OpenMP, and OpenCL.

- [Parboil](https://github.com/abduld/Parboil). Collection of
  compute-intensive CPU/GPU kernels suitable for accelerator performance
  studies.

### HPC

- [HPC Challenge (HPCC)](https://hpcchallenge.org/hpcc/). Benchmark
  suite designed to evaluate multiple characteristics of HPC systems.
  Includes DGEMM, the dense matrix multiplication operation closely
  related to the LINPACK benchmark used for the TOP500.

- [ECP Proxy Applications](https://proxyapps.exascaleproject.org/).
  Smaller applications and kernels representing important computational
  patterns found in U.S. Exascale Computing Project applications.

- [OpenDwarfs](https://github.com/vtsynergy/OpenDwarfs). Benchmark suite
  organized around commonly occurring computation and communication
  patterns in parallel applications.

### AI and Code {#ai-benchmarks}

- [HeCBench](https://github.com/ORNL/HeCBench). Collection of
  heterogeneous computing benchmarks that can be useful for CPU/GPU
  performance studies.

- [HumanEval](https://github.com/openai/human-eval/tree/master). Dataset
  of programming problems commonly used to evaluate code-generation
  models.

- [CodeNet](https://github.com/IBM/Project_CodeNet). Large collection of
  source-code submissions covering many programming problems and
  languages.

## Benchmark Generators

When an existing benchmark suite is not sufficient, benchmark-generation
tools can provide larger or more diverse program collections.

- **CSmith:** Random C program generator useful for compiler testing and
  program-analysis experiments.
  [\[Web\]](https://embed.cs.utah.edu/csmith/)
  [\[GitHub\]](https://github.com/csmith-project/csmith)

- **BenchPress:** ML-driven benchmark generation framework that can be
  used to generate programs with particular characteristics.
  [\[Paper\]](https://arxiv.org/abs/2208.06555)
  [\[GitHub\]](https://github.com/fivosts/BenchPress)
