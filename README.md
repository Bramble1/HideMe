# HideMe
Hide Encrypted data, or private keys  inside an unsuspecting binary file


# To do

1. Create a structure to contain information about where the secret file is located
   inside the binary, as well as the size for extraction.

2. Store this structure inside the .note section if we can, or any other unused
   space, so when we are extracting a secret key or file from an unsuspecting binary
   we can obtain this structure in order to understand if a secret file exists
   and it's relevant information for extraction.
