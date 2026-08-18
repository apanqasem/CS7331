# Tutorial 3: Interactive Development with `idev`

[← Back to the TACC Tutorials](tacc_tutorials.md)

Batch jobs are the normal way to run production work on a supercomputer, but they can be inconvenient when you are repeatedly compiling, testing, and debugging a program.

TACC provides [`idev`](https://docs.tacc.utexas.edu/software/idev/) for this purpose. `idev` requests compute resources through Slurm and, once the job starts, gives you an interactive shell on a compute node.

> **Key idea:** `idev` does not bypass Slurm. It submits an interactive job to the scheduler, typically using a development partition where waits are often short.

## Part 1: Start an `idev` Session

After logging in to Stampede3, first move to the directory where you want to work. For example:

```bash
cd $SCRATCH/hpc_tutorial
```

Then start an interactive session:

```bash
idev
```

If this is your first `idev` session and you belong to multiple TACC projects, `idev` may ask you to select an allocation.

While waiting, `idev` reports the Slurm job state. Common states include:

- `PD` — the interactive job is **pending**;
- `R` — the job is **running**.

Once resources are available, your shell prompt changes from a Stampede3 login node to a compute-node prompt.

By default, `idev` requests:

- **1 compute node**
- **30 minutes** of interactive time

The exact partition and other defaults can depend on the system and your configuration.

## Part 2: Customize the Request

You can change the resources requested by `idev` using command-line options.

Common options include:

| Option | Meaning |
|---|---|
| `-t hh:mm:ss` | Requested wall-clock time |
| `-m minutes` | Requested time in minutes |
| `-N nodes` | Number of compute nodes |
| `-n tasks` | Total number of tasks |
| `-tpn tasks` | Tasks per node |
| `-A account` | TACC allocation/account |
| `-p partition` | Slurm partition |

Display the current options supported by TACC with:

```bash
idev --help
```

### Example: Request One Hour

```bash
idev -t 01:00:00
```

### Example: Request Two Nodes for 45 Minutes

```bash
idev -N 2 -t 00:45:00
```

### Example: Specify the Course Allocation

```bash
idev -A YOUR_CLASS_ACCOUNT_ID
```

Replace `YOUR_CLASS_ACCOUNT_ID` with the allocation ID provided by your instructor.

> **Resource limits:** Development-partition limits can change. Use `qlimits` on Stampede3 and consult the [Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/) for current limits.

## Part 3: Work on the Compute Node

Once the `idev` session starts, you can interact with the compute node directly.

### Compile Code

For example, compile a C program using [GCC](https://gcc.gnu.org/onlinedocs/):

```bash
gcc hello.c -o hello
```

For MPI programs, the appropriate compiler wrapper depends on the loaded TACC software environment. You can inspect available modules with:

```bash
module list
module avail
```

### Run a Serial Program

```bash
./hello
```

### Run an MPI Program

On TACC systems, MPI applications are normally launched with `ibrun`. For example:

```bash
ibrun ./my_mpi_program
```

See the [Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/) for TACC's MPI launch guidance.

### Edit Files

You can use a terminal editor such as:

- [Emacs](https://www.gnu.org/software/emacs/manual/emacs.html)
- [Vim](https://www.vim.org/docs.php)

For example:

```bash
emacs hello.c
```

## Part 4: Leave the Interactive Session

When you are finished, exit the interactive shell:

```bash
exit
```

This ends the `idev` session and releases the allocated compute resources.

Do not keep an interactive allocation running when you are no longer using it; those resources are unavailable to other users while your job is active.

## Quick Reference

```bash
# Start a default interactive session
idev

# Request one hour
idev -t 01:00:00

# See idev options
idev --help

# Check current TACC queue limits
qlimits

# Leave the compute node
exit
```

## Additional Resources

- [TACC `idev` User Guide](https://docs.tacc.utexas.edu/software/idev/)
- [Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/)
- [GCC Online Documentation](https://gcc.gnu.org/onlinedocs/)
- [Slurm Documentation](https://slurm.schedmd.com/documentation.html)
