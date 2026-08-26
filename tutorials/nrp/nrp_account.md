# Tutorial: Creating an NRP Account and Accessing the Course JupyterHub

[← Back to HPC Tutorials](../tutorials.md)

In this class, you will use compute resources provided by the [National Research Platform
(NRP)](https://nrp.ai/). NRP provides shared research computing resources, including GPU-enabled
systems and a hosted [JupyterHub](https://nrp.ai/documentation/userdocs/jupyter/jupyterhub-service/)
environment. 

Before you can use the course JupyterHub, you must:

1. sign in to NRP and register your account;
2. notify the instructor so you can be added to the course namespace/project; and
3. sign in to the NRP JupyterHub and start a notebook server.

> **For TXST students:** NRP uses institutional authentication through CILogon. You should be able
> to select **Texas State University** as your identity provider and sign in using your TXST
> credentials. If Texas State does not appear in the CILogon list, follow the troubleshooting note
> below rather than creating a separate personal account.  

## Part 1: Create/Register Your NRP Account

If you already have an NRP account, skip to [Part 2](#part-2-notify-the-instructor).

1. Go to the [NRP Portal](https://nrp.ai/).
2. Click **Log In** in the upper-right corner.
3. You will be redirected to the NRP authentication system, which uses
   [CILogon](https://www.cilogon.org/) for institutional authentication. 
4. In the identity-provider list, select **Texas State University**.
5. Sign in using your TXST NetID and password.
6. On your first login, review and accept the NRP **Acceptable Use Policy (AUP)**.
7. After authentication is complete, you should be returned to the NRP portal as a registered user.

NRP does not require you to create a separate username and password when you authenticate through
your institution. Your institutional identity is used to establish your NRP account. 

> **If you do not see Texas State University:** Do not immediately create a second NRP identity
> using a personal Google or GitHub account. First, verify that you searched for the full
> institution name in the CILogon identity-provider list. If TXST is still unavailable, contact the
> instructor so we can confirm the correct identity provider for the class. 

**References:**

- [NRP: Getting Access](https://nrp.ai/get-access/)
- [NRP: Getting Started with Nautilus](https://nrp.ai/documentation/userdocs/start/getting-started/)
- [CILogon](https://www.cilogon.org/)

## Part 2: Notify the Instructor

After you have successfully logged in to the NRP portal, notify the instructor on the course [Slack
workspace](https://cs7331-txst.slack.com). 

Send a message to **@Apan** indicating that your NRP account has been created.

The instructor will add you to the NRP project/namespace used for this class. You will receive a
confirmation message when your access has been configured. 

> **Important:** Creating an NRP account does not automatically give you access to the course
> compute resources. NRP separates **authentication** (verifying who you are) from **authorization**
> (determining which project or namespace you can use). You must be added to the course
> project/namespace before using the class resources. 

NRP documentation similarly notes that students should have their supervisor or project
administrator add them to the appropriate namespace/project. 

## Part 3: Access the NRP JupyterHub

After you receive confirmation that you have been added to the course resources:

1. Go to the [NRP JupyterHub](https://jupyterhub-west.nrp-nautilus.io/).
2. Click the sign-in option and authenticate using your institutional credentials.
3. Select **Texas State University** through CILogon, if prompted.
4. After authentication, JupyterHub will display a form for selecting the resources for your notebook server.
5. For this class, use the configuration specified by your instructor. Unless otherwise stated, the
   recommended starting configuration is: 

   - **1 GPU**
   - **16 CPU cores**

6. Start the server.
7. After the server launches, the JupyterLab interface should appear in your browser.

> **Resource usage:** Request only the resources needed for your assignment. GPUs are shared
> resources, so do not request a GPU for work that can run efficiently on CPUs alone. 

**Reference:** [NRP JupyterHub Service](https://nrp.ai/documentation/userdocs/jupyter/jupyterhub-service/)

## Part 4: Important JupyterHub Behavior

The NRP-hosted JupyterHub provides a persistent home directory, but the notebook server itself is
not intended to run indefinitely. 

According to the current NRP documentation:

- your JupyterHub home directory initially has **5 GB** of persistent storage;
- your notebook server may be shut down after your browser has been disconnected for approximately
  **1 hour**; and 
- for longer-running jobs, you should not rely on an interactive browser session remaining active.

For this course, JupyterHub is primarily intended for interactive development, testing, and
notebook-based assignments. 

> **Save your work regularly.** Do not assume that an active notebook server will remain running if
> you close your browser or disconnect for an extended period. 

## Troubleshooting

### I can log in to NRP, but I cannot use the course resources

You may not yet have been added to the class project/namespace. Contact the instructor on Slack.

### Texas State University does not appear in the identity-provider list

Do not create a second identity using a personal account unless instructed to do so. Contact the instructor so the correct CILogon identity provider can be confirmed.

### JupyterHub does not let me start a server

First verify that:

1. you successfully registered your NRP account;
2. the instructor confirmed that you were added to the course resources; and
3. you are signing in using the same institutional identity that you used when registering with NRP.

If the problem persists, send the instructor the **error message shown by JupyterHub**. Do not send
your TXST password or authentication codes. 

## Next Step

Continue to **[Tutorial 2: Running Jobs on JupyterHub](nrp_jupyter_hub.md)**.

## Additional Resources

- [National Research Platform](https://nrp.ai/)
- [NRP Getting Started Guide](https://nrp.ai/documentation/userdocs/start/getting-started/)
- [NRP JupyterHub Documentation](https://nrp.ai/documentation/userdocs/jupyter/jupyterhub-service/)
- [NRP Nautilus Usage Guide](https://nrp.ai/documentation/userdocs/start/using-nautilus/)
- [JupyterLab Documentation](https://jupyterlab.readthedocs.io/)
- [CILogon](https://www.cilogon.org/)
