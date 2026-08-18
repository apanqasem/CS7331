# Tutorial 1: Setting Up Your TACC Account and MFA

[← Back to the TACC Tutorials](tacc_tutorials.md)

Before you can use [Stampede3](https://docs.tacc.utexas.edu/hpc/stampede3/), you need an active account with the [Texas Advanced Computing Center (TACC)](https://tacc.utexas.edu/) and a configured multi-factor authentication (MFA) method.

> **For this class:** Use your official university email address when creating your TACC account. Your instructor will use that affiliation to add you to the course allocation.

## Part 1: Create a TACC Account

If you already have an active TACC account, skip to [Part 2](#part-2-set-up-multi-factor-authentication-mfa).

1. Open the [TACC Accounts Portal](https://accounts.tacc.utexas.edu/).
2. Select **Create a New Account**.
3. Complete the registration form using your official university email address.
4. Follow the instructions to configure MFA.
5. Check your email for the TACC confirmation/activation message and follow the activation link.
6. Return to the Accounts Portal and verify that your account status is **Active**.

TACC requires an approved institutional affiliation for new accounts. An active TACC account is also separate from access to a particular supercomputer: your account must be associated with an active allocation before you can log in to Stampede3.

**Reference:** [Managing Your TACC Account](https://docs.tacc.utexas.edu/basics/accounts/)

## Part 2: Set Up Multi-Factor Authentication (MFA)

TACC requires MFA when accessing its compute and storage resources. TACC supports authenticator applications as well as SMS for eligible users.

For this tutorial, we will use **Google Authenticator**, but another TACC-supported authenticator app can be used instead.

### Install Google Authenticator

Install Google Authenticator on your phone:

- [Google Authenticator for iPhone/iPad](https://apps.apple.com/us/app/google-authenticator/id388497605)
- [Google Authenticator for Android](https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2)

### Pair the App with TACC

1. Log in to the [TACC Accounts Portal](https://accounts.tacc.utexas.edu/).
2. Select **Multi-Factor Auth** from the left-hand menu.
3. Choose **Authenticator App** as the pairing method.
4. TACC will display a QR code.
5. Open Google Authenticator on your phone.
6. Tap **+** and select **Scan a QR code**.
7. Scan the QR code displayed by the TACC Accounts Portal.
8. Google Authenticator will create a TACC entry and display a temporary numeric code.
9. Enter the current code in the TACC web form to complete the pairing process.

> **Security note:** Do not share or save a screenshot of the MFA QR code. Anyone with that pairing information may be able to generate valid authentication codes for your account.

**Reference:** [Multi-Factor Authentication at TACC](https://docs.tacc.utexas.edu/basics/mfa/)

## Part 3: Confirm Your Account Is Ready

Before proceeding to Tutorial 2, verify the following:

- Your TACC account status is **Active**.
- MFA is successfully paired.
- You have been added to the course allocation for Stampede3.

You can review account and allocation information through the [TACC Accounts Portal](https://accounts.tacc.utexas.edu/) and [TACC User Portal](https://tacc.utexas.edu/portal/).

> **Important:** Having an active TACC account does not, by itself, provide access to Stampede3. You must also belong to an active allocation with access to the resource.

## Next Step

Continue to **[Tutorial 2: Connecting to Stampede3 and Running a Slurm Job](connecting_to_stampede3_tacc.md)**.
