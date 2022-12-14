# Red Black Tree
The repository contains an implementation of a Red-black tree working with int

## Directory layout:
* `docs/`
     Sources for building the documentation of the project.
* `lib/`
     External libraries.
* `src/`
     Sources for building the project.
* `tests/`
     Tests related to the project.

## External Libraries and Git Submodules

The following libraries are made available as git submodules. They can be found in the `lib/` subdirectory.

* [Catch](https://github.com/catchorg/Catch2.git) -- A library for unit testing.
* [Spdlog](https://github.com/gabime/spdlog.git)  -- A library for recording logs.

## Cloning the repository

As the repository contains submodules, you can clone and populate them in a single step like this:

```bash
git clone --recurse-submodules git@gitlab.com:vasil_angelov20/red-black-tree.git <your-project-name>
```

where `<your-project-name>` is the name you want to give to your project.

Another option is to clone it and initialize the submodules manually:

```bash
git clone git@gitlab.com:vasil_angelov20/red-black-tree.git <your-project-name>
cd <your-project-name>
git submodule init
git submodule update
```