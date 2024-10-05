# HideMe
Hide Encrypted .gpg Files  inside an unsuspecting binary file Features being to divide and hide and divide/randomise and hide. Proof of concept, Educational purposes only. This is a basic proof of concept which tests the concept by dividing the pgp file to hide up to a maximum of 3 segments in this example.

# Compile

	./compile	#Will create a test lab in the directory.

# Usage

      ./HideMe          #Run Without arguments and the help menu will be printed as shown below:
      
       "[?]write/read to binary:
	./Hideme <w> <divisor[1-3] > <Target_Binary> <file.pgp>
	./HideMe <r> <Target_binary> <extracted.pgp>
      __________________________________
      [?]randomised write/read to binary:
	./Hideme <rw> <divisor[2-3] > <Target_Binary> <file.pgp>
	./Hideme <rr> <Target_Binary> <extracted.pgp>"

# Todo

Find a better place to hide the map structure. Annotate code. 

Use a user defined config for important information the program relies upon,otherwise it's hardwwired into the code, which can be reverse engineered,and it being
open source, the program should not be used in the real world because of this, as understanding the program, someone will then know what to look out for
when inspecting a suspicious binary. And furthermore can use the program itself to check.

So to mitigate the prior issue, The next version the user will be required to enter a password which will be hashed and stored within each binary.

Again this is just an educational proof of concept program. Nothing more. Which others can learn from, if they wish.


# Arguably Still usable in current state.

Even though This program is open source. It can still be used successfully if an administrator or anyone is being lazy and not thoroughly checking
each binary, So you could send pgp files over a network hidden in a compiled C program in this case. So if it get's intertecpted, or found on a hdd. It's hidden
in the binary and and the first assumption is that it's an ELF Binary. And Worst case scenario, it's found and pieced backtogether in the correct order and extracted, it's still going to be encrypted, and thus now finding the pgp file, they then now need to break that, so I would say it's better than just sending
pgp files to someone and them being stored or sent in plain site. This adds another level of obscurity and This program is not very popular, so I doubt most people will be aware of this program.

# Not being maintained

