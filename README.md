# HideMe
Hide Encrypted .gpg Files  inside an unsuspecting binary file

Program only accepts already encrypted files to then hide inside an executable binary, to add another level of security in terms of hiding encrypted file


# To do

1. Create a structure to contain information about where the secret file is located
   inside the binary, as well as the size for extraction.

2. Store this structure inside the .note section if we can, or any other unused
   space, so when we are extracting a secret key or file from an unsuspecting binary
   we can obtain this structure in order to understand if a secret file exists
   and it's relevant information for extraction.


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
