# Tutorial 1: Setting Up TACC and ACCESS Accounts

[← Back to the TACC Tutorials](tacc_tutorials.md)

Before you can use [Stampede3](https://docs.tacc.utexas.edu/hpc/stampede3/) for this class, you need:

1. an active account with the [Texas Advanced Computing Center (TACC)](https://tacc.utexas.edu/);
2. an [ACCESS](https://access-ci.org/) account and ACCESS ID; and
3. access to the course allocation on Stampede3.

TACC provides the computing resource, while ACCESS is now used to manage Stampede3 allocations and
project membership. 

> **Important:** Your **TACC username** and **ACCESS ID** are separate identities and may not be the
> same. You will use your TACC credentials to log in to Stampede3, but your instructor needs your
> ACCESS ID to add you to the course allocation. 

---

## Part 1: Create a TACC Account

If you already have an active TACC account, skip to [Part 2](#part-2-set-up-tacc-multi-factor-authentication-mfa). 

1. Open the [TACC Accounts Portal](https://accounts.tacc.utexas.edu/).
2. Select **Create a New Account**.
3. Complete the registration form using your official university email address.
4. Follow the activation instructions sent by TACC.
5. Return to the Accounts Portal and verify that your account status is **Active**.

> **For this class:** Use your official TXST email address when creating your TACC account.

An active TACC account does **not** by itself provide access to Stampede3. You must also be added to
an allocation that includes Stampede3. 

**Reference:** [Managing Your TACC Account](https://docs.tacc.utexas.edu/basics/accounts/)

---

## Part 2: Set Up TACC Multi-Factor Authentication (MFA)

TACC requires multi-factor authentication for remote access to its systems.

For this tutorial, we will use **Google Authenticator**, although another TACC-supported
authenticator application can also be used. 

### Install Google Authenticator

- [Google Authenticator for iPhone/iPad](https://apps.apple.com/us/app/google-authenticator/id388497605)
- [Google Authenticator for
  Android](https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2) 

### Pair the App with TACC

1. Log in to the [TACC Accounts Portal](https://accounts.tacc.utexas.edu/).
2. Select **Multi-Factor Auth** from the left-hand menu.
3. Choose **Authenticator App** as the pairing method.
4. TACC will display a QR code.
5. Open Google Authenticator on your phone.
6. Tap **+** and select **Scan a QR code**.
7. Scan the QR code displayed by TACC.
8. Enter the current authentication code in the TACC web form to complete the pairing process.

> **Security note:** Do not share your password, MFA code, or MFA QR code with anyone.

**Reference:** [TACC Multi-Factor Authentication](https://docs.tacc.utexas.edu/basics/mfa/)

---

## Part 3: Create an ACCESS Account

Stampede3 allocations are now managed through the NSF-funded [ACCESS](https://access-ci.org/) program.

**Every student must have an ACCESS ID** before the instructor can add the student to the course allocation.

1. Go to the [ACCESS website](https://access-ci.org/).
2. Select **Create an Account** / **Register for an ACCESS ID**.
3. Register using your official TXST university email address.
4. Follow the ACCESS account-registration and identity-verification steps.
5. After registration is complete, record your **ACCESS ID**.

Your ACCESS ID is the username that identifies you within ACCESS. It may be different from your TACC username.

> **Important:** Do not assume that your TACC username is your ACCESS ID. The instructor must add
> you to the allocation using the **ACCESS ID** shown in your ACCESS account. 

**References:**

- [ACCESS: For Educators](https://access-ci.org/get-started/for-educators/)
- [ACCESS Allocations Portal](https://allocations.access-ci.org/)
- [ACCESS Account Information](https://support.access-ci.org/documentation/your-access-account)

---

## Part 4: Send Your ACCESS ID to the Instructor

After you have created both accounts:

1. Verify that your **TACC account is Active**.
2. Verify that you can log in to ACCESS.
3. Send your **ACCESS ID** to the instructor using the communication method specified for the class.

The instructor will add your ACCESS ID to the course project and grant access to the Stampede3 allocation.

> **Do not send your TACC or ACCESS password.** The instructor only needs your ACCESS ID.

### Why Is This Necessary?

ACCESS now manages Stampede3 allocation membership. The instructor adds students to the project
through the [ACCESS Allocations Portal](https://allocations.access-ci.org/), using each student's
ACCESS ID. 

Creating a TACC account gives you an identity at TACC. Creating an ACCESS account gives you an
identity that can be attached to the course allocation. Both are needed for this workflow. 

---

## Part 5: Wait for Stampede3 Access to Be Provisioned

Being added to the ACCESS project does not necessarily give you immediate access to Stampede3.

ACCESS notes that new resource accounts may take several days to be provisioned and recommends
allowing **up to one week** when adding students to a class allocation. 

You can check the status through the [ACCESS Allocations Portal](https://allocations.access-ci.org/):

1. Log in with your ACCESS account.
2. Open **My Projects**.
3. Locate the course project.
4. Check the resource/account information associated with Stampede3.

The username shown for the actual computing resource may differ from your ACCESS ID.

Once provisioning is complete, you should be able to log in to Stampede3 using your TACC credentials.

---

## Part 6: Confirm That You Are Ready

Before continuing to Tutorial 2, verify the following:

- [ ] My TACC account is **Active**.
- [ ] TACC MFA is configured.
- [ ] I have created an **ACCESS account**.
- [ ] I have sent my **ACCESS ID** to the instructor.
- [ ] I have been added to the course allocation.
- [ ] My Stampede3 access has been provisioned.

If all six items are complete, you are ready to connect to Stampede3.

---

## Next Step

Continue to **[Tutorial 2: Connecting to Stampede3 and Running a Slurm Job](connecting_to_stampede3_tacc.md)**.

---

## Additional Resources

- [TACC Accounts Portal](https://accounts.tacc.utexas.edu/)
- [TACC Stampede3 User Guide](https://docs.tacc.utexas.edu/hpc/stampede3/)
- [TACC Allocations](https://tacc.utexas.edu/use-tacc/allocations/)
- [ACCESS](https://access-ci.org/)
- [ACCESS for Educators](https://access-ci.org/get-started/for-educators/)
- [ACCESS Allocations Portal](https://allocations.access-ci.org/)
- [ACCESS: How to Add Users to a Project](https://allocations.access-ci.org/how-to)
