# cluster-connections

A clustering algorithm that classifies second nearest neighbor atom pairs based 
on the number of interconnecting first neighbors.

Some physical systems present a splitting of the second peak of the radial 
distribution function ([RDF](https://en.wikipedia.org/wiki/Radial_distribution_function)). 
This code can be used to analyze the causes of this splitting in terms of cluster 
connections. This is done by calculating the partial RDF of different categories, 
where each one is defined by the number of interact atoms in the first 
coordination shell of a central atom that interconnects it with its second 
interacting neighbors. This type of cluster analysis was performed and published 
in a [scientific article](https://doi.org/10.1039/D1CP02216D), and the idea is 
similar to the one presented in the work of [Ding et al.](https://doi.org/10.1038/srep17429)


## Requirements

You need a C++ compiler and an NVT molecular dynamics trajectory in 
[XYZ file format](https://en.wikipedia.org/wiki/XYZ_file_format).


## Usage

### Compilation

Compile the executable with make:
```bash
cd src/ 
make clean
make
```

### Execution

```bash
./cluster-connections < in.config
```

where in.config is a file with information in the following order, no comments
or extra lines:
```
file with the trajectory in xyz format
central atom type
interact atom type
box size in x, y, z direction
true if periodic boundary conditions must be considered
inner cutoff radius
outer cutoff radius
number of categories
number of bins in partial rdfs
```

For more details, check the [example](https://github.com/fernandezfran/cluster-connections/tree/main/example).


## License

[MIT License](https://github.com/fernandezfran/cluster-connections/blob/master/LICENSE)


## Contact

For errors, questions or suggestions, please contact at the following e-mail address:
<fernandezfrancisco2195@gmail.com>
