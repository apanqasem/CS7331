# Tutorial 2: Connecting to Stampede3 and Running a Slurm Job

[← Back to the TACC Tutorials](tacc_tutorials.md)

In this tutorial, you will log in to [Stampede3](https://docs.tacc.utexas.edu/hpc/stampede3/) using SSH, create a small [Slurm](https://slurm.schedmd.com/) batch script, submit it to the scheduler, and inspect its output.

## Before You Begin

You should have:

- an **active TACC account**;
- a configured **MFA method**;
- access to the **course allocation**; and
- a terminal with an [SSH](https://www.openssh.com/) client.

If you have not completed the account setup, start with **[Tutorial 1: Setting Up Your TACC Account and MFA](tacc_account.md)**.

## Part 1: Connect via SSH

Open a terminal:

- **macOS/Linux:** Terminal
- **Windows:** PowerShell or Windows Terminal

Connect to Stampede3 with:

```bash
ssh your_username@stampede3.tacc.utexas.edu
```

Replace `your_username` with your TACC username.

You will be prompted for your TACC credentials and MFA authentication. When entering a password in a terminal, **no characters or dots are displayed**; this is normal.

After a successful login, your prompt will indicate that you are on one of the Stampede3 login nodes.

> **Do not run computationally intensive programs on a login node.** Login nodes are shared resources intended for tasks such as file management, editing, environment setup, and job submission. Computational work should run on compute nodes through Slurm.

Useful references:

- [Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/)
- [TACC SSH Documentation](https://docs.tacc.utexas.edu/datatransfer/ssh/)

## Part 2: Move to Your Scratch Directory

TACC recommends running computational jobs from the system's scratch file system rather than from your home directory.

Move to your Stampede3 scratch directory:

```bash
cd $SCRATCH
```

Create a directory for this tutorial and enter it:

```bash
mkdir -p hpc_tutorial
cd hpc_tutorial
```

You can confirm your current directory with:

```bash
pwd
```

## Part 3: Create Your First Slurm Batch Script

Stampede3 uses the [Slurm Workload Manager](https://slurm.schedmd.com/) to allocate compute resources and run jobs.

Create a file named `test_job.slurm` using a terminal editor. For example:

```bash
emacs test_job.slurm
```

You may instead use another editor such as [Vim](https://www.vim.org/docs.php).

Paste the following into the file:

```bash
#!/bin/bash

#SBATCH -J hpc_test_job           # Job name
#SBATCH -o hpc_test_job.o%j       # Standard output (%j = job ID)
#SBATCH -e hpc_test_job.e%j       # Standard error
#SBATCH -p skx-dev                # Stampede3 development partition
#SBATCH -N 1                      # Number of compute nodes
#SBATCH -n 4                      # Total number of tasks
#SBATCH -t 00:05:00               # Maximum run time (hh:mm:ss)

echo "Hello from Stampede3!"
echo "This job is running on node: $(hostname)"
echo "Current date and time:"
date
```

Replace `YOUR_CLASS_ACCOUNT_ID` with the allocation ID provided by your instructor.

> **Note:** Stampede3 queue names and limits can change. The [Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/) contains the current queue configuration, and the TACC `qlimits` command displays current limits on the system.

If you are using Emacs, save with **Ctrl-x Ctrl-s** and exit with **Ctrl-x Ctrl-c**.

## Part 4: Submit and Track the Job

Submit the script with [`sbatch`](https://slurm.schedmd.com/sbatch.html):

```bash
sbatch test_job.slurm
```

Slurm will respond with a message similar to:

```text
Submitted batch job 1234567
```

The number is your **job ID**.

Check your jobs with [`squeue`](https://slurm.schedmd.com/squeue.html):

```bash
squeue -u $USER
```

Common job states include:

- `PD` — **Pending**: the job is waiting for resources.
- `R` — **Running**: the job is executing.
- no entry — the job has likely completed or exited.

## Part 5: View the Results

List the files in your directory:

```bash
ls
```

After the job completes, you should see files similar to:

```text
hpc_test_job.o1234567
hpc_test_job.e1234567
```

View standard output with:

```bash
cat hpc_test_job.o*
```

If the error file is non-empty, inspect it with:

```bash
cat hpc_test_job.e*
```

A successful run should print a greeting, the name of the compute node assigned by Slurm, and the date.

## Useful Slurm Commands

| Command | Purpose |
|---|---|
| `sbatch script.slurm` | Submit a batch job |
| `squeue -u $USER` | Show your queued/running jobs |
| `scancel JOB_ID` | Cancel a job |
| `qlimits` | Display current TACC queue limits |

See the [official Slurm command documentation](https://slurm.schedmd.com/man_index.html) for additional commands.

## Next Step

For compiling, testing, and debugging interactively on a compute node, continue to **[Tutorial 3: Interactive Development with `idev`](tacc_idev.md)**.
