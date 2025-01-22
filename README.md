# SafeWrite Hook POC :gear:

[![MIT License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## **Overview**

The **SafeWrite Hook** demonstrates how to intercept file writes on a Windows system. Instead of writing directly to disk, changes are stored in memory until you decide whether to commit them or not. This approach allows you to test updates, modifications, or temporary data without risking permanent changes. It’s a sandbox-like mechanism that helps protect your files and system integrity.

## **Key Highlights**

- **Intercept & Queue:** Captures all file-write operations before they reach the disk.
- **Controlled Commits:** Lets you choose to apply (commit) or discard queued data.
- **Optional Encryption:** Keeps queued data safe in memory.
- **Partial or Full Writes:** Commit only parts of your changes or everything at once.

## **Simple Flow**

1. **Run the Tool** – It hooks into the Windows `WriteFile` function at runtime.
2. **Make Changes** – As you or another program attempts to write data, it gets stored (and optionally encrypted) in memory.
3. **Choose an Action** – Commit some or all changes, or discard them altogether.
4. **See Results** – Check the target file on disk to view the final outcome.

## **Who Benefits?**

- **Developers** who need a safe testing environment.
- **Security Enthusiasts** looking at data interception techniques.
- **Anyone** interested in controlling how data is written to disk.

## **License**

This project is released under the [MIT License](LICENSE). Feel free to fork, modify, and share!

---
