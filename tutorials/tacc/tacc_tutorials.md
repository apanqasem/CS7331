# TACC Tutorials for HPC Students

These short tutorials provide the minimum setup needed to begin using [Texas Advanced Computing Center (TACC)](https://tacc.utexas.edu/) resources for class assignments. The examples use [Stampede3](https://docs.tacc.utexas.edu/hpc/stampede3/), TACC's Slurm-managed HPC system.

Complete the tutorials **in order** if this is your first time using TACC.

## Tutorials

### 1. [Setting Up Your TACC Account and MFA](tacc_access_accounts.md)

Create your TACC account, configure multi-factor authentication (MFA), and make sure your account is ready to be added to the class allocation.

**You will learn how to:**

- create and activate a TACC account;
- pair an authenticator application with TACC;
- verify your account status; and
- distinguish a TACC account from access to a specific allocation/resource.

---

### 2. [Connecting to Stampede3 and Running a Slurm Job](connecting_to_stampede3_tacc.md)

Log in to Stampede3 from a terminal and submit a simple job through the Slurm scheduler.

**You will learn how to:**

- connect to Stampede3 using SSH;
- work from your `$SCRATCH` directory;
- create a Slurm batch script;
- submit a job with `sbatch`;
- monitor it with `squeue`; and
- inspect the job's output and error files.

---

### 3. [Interactive Development with `idev`](tacc_idev.md)

Request an interactive compute node for short development, compilation, testing, and debugging sessions.

**You will learn how to:**

- start an `idev` session;
- request different time/node/task configurations;
- compile and run code on a compute node; and
- release the allocation when you are finished.

---

## The Basic TACC Workflow

Once your account is configured, most class work will follow this pattern:

```text
Your computer
    |
    |  ssh
    v
Stampede3 login node
    |
    |-- sbatch --> Slurm queue --> compute node(s) --> batch results
    |
    `-- idev  --> Slurm queue --> interactive compute node
```

The **login node is the entry point**, not the place to run computationally intensive code. Slurm allocates compute nodes for your jobs.

## Essential External Resources

- [TACC HPC Documentation](https://docs.tacc.utexas.edu/)
- [Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/)
- [TACC Accounts Portal](https://accounts.tacc.utexas.edu/)
- [TACC Multi-Factor Authentication Guide](https://docs.tacc.utexas.edu/basics/mfa/)
- [TACC `idev` Guide](https://docs.tacc.utexas.edu/software/idev/)
- [Slurm Documentation](https://slurm.schedmd.com/documentation.html)
- [GCC Documentation](https://gcc.gnu.org/onlinedocs/)
- [OpenSSH](https://www.openssh.com/)
- [GNU Emacs Manual](https://www.gnu.org/software/emacs/manual/emacs.html)
- [Vim Documentation](https://www.vim.org/docs.php)

## Before Asking for Help

When reporting a problem, include:

1. the tutorial step you were following;
2. the command you entered;
3. the complete error message; and
4. your Slurm job ID, if the problem involves a submitted job.

Do **not** send your password, MFA code, or MFA QR code to anyone.
