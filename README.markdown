**P Chat** Private Chat 
=======================================================================

Created By Ethan Joachim Eldridge
License: MIT

**What is this?**   
This is a simple chat program I made to talk to someone without needing
a username or password on some random service somewhere.

**Why would you do that?**  
Because when people go to other countries and don't check their
facebook account it's nice to chat with them somehow. 

**How do I use it?**  

**Install qdecoder**  
You need to install [qdecoder] in order to be able to parse and handle
the CGI requests. qDecoder was chosen after reviewing the various
libraries [listed here]. To install it, download the tarball and untar
it `tar -xzvf <tarballname>` then run `./configure` and `make install`,
you may need to run the `make` command as your super user.

**Setup WebServer**  
Next, setup Apache for CGI. An example configuration might look like
this:

	<VirtualHost *:80>
	         ServerName www.pci.dev
	         DocumentRoot /home/user/programs/pci/www
	         <Directory />
	                 Options Indexes
	                 AllowOverride None
	         </Directory>
	         Alias /pci /home/user/programs/pci/bin
	         <Directory />
	                 AddHandler cgi-script .cgi
	                 AllowOverride None
	                 Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
	                 Order allow,deny
	                 Allow from all
	         </Directory>
	 
	        ErrorLog ${APACHE_LOG_DIR}/error.cgi.log
	        LogLevel warn

	</VirtualHost>

Note that it is important to setup both static front end (www) and the
CGI directory (pci). One of the biggest 'gotchas' is the permissions on
the data directory. If after attempting to create an account the data 
directory does not exist or repeatedly fails, then you should check the
permissions of the place specified by the `config.h` file and make sure 
it is writable by apache.  One of the best guides for doing so is 
contained in [this S.O. question]  

**Setup Your Config file**  
A sample config file is included in the headers, the best way to setup a
config file is to copy this file, edit it, and rename it to config.h. If 
you do not want to do this manually, you can run `make config` and it will 
save you the grunt work of typing the commands to do so. Note that if a 
config file already exists it will be moved to config.h.bak. Config
files are opened with vi(m) when you use the make command. 

**Compile the Scripts**  
All you need to do is run `make` from the root of the repository.

**Compile the WebSite**  
What you say? Compile the website? Yes. The HTML is generated via
[Harp] and you'll need to have that installed. Simply update the 
`harp.json` file with the proper domain name and run a `harp compile`. 
It shouldn't take long and then you'll have the front end setup and 
ready.

**Setup your hosts file (if local)**  
On most linux systems the hosts file is at /etc/hosts, on windows it is
in system32 I believe (it's been a while since I've been on windozer). 
Add in the name that you have placed into your config's `BASE_URL` 
definition pointing to`127.0.0.1`

**Technologies used**  
 
 - [Harp]
 - [qdecoder]
 - C
 - HTML/JS/CSS

**General Outline of Code**

#####CGI:
<hr>
#####internal:

    data.c -> routines to handle data processing such as:
     - Open and Create Files
     - Open and Create Directories
     - Check last modified times of files
    
    user.c -> routines to handle user creation and registration
     - Create a user
     - Check for existence of a user
    
    message.c -> routes to handle conversations
     - Create a conversation between 2 users
     - Update a conversation between 2 users
     - High level check if message has been updated since X
    
#####external:

    create-user.c -> cgi script to handle POST to create account
    list-user.c -> cgi script to list users for message selection
    reply.c -> cgi script to update or create a conversation between users
    login.c
    register.c

#####Static:
<hr>
#####Website:

	index -> login or register links
	login -> login to the system
	register -> register a user on the system
	talk -> have a conversation with someone

	... more to come (js)

[qdecoder]:http://www.qdecoder.org/wiki/qdecoder
[listed here]:http://cgi.resourceindex.com/programs_and_scripts/c_and_c++/libraries_and_classes/
[this S.O. question]:http://serverfault.com/questions/124800/how-to-setup-linux-permissions-for-the-www-folder
[Harp]:http://harpjs.com
