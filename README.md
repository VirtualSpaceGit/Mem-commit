# SafeWrite Hook :gear:

[![MIT License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## **Overview**

The **SafeWrite Hook** demonstrates how to intercept file write operations on Windows. Rather than immediately writing data to disk, it temporarily holds changes in memory until explicitly committed. This allows for secured experimentation with updates, modifications, or temporary file changes without risking unintended, permanent alterations. Essentially, it's a sandbox mechanism designed to safeguard file integrity, providing a straightforward Proof of Concept for VirtualSpace. 🤝

## **Key Highlights**

- **Intercept & Queue:** Captures all file-write operations before they reach the disk using **__minhook.__**
- **Controlled Commits:** Lets you choose to apply (commit) or discard queued data in your sandbox.
- **Optional Encryption:** Keeps queued data safe in memory/sandbox. Using standard XOR for PoC purposes.
- **Partial or Full Writes:** Commit only parts of your changes or everything at once, essential for debugging.

## **Simple Flow**

1. **Compile & Run** – It hooks into the Windows `WriteFile` function at runtime.
2. **Make Changes** – As you or another program attempts to write data, it gets stored (and encrypted) in memory.
3. **Choose an Action** – Commit some or all changes, or discard them altogether.
4. **See Results** – Check the target file on disk to view the final outcome.

## **Who Benefits?**

- **Developers** who need a safe testing environment.
- **Security Enthusiasts** looking at data interception techniques.
- **Anyone** interested in controlling how data is written to disk.

## **Notice**

⚠️ This is strictly a **Proof of Concept**. It **does not** represent a production-ready tool and should **not** be relied on for actual security or data protection. Use it solely for demonstration and educational purposes.

## **License**

This project is released under the [MIT License](LICENSE). Feel free to fork, modify, and share!
