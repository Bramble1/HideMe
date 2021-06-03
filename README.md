# HideMe
Hide Encrypted .gpg Files  inside an unsuspecting binary file

Program only accepts already encrypted files to then hide inside an executable binary, to add another level of security in terms of hiding encrypted file

# Usage

      ./HideMe h    (for help on how to use program)


# Complete

1. Create a structure to contain information about where the secret file is located
   inside the binary, as well as the size for extraction.

2. Store this structure inside the .note section if we can, or any other unused
   space, so when we are extracting a secret key or file from an unsuspecting binary
   we can obtain this structure in order to understand if a secret file exists
   and it's relevant information for extraction.
   
# Currently in progress (refer to development branch for updates)

1. divide the binary data and store each peice in empty code caves within a linked list data structure.
2. Will be using a hashing algorithm to get a randomised value, which will be stored hidden in the binary, so the extraction program knows which order the divided binary data needs to be constructed in, as the linked list will allow us to store the dividided data in a random order, thus needs to be constructed back in the correct order to restore the .pgp file.


Algorithm
----------

1. Open supplied binary into memory

2. Get the secret file, supplied 

3. Go through the program header table and check the size difference between each segment
   to determine any code caves (empty bytes) big enough that would fit the size of the secret
   file. If Found then store the coordinates.

4. Using a structure store information of those coordinates, as well as the size. Using this structure
   to then write the secret file into the code cave.

5. The Write the structure itself (should be small enough) to the .NOTE segment(overrite it)

---------------
Then When Extracting the secret key file

1. Obtain struct from .Note

2. Using Struct to determine where the secret file is located, and extract the secret key
3. 
